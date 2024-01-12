#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stringparser.h"

static char status = 0; //0 - expected title, 1 - expected left side 2 - expected right side
static char right = 0;  //kurioj pusej rasyt
static char clDiv = 0;  //ar reikia uzdaryti buvusi div
LIST* init(){
	return NULL;
}

LIST* add(HTML data, LIST** LIS){
	LIST* l = calloc(1, sizeof(LIST));
	l->data = data;
	l->next = NULL;
    if(*LIS == NULL){
        (*LIS) = l;
        return l;
    }
	LIST* tmp = *LIS;
	while(tmp -> next != NULL){
		tmp = tmp -> next;
	}
	tmp -> next = l;
	return *LIS;
}

HTML pop(LIST** LIS){
	LIST* l = *LIS;
    if(l == NULL){
        exit(2);
    }
    HTML ret;
    ret = (*LIS) ->data;
    *LIS = l->next;
    free(l);
	return ret;
}
char* myConcat(char* start, char* end, int canFreeStart, int canFreeEnd){
    int len;
    int len1;
    if(start == NULL){
        len1 = 0;
    }else{
        len1 = strlen(start);
    }
    if(end == NULL){
        len = 0;
    }else{
        len = strlen(end);
    }
    //assume \0 term
    char *newString = calloc(len + len1 + 1, sizeof(char));
    int offset = 0;
    for(int i = 0; i < len1; i++){
        newString[offset] = start[i];
        offset++;
    }
    for(int i = 0; i < len; i++){
        newString[offset] = end[i];
        offset++;
    }
    newString[offset] = '\0';
    if(start != NULL && canFreeStart){
        free(start);
    }
    if(end != NULL && canFreeEnd){
        free(end);
    }
    return newString;
}
void parseString(char* line, LIST** list){
	int lineLength = strlen(line);
	char first = *line;
	char *linecopy;
	int offset = 0;
	HTML data = {0};
    data.BULLETPOINT = 1;

	if(first == '!'){
		if(*(line + 1) != '\n'){
			data.TITLE = 1;
            data.BULLETPOINT = 0;
            line++;
		}
	}else if(first == '@'){
        data.BULLETPOINT = 0;
        line++;
    }


    first = *(line);
    if(first == '['){
        //kolkas ignoruosiu tuos Bold, Italic, etc. nzn ar isvis juos intepretuot tbh
        while(*(line + offset) != '['){
            offset++;
        }
        line+=offset;
        line++;

    }
    linecopy = calloc(lineLength + 1, sizeof(char));
    strcpy(linecopy, line);
	data.Str = linecopy;

	add(data, list);
}

char* dumpHTML(LIST* list){
    char *rez = NULL;
    while(list != NULL) {
        HTML data = pop(&list);
        int runNext = 1;
        if (status == 0 && runNext) {
            rez = myConcat(rez, "<header><h1 id=\"name\">", 1, 0);
            rez = myConcat(rez, data.Str, 1, 0);
            rez = myConcat(rez, "</h1></header>", 1, 0);
            rez = myConcat(rez, "<div id=\"content-block\"><div id=\"left-block\">", 1, 0);
            status++;
            runNext = 0;
        }
        if(*data.Str == '!'){
            if(status == 2){
                rez = myConcat(rez, "</ul>", 1, 0);
                status = 1;
            }
            rez = myConcat(rez, "</div>", 1, 0);
            if(clDiv){
                rez = myConcat(rez, "</div>", 1, 0);
            }
            rez = myConcat(rez, "<div id=\"right-block\">", 1, 0);
            runNext = 0;
            clDiv = 0;
            right = 1;
        }
        if(data.BULLETPOINT && runNext && status != 2){
            rez = myConcat(rez, "<ul>", 1, 0);
            status = 2;
            rez = myConcat(rez, "<li>", 1, 0);
            rez = myConcat(rez, data.Str, 1, 1);
            rez = myConcat(rez, "</li>", 1, 0);
            runNext = 0;
        }
        if(!data.BULLETPOINT && runNext && status == 2){
            rez = myConcat(rez, "</ul>", 1, 0);
            status = 1;
        }
        if(data.BULLETPOINT && runNext && status == 2){
            rez = myConcat(rez, "<li>", 1, 0);
            rez = myConcat(rez, data.Str, 1, 1);
            rez = myConcat(rez, "</li>", 1, 0);
            runNext = 0;
        }
        if(data.TITLE && runNext){
            if(clDiv == 1){
                rez = myConcat(rez, "</div>", 1, 0);
            }
            if(right == 1){
                rez = myConcat(rez, "<div class=\"right-block-item\">", 1, 0);
            }else{
                rez = myConcat(rez, "<div class=\"left-block-item\">", 1, 0);
            }
            clDiv = 1;
            rez = myConcat(rez, "<h3 class=\"content-title\">", 1, 0);
            rez = myConcat(rez, data.Str, 1, 1);
            rez = myConcat(rez, "</h3>", 1, 0);
            runNext = 0;
        }
        if(runNext){
            rez = myConcat(rez, "<p>", 1, 0);
            rez = myConcat(rez, data.Str, 1, 1);
            rez = myConcat(rez, "</p>", 1, 0);
            runNext = 0;
        }
    }
    if(status == 2){
        rez = myConcat(rez, "</ul>", 1, 0);
    }
    if(clDiv){
        rez = myConcat(rez, "</div>", 1, 0);
    }
    rez = myConcat(rez, "</div>", 1, 0);
    rez = myConcat(rez, "</div>", 1, 0);
    return rez;
}