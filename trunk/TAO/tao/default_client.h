// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//     default_client.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#if !defined (TAO_DEFAULT_CLIENT_H)
#  define TAO_DEFAULT_CLIENT_H

#include "tao/corba.h"

class TAO_Default_Client_Strategy_Factory : public TAO_Client_Strategy_Factory
{
  // = TITLE
  //   This is the "default" client strategy factor for TAO.  It
  //   includes strategies that are configured through command-line
  //   options so that everything operates as if there were no
  //   dynamically-linkable strategies.
public:
  // = Initialization and termination methods.
  TAO_Default_Client_Strategy_Factory (void);
  // Constructor.

  virtual ~TAO_Default_Client_Strategy_Factory (void);
  // Destructor.
  
  // = Service Configurator hooks.
  virtual int init (int argc, char* argv[]);
  // Dynamic linking hook

  int parse_args(int argc, char* argv[]);
  // Parse svc.conf arguments
  // <-ORBiiopprofilelock> <{which}>
  //   where <{which}> is one of <thread> or <null> (default <thread>)

  ACE_Lock* create_iiop_profile_lock (void);
  // create the lock for the forwarding IIOP Profile used by 
  // the TAO_GIOP_Invocation::location_forward and the 
  // TAO_GIOP_Invocation::start

private:
  enum Lock_Type
  {
    TAO_NULL_LOCK,
    TAO_THREAD_LOCK
  };

  Lock_Type iiop_profile_lock_type_;
  // the lock type for forwarding IIOP Profile

};

#if defined (__ACE_INLINE__)
# include "tao/default_client.i"
#endif /* __ACE_INLINE__ */

ACE_FACTORY_DECLARE (TAO, TAO_Default_Client_Strategy_Factory)

#endif /* TAO_DEFAULT_CLIENT_H */
