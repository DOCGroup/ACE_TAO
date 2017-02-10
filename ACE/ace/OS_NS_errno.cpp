// -*- C++ -*-
#include "ace/OS_NS_errno.h"

#if !defined (ACE_HAS_INLINED_OSCALLS)
# include "ace/OS_NS_errno.inl"
#endif /* ACE_HAS_INLINED_OSCALLS */


// Perform a mapping of Win32 error numbers into POSIX errnos.
// NOTE: Keep this out-of-line as list may grow in future
int
ACE_OS::adapt_last_error(int error)
{
    ACE_OS_TRACE("ACE_OS::adapt_last_error");

#if defined(ACE_WIN32)
    // Perform a mapping of Win32 error numbers into POSIX errnos.
    switch (error)
    {
    case ERROR_NOT_ENOUGH_MEMORY:   error = ENOMEM          ; break;
    case ERROR_FILE_EXISTS:         error = EEXIST          ; break;
    case ERROR_SHARING_VIOLATION:   error = EACCES          ; break;
    case ERROR_PATH_NOT_FOUND:      error = ENOENT          ; break;
    case ERROR_ACCESS_DENIED:       error = EPERM           ; break;
    case ERROR_SEM_TIMEOUT:         error = ETIME           ; break;
    case ERROR_TIMEOUT:             error = ETIME           ; break;
//
//  Reverse WSAGetLastError mappings that are not handled by
//  the direct mappings in <ace/os_include/os_errno.h>
//
#if defined(_CRT_NO_POSIX_ERROR_CODES) // see <errno.h>
    case WSAEWOULDBLOCK:            error = EWOULDBLOCK     ; break;
    case WSAEINPROGRESS:            error = EINPROGRESS     ; break;
    case WSAEALREADY:               error = EALREADY        ; break;
    case WSAENOTSOCK:               error = ENOTSOCK        ; break;
    case WSAEDESTADDRREQ:           error = EDESTADDRREQ    ; break;
    case WSAEMSGSIZE:               error = EMSGSIZE        ; break;
    case WSAEPROTOTYPE:             error = EPROTOTYPE      ; break;
    case WSAENOPROTOOPT:            error = ENOPROTOOPT     ; break;
    case WSAEPROTONOSUPPORT:        error = EPROTONOSUPPORT ; break;
    case WSAESOCKTNOSUPPORT:        error = ESOCKTNOSUPPORT ; break;
    case WSAEOPNOTSUPP:             error = EOPNOTSUPP      ; break;
    case WSAEPFNOSUPPORT:           error = EPFNOSUPPORT    ; break;
    case WSAEAFNOSUPPORT:           error = EAFNOSUPPORT    ; break;
    case WSAEADDRINUSE:             error = EADDRINUSE      ; break;
    case WSAEADDRNOTAVAIL:          error = EADDRNOTAVAIL   ; break;
    case WSAENETDOWN:               error = ENETDOWN        ; break;
    case WSAENETUNREACH:            error = ENETUNREACH     ; break;
    case WSAENETRESET:              error = ENETRESET       ; break;
    case WSAECONNABORTED:           error = ECONNABORTED    ; break;
    case WSAECONNRESET:             error = ECONNRESET      ; break;
    case WSAENOBUFS:                error = ENOBUFS         ; break;
    case WSAEISCONN:                error = EISCONN         ; break;
    case WSAENOTCONN:               error = ENOTCONN        ; break;
    case WSAESHUTDOWN:              error = ESHUTDOWN       ; break;
    case WSAETOOMANYREFS:           error = ETOOMANYREFS    ; break;
    case WSAETIMEDOUT:              error = ETIMEDOUT       ; break;
    case WSAECONNREFUSED:           error = ECONNREFUSED    ; break;
    case WSAELOOP:                  error = ELOOP           ; break;
    case WSAEHOSTDOWN:              error = EHOSTDOWN       ; break;
    case WSAEHOSTUNREACH:           error = EHOSTUNREACH    ; break;
    case WSAEPROCLIM:               error = EPROCLIM        ; break;
    case WSAEUSERS:                 error = EUSERS          ; break;
    case WSAEDQUOT:                 error = EDQUOT          ; break;
    case WSAESTALE:                 error = ESTALE          ; break;
    case WSAEREMOTE:                error = EREMOTE         ; break;
#endif  /* defined(_CRT_NO_POSIX_ERROR_CODES) */
/* ENAMETOOLONG and ENOTEMPTY are defined by the 'standard' library. */
#if defined(ENAMETOOLONG)
    case WSAENAMETOOLONG:           error = ENAMETOOLONG    ; break;
#endif
#if defined(ENOTEMPTY)
    case WSAENOTEMPTY:              error = ENOTEMPTY       ; break;
#endif
    }
#endif  /* defined(ACE_WIN32) */

    return ACE_OS::last_error(error);
}
