#include "htmlpage.h"
int error_number = 0;
#define MALLOC_FAILED 1
#define INPUT_NULL 2
#define REALLOC_FAILED 3
#define FOPEN_FAILED 4
#define HTML_PAGE_NULL 5

//--------- HtmlElement FUNCTIONS ----------------------

//FUNKCIJA GRAZINA ADRESA JEIGU VISKAS SEKMINGAI PAVYKO, KITU ATVEJU GRAZINA NULL
HtmlElement* initHtmlElement(char* htmlTag) {
  HtmlElement* htmlElement = malloc(sizeof(HtmlElement));
  
  if (htmlElement == NULL) {
      error_number = MALLOC_FAILED;
      return NULL;
  }
  if (htmlTag == NULL){
      error_number = INPUT_NULL;
      return NULL;
  }
  
  // Initialize all variables with default values
  htmlElement->_htmlTag = htmlTag;
  
  htmlElement->text = NULL;
  htmlElement->id = NULL;
  htmlElement->class = NULL;
  
  htmlElement->_childrenCount = 0;
  htmlElement->_childrenSize = 2;
  
  // Initialize the pointer array to other HtmlElements
  htmlElement->_children =
      malloc(sizeof(HtmlElement**) * htmlElement->_childrenSize);
  
  if (htmlElement->_children == NULL) {
      error_number = MALLOC_FAILED;
      return NULL;
  }
  
  return htmlElement;
}

HtmlElement* addChild(HtmlElement* parent, HtmlElement** child) {
  if (parent == NULL || *child == NULL) {
      error_number = INPUT_NULL;
    return NULL;
  }
  
  //JEIGU UZPILDYTAS MASYVAS, REALOKUOJAMA DAR VIETOS (NAUJOS_DYDIS = PRAEITOS_DYDIS * 2)
  if (parent->_childrenCount >= parent->_childrenSize) {
    parent->_childrenSize *= 2;
    parent->_children = realloc(parent->_children, sizeof(HtmlElement**) * parent->_childrenSize);

    //JEIGU IVYKO KLAIDA
    if (parent->_children == NULL) {
        error_number = REALLOC_FAILED;
      //NEPAVYKO REALOKUOTI NAUJOS VIETOS, DYDIS PALIEKAMAS KOKS BUVO
      parent->_childrenSize /= 2;
      return NULL;
    }
  }  
  
  // GALIMA DABAR PRIDETI NAUJA ELEMENTA
  *(parent->_children + parent->_childrenCount) = child;
  parent->_childrenCount += 1;

  return *child;
}

void _freeHtmlElement(HtmlElement **htmlElement) {
  // sitas if tiesiog, kad jeigu NULL net nebelistu i for loopa
  if (*htmlElement == NULL) {
    return;
  }
  
  for (int i = 0; i < (*htmlElement)->_childrenCount; i++) {
    HtmlElement** child = *((*htmlElement)->_children + i);
    
    _freeHtmlElement(child);
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
HtmlPage* initHtmlPage(char* fileName) {
  HtmlPage* htmlPage = malloc(sizeof(HtmlPage));

  if (htmlPage == NULL) {
      error_number = MALLOC_FAILED;
    return NULL;
  }
  
  FILE* htmlFile;

  htmlFile = fopen(fileName, "w");

  if(htmlFile == NULL){
      error_number = FOPEN_FAILED;
    return NULL;
  }

  htmlPage->_htmlFile = htmlFile;

  //SUKURIA <HEAD> HTML ELEMENTA
  HtmlElement* htmlHead = initHtmlElement("head");
  if (htmlHead == NULL) {
      error_number = error_number;
    return NULL;
  }
  
  htmlPage->_htmlHead = htmlHead;

  //SUKURIA <BODY> HTML ELEMENTA
  HtmlElement* htmlBody = initHtmlElement("body");
  if (htmlBody == NULL) {
      error_number = error_number;
    return NULL;
  }

  htmlPage->_htmlBody = htmlBody;

  htmlPage->cssFileName = NULL;

  return htmlPage;
}

HtmlElement* addBodyElement(HtmlPage* htmlPage, HtmlElement** htmlElement) {
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

HtmlElement* addHeadElement(HtmlPage* htmlPage, HtmlElement** htmlElement) {
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

void _writeHtmlElement(HtmlPage* htmlPage, HtmlElement* htmlElement, unsigned short depth) {
  FILE* htmlFile = htmlPage->_htmlFile;
  
  // tarpu pridejimas
  for (int i = 0; i < depth; i++) {
    fprintf(htmlFile, "  ");
  }

  // elemento html tago atspausdinimas
  fprintf(htmlFile, "<%s", htmlElement->_htmlTag);

  // papildomi attributes, kurie eina i tago vidu (id, class)
  if (htmlElement->id != NULL) {
    fprintf(htmlFile, " id=\"%s\"", htmlElement->id);
  }

  if (htmlElement->id != NULL) {
    fprintf(htmlFile, " class=\"%s\"", htmlElement->class);
  }

  fprintf(htmlFile, ">");

  //CIA DALYKAI AKTUALUS TIK <HEAD> ELEMENTUI
  if (htmlPage->_htmlHead == htmlElement) {
    
    if (htmlPage->cssFileName != NULL) {
      fprintf(htmlFile, "\n  <link rel=\"stylesheet\" href=\"%s\">", htmlPage->cssFileName);
    }
    
  }  

  // teksto, jei jis yra atspausdinimas
  if (htmlElement->text != NULL) {
    fprintf(htmlFile, "%s", htmlElement->text);
  }

  // cia jeigu html elmentas turi children, spausdinti kitokiu formatu ir dar jo vaikus
  if (htmlElement->_childrenCount != 0) {
      fprintf(htmlFile, "\n");
  
      // elemento 'children' atspausdinimas
      for (int i = 0; i < htmlElement->_childrenCount; i++) {
	HtmlElement** child = *(htmlElement->_children + i);
    
	_writeHtmlElement(htmlPage, *child, depth+1);
      }

      // tarpu pridejimas
      for (int i = 0; i < depth; i++) {
	fprintf(htmlFile, "  ");
      }
  }

  // elemento html tag uzdarymas
  fprintf(htmlFile, "</%s>\n", htmlElement->_htmlTag);
}

void _freeHtmlPage(HtmlPage** htmlPage) {
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

void createHtmlPage(HtmlPage** htmlPage) {
  if (*htmlPage == NULL) {
      error_number = HTML_PAGE_NULL;
    return;
  }
  
  FILE *htmlFile = (*htmlPage)->_htmlFile;

  fprintf(htmlFile, "<!DOCTYPE html>\n");

  HtmlElement* htmlHead = (*htmlPage)->_htmlHead;

  // sukuria i html visa head
  _writeHtmlElement(*htmlPage, htmlHead, 0);
  
  HtmlElement *htmlBody = (*htmlPage)->_htmlBody;
  
  // sukuria i html visa body
  _writeHtmlElement(*htmlPage, htmlBody, 0);

  _freeHtmlPage(htmlPage);
}
