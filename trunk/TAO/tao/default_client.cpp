// $Id$

#include "tao/default_client.h"
#include "tao/ORB_Core.h"

#if !defined (__ACE_INLINE__)
# include "tao/default_client.i"
#endif /* ! __ACE_INLINE__ */

TAO_Default_Client_Strategy_Factory::TAO_Default_Client_Strategy_Factory (void)
: iiop_profile_lock_type_ (TAO_THREAD_LOCK)
{
}

TAO_Default_Client_Strategy_Factory::~TAO_Default_Client_Strategy_Factory (void)
{
}

int
TAO_Default_Client_Strategy_Factory::init (int argc, char *argv[])
{
  return this->parse_args (argc, argv);
}

int
TAO_Default_Client_Strategy_Factory::parse_args (int argc, char ** argv)
{
  ACE_TRACE ("TAO_Default_Client_Strategy_Factory::parse_args");

  int curarg;

  for (curarg = 0; curarg < argc && argv[curarg]; curarg++)
    if (ACE_OS::strcmp (argv[curarg], "-ORBiiopprofilelock") == 0)
      {
        curarg++;
        if (curarg < argc)
          {
            char *name = argv[curarg];

            if (ACE_OS::strcasecmp (name, "thread") == 0)
              this->iiop_profile_lock_type_ = TAO_THREAD_LOCK;
            else if (ACE_OS::strcasecmp (name, "null") == 0)
              this->iiop_profile_lock_type_ = TAO_NULL_LOCK;
          }
      }
  
  return 0;
}

ACE_Lock *
TAO_Default_Client_Strategy_Factory::create_iiop_profile_lock (void)
{
  ACE_Lock* the_lock = 0;

  if (this->iiop_profile_lock_type_ == TAO_NULL_LOCK)
    ACE_NEW_RETURN (the_lock,
		    ACE_Lock_Adapter<ACE_SYNCH_NULL_MUTEX> (),
		    0);
  else
    ACE_NEW_RETURN (the_lock,
		    ACE_Lock_Adapter<ACE_SYNCH_MUTEX> (),
		    0);

  return the_lock;
}

ACE_FACTORY_DEFINE (TAO, TAO_Default_Client_Strategy_Factory)
