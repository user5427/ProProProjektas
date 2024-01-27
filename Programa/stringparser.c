#include <string.h>
#include <stdlib.h>
#include "stringparser.h"
LIST* init(){
	return NULL;
}
static int IDCOUNTER = 0;

unsigned char toDec(char* hex){
    char first = *hex, second = *(hex + 1);
    unsigned char color = 0;

    if(first >= '0' && first <= '9'){
        first -= '0';
    }else if(first >= 'A' && first <= 'F'){
        first -= 'A';
        first += 10;
    }
    if(second >= '0' && second <= '9'){
        second -= '0';
    }else if(second >= 'A' && second <= 'F'){
        second -= 'A';
        second += 10;
    }
    color = first * 16 + second;
    return color;
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
void parseString(char* line, LIST** list){
	int lineLength = strlen(line);
	char first = *line;
	char *linecopy;
    char *optionlessKeys[3] = {"Bold", "Italic", "Underline"};
    char *Keys[3] = {"Color", "Font", "Size"};
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
        int repeat = 1;
        while(repeat){
            char* pos = line + 1;
            while(*pos != ',' || *pos != ']'){
                pos++;
            }
            if(*pos == ']'){
                repeat = 0;
            }
            int diff = pos - line - 1;
            char* key = calloc(diff + 1, sizeof(char));
            strncpy(key, line + 1, diff);
            // zinau kad cia ultra fugly bet whatever tingisi galvot pries diskreciaja
            if(!strcmp(key, optionlessKeys[0])){
                data.BOLD = 1;
            }
            if(!strcmp(key, optionlessKeys[1])){
                data.UNDERLINE = 1;
            }
            if(!strcmp(key, optionlessKeys[2])){
                data.ITALIC = 1;
            }
            if(!strncmp(key, Keys[0], 5)){//Color
                char *equals= line + 6;
                if(*equals != '='){
                    exit(-3);
                }
                equals++;
                //wack kodas
                char *color = calloc(diff - 5, sizeof(char));
                strncpy(color, equals, diff - 6);
                data.RED = toDec(color);
                data.GREEN = toDec(color + 2);
                data.BLUE = toDec(color + 4);
                free(color);
            }else if(!strncmp(key, Keys[1], 4)){//Font
                char *equals= line + 5;
                if(*equals != '='){
                    exit(-3);
                }
                equals++;
                char *font = calloc(diff - 4, sizeof(char));
                strncpy(font, equals, diff - 5);
                data.FONT = font;
            }else if(!strncmp(key, Keys[2], 4)){//size
                char *equals= line + 5;
                if(*equals != '='){
                    exit(-3);
                }
                equals++;
                char *size = calloc(diff - 4, sizeof(char));
                strncpy(size, equals, diff - 5);
                char *iter = size;
                unsigned char tsize = 0;
                while(*iter != '\0'){
                    tsize *= 10;
                    tsize += *iter - '0';
                    //reiketu error checkinimo bet whatever, del manes gali crashint jei kazkas nemoka skaiciaus parasyt
                }
                data.TEXTSIZE = tsize;
            }
            line = pos + 1;
        }
    }
    linecopy = calloc(lineLength + 1, sizeof(char));
    strcpy(linecopy, line);
	data.Str = linecopy;
    data.ID = IDCOUNTER;
    IDCOUNTER++;
	add(data, list);
}