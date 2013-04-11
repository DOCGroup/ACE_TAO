// -*- C++ -*-

//=============================================================================
/**
 *  @file    Log_Category.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <huangh@ociweb.com>
 */
//=============================================================================

#ifndef ACE_LOG_CATEGORY_H
#define ACE_LOG_CATEGORY_H

#include /**/ "ace/pre.h"

#include "ace/Log_Priority.h"
#include "ace/Thread_Mutex.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

#if defined (ACE_NLOGGING)
#if !defined (ACELIB_HEX_DUMP)
# define ACELIB_HEX_DUMP(X) do {} while (0)
#endif
#if !defined (ACELIB_RETURN)
# define ACELIB_RETURN(Y) do { return (Y); } while (0)
#endif
#if !defined (ACELIB_ERROR_RETURN)
# define ACELIB_ERROR_RETURN(X, Y) return (Y)
#endif
#if !defined (ACELIB_ERROR_BREAK)
# define ACELIB_ERROR_BREAK(X) { break; }
#endif
#if !defined (ACELIB_ERROR)
# define ACELIB_ERROR(X) do {} while (0)
#endif
#if !defined (ACELIB_DEBUG)
# define ACELIB_DEBUG(X) do {} while (0)
#endif
#if !defined (ACELIB_ERROR_INIT)
# define ACELIB_ERROR_INIT(VALUE, FLAGS)
#endif
#else
#if !defined (ACELIB_HEX_DUMP)
#define ACELIB_HEX_DUMP(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = ACE_Log_Category::ace_lib().per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log_hexdump X; \
  } while (0)
#endif
#if !defined (ACELIB_RETURN)
#define ACELIB_RETURN(Y) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Msg *ace___ = ACE_Log_Msg::instance(); \
    ace___->set (__FILE__, __LINE__, Y, __ace_error, ace___->restart (), \
                 ace___->msg_ostream (), ace___->msg_callback ()); \
    return Y; \
  } while (0)
#endif
#if !defined (ACELIB_ERROR_RETURN)
#define ACELIB_ERROR_RETURN(X, Y) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = ACE_Log_Category::ace_lib().per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, Y, __ace_error); \
    ace___->log X; \
    return Y; \
  } while (0)
#endif
#if !defined (ACELIB_ERROR)
#define ACELIB_ERROR(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = ACE_Log_Category::ace_lib().per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, -1, __ace_error); \
    ace___->log X; \
  } while (0)
#endif
#if !defined (ACELIB_DEBUG)
#define ACELIB_DEBUG(X) \
  do { \
    int const __ace_error = ACE_Log_Msg::last_error_adapter (); \
    ACE_Log_Category_TSS *ace___ = ACE_Log_Category::ace_lib().per_thr_obj(); \
    if (ace___ == 0) break;\
    ace___->conditional_set (__FILE__, __LINE__, 0, __ace_error); \
    ace___->log X; \
  } while (0)
#endif
#if !defined (ACELIB_ERROR_BREAK)
#define ACELIB_ERROR_BREAK(X) { ACELIB_ERROR (X); break; }
#endif
#endif /* ACE_NLOGGING */


class ACE_Log_Msg;
class ACE_Log_Category;

class ACE_Export ACE_Log_Category_TSS
{
public:
  ACE_Log_Category_TSS(ACE_Log_Category* category);

  const char* name();
  unsigned int id();

  ACE_Log_Msg* logger();
  /// Get the current ACE_Log_Priority mask.
  u_long priority_mask ();

  /// Set the ACE_Log_Priority mask, returns original mask.
  u_long priority_mask (u_long);
  /// Return true if the requested priority is enabled.
  int log_priority_enabled (ACE_Log_Priority log_priority);

  /**
   * Set the line number, file name, operational status, error number,
   * restart flag, ostream, and the callback object.  This combines
   * all the other set methods into a single method.
   */
  void set (const char *file,
            int         line,
            int         op_status = -1,
            int         errnum = 0);

  /// These values are only actually set if the requested priority is
  /// enabled.
  void conditional_set (const char *file,
                        int         line,
                        int         op_status,
                        int         errnum);

  ssize_t log (ACE_Log_Priority priority, const ACE_TCHAR *format, ...);

#if defined (ACE_HAS_WCHAR)
  ssize_t log (ACE_Log_Priority priority, const ACE_ANTI_TCHAR *format, ...);
#endif /* ACE_HAS_WCHAR */

  ssize_t log (const ACE_TCHAR *format,
               ACE_Log_Priority priority,
               va_list          argp);

  /**
   * Method to log hex dump.  This is useful for debugging.  Calls
   * log() to do the actual print, but formats first to make the chars
   * printable.
   */
  int log_hexdump (ACE_Log_Priority log_priority,
                   const char *     buffer,
                   size_t           size,
                   const ACE_TCHAR *text = 0);

private:
  ACE_Log_Category* category_;
  ACE_Log_Msg* logger_;
  u_long priority_mask_;
};


class ACE_Export ACE_Log_Category
{
public:

  /// Notice that ACE_Log_Category does not
  /// deep copy the passed \a name; therefore,
  /// you must keep the lifetime of \a name
  /// longer than the newly create ACE_Log_Category
  /// object.
  ACE_Log_Category(const char* name);
  ~ACE_Log_Category();

  unsigned int id();
  const char* name();

  ACE_Log_Category_TSS* per_thr_obj();

  /// Get the process  ACE_Log_Priority mask.
  u_long priority_mask ();

  /// Set the process ACE_Log_Priority mask, returns original mask.
  u_long priority_mask (u_long);


  static ACE_Log_Category& ace_lib();

private:
  friend class ACE_Log_Category_TSS;
  static void tss_destroy(void * p);

  // disable copying
  ACE_Log_Category(const ACE_Log_Category&);
  ACE_Log_Category& operator = (const ACE_Log_Category&);

  const char* name_;
  unsigned int id_;
  u_long priority_mask_;

  /// we couldn't directly use ACE_TSS because it would
  /// create circular dependency

  /// Avoid race conditions during initialization.
  ACE_Thread_Mutex keylock_;
  /// "First time in" flag.
  /// Key for the thread-specific error data.
  ACE_thread_key_t key_;
};


ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Log_Category.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_LOG_CATEGORY_H */
