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
#include "tg-config.h"

static const char *module = "tg-config";

static TidyDoc tdoc = 0;    // tidyCreate();

#if 0 // 000000000000000000000000000000000000000000000

struct _TidyDocImpl;
typedef struct _TidyDocImpl TidyDocImpl;

typedef struct _tidy_option TidyOptionImpl;

typedef Bool (ParseProperty)( TidyDocImpl* doc, const TidyOptionImpl* opt );

struct _tidy_option
{
    TidyOptionId        id;
    TidyConfigCategory  category;   /* put 'em in groups */
    ctmbstr             name;       /* property name */
    TidyOptionType      type;       /* string, int or bool */
    ulong               dflt;       /* default for TidyInteger and TidyBoolean */
    ParseProperty*      parser;     /* parsing method, read-only if NULL */
    const ctmbstr*      pickList;   /* pick list */
    ctmbstr             pdflt;      /* default for TidyString */
};

/* Arrange so index can be cast to enum
*/
static const ctmbstr boolPicks[] = 
{
  "no",
  "yes",
  NULL
};

static const ctmbstr autoBoolPicks[] = 
{
  "no",
  "yes",
  "auto",
  NULL
};

static const ctmbstr repeatAttrPicks[] = 
{
  "keep-first",
  "keep-last",
  NULL
};

static const ctmbstr accessPicks[] = 
{
  "0 (Tidy Classic)",
  "1 (Priority 1 Checks)",
  "2 (Priority 2 Checks)",
  "3 (Priority 3 Checks)",
  NULL
};

static const ctmbstr charEncPicks[] = 
{
  "raw",
  "ascii",
  "latin0",
  "latin1",
  "utf8",
#ifndef NO_NATIVE_ISO2022_SUPPORT
  "iso2022",
#endif
  "mac",
  "win1252",
  "ibm858",

#if SUPPORT_UTF16_ENCODINGS
  "utf16le",
  "utf16be",
  "utf16",
#endif

#if SUPPORT_ASIAN_ENCODINGS
  "big5",
  "shiftjis",
#endif

  NULL
};

static const ctmbstr newlinePicks[] = 
{
  "LF",
  "CRLF",
  "CR",
  NULL
};

static const ctmbstr doctypePicks[] = 
{
  "html5",
  "omit",
  "auto",
  "strict",
  "transitional",
  "user",
  NULL 
};

static const ctmbstr sorterPicks[] = 
{
  "none",
  "alpha",
  NULL
};

#define MU TidyMarkup
#define DG TidyDiagnostics
#define PP TidyPrettyPrint
#define CE TidyEncoding
#define MS TidyMiscellaneous

#define IN TidyInteger
#define BL TidyBoolean
#define ST TidyString

#define XX (TidyConfigCategory)-1
#define XY (TidyOptionType)-1

/** TidyNewline option values to control output line endings.
*/
//typedef enum {
//    TidyLF,         /**< Use Unix style: LF */
//    TidyCRLF,       /**< Use DOS/Windows style: CR+LF */
//    TidyCR          /**< Use Macintosh style: CR */
//} TidyLineEnding;

#if   defined(MAC_OS_CLASSIC)
#define DEFAULT_NL_CONFIG TidyCR
#elif defined(_WIN32) || defined(OS2_OS)
#define DEFAULT_NL_CONFIG TidyCRLF
#else
#define DEFAULT_NL_CONFIG TidyLF
#endif

#define DLF DEFAULT_NL_CONFIG

/* character encodings
*/
#define RAW         0
#define ASCII       1
#define LATIN0      2
#define LATIN1      3
#define UTF8        4
#define ISO2022     5
#define MACROMAN    6
#define WIN1252     7
#define IBM858      8

#if SUPPORT_UTF16_ENCODINGS
#define UTF16LE     9
#define UTF16BE     10
#define UTF16       11
#endif

/* Note that Big5 and SHIFTJIS are not converted to ISO 10646 codepoints
** (i.e., to Unicode) before being recoded into UTF-8. This may be
** confusing: usually UTF-8 implies ISO10646 codepoints.
*/
#if SUPPORT_ASIAN_ENCODINGS
#if SUPPORT_UTF16_ENCODINGS
#define BIG5        12
#define SHIFTJIS    13
#else
#define BIG5        9
#define SHIFTJIS    10
#endif
#endif

#ifdef TIDY_WIN32_MLANG_SUPPORT
/* hack: windows code page numbers start at 37 */
#define WIN32MLANG  36
#endif

/* If Accessibility checks not supported, make config setting read-only */
#if SUPPORT_ACCESSIBILITY_CHECKS
#define ParseAcc ParseInt
#else
#define ParseAcc NULL 
#endif

//#if 0 // 000000000000000000000000000000000000000000000
/* parser for integer values */
static ParseProperty ParseInt;
/* parser for 't'/'f', 'true'/'false', 'y'/'n', 'yes'/'no' or '1'/'0' */
static ParseProperty ParseBool;
/* parser for 't'/'f', 'true'/'false', 'y'/'n', 'yes'/'no', '1'/'0'
   or 'auto' */
static ParseProperty ParseAutoBool;
/* a string excluding whitespace */
static ParseProperty ParseName;
/* a CSS1 selector - CSS class naming for -clean option */
static ParseProperty ParseCSS1Selector;
/* a string including whitespace */
static ParseProperty ParseString;
/* a space or comma separated list of tag names */
static ParseProperty ParseTagNames;
/* alpha */
static ParseProperty ParseSorter;
/* RAW, ASCII, LATIN0, LATIN1, UTF8, ISO2022, MACROMAN, 
   WIN1252, IBM858, UTF16LE, UTF16BE, UTF16, BIG5, SHIFTJIS
*/
static ParseProperty ParseCharEnc;
static ParseProperty ParseNewline;
/* html5 | omit | auto | strict | loose | <fpi> */
static ParseProperty ParseDocType;
/* keep-first or keep-last? */
static ParseProperty ParseRepeatAttr;
/*\
 * 20150515 - support using tabs instead of spaces - Issue #108
 * (a) parser for 't'/'f', 'true'/'false', 'y'/'n', 'yes'/'no' or '1'/'0' 
 * (b) sets the TidyIndentSpaces to 1 if 'yes'
 * (c) sets the indent_char to '\t' or ' '
\*/
static ParseProperty ParseTabs;
// #endif // 00000000000000000000000000000000000000000000

#define ParseCSS1Selector NULL
#define ParseString NULL
#define ParseTabs NULL
#define ParseRepeatAttr NULL
#define ParseCharEnc NULL
#define ParseNewline NULL
#define ParseSorter NULL
#define ParseName NULL
#define ParseTagNames NULL
#define ParseInt NULL
#define ParseBool NULL
#define ParseDocType NULL
#define ParseAutoBool NULL
static const TidyOptionImpl option_defs[] =
{
  { TidyUnknownOption,           MS, "unknown!",                    IN, 0,               NULL,              NULL            },
  { TidyIndentSpaces,            PP, "indent-spaces",               IN, 2,               ParseInt,          NULL            },
  { TidyWrapLen,                 PP, "wrap",                        IN, 68,              ParseInt,          NULL            },
  { TidyTabSize,                 PP, "tab-size",                    IN, 8,               ParseInt,          NULL            },
  { TidyCharEncoding,            CE, "char-encoding",               IN, UTF8,            ParseCharEnc,      charEncPicks    },
  { TidyInCharEncoding,          CE, "input-encoding",              IN, UTF8,            ParseCharEnc,      charEncPicks    },
  { TidyOutCharEncoding,         CE, "output-encoding",             IN, UTF8,            ParseCharEnc,      charEncPicks    },
  { TidyNewline,                 CE, "newline",                     IN, DLF,             ParseNewline,      newlinePicks    },
  { TidyDoctypeMode,             MU, "doctype-mode",                IN, TidyDoctypeAuto, NULL,              doctypePicks    },
  { TidyDoctype,                 MU, "doctype",                     ST, 0,               ParseDocType,      doctypePicks    },
  { TidyDuplicateAttrs,          MU, "repeated-attributes",         IN, TidyKeepLast,    ParseRepeatAttr,   repeatAttrPicks },
  { TidyAltText,                 MU, "alt-text",                    ST, 0,               ParseString,       NULL            },

  /* obsolete */
  { TidySlideStyle,              MS, "slide-style",                 ST, 0,               ParseName,         NULL            },

  { TidyErrFile,                 MS, "error-file",                  ST, 0,               ParseString,       NULL            },
  { TidyOutFile,                 MS, "output-file",                 ST, 0,               ParseString,       NULL            },
  { TidyWriteBack,               MS, "write-back",                  BL, no,              ParseBool,         boolPicks       },
  { TidyShowMarkup,              PP, "markup",                      BL, yes,             ParseBool,         boolPicks       },
  { TidyShowInfo,                DG, "show-info",                   BL, yes,             ParseBool,         boolPicks       },
  { TidyShowWarnings,            DG, "show-warnings",               BL, yes,             ParseBool,         boolPicks       },
  { TidyQuiet,                   MS, "quiet",                       BL, no,              ParseBool,         boolPicks       },
  { TidyIndentContent,           PP, "indent",                      IN, TidyNoState,     ParseAutoBool,     autoBoolPicks   },
  { TidyCoerceEndTags,           MU, "coerce-endtags",              BL, yes,             ParseBool,         boolPicks       },
  { TidyOmitOptionalTags,        MU, "omit-optional-tags",          BL, no,              ParseBool,         boolPicks       },
  { TidyHideEndTags,             MU, "hide-endtags",                BL, no,              ParseBool,         boolPicks       },
  { TidyXmlTags,                 MU, "input-xml",                   BL, no,              ParseBool,         boolPicks       },
  { TidyXmlOut,                  MU, "output-xml",                  BL, no,              ParseBool,         boolPicks       },
  { TidyXhtmlOut,                MU, "output-xhtml",                BL, no,              ParseBool,         boolPicks       },
  { TidyHtmlOut,                 MU, "output-html",                 BL, no,              ParseBool,         boolPicks       },
  { TidyXmlDecl,                 MU, "add-xml-decl",                BL, no,              ParseBool,         boolPicks       },
  { TidyUpperCaseTags,           MU, "uppercase-tags",              BL, no,              ParseBool,         boolPicks       },
  { TidyUpperCaseAttrs,          MU, "uppercase-attributes",        BL, no,              ParseBool,         boolPicks       },
  { TidyMakeBare,                MU, "bare",                        BL, no,              ParseBool,         boolPicks       },
  { TidyMakeClean,               MU, "clean",                       BL, no,              ParseBool,         boolPicks       },
  { TidyGDocClean,               MU, "gdoc",                        BL, no,              ParseBool,         boolPicks       },
  { TidyLogicalEmphasis,         MU, "logical-emphasis",            BL, no,              ParseBool,         boolPicks       },
  { TidyDropPropAttrs,           MU, "drop-proprietary-attributes", BL, no,              ParseBool,         boolPicks       },
  { TidyDropFontTags,            MU, "drop-font-tags",              BL, no,              ParseBool,         boolPicks       },
  { TidyDropEmptyElems,          MU, "drop-empty-elements",         BL, yes,             ParseBool,         boolPicks       },
  { TidyDropEmptyParas,          MU, "drop-empty-paras",            BL, yes,             ParseBool,         boolPicks       },
  { TidyFixComments,             MU, "fix-bad-comments",            BL, yes,             ParseBool,         boolPicks       },
  { TidyBreakBeforeBR,           PP, "break-before-br",             BL, no,              ParseBool,         boolPicks       },

  /* obsolete */
  { TidyBurstSlides,             PP, "split",                       BL, no,              ParseBool,         boolPicks       },

  { TidyNumEntities,             MU, "numeric-entities",            BL, no,              ParseBool,         boolPicks       },
  { TidyQuoteMarks,              MU, "quote-marks",                 BL, no,              ParseBool,         boolPicks       },
  { TidyQuoteNbsp,               MU, "quote-nbsp",                  BL, yes,             ParseBool,         boolPicks       },
  { TidyQuoteAmpersand,          MU, "quote-ampersand",             BL, yes,             ParseBool,         boolPicks       },
  { TidyWrapAttVals,             PP, "wrap-attributes",             BL, no,              ParseBool,         boolPicks       },
  { TidyWrapScriptlets,          PP, "wrap-script-literals",        BL, no,              ParseBool,         boolPicks       },
  { TidyWrapSection,             PP, "wrap-sections",               BL, yes,             ParseBool,         boolPicks       },
  { TidyWrapAsp,                 PP, "wrap-asp",                    BL, yes,             ParseBool,         boolPicks       },
  { TidyWrapJste,                PP, "wrap-jste",                   BL, yes,             ParseBool,         boolPicks       },
  { TidyWrapPhp,                 PP, "wrap-php",                    BL, yes,             ParseBool,         boolPicks       },
  { TidyFixBackslash,            MU, "fix-backslash",               BL, yes,             ParseBool,         boolPicks       },
  { TidyIndentAttributes,        PP, "indent-attributes",           BL, no,              ParseBool,         boolPicks       },
  { TidyXmlPIs,                  MU, "assume-xml-procins",          BL, no,              ParseBool,         boolPicks       },
  { TidyXmlSpace,                MU, "add-xml-space",               BL, no,              ParseBool,         boolPicks       },
  { TidyEncloseBodyText,         MU, "enclose-text",                BL, no,              ParseBool,         boolPicks       },
  { TidyEncloseBlockText,        MU, "enclose-block-text",          BL, no,              ParseBool,         boolPicks       },
  { TidyKeepFileTimes,           MS, "keep-time",                   BL, no,              ParseBool,         boolPicks       },
  { TidyWord2000,                MU, "word-2000",                   BL, no,              ParseBool,         boolPicks       },
  { TidyMark,                    MS, "tidy-mark",                   BL, yes,             ParseBool,         boolPicks       },
  { TidyEmacs,                   MS, "gnu-emacs",                   BL, no,              ParseBool,         boolPicks       },
  { TidyEmacsFile,               MS, "gnu-emacs-file",              ST, 0,               ParseString,       NULL            },
  { TidyLiteralAttribs,          MU, "literal-attributes",          BL, no,              ParseBool,         boolPicks       },
  { TidyBodyOnly,                MU, "show-body-only",              IN, no,              ParseAutoBool,     autoBoolPicks   },
  { TidyFixUri,                  MU, "fix-uri",                     BL, yes,             ParseBool,         boolPicks       },
  { TidyLowerLiterals,           MU, "lower-literals",              BL, yes,             ParseBool,         boolPicks       },
  { TidyHideComments,            MU, "hide-comments",               BL, no,              ParseBool,         boolPicks       },
  { TidyIndentCdata,             MU, "indent-cdata",                BL, no,              ParseBool,         boolPicks       },
  { TidyForceOutput,             MS, "force-output",                BL, no,              ParseBool,         boolPicks       },
  { TidyShowErrors,              DG, "show-errors",                 IN, 6,               ParseInt,          NULL            },
  { TidyAsciiChars,              CE, "ascii-chars",                 BL, no,              ParseBool,         boolPicks       },
  { TidyJoinClasses,             MU, "join-classes",                BL, no,              ParseBool,         boolPicks       },
  { TidyJoinStyles,              MU, "join-styles",                 BL, yes,             ParseBool,         boolPicks       },
  { TidyEscapeCdata,             MU, "escape-cdata",                BL, no,              ParseBool,         boolPicks       },
#if SUPPORT_ASIAN_ENCODINGS
  { TidyLanguage,                CE, "language",                    ST, 0,               ParseName,         NULL            },
  { TidyNCR,                     MU, "ncr",                         BL, yes,             ParseBool,         boolPicks       },
#endif
#if SUPPORT_UTF16_ENCODINGS
  { TidyOutputBOM,               CE, "output-bom",                  IN, TidyAutoState,   ParseAutoBool,     autoBoolPicks   },
#endif
  { TidyReplaceColor,            MU, "replace-color",               BL, no,              ParseBool,         boolPicks       },
  { TidyCSSPrefix,               MU, "css-prefix",                  ST, 0,               ParseCSS1Selector, NULL            },
  { TidyInlineTags,              MU, "new-inline-tags",             ST, 0,               ParseTagNames,     NULL            },
  { TidyBlockTags,               MU, "new-blocklevel-tags",         ST, 0,               ParseTagNames,     NULL            },
  { TidyEmptyTags,               MU, "new-empty-tags",              ST, 0,               ParseTagNames,     NULL            },
  { TidyPreTags,                 MU, "new-pre-tags",                ST, 0,               ParseTagNames,     NULL            },
  { TidyAccessibilityCheckLevel, DG, "accessibility-check",         IN, 0,               ParseAcc,          accessPicks     },
  { TidyVertSpace,               PP, "vertical-space",              BL, no,              ParseBool,         boolPicks       },
#if SUPPORT_ASIAN_ENCODINGS
  { TidyPunctWrap,               PP, "punctuation-wrap",            BL, no,              ParseBool,         boolPicks       },
#endif
  { TidyMergeEmphasis,           MU, "merge-emphasis",              BL, yes,             ParseBool,         boolPicks       },
  { TidyMergeDivs,               MU, "merge-divs",                  IN, TidyAutoState,   ParseAutoBool,     autoBoolPicks   },
  { TidyDecorateInferredUL,      MU, "decorate-inferred-ul",        BL, no,              ParseBool,         boolPicks       },
  { TidyPreserveEntities,        MU, "preserve-entities",           BL, no,              ParseBool,         boolPicks       },
  { TidySortAttributes,          PP, "sort-attributes",             IN, TidySortAttrNone,ParseSorter,       sorterPicks     },
  { TidyMergeSpans,              MU, "merge-spans",                 IN, TidyAutoState,   ParseAutoBool,     autoBoolPicks   },
  { TidyAnchorAsName,            MU, "anchor-as-name",              BL, yes,             ParseBool,         boolPicks       },
  { TidyPPrintTabs,              PP, "indent-with-tabs",            BL, no,              ParseTabs,         boolPicks       }, /* 20150515 - Issue #108 */
  { N_TIDY_OPTIONS,              XX, NULL,                          XY, 0,               NULL,              NULL            }
};

#endif // 00000000000000000000000000000000000000000000000000000000000000000000000000

bool openTidyLib()
{
    if (tdoc == 0)
        tdoc = tidyCreate();
    return true;
}

void closeTidyLib()
{
    if (tdoc)   /* called to free hash tables etc. */
        tidyRelease( tdoc );
    tdoc = 0;
}

#if 0 // 0000000000000000000000000000000000000000000
TidyOptionId getTidyOptionId2( const char *item )
{
    int i;
    for (i = 1; i < N_TIDY_OPTIONS; i++) {
        if (strcmp(item, option_defs[i].name) == 0) {
            return option_defs[i].id;
        }
    }
    return (TidyOptionId)N_TIDY_OPTIONS;
}
#endif // 0000000000000000000000000000000000000000000

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
