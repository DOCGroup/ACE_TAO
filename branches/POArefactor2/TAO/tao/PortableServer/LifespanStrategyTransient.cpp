// -*- C++ -*-

//=============================================================================
/**
 *  @file    Lifespan_Strategy.cpp
 *
 *  $Id$
 *
 */
//=============================================================================

#include "LifespanStrategyTransient.h"

ACE_RCSID (PortableServer,
           Lifespan_Strategy,
           "$Id$")

#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_string.h"
#include "Creation_Time.h"

namespace TAO
{
  namespace Portable_Server
  {
    Transient_Lifespan_Strategy::Transient_Lifespan_Strategy () :
      creation_time_ (ACE_OS::gettimeofday ())
    {
    }

    Transient_Lifespan_Strategy::~Transient_Lifespan_Strategy ()
    {
    }

    void
    Transient_Lifespan_Strategy::notify_startup (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }

    void
    Transient_Lifespan_Strategy::notify_shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    {
    }

    bool
    Transient_Lifespan_Strategy::validate (
      CORBA::Boolean is_persistent,
      const TAO::Portable_Server::Temporary_Creation_Time &creation_time) const
    {
      return (!is_persistent && this->creation_time_ == creation_time);
    }

    CORBA::ULong
    Transient_Lifespan_Strategy::key_length (void) const
    {
      CORBA::ULong keylength = sizeof (char);

      #if (POA_NO_TIMESTAMP == 0)
        // Calculate the space required for the timestamp.
          keylength += TAO::Portable_Server::Creation_Time::creation_time_length ();
      #endif /* POA_NO_TIMESTAMP */

      return keylength;
    }

    void
    Transient_Lifespan_Strategy::create_key (
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
    Transient_Lifespan_Strategy::key_type (void) const
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
  } /* namespace Portable_Server */
} /* namespace TAO */

