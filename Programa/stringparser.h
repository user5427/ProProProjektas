#ifndef STRINGPARSER_H
#define STRINGPARSER_H

typedef struct {
	unsigned char BOLD:1;
	unsigned char ITALIC:1;
	unsigned char UNDERLINE:1;
	unsigned char TITLE:1;
	unsigned char BULLETPOINT:1;
	unsigned char UNUSED:3;
	unsigned char SEGMENT_NUMBER:4;
	unsigned char TOTAL_SEGMENTS:4;
	unsigned char TEXTSIZE;
	unsigned char RED, GREEN, BLUE;
	char* FONT;
	int ID;
	char* Str;
} HTML;

typedef struct LIST{
	HTML data;
	struct LIST *next;
} LIST;

unsigned char toDec(char*);
void parseString(char*, LIST**);
LIST* init();
LIST* add(HTML, LIST**);
HTML pop(LIST**);
char* myConcat(char*, char*, int, int);
#endif
