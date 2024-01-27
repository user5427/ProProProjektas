#include <stdio.h>
#include "Functions/stringparser.h"
#include "Functions/getline.h"
#include "Functions/htmlpage.h"


void perduoti(char* output_file, LIST* data_list){
    HtmlPage *htmlpage = initHtmlPage(output_file);

    char list_status = 0;
    while(data_list != NULL){

        //if (data_list->data.TITLE)

        //if (data_list->data.BOLD)
        //    addStyle(htmlElement, "strong", 0);
        //printf("\nBOLD: %d", data_list->data.BOLD);
        //printf("\nITALIC: %d", data_list->data.ITALIC);
        //printf("\nUNDERLINE: %d", data_list->data.UNDERLINE);
        //printf("\nTITLE: %d", data_list->data.TITLE);
        //printf("\nBULLETPOINT: %d", data_list->data.BULLETPOINT);
        //printf("\nUNUSED: %d", data_list->data.UNUSED);
        //printf("\nSEGMENT_NUMBER: %d", data_list->data.SEGMENT_NUMBER);
        //printf("\nTOTAL_SEGMENTS: %d", data_list->data.TOTAL_SEGMENTS);
        //printf("\nTEXTSIZE: %d", data_list->data.TEXTSIZE);
        //printf("\nFONT: %s", data_list->data.FONT);
        //printf("\nID: %d", data_list->data.ID);
        printf("\nStr: %s", data_list->data.Str);

        data_list = data_list->next;

    }
    HtmlElement *htmlElement = initHtmlElement("p");
    htmlElement->text = "HALLO";
    addBodyElement(htmlpage, &htmlElement);
    htmlElement = NULL;
    htmlElement = initHtmlElement("p");
    htmlElement->text = "HELLO";
    addBodyElement(htmlpage, &htmlElement);

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
