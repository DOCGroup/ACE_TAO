// -*- C++ -*-
// $Id$

#include "tao/PortableServer/LifespanStrategyTransient.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_string.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/ORB_Constants.h"
#include "tao/PortableServer/Creation_Time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategyTransient::LifespanStrategyTransient () :
      creation_time_ (ACE_OS::gettimeofday ())
    {
    }

    void
    LifespanStrategyTransient::notify_startup (void)
    {
    }

    void
    LifespanStrategyTransient::notify_shutdown (void)
    {
    }

    CORBA::Boolean
    LifespanStrategyTransient::is_persistent (void) const
    {
      return false;
    }

    bool
    LifespanStrategyTransient::validate (CORBA::Boolean is_persistent,
                                         const TAO::Portable_Server::
                                         Temporary_Creation_Time
                                         &creation_time) const
    {
      return (!is_persistent && this->creation_time_ == creation_time);
    }

    CORBA::ULong
    LifespanStrategyTransient::key_length (void) const
    {
      CORBA::ULong keylength = sizeof (char);

#if (POA_NO_TIMESTAMP == 0)
      // Calculate the space required for the timestamp.
      keylength += TAO::Portable_Server::Creation_Time::creation_time_length
        ();
#endif /* POA_NO_TIMESTAMP */

      return keylength;
    }

    void
    LifespanStrategyTransient::create_key (CORBA::Octet *buffer,
                                           CORBA::ULong &starting_at)
    {
      // Copy the persistence byte.
      buffer[starting_at] = (CORBA::Octet)this->key_type ();
      starting_at += this->key_type_length ();

#if (POA_NO_TIMESTAMP == 0)
      // Then copy the timestamp for transient POAs.
      ACE_OS::memcpy (&buffer[starting_at],
                      this->creation_time_.creation_time (),
                      TAO::Portable_Server::Creation_Time::creation_time_length
                      ());
      starting_at += TAO::Portable_Server::Creation_Time::creation_time_length
        ();
#endif /* POA_NO_TIMESTAMP */
    }

    char
    LifespanStrategyTransient::key_type (void) const
    {
      return 'T';
    }

    void
    LifespanStrategyTransient::check_state (void)
    {
      if (this->poa_->tao_poa_manager ().get_state_i () ==
        PortableServer::POAManager::INACTIVE)
        {
          throw ::CORBA::OBJECT_NOT_EXIST (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
        }
      else
        {
          this->poa_->tao_poa_manager ().check_state ();
        }
    }

    bool
    LifespanStrategyTransient::use_imr () const
    {
      return false;
    }

    ::PortableServer::LifespanPolicyValue
    LifespanStrategyTransient::type () const
    {
      return ::PortableServer::TRANSIENT;
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

TAO_END_VERSIONED_NAMESPACE_DECL
