// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    client.cpp
//
// = DESCRIPTION 
//    Test client for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "client.h"

Client::Client (void)
{
}

int
Client::init (int argc, 
              char **argv,
              CORBA::Environment &env)
{

  // init the ORB
  manager_.init (argc,
                 argv,
                 "avstreams_poa",
                 env);

  TAO_CHECK_ENV_RETURN (env, 1);
                                             
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;
  char *ior = 0;
  
  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	ior = 
          ACE_OS::strdup (get_opts.optarg);
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-k mmdevice_key]"
                           "\n",
                           argv [0]),
                          -1);
      }
  if (ior == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-k mmdevice_key]"
                           "\n",
                           argv [0]),
                          -1);
  
  CORBA::Object_var mmdevice_object = 
    this->manager_.orb()->string_to_object (ior,
                                            env);
  TAO_CHECK_ENV_RETURN (env, 1);

  this->remote_mmdevice_ = 
    AVStreams::MMDevice::_narrow (mmdevice_object.in (),
                                  env);
  TAO_CHECK_ENV_RETURN (env, 1);

  if (CORBA::is_nil (this->remote_mmdevice_.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "invalid key <%s>\n",
                         ior),
                        -1);
    }

  ACE_DEBUG ((LM_DEBUG, "Object received OK\n"));
  
  ACE_DEBUG ((LM_DEBUG, 
              "\n(%P|%t) Bound to remote mmdevice"));

  return 0;
}

int
Client::run (CORBA::Environment &env)
{
  return 0;
}

int
main (int argc, char **argv)
{
  TAO_TRY
    {
      Client client;
      if (client.init (argc, 
                       argv, 
                       TAO_TRY_ENV) == -1)
        return 1;
      TAO_CHECK_ENV;
      client.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Cubit::init");
      return -1;
    }
  TAO_ENDTRY;
}
