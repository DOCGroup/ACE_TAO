#if ! defined (ACE_CONFIG_WINCE_H)
#define ACE_CONFIG_WINCE_H

#define ACE_HAS_WINNT4 0
#define ACE_HAS_MFC 1
#define ACE_HAS_UNICODE
#define ACE_USES_WINCE_SEMA_SIMULATION
#define ACE_HAS_STRICT 1


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
#define ACE_LACKS_GETS
#define ACE_LACKS_ACCESS
#define ACE_LACKS_FILELOCKS
#define ACE_LACKS_EXEC
#define ACE_LACKS_MKTEMP
#define ACE_HAS_MOSTLY_UNICODE_APIS

// temporary workspace
#define  FILE  void             // Try to map FILE* to HANDLE
#define ostream FILE                  // Fake a ostream for now.
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

// @@ This needs to be defined and initialized as a static. (Singleton?)
#define ACE_CE_DEFAULT_LOG_STREAM 0

#endif /* ACE_CONFIG_WINCE_H */
