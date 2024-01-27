//
// Created on 2023-12-23.
// TODO comment what each function does here

#ifndef PROPROPRO_MODULIS_HTMLPAGE_H
#define PROPROPRO_MODULIS_HTMLPAGE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern int error_number;

typedef struct HtmlElement {
    //PUBLIC KINTAMIEJI:
    char *text;
    char *id;
    char *class;

    //PRIVATE KINTAMIEJI: (NETUREU BUTI NAUDOJAMI)
    char *_htmlTag; //Html tago pavadinimas
    char *_style;
    unsigned int _styleSize;
    unsigned int _styleCount;


    struct HtmlElement ***_children; // An array of double pointers to Html element
    unsigned int _childrenCount; // Pointer array capacity
    unsigned int _childrenSize; // Pointer array current size

    //CLASS, ID dar prideti reikes

} HtmlElement;

HtmlElement *initHtmlElement(char *htmlTag);
HtmlElement *addChild(HtmlElement *parent, HtmlElement **child);
void _freeHtmlElement(HtmlElement **htmlElement);

typedef struct {
    //PUBLIC KINTAMIEJI:
    char *cssFileName;

    //PRIVATE KINTAMIEJI:
    FILE *_htmlFile;

    HtmlElement *_htmlHead;
    HtmlElement *_htmlBody;

} HtmlPage;

HtmlPage *initHtmlPage(char *fileName);
HtmlElement *addBodyElement(HtmlPage *htmlPage, HtmlElement **htmlElement);
HtmlElement *addHeadElement(HtmlPage *htmlPage, HtmlElement **htmlElement);

//void _writeHtmlElement(HtmlPage* htmlPage, HtmlElement* htmlElement, unsigned short depth); //TODO ar reikia vartotojui sitos funkcijos? no, the user is the developer btw
//void _freeHtmlPage(HtmlPage** htmlPage); probably aint needed for developer, right?
void createHtmlPage(HtmlPage **htmlPage);

#endif //PROPROPRO_MODULIS_HTMLPAGE_H
