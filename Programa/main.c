#include <stdio.h>
#include "stringparser.h"
int main(int argc, char** argv){
	LIST* l = init();
	if(argc == 1 || argc > 3){
		printf("Klaidinga įvestis, įveskite duomenų failo pavadinima ir, jei norite pageidaujamą išvesties failo pavadinimą\n");
	    return 1;
    }
	FILE *input = fopen(argv[1], "r");
	FILE *output = NULL;
	if(argc == 3){
		output = fopen(argv[2], "w");
	}else{
		output = fopen("Output.html", "w");
	}
	if(input == NULL){//output nebus NULL tai nzn gal veliau patikrinsiu ar pasibaigia .html or smthn xdd
		printf("Nu blin...");
        return 1;
	}
	long unsigned len = 0;
	char* line = NULL;
	int read;
	while((read = getline(&line, &len, input)) != -1){
		parseString(line, &l);
	}
	fputs("</!DOCTYPE html><html><head><meta charset=\"utf-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"><link rel=\"stylesheet\" href=\"style.css\"><title>Portfolio</title></head><body>", output);
	fputs(dumpHTML(l), output);
	fputs("</body></html>", output);
	return 0;
}
