#include <stdio.h>
#include "Functions/stringparser.h"
#include "Functions/getline.h"
#include "Functions/htmlpage.h"


void perduoti(char* output_file, LIST* data_list){
    HtmlPage *htmlpage = initHtmlPage(output_file);

    char list_status = 0;
    HtmlElement *list;
    while(data_list != NULL){
        printf("\nTITLE: %d", data_list->data.TITLE);


        HtmlElement *htmlElement = initHtmlElement("p");
        if (!data_list->data.BULLETPOINT){
            htmlElement = initHtmlElement("p");
        }
        else {
            htmlElement = initHtmlElement("li");
        }
        if (list_status == 0 && data_list->data.BULLETPOINT == 1){
            list_status = 1;
            list = initHtmlElement("ul");
        }
        else if (list_status == 1 && data_list->data.BULLETPOINT == 0){
            list_status = 0;
            addBodyElement(htmlpage, &list);
        }


        HtmlElement *modifiers = NULL;
        if (data_list->data.BOLD){
            HtmlElement *bald = initHtmlElement("strong");
            bald->text = data_list->data.Str;
            modifiers = bald;
        }
        if (data_list->data.ITALIC){
            HtmlElement *italics = initHtmlElement("i");
            if (modifiers == NULL){
                italics->text = data_list->data.Str;
                modifiers = italics;
            } else {
                addChild(italics, &modifiers);
                modifiers = italics;
            }
        }
        if (data_list->data.UNDERLINE){
            HtmlElement *underline = initHtmlElement("u");
            if (modifiers == NULL){
                underline->text = data_list->data.Str;
                modifiers = underline;
            } else {
                addChild(underline, &modifiers);
                modifiers = underline;
            }
        }
        if (data_list->data.TITLE){
            HtmlElement *title = initHtmlElement("h1");
            if (modifiers == NULL){
                title->text = data_list->data.Str;
                modifiers = title;
            } else {
                addChild(title, &modifiers);
                modifiers = title;
            }
        }

        if (modifiers == NULL) {
            htmlElement->text = data_list->data.Str;
        } else {
            addChild(htmlElement, &modifiers);
        }

        if (!data_list->data.BULLETPOINT){
            addBodyElement(htmlpage, &htmlElement);
        } else {
            addChild(list, &htmlElement);
        }
        data_list = data_list->next;
    }

    if (list_status)
        addBodyElement(htmlpage, &list);

    createHtmlPage(&htmlpage);
}
//

int main(int argc, char** argv){
    /*
    printf("\nargc %d: ", argc);
    printf("\nargv[0]: %s", argv[0]);
    printf("\nargv[1]: %s", argv[1]);
    printf("\nargv[2]: %s", argv[2]);
     */
    LIST* l = init();
	if(argc == 1 || argc > 3){
		printf("\nKlaidinga įvestis, įveskite duomenų failo pavadinima ir, jei norite pageidaujamą išvesties failo pavadinimą\n");
	    return 1;
    }
	FILE *input = fopen(argv[1], "r");
	char *output = NULL;
	if(argc == 3){
		output = argv[2];
	}else{
		output = "Output.html";
	}
	if(input == NULL){//output nebus NULL tai nzn gal veliau patikrinsiu ar pasibaigia .html or smthn xdd
		printf("\nFAILED TO OPEN INPUT FILE!");
        return 1;
	}
	long unsigned len = 0;
	char* line = NULL;
	int read;
	while((read = getline(&line, &len, input)) != -1){
		parseString(line, &l);
	}
	perduoti(output, l);
	return 0;
}
