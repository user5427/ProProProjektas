#ifndef STRINGPARSER_H
#define STRINGPARSER_H

typedef struct {
	unsigned char BOLD:1;
	unsigned char ITALIC:1;
	unsigned char UNDERLINE:1;
	unsigned char TITLE:1;
	unsigned char BULLETPOINT:1;
	unsigned char UNUSED:3;
	unsigned char TEXTSIZE;
	unsigned char RED, GREEN, BLUE;
	char* FONT;
	char* ID;
	char* Str;
} HTML;

typedef struct LIST{
	HTML data;
	struct LIST *next;
} LIST;

void parseString(char*, LIST**);
LIST* init();
LIST* add(HTML, LIST**);
HTML pop(LIST**);
char* dumpHTML(LIST*);
char* myConcat(char*, char*, int, int);
#endif