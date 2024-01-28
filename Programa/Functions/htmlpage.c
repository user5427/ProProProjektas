#include "htmlpage.h"

int error_number = 0;
#define MALLOC_FAILED 1
#define INPUT_NULL 2
#define REALLOC_FAILED 3
#define FOPEN_FAILED 4
#define HTML_PAGE_NULL 5

//--------- HtmlElement FUNCTIONS ----------------------

//FUNKCIJA GRAZINA ADRESA JEIGU VISKAS SEKMINGAI PAVYKO, KITU ATVEJU GRAZINA NULL
HtmlElement *initHtmlElement(char *htmlTag) {
    HtmlElement *htmlElement = malloc(sizeof(HtmlElement));

    if (htmlElement == NULL) {
        error_number = MALLOC_FAILED;
        return NULL;
    }
    if (htmlTag == NULL) {
        error_number = INPUT_NULL;
        return NULL;
    }

    // Initialize all variables with default values
    htmlElement->_htmlTag = htmlTag;

    htmlElement->text = NULL;
    htmlElement->id = NULL;
    htmlElement->class = NULL;

    htmlElement->_style = NULL;
    htmlElement->_styleSize = 0;
    htmlElement->_styleCount = 0;

    htmlElement->_childrenCount = 0;
    htmlElement->_childrenSize = 2;

    // Initialize the pointer array to other HtmlElements
    htmlElement->_children =
            malloc(sizeof(HtmlElement **) * htmlElement->_childrenSize);

    if (htmlElement->_children == NULL) {
        error_number = MALLOC_FAILED;
        return NULL;
    }

    return htmlElement;
}

HtmlElement *addChild(HtmlElement *parent, HtmlElement **child) {
    if (parent == NULL || *child == NULL) {
        error_number = INPUT_NULL;
        return NULL;
    }

    //JEIGU UZPILDYTAS MASYVAS, REALOKUOJAMA DAR VIETOS (NAUJOS_DYDIS = PRAEITOS_DYDIS * 2)
    if (parent->_childrenCount >= parent->_childrenSize) {
        parent->_childrenSize *= 2;
        parent->_children = realloc(parent->_children, sizeof(HtmlElement **) * parent->_childrenSize);

        //JEIGU IVYKO KLAIDA
        if (parent->_children == NULL) {
            error_number = REALLOC_FAILED;
            //NEPAVYKO REALOKUOTI NAUJOS VIETOS, DYDIS PALIEKAMAS KOKS BUVO
            parent->_childrenSize /= 2;
            return NULL;
        }
    }

    // GALIMA DABAR PRIDETI NAUJA ELEMENTA
    parent->_children[parent->_childrenCount] = *child;
    parent->_childrenCount += 1;

    return *child;
}

// FUNKCIJA GRAZINA POINTERI I HTMLELEMENT, JEI NEPAVYKO -> NULL
HtmlElement *addStyle(HtmlElement *htmlElement, char *property, char *value) {
    if (htmlElement == NULL || property == NULL || value == NULL) {
        return NULL;
    }

    // patikrina ar jau buvo prideta style kokiu elementu, jei ne sukuria vietos atminty
    if (htmlElement->_styleSize == 0) {
        htmlElement->_styleSize = 20;
        htmlElement->_styleCount = 0;

        //calloc funkcija, nes reikia kad viska uzpildytu '\0'
        htmlElement->_style = calloc(htmlElement->_styleSize, sizeof(char));

        if (htmlElement->_style == NULL) {
            return NULL;
        }
    }

    unsigned long lenproperty = strlen(property);
    unsigned long lenvalue = strlen(value);

    // cia laisva vieta, kuria dar galima naudoti; -1 nes turi buti palikta vieta '\0'
    unsigned int freeSpace = htmlElement->_styleSize - htmlElement->_styleCount - 1;

    // style formatas -> "property:value;"

    // tikrina ar dydis pakankamas +2 yra ':', ';' zenklai
    while (lenproperty + lenvalue + 2 >= freeSpace) {
        htmlElement->_styleSize *= 2;

        htmlElement->_style = realloc(htmlElement->_style, htmlElement->_styleSize);

        if (htmlElement->_style == NULL) {
            return NULL;
        }

        // reallocas neuzpildo atminties automatiskai nuliais, del to ta reikia padaryti
        int initializeSize = htmlElement->_styleSize / 2;

        // uzpildymas nuliais
        for (int i = initializeSize; i < htmlElement->_styleSize; ++i) {
            *(htmlElement->_style + i) = 0;
        }

        freeSpace = htmlElement->_styleSize - htmlElement->_styleCount - 1;
    }

    // alokavus pakankamai atminties galima irasyti style informacija

    // style formatas -> "property:value;"

    strcat(htmlElement->_style, property);
    htmlElement->_styleCount += lenproperty;

    *(htmlElement->_style + htmlElement->_styleCount) = ':';
    htmlElement->_styleCount += 1;

    strcat(htmlElement->_style, value);
    htmlElement->_styleCount += lenvalue;

    *(htmlElement->_style + htmlElement->_styleCount) = ';';
    htmlElement->_styleCount += 1;

    return htmlElement;
}

void _freeHtmlElement(HtmlElement **htmlElement) {
    // sitas if tiesiog, kad jeigu NULL net nebelistu i for loopa
    if (*htmlElement == NULL) {
        return;
    }

    if ((*htmlElement)->_style != NULL) {
        free((*htmlElement)->_style);
    }

    for (int i = 0; i < (*htmlElement)->_childrenCount; i++) {
        HtmlElement *child = *((*htmlElement)->_children + i);
        _freeHtmlElement(&child);
    }

    // sitas antras if irgi reikalingas in case parent elementas buvo
    // toks pat kaip ir jo kazkoks child
    if (*htmlElement == NULL) {
        return;
    }

    free(*htmlElement);
    *htmlElement = NULL;
}

//--------- HtmlElement FUNCTIONS ----------------------

//--------- HtmlPage FUNCTIONS ----------------------


//FUNKCIJA GRAZINA ADRESA JEIGU VISKAS SEKMINGAI PAVYKO, KITU ATVEJU GRAZINA NULL
HtmlPage *initHtmlPage(char *fileName) {
    HtmlPage *htmlPage = malloc(sizeof(HtmlPage));

    if (htmlPage == NULL) {
        error_number = MALLOC_FAILED;
        return NULL;
    }

    FILE *htmlFile;

    htmlFile = fopen(fileName, "w");

    if (htmlFile == NULL) {
        error_number = FOPEN_FAILED;
        return NULL;
    }

    htmlPage->_htmlFile = htmlFile;

    //SUKURIA <HEAD> HTML ELEMENTA
    HtmlElement *htmlHead = initHtmlElement("head");
    if (htmlHead == NULL) {
        error_number = error_number;
        return NULL;
    }

    htmlPage->_htmlHead = htmlHead;

    //SUKURIA <BODY> HTML ELEMENTA
    HtmlElement *htmlBody = initHtmlElement("body");
    if (htmlBody == NULL) {
        error_number = error_number;
        return NULL;
    }

    htmlPage->_htmlBody = htmlBody;

    htmlPage->cssFileName = NULL;

    return htmlPage;
}

HtmlElement *addBodyElement(HtmlPage *htmlPage, HtmlElement **htmlElement) {
    if (htmlPage == NULL || *htmlElement == NULL) {
        return NULL;
    }

    // Prideda nauja elementa i body
    if (addChild(htmlPage->_htmlBody, htmlElement) == NULL) {
        error_number = error_number;
        return NULL;
    }

    return *htmlElement;
}

HtmlElement *addHeadElement(HtmlPage *htmlPage, HtmlElement **htmlElement) {
    if (htmlPage == NULL || *htmlElement == NULL) {
        error_number = INPUT_NULL;
        return NULL;
    }

    if (addChild(htmlPage->_htmlHead, htmlElement) == NULL) {
        error_number = error_number;
        return NULL;
    }

    return *htmlElement;
}

void _writeHtmlElement(HtmlPage *htmlPage, HtmlElement *htmlElement, unsigned short depth) {
    FILE *htmlFile = htmlPage->_htmlFile;

    // tarpu pridejimas
    for (int i = 0; i < depth; i++){
        fprintf(htmlFile, "  ");
    }
    // elemento html tago atspausdinimas
    fprintf(htmlFile, "<%s", htmlElement->_htmlTag);

    // papildomi attributes, kurie eina i tago vidu (id, class)
    if (htmlElement->id != NULL) {
        fprintf(htmlFile, " id=\"%s\"", htmlElement->id);
        fprintf(htmlFile, " class=\"%s\"", htmlElement->class);
    }
    if (htmlElement->_style != NULL) {
        fprintf(htmlFile, " style=\"%s\"", htmlElement->_style);
        printf(" style=\"%s\"", htmlElement->_style);
    }
    fprintf(htmlFile, ">");

    //CIA DALYKAI AKTUALUS TIK <HEAD> ELEMENTUI
    if (htmlPage->_htmlHead == htmlElement && htmlPage->cssFileName != NULL) {
        fprintf(htmlFile, "\n  <link rel=\"stylesheet\" href=\"%s\">", htmlPage->cssFileName);
    }
    // teksto, jei jis yra atspausdinimas
    if (htmlElement->text != NULL) {
        int i = 0;
        while (htmlElement->text[i] != '\n'){ // TODO fix this in stringparser.c instead
            ++i;
        }
        htmlElement->text[i] = '\0';

        fprintf(htmlFile, "%s", htmlElement->text);
    }

    if (htmlElement->_childrenCount != 0){
        fprintf(htmlFile, "\n");
    }

    for (int i = 0; i < htmlElement->_childrenCount; ++i) {
        HtmlElement *child = htmlElement->_children[i];

        _writeHtmlElement(htmlPage, child, depth + 1);
    }

    if (htmlElement->_childrenCount != 0){
        for (int i = 0; i < depth; i++) {
            fprintf(htmlFile, "  ");
        }
    }
    fprintf(htmlFile, "</%s>\n", htmlElement->_htmlTag);

    if (htmlElement->_childrenCount != 0){

    }
}

void _freeHtmlPage(HtmlPage **htmlPage) {
    if (htmlPage == NULL) {
        error_number = INPUT_NULL;
        return;
    }
    _freeHtmlElement(&(*htmlPage)->_htmlHead);
    _freeHtmlElement(&(*htmlPage)->_htmlBody);

    // pafreeinimas ir pats HtmlPage galiausiai
    free(*htmlPage);
    *htmlPage = NULL;

}

void createHtmlPage(HtmlPage **htmlPage) {
    if (*htmlPage == NULL) {
        error_number = HTML_PAGE_NULL;
        return;
    }

    FILE *htmlFile = (*htmlPage)->_htmlFile;

    fprintf(htmlFile, "<!DOCTYPE html>\n");

    HtmlElement *htmlHead = (*htmlPage)->_htmlHead;

    // sukuria i html visa head
    _writeHtmlElement(*htmlPage, htmlHead, 0);

    HtmlElement *htmlBody = (*htmlPage)->_htmlBody;

    // sukuria i html visa body
    _writeHtmlElement(*htmlPage, htmlBody, 0);

    _freeHtmlPage(htmlPage);
}
