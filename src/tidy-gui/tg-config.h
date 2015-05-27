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

extern bool openTidyLib();  // init library
extern void closeTidyLib(); // close libray

extern bool getConfigBool( const char *item );
extern int getConfigInt( const char *item );
extern const char *getConfigStg( const char *item );
extern const char *getConfigEnc( const char *item );
extern const char *getConfigEnum( const char *item );
extern const char *getConfigPick( const char *item );
extern TidyTriState getConfigABool( const char *item );


#endif // #ifndef _TABCONFIG_H_
// eof - tabconfig.h
