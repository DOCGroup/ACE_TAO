/**
 * This is a configuration file to define all the macros that Doxygen
 * needs
 *
 * @file config-doxygen.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 * @author Darrell Brunsch <brunsch@uci.edu>
 *
 */
#ifndef ACE_CONFIG_DOXYGEN_H
#define ACE_CONFIG_DOXYGEN_H

/// Make the wchar_t interfaces available.
#define ACE_HAS_WCHAR

/// Make all the emulation versions of string operations visible
// #define ACE_LACKS_WCSTOK
#define ACE_LACKS_ITOW
#define ACE_LACKS_STRCASECMP
#define ACE_LACKS_STRCSPN
#define ACE_LACKS_STRCHR
#define ACE_LACKS_STRRCHR
#define ACE_LACKS_WCSCAT
#define ACE_LACKS_WCSCHR
#define ACE_LACKS_WCSCMP
#define ACE_LACKS_WCSCPY
#define ACE_LACKS_WCSICMP
#define ACE_LACKS_WCSLEN
#define ACE_LACKS_WCSNCAT
#define ACE_LACKS_WCSNCMP
#define ACE_LACKS_WCSNCPY
#define ACE_LACKS_WCSNICMP
#define ACE_LACKS_WCSPBRK
#define ACE_LACKS_WCSRCHR
#define ACE_LACKS_WCSCSPN
#define ACE_LACKS_WCSSPN
#define ACE_LACKS_WCSSTR

/// Support for threads enables several important classes
#define ACE_HAS_THREADS

/// Support for Win32 enables the WFMO_Reactor and several Async I/O
/// classes
#define ACE_WIN32

/// Enable support for POSIX Asynchronous I/O calls
#define ACE_HAS_AIO_CALLS

/// Enable support for TLI interfaces
#define ACE_HAS_TLI

/// Enable support for the SSL wrappers
#define ACE_HAS_SSL 1

/// Several GUI Reactors that are only enabled in some platforms.
#define ACE_HAS_XT
#define ACE_HAS_FL
#define ACE_HAS_QT
#define ACE_HAS_TK
#define ACE_HAS_GTK

/// Enable exceptions
#define ACE_HAS_EXCEPTIONS

/// TAO features that should be documented too
#define TAO_HAS_RT_CORBA 1
#define TAO_HAS_MINIMUM_CORBA 0
#define TAO_HAS_AMI 1
#define TAO_HAS_INTERCEPTORS 1

#endif /* ACE_CONFIG_DOXYGEN_H */
