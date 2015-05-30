/*\
 * tg-config.h
 *
 * Copyright (c) 2014 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#ifndef _TABCONFIG_H_
#define _TABCONFIG_H_
#include <tidy.h>

extern Bool openTidyLib();  // init library
extern void closeTidyLib(); // close libray
extern void runTidyLib( const char *file );


extern bool getConfigBool( const char *item );
extern int getConfigInt( const char *item );
extern const char *getConfigStg( const char *item );
extern const char *getConfigEnc( const char *item );
extern const char *getConfigEnum( const char *item );
extern const char *getConfigPick( const char *item );
extern TidyTriState getConfigABool( const char *item );

extern Bool setConfigEnc( const char *item, const char *val );   // set encoding
extern Bool setConfigEnum( const char *item, const char *val );  // set enum and doctype
extern Bool setConfigBool( const char *item, Bool val );
extern Bool setConfigABool( const char *item, Bool val );
extern Bool setConfigStg( const char *item, const char *stg );
extern Bool setConfigInt( const char *item, int val );


extern int showConfig();


#endif // #ifndef _TABCONFIG_H_
// eof - tabconfig.h
