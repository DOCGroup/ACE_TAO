// $Id$

#include "ODBC_Env.h"

#if !defined (__ADBC_INLINE__)
#include "ODBC_Env.inl"
#endif

#include "ODBC_Exception.h"

namespace ADBC
{
namespace ODBC
{
//
// delete_singleton_env_
//
bool Environment::delete_singleton_env_ = false;

//
// singleton_env_
//
Environment * Environment::singleton_env_ = 0;

//
// ~Environment
//
Environment::~Environment (void)
{
  if (this->handle_ != SQL_NULL_HANDLE)
    ::SQLFreeHandle (SQL_HANDLE_ENV, this->handle_);
}

//
// init
//
void Environment::init (void)
{
  // Allocate environment handle. Eventually we would like to be
  // able to attach to user-defined environments.
  SQL_VERIFY (::SQLAllocHandle (SQL_HANDLE_ENV,
                                SQL_NULL_HANDLE,
                                &this->handle_),
              Exception ("failed to allocate environment handle"));

  // Set the version of ODBC. Right now we only support ODBC 3.x
  SQL_ENV_VERIFY (::SQLSetEnvAttr (this->handle_,
                  SQL_ATTR_ODBC_VERSION,
                  (void *)SQL_OV_ODBC3,
                  0),
                  this->handle_);
}

//
// instance
//
Environment * Environment::instance (Environment * new_env)
{
  ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                            *ACE_Static_Object_Lock::instance (), 0));

  Environment * old_env = Environment::singleton_env_;

  // We can't safely delete it since we don't know who created it!
  Environment::delete_singleton_env_ = false;
  Environment::singleton_env_ = new_env;

  return old_env;
}

//
// instance
//
Environment * Environment::instance (void)
{
  if (Environment::singleton_env_ == 0)
  {
    // Perform Double-Checked Locking Optimization.
    ACE_MT (ACE_GUARD_RETURN (ACE_Recursive_Thread_Mutex, ace_mon,
                              *ACE_Static_Object_Lock::instance (), 0));

    if (Environment::singleton_env_ == 0)
    {
      ACE_NEW_RETURN (Environment::singleton_env_,
                      Environment (),
                      0);

      Environment::delete_singleton_env_ = true;
    }
  }

  return Environment::singleton_env_;
}

//
// close_singleton
//
void Environment::close_singleton (void)
{
  ACE_MT (ACE_GUARD (ACE_Recursive_Thread_Mutex, ace_mon,
                     *ACE_Static_Object_Lock::instance ()));

  if (Environment::singleton_env_)
  {
    // Delete the existing environment.
    delete Environment::singleton_env_;

    Environment::singleton_env_ = 0;
    Environment::delete_singleton_env_ = false;
  }
}

}
}
