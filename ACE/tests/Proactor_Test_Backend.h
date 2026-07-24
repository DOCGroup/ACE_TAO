// ============================================================================
/**
 *  @file Proactor_Test_Backend.h
 *
 *  Shared backend selection support for ACE Proactor tests.
 */
// ============================================================================

#ifndef ACE_TESTS_PROACTOR_TEST_BACKEND_H
#define ACE_TESTS_PROACTOR_TEST_BACKEND_H

#include "ace/Init_ACE.h"
#include "ace/Log_Msg.h"
#include "test_config.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)

#include "ace/Get_Opt.h"
#include "ace/Proactor.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_string.h"

#if defined (ACE_WIN32)
#  include "ace/WIN32_Proactor.h"
#endif /* ACE_WIN32 */

#if defined (ACE_HAS_AIO_CALLS)
#  include "ace/POSIX_Proactor.h"
#  include "ace/POSIX_CB_Proactor.h"
#  include "ace/SUN_Proactor.h"
#endif /* ACE_HAS_AIO_CALLS */

#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)
#  include "ace/Uring_Proactor.h"
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */

namespace Proactor_Test_Backend
{
  /// Canonical backend identifiers accepted by the shared test parser.
  enum Type
  {
    BACKEND_DEFAULT = 0,
    BACKEND_WIN32,
    BACKEND_AIOCB,
    BACKEND_SIG,
    BACKEND_SUN,
    BACKEND_CB,
    BACKEND_URING
  };

  /// Return the canonical command-line name for a backend.
  inline const ACE_TCHAR *
  name (Type type)
  {
    switch (type)
      {
      case BACKEND_WIN32:
        return ACE_TEXT ("win32");
      case BACKEND_AIOCB:
        return ACE_TEXT ("aiocb");
      case BACKEND_SIG:
        return ACE_TEXT ("sig");
      case BACKEND_SUN:
        return ACE_TEXT ("sun");
      case BACKEND_CB:
        return ACE_TEXT ("cb");
      case BACKEND_URING:
        return ACE_TEXT ("uring");
      case BACKEND_DEFAULT:
      default:
        return ACE_TEXT ("default");
      }
  }

  /// Parse a backend name or alias into its enum value.
  inline int
  parse_type (const ACE_TCHAR *arg, Type &type)
  {
    if (arg == 0 || arg[0] == 0)
      return -1;

    ACE_TCHAR folded[32];
    size_t index = 0;
    for (; index + 1 < sizeof (folded) / sizeof (folded[0]) && arg[index] != 0; ++index)
      folded[index] = static_cast<ACE_TCHAR> (ACE_OS::ace_tolower (arg[index]));
    folded[index] = 0;

    if (ACE_OS::strcmp (folded, ACE_TEXT ("d")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("default")) == 0)
      {
        type = BACKEND_DEFAULT;
        return 0;
      }
    if (ACE_OS::strcmp (folded, ACE_TEXT ("w")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("win32")) == 0)
      {
        type = BACKEND_WIN32;
        return 0;
      }
    if (ACE_OS::strcmp (folded, ACE_TEXT ("a")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("aiocb")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("posix_aiocb")) == 0)
      {
        type = BACKEND_AIOCB;
        return 0;
      }
    if (ACE_OS::strcmp (folded, ACE_TEXT ("i")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("sig")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("posix_sig")) == 0)
      {
        type = BACKEND_SIG;
        return 0;
      }
    if (ACE_OS::strcmp (folded, ACE_TEXT ("s")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("sun")) == 0)
      {
        type = BACKEND_SUN;
        return 0;
      }
    if (ACE_OS::strcmp (folded, ACE_TEXT ("c")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("cb")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("posix_cb")) == 0)
      {
        type = BACKEND_CB;
        return 0;
      }
    if (ACE_OS::strcmp (folded, ACE_TEXT ("u")) == 0
        || ACE_OS::strcmp (folded, ACE_TEXT ("uring")) == 0)
      {
        type = BACKEND_URING;
        return 0;
      }

    return -1;
  }

  /// Return non-zero if the backend is available in the current build.
  inline int
  is_available (Type type)
  {
    switch (type)
      {
      case BACKEND_DEFAULT:
        return 1;

      case BACKEND_WIN32:
#if defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO)
        return 1;
#else
        return 0;
#endif /* ACE_WIN32 && ACE_HAS_WIN32_OVERLAPPED_IO */

      case BACKEND_AIOCB:
#if defined (ACE_HAS_AIO_CALLS)
        return 1;
#else
        return 0;
#endif /* ACE_HAS_AIO_CALLS */

      case BACKEND_SIG:
#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
        return 1;
#else
        return 0;
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_POSIX_REALTIME_SIGNALS */

      case BACKEND_SUN:
#if defined (ACE_HAS_AIO_CALLS) && defined (sun)
        return 1;
#else
        return 0;
#endif /* ACE_HAS_AIO_CALLS && sun */

      case BACKEND_CB:
#if defined (ACE_HAS_AIO_CALLS) && !defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)
        return 1;
#else
        return 0;
#endif /* ACE_HAS_AIO_CALLS && !ACE_HAS_BROKEN_SIGEVENT_STRUCT */

      case BACKEND_URING:
#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)
        return 1;
#else
        return 0;
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */
      }

    return 0;
  }

  /// Resolve the default selector to the backend chosen by the build.
  inline Type
  concrete_type (Type type)
  {
    if (type != BACKEND_DEFAULT)
      return type;

#if defined (ACE_HAS_AIO_CALLS)
#  if defined (ACE_POSIX_AIOCB_PROACTOR)
    return BACKEND_AIOCB;
#  elif defined (ACE_POSIX_SIG_PROACTOR)
    return BACKEND_SIG;
#  else /* Default order: CB, SIG, AIOCB */
#    if !defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)
    return BACKEND_CB;
#    else
#      if defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
    return BACKEND_SIG;
#      else
    return BACKEND_AIOCB;
#      endif /* ACE_HAS_POSIX_REALTIME_SIGNALS */
#    endif /* !ACE_HAS_BROKEN_SIGEVENT_STRUCT */
#  endif /* ACE_POSIX_AIOCB_PROACTOR */
#elif defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO)
    return BACKEND_WIN32;
#else
    return BACKEND_DEFAULT;
#endif /* ACE_HAS_AIO_CALLS */
  }

  /// Return non-zero if the backend supports scatter/gather test coverage.
  inline int
  supports_scatter_gather (Type type)
  {
    switch (concrete_type (type))
      {
      case BACKEND_WIN32:
#if defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO)
        return 1;
#else
        return 0;
#endif /* ACE_WIN32 && ACE_HAS_WIN32_OVERLAPPED_IO */

      case BACKEND_URING:
#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)
        return 1;
#else
        return 0;
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */

      default:
        return 0;
      }
  }

  /// Print the shared backend selection usage text for a test executable.
  inline int
  print_type_usage (ACE_TCHAR *argv0)
  {
    const ACE_TCHAR *sun_backend =
#if defined (sun)
      ACE_TEXT ("\n    s sun")
#else
      ACE_TEXT ("")
#endif /* sun */
      ;

    const ACE_TCHAR *uring_backend =
#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)
      ACE_TEXT ("\n    u uring")
#else
      ACE_TEXT ("")
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */
      ;

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%s")
                ACE_TEXT ("\n-t <Proactor type>:")
                ACE_TEXT ("\n    d default")
                ACE_TEXT ("\n    w win32")
                ACE_TEXT ("\n    a aiocb / posix_aiocb")
                ACE_TEXT ("\n    i sig / posix_sig")
                ACE_TEXT ("\n    c cb / posix_cb")
                ACE_TEXT ("%s")
                ACE_TEXT ("%s")
                ACE_TEXT ("\n"),
                argv0,
                sun_backend,
                uring_backend));
    return -1;
  }

  /// Report that the requested backend is unavailable in this build.
  inline int
  unsupported (Type type)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Requested proactor backend '%s' is unavailable in this build.\n"),
                name (type)));
    return -1;
  }

  /// Create the requested proactor implementation.
  inline int
  create_impl (Type type,
               size_t max_aio_operations,
               ACE_Proactor_Impl *&implementation)
  {
    implementation = 0;

    switch (type)
      {
      case BACKEND_DEFAULT:
#if defined (ACE_HAS_AIO_CALLS)
#  if defined (ACE_POSIX_AIOCB_PROACTOR)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_AIOCB_Proactor (max_aio_operations),
                        -1);
        return 0;
#  elif defined (ACE_POSIX_SIG_PROACTOR)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_SIG_Proactor (max_aio_operations),
                        -1);
        return 0;
#  else /* Default order: CB, SIG, AIOCB */
#    if !defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_CB_Proactor (max_aio_operations),
                        -1);
        return 0;
#    else
#      if defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_SIG_Proactor (max_aio_operations),
                        -1);
        return 0;
#      else
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_AIOCB_Proactor (max_aio_operations),
                        -1);
        return 0;
#      endif /* ACE_HAS_POSIX_REALTIME_SIGNALS */
#    endif /* !ACE_HAS_BROKEN_SIGEVENT_STRUCT */
#  endif /* ACE_POSIX_AIOCB_PROACTOR */
#elif defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)
        ACE_NEW_RETURN (implementation,
                        ACE_WIN32_Proactor,
                        -1);
        return 0;
#else
        return unsupported (type);
#endif /* ACE_HAS_AIO_CALLS */

      case BACKEND_WIN32:
#if defined (ACE_WIN32) && defined (ACE_HAS_WIN32_OVERLAPPED_IO)
        ACE_NEW_RETURN (implementation,
                        ACE_WIN32_Proactor,
                        -1);
        return 0;
#else
        return unsupported (type);
#endif /* ACE_WIN32 && ACE_HAS_WIN32_OVERLAPPED_IO */

      case BACKEND_AIOCB:
#if defined (ACE_HAS_AIO_CALLS)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_AIOCB_Proactor (max_aio_operations),
                        -1);
        return 0;
#else
        return unsupported (type);
#endif /* ACE_HAS_AIO_CALLS */

      case BACKEND_SIG:
#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_POSIX_REALTIME_SIGNALS)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_SIG_Proactor (max_aio_operations),
                        -1);
        return 0;
#else
        return unsupported (type);
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_POSIX_REALTIME_SIGNALS */

      case BACKEND_SUN:
#if defined (ACE_HAS_AIO_CALLS) && defined (sun)
        ACE_NEW_RETURN (implementation,
                        ACE_SUN_Proactor (max_aio_operations),
                        -1);
        return 0;
#else
        return unsupported (type);
#endif /* ACE_HAS_AIO_CALLS && sun */

      case BACKEND_CB:
#if defined (ACE_HAS_AIO_CALLS) && !defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)
        ACE_NEW_RETURN (implementation,
                        ACE_POSIX_CB_Proactor (max_aio_operations),
                        -1);
        return 0;
#else
        return unsupported (type);
#endif /* ACE_HAS_AIO_CALLS && !ACE_HAS_BROKEN_SIGEVENT_STRUCT */

      case BACKEND_URING:
#if defined (ACE_HAS_AIO_CALLS) && defined (ACE_HAS_IO_URING)
        {
          ACE_Uring_Proactor *uring = 0;
          ACE_NEW_RETURN (uring,
                          ACE_Uring_Proactor (max_aio_operations),
                          -1);
          if (!uring->is_initialized ())
            {
              delete uring;
              errno = ENODEV;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Failed to initialize uring proactor\n")),
                                -1);
            }
          implementation = uring;
        }
        return 0;
#else
        return unsupported (type);
#endif /* ACE_HAS_AIO_CALLS && ACE_HAS_IO_URING */
      }

    return -1;
  }

  /// Create an ACE_Proactor wrapper for the requested backend implementation.
  inline int
  create_proactor (Type type,
                   size_t max_aio_operations,
                   ACE_Proactor *&proactor,
                   bool install_singleton = true)
  {
    ACE_Proactor_Impl *implementation = 0;
    if (create_impl (type, max_aio_operations, implementation) != 0)
      return -1;

    ACE_NEW_NORETURN (proactor,
                      ACE_Proactor (implementation, 1));
    if (proactor == 0)
      {
        delete implementation;
        return -1;
      }

    if (install_singleton)
      ACE_Proactor::instance (proactor, 1);

    return 0;
  }
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */

#endif /* ACE_TESTS_PROACTOR_TEST_BACKEND_H */
