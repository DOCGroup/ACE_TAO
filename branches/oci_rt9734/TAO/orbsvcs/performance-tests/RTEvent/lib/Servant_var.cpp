/**
 * @file Servant_var.cpp
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@uci.edu>
 */
#ifndef TAO_PERF_RTEC_SERVANT_VAR_CPP
#define TAO_PERF_RTEC_SERVANT_VAR_CPP

#include "Servant_var.h"
#include "tao/Environment.h"

#if !defined(__ACE_INLINE__)
#include "Servant_var.inl"
#endif /* __ACE_INLINE__ */

template<class SERVANT>
Servant_var<SERVANT>::~Servant_var ()
{
  if (this->ptr_ == 0)
    return;

  try{
    this->ptr_->_remove_ref ();
  } catch (const CORBA::Exception&) {
    // @@ TODO This event should be logged. Cannot throw because that
    //    would make it impossible to use this class effectively.
    //
    //  Suggested implementation:
    //
    //  class Servant_Logger {
    //  public:
    //    static void log_error (void *servant_pointer)
    //    {
    //      if (Servant_Logger::instance_ == 0)
    //        return;
    //      Servant_Logger::instance_->logger_implementation (servant_pointer);
    //    }
    //    static Servant_Logger *set_instance (Servant_Logger*);
    //
    //    virtual void logger_implementation (void *servant_pointer) = 0;
    //
    //  private:
    //    static Servant_Logger *instance_;
    //  };
    //
    // Using an abstract class for logging decouples the code from any
    // particular implementation, and provides a light-weight,
    // high-performance solution for applications that want to avoid
    // logging.
  }
}

#endif /* TAO_PERF_RTEC_SERVANT_VAR_CPP */
