// -*- C++ -*-

#include "LifespanStrategyTransient.h"

ACE_RCSID (PortableServer,
           Lifespan_Strategy,
           "$Id$")

#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_string.h"
#include "POAManager.h"
#include "Root_POA.h"
#include "tao/ORB_Constants.h"
#include "Creation_Time.h"

namespace TAO
{
  namespace Portable_Server
  {
    LifespanStrategyTransient::LifespanStrategyTransient () :
      creation_time_ (ACE_OS::gettimeofday ())
    {
    }

    LifespanStrategyTransient::~LifespanStrategyTransient ()
    {
    }

    void
    LifespanStrategyTransient::notify_startup (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }

    void
    LifespanStrategyTransient::notify_shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }

    CORBA::Boolean
    LifespanStrategyTransient::is_persistent (void) const
    {
      return false;
    }

    bool
    LifespanStrategyTransient::validate (
      CORBA::Boolean is_persistent,
      const TAO::Portable_Server::Temporary_Creation_Time &creation_time) const
    {
      return (!is_persistent && this->creation_time_ == creation_time);
    }

    CORBA::ULong
    LifespanStrategyTransient::key_length (void) const
    {
      CORBA::ULong keylength = sizeof (char);

      #if (POA_NO_TIMESTAMP == 0)
        // Calculate the space required for the timestamp.
          keylength += TAO::Portable_Server::Creation_Time::creation_time_length ();
      #endif /* POA_NO_TIMESTAMP */

      return keylength;
    }

    void
    LifespanStrategyTransient::create_key (
      CORBA::Octet *buffer,
      CORBA::ULong& starting_at)
    {
      // Copy the persistence byte.
      buffer[starting_at] = (CORBA::Octet) this->key_type ();
      starting_at += this->key_type_length ();

      #if (POA_NO_TIMESTAMP == 0)
        // Then copy the timestamp for transient POAs.
        ACE_OS::memcpy (&buffer[starting_at],
                        this->creation_time_.creation_time (),
                       TAO::Portable_Server::Creation_Time::creation_time_length ());
        starting_at += TAO::Portable_Server::Creation_Time::creation_time_length ();
      #endif /* POA_NO_TIMESTAMP */
    }

    char
    LifespanStrategyTransient::key_type (void) const
    {
      // @@ Johnny, why can;t use the enum's defined within
      // PortableServer IDL? Wouldn't they make life much simpler?
      // @bala, this if for the IOR setup/parsing, I can't find these charachters
      // in the pidl file. In the POA it are static methods, seems that this
      // normal methods doesn't work, because in some static functions these are
      // used. This still needs some work, but I want to get those info out of
      // the POA.
      return 'T';
    }

    void
    LifespanStrategyTransient::check_state (ACE_ENV_SINGLE_ARG_DECL)
    {
      if (this->poa_->tao_poa_manager().get_state_i () ==
            PortableServer::POAManager::INACTIVE)
      {
        ACE_THROW (CORBA::OBJECT_NOT_EXIST (CORBA::OMGVMCID | 4,
                                            CORBA::COMPLETED_NO));
      }
      else
      {
        this->poa_->tao_poa_manager().check_state (ACE_ENV_SINGLE_ARG_PARAMETER);
      }
    }

    bool
    LifespanStrategyTransient::use_imr () const
    {
      return false;
    }

    ::PortableServer::LifespanPolicyValue
    LifespanStrategyTransient::type() const
    {
      return ::PortableServer::TRANSIENT;
    }
  } /* namespace Portable_Server */
} /* namespace TAO */

