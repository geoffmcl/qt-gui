/*\
 * tabconfig.cpp
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <stdio.h>
#include <string.h>
#include "tabconfig.h"

static const char *module = "tabconfig";

// implementation
bool getConfigBool( const char *item )
{
    return false;
}

int getConfigInt( const char *item )
{
    if (strcmp(item,"indent-spaces") == 0) {
        return 2;
    } else if (strcmp(item,"wrap") == 0) {
        return 68;
    } else if (strcmp(item,"tab-size") == 0) {
        return 8;
    } else if (strcmp(item,"show-errors") == 0) {
        return 6;
    } 
    return 0;
}


const char *getConfigStg( const char *item )
{

    return "";
}

const char *getConfigEnc( const char *item )
{
    if (strcmp(item,"char-encoding") == 0) {
        return "utf8";
    } else if (strcmp(item,"input-encoding") == 0) {
        return "utf8";
    } else if (strcmp(item,"output-encoding") == 0) {
        return "utf8";
    }
    return "";
}





// eof = tabconfig.cpp
