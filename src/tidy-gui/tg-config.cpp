/*\
 * tg-config.cpp
 *
 * Copyright (c) 2015 - Geoff R. McLane
 * Licence: GNU GPL version 2
 *
\*/

#include <QtGlobal> 
#include <stdio.h>
#include <string.h>
#include <tidy.h>
#include <buffio.h>
#include "tg-config.h"
/*\
 * This module deals with all thing Tidy
 *
 * INIT and CLOSE library
 * GET and SET config items as they change
 * 
 * Setting the IO
 * Initialize a buffer input source
 * TIDY_EXPORT void TIDY_CALL tidyInitInputBuffer( TidyInputSource* inp, TidyBuffer* buf );
 * Initialize a buffer output sink 
 * TIDY_EXPORT void TIDY_CALL tidyInitOutputBuffer( TidyOutputSink* outp, TidyBuffer* buf );
 *
 *
 *
\*/

static const char *module = "tg-config";

static TidyDoc tdoc = 0;    // tidyCreate();
static TidyBuffer output;
static TidyBuffer errbuf;

Bool openTidyLib()
{
    Bool done = no;
    if (tdoc == 0) {
        tdoc = tidyCreate();
        tidyBufInit( &output );
        tidyBufInit( &errbuf );
        if (tidySetErrorBuffer( tdoc, &errbuf ) >= 0) {    // Capture diagnostics
            done = yes;
        }
    }
    return done;
}

void closeTidyLib()
{
    if (tdoc)  { /* called to free hash tables etc. */
        tidyBufFree( &output );
        tidyBufFree( &errbuf );
        tidyRelease( tdoc );
    }
    tdoc = 0;
}

TidyOptionId getTidyOptionId( const char *item )
{
    return tidyOptGetIdForName(item);
}

// implementation
bool getConfigBool( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
        return tidyOptGetBool(tdoc, id);
    return false;
}

Bool setConfigBool( const char *item, Bool val )
{
    Bool done = no;
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS) {
        Bool curr = tidyOptGetBool(tdoc, id);
        if (curr != val) {
            done = tidyOptSetBool(tdoc, id, val);
        }
    }
    return done;
}


// TODO: CHECK AutoBool to triSate mappings
// Tidy AutoBool has no(0), yes(1), auto(2)
// Qt4  triState has no(0), yes(2), part(1)
TidyTriState getConfigABool( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
        return (TidyTriState)tidyOptGetInt(tdoc,id);
        // return cfgAutoBool(tdoc, id);
    return TidyAutoState;
}

Bool setConfigABool( const char *item, Bool val )
{
    Bool done = no;
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS) {
        Bool curr = (Bool)tidyOptGetInt(tdoc,id);
        if (curr != val) {
            done = tidyOptSetInt(tdoc,id,val);
        }
    }
    return done;
}


int getConfigInt( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
         return tidyOptGetInt(tdoc,id);

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

Bool setConfigInt( const char *item, int val )
{
    Bool done = no;
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS) {
        int curr = tidyOptGetInt(tdoc,id);
        if (curr != val) {
            done = tidyOptSetInt(tdoc,id,val);
        }
    }
    return done;
}

const char *getConfigStg( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
         return tidyOptGetValue(tdoc,id);
    return "";
}

Bool setConfigStg( const char *item, const char *stg )
{
    Bool done = no;
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS) {
        const char *curr = tidyOptGetValue(tdoc,id);
        if (!curr || strcmp(curr,stg)) {
            done = tidyOptSetValue(tdoc,id,stg);
        }
    }
    return done;
}


const char *getConfigEnc( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
         return tidyOptGetEncName(tdoc,id);
    if (strcmp(item,"char-encoding") == 0) {
        return "utf8";
    } else if (strcmp(item,"input-encoding") == 0) {
        return "utf8";
    } else if (strcmp(item,"output-encoding") == 0) {
        return "utf8";
    } else if (strcmp(item,"newline") == 0) {
#ifdef Q_WS_WIN
        return "CRLF";
#else
#ifdef  Q_WS_MAC
        return "LF";
#else
        return "CR";
#endif
#endif
    }
    return "";
}

Bool setConfigEnc( const char *item, const char *val )
{
    Bool done = no;
    const char *curr = 0;
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS) {
         curr = tidyOptGetEncName(tdoc,id);
         if (strcmp(curr,item)) {
             done = tidyOptSetValue(tdoc,id,val);
         }
    }
    return done;
}

const char *getConfigEnum( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
         return tidyOptGetCurrPick(tdoc,id);
    return "";
}

Bool setConfigEnum( const char *item, const char *val )
{
    Bool done = no;
    const char *curr = 0;
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS) {
         curr = tidyOptGetCurrPick(tdoc,id);
         if (strcmp(curr,item)) {
             done = tidyOptSetValue(tdoc,id,val);
         }
    }
    return done;
}

const char *getConfigPick( const char *item )
{
    TidyOptionId id = getTidyOptionId(item);
    if (id < N_TIDY_OPTIONS)
         return tidyOptGetCurrPick(tdoc,id);
    return "";
}

typedef struct tagSINKDATA {
    int context;
}SINKDATA, PSINKDATA;

// TIDY_EXPORT int TIDY_CALL         tidyOptSaveSink( TidyDoc tdoc, TidyOutputSink* sink );
static TidyOutputSink sink;
static SINKDATA sinkdata;
static void TIDY_CALL putByteFunc(void* sinkData, byte bt )
{
    // do something with the byte
    if (sinkData && bt) {
        printf("%c",bt);
    }
}

int showConfig()
{
    int iret = 1;
    sinkdata.context = 1;
    printf("Display of configuration items not equal default...\n");
    if (tidyInitSink( &sink, &sinkdata, &putByteFunc )) {
        iret = tidyOptSaveSink(tdoc, &sink);
    }
    return iret;
}

// eof = tg-conifg.cpp
