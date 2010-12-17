// -*- C++ -*-

//=============================================================================
/**
 * @file        ODBC_Env.h
 *
 * $Id$
 *
 * @author      James H. Hill
 */
//=============================================================================

#ifndef _ADBC_ODBC_ENV_H_
#define _ADBC_ODBC_ENV_H_

#include "ace/Singleton.h"
#include "ace/Thread_Mutex.h"
#include "ODBC.h"

namespace ADBC
{
namespace ODBC
{
/**
 * @class Environment
 *
 * Environment for an existing ODBC connection.
 */
class ADBC_ODBC_Export Environment
{
public:
  /// Default constructor.
  Environment (void);

  /// Destructor.
  ~Environment (void);

  /// Get the underlying environment handle.
  HENV handle (void) const;

  /// Get pointer to a process-wide Environment.
  static Environment *instance (void);

  /// Set pointer to a process-wide Environment and return
  /// existing pointer.
  static Environment *instance (Environment * env);

  /// Delete the dynamically allocated Singleton
  static void close_singleton (void);

private:
  /// Initialize the environment.
  void init (void);

  /// Handle to the environment.
  HENV handle_;

  /// Pointer to a process-wide ACE_Thread_Manager.
  static Environment * singleton_env_;

  /// Must delete the thr_mgr_ if true.
  static bool delete_singleton_env_;
};

}
}

#if defined (__ADBC_INLINE__)
#include "ODBC_Env.inl"
#endif

#endif  // !defined _ADBC_ODBC_ENV_H_
