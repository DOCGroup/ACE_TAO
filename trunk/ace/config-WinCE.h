// $Id$

#ifndef ACE_CONFIG_WINCE_H
#define ACE_CONFIG_WINCE_H

#if !defined (ACE_HAS_WINCE)
#define ACE_HAS_WINCE 1
#endif

// Only DLL version is supported on CE.
#if defined (ACE_HAS_DLL)
# undef ACE_HAS_DLL
#endif /* ACE_HAS_DLL */
#define ACE_HAS_DLL 1

// CE is not NT.
#if defined (ACE_HAS_WINNT4)
# undef ACE_HAS_WINNT4
#endif /* ACE_HAS_WINNT4 */
#define ACE_HAS_WINNT4 0

// You must use MFC with ACE on CE.
#if defined (ACE_HAS_MFC)
# undef ACE_HAS_MFC
#endif /* ACE_HAS_MFC */
#define ACE_HAS_MFC 1

// So is UNICODE.
#if defined (ACE_HAS_UNICODE)
# undef ACE_HAS_UNICODE
#endif /* ACE_HAS_UNICODE */
#define ACE_HAS_UNICODE
#define ACE_HAS_MOSTLY_UNICODE_APIS

#define ACE_USES_WINCE_SEMA_SIMULATION

#define ACE_LACKS_IOSTREAM_TOTALLY

#if defined (ACE_HAS_STRICT)
# undef ACE_HAS_STRICT
#endif /* ACE_HAS_STRICT */
#define ACE_HAS_STRICT 1

// Non-static object manager does not quite work with CE (yet.)
#define ACE_HAS_NONSTATIC_OBJECT_MANAGER 0

// We need to rename program entry name "main" with ace_ce_main here
// so that we can call it from CE's bridge class.
#define ACE_MAIN ace_ce_main
#define ACE_MAIN_OBJECT_MANAGER

// SH3 cross-compiler can't handle inline functions correctly (along with other bugs.)
#if defined (SH3) && defined (DEBUG)
#define ACE_LACKS_INLINE_FUNCTIONS
#endif /* SH3 && _DEBUG */

#ifndef ACE_DEFAULT_SERVER_HOST
# define ACE_DEFAULT_SERVER_HOST L"localhost"
#endif /* ACE_DEFAULT_SERVER_HOST */

// @@ Need to remap every function that uses any of these flags to
//    Win32 API.  These are for ANSI styled function and are not
//    available on WinCE.

#define _O_RDONLY       0x0000  /* open for reading only */
#define _O_WRONLY       0x0001  /* open for writing only */
#define _O_RDWR         0x0002  /* open for reading and writing */
#define _O_APPEND       0x0008  /* writes done at eof */

#define _O_CREAT        0x0100  /* create and open file */
#define _O_TRUNC        0x0200  /* open and truncate */
#define _O_EXCL         0x0400  /* open only if file doesn't already exist */

/* O_TEXT files have <cr><lf> sequences translated to <lf> on read()'s,
** and <lf> sequences translated to <cr><lf> on write()'s
*/

#define _O_TEXT         0x4000  /* file mode is text (translated) */
#define _O_BINARY       0x8000  /* file mode is binary (untranslated) */

/* macro to translate the C 2.0 name used to force binary mode for files */

//#define _O_RAW  _O_BINARY

/* Open handle inherit bit */

//#define _O_NOINHERIT    0x0080  /* child process doesn't inherit file */

/* Temporary file bit - file is deleted when last handle is closed */

#define _O_TEMPORARY    0x0040  /* temporary file bit */

/* temporary access hint */

//#define _O_SHORT_LIVED  0x1000  /* temporary storage file, try not to flush */

/* sequential/random access hints */

//#define _O_SEQUENTIAL   0x0020  /* file access is primarily sequential */
//#define _O_RANDOM       0x0010  /* file access is primarily random */


// Non-ANSI names
#define O_RDONLY        _O_RDONLY
#define O_WRONLY        _O_WRONLY
#define O_RDWR          _O_RDWR
#define O_APPEND        _O_APPEND
#define O_CREAT         _O_CREAT
#define O_TRUNC         _O_TRUNC
#define O_EXCL          _O_EXCL
#define O_TEXT          _O_TEXT
#define O_BINARY        _O_BINARY
//#define O_RAW           _O_BINARY
#define O_TEMPORARY     _O_TEMPORARY
//#define O_NOINHERIT     _O_NOINHERIT
//#define O_SEQUENTIAL    _O_SEQUENTIAL
//#define O_RANDOM        _O_RANDOM


// @@ NSIG value.  This is definitely not correct.
#define NSIG 23


// @@ For some reason, WinCE forgot to define this.
//    Need to find out what it is. (Used in MapViewOfFile ().)
#define FILE_MAP_COPY 0


#define ACE_LACKS_STRCASECMP    // WinCE doesn't support _stricmp
#define ACE_LACKS_GETSERVBYNAME
#define ACE_LACKS_ACCESS
#define ACE_LACKS_FILELOCKS
#define ACE_LACKS_EXEC
#define ACE_LACKS_MKTEMP
#define ACE_LACKS_STRRCHR
#define ACE_LACKS_BSEARCH

// @@ Followings are used to keep existing programs happy.

#define  FILE  void             // Try to map FILE* to HANDLE
#define ostream FILE
#if !defined (BUFSIZ)
#  define BUFSIZ   1024
#endif /* BUFSIZ */

#define EOF  -1

typedef void (*__sighandler_t)(int); // keep Signal compilation happy
typedef long off_t;
#define stderr 0

#define SEEK_SET FILE_BEGIN
#define SEEK_CUR FILE_CURRENT
#define SEEK_END FILE_END

#define stdin 0
#define stdout 0

#define ACE_HAS_STRDUP_EMULATION

// @@ This needs to be defined and initialized as a static. (Singleton?)
#define ACE_DEFAULT_LOG_STREAM 0

#endif /* ACE_CONFIG_WINCE_H */
