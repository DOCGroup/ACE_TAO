// $Id$

// ============================================================================
//
// = LIBRARY
//    CORBA Tao marshal benchmark
// 
// = FILENAME
//    tao_marshal_server.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

#include "tao_marshal_server.h"
#include <iostream.h>
#include <fstream.h>

#if !defined (__ACE_INLINE__)
#include "tao_marshal_server.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, tao_marshal_server, "$Id$")

int
Tao_Marshal_Server_Proxy::run (CORBA_Marshal_Proxy::OPTIONS &options,
				CORBA_Marshal_Proxy::RESULTS &results)
{
  char **argv;
  int argc;
  CORBA_Environment env;
  CORBA_String str; // for the IOR string
  fstream iorfile;  // persistent storage for IOR

  // get the argument list to be passed on to the ORB_init method
  argv = options.orb_init_args (argc);

  // get the orb. Pass any ORB_init arguments
  this->orb_ = CORBA_ORB_init (argc, (char *const *)argv, "IOR", env);
  if (this->orb_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ORB_init failed\n"));
      return -1;
    }

  // get the argument list to be passed on to the BOA_init method
  argv = options.boa_init_args (argc);

  char *foo [] = {"-OAhost","mambo"}; argc = 2;
  // get the orb. Pass any ORB_init arguments
  this->boa_ = this->orb_->BOA_init (argc, foo, "ROA");//(argc, (char *const *)argv);
  if (this->boa_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ORB_init failed\n"));
      return -1;
    }

  // tell the "results" object what file it must store the results into
  results.filename (options.filename ());

  // use SII or DII accordingly
  switch (options.policy ())
    {
    case CORBA_Marshal_Options::STATIC : // use SSI
      this->ssi_ = new Marshal_SSI_Impl (options.object_name ());
      // register with the BOA
      //      this->boa_->obj_is_ready (this->ssi_);

      // get a stringified representation of the object reference created above
      str = this->orb_->object_to_string (this->dsi_, env);
      if (env.exception() != 0)
	{
	  //	  print_exception (env.exception(), "object_to_string", stdout);
	  return 1;
	}

      // save the IOR in persistent storage so that clients can read this and obtain
      // object references. We need to do this since we do not yet have some kind
      // of a naming service that will allow clients to "bind" to desired objects
      iorfile.open ("./ior.txt", ios::out);
      iorfile << ((char *)str) << endl;
      iorfile.close ();
      break;
    case CORBA_Marshal_Options::DYNAMIC : // use DSI
      this->dsi_ = new Marshal_DSI_Impl (this->orb_, ACE_OS::strdup (options.object_name ()));
      // register with the BOA
      //      this->boa_->obj_is_ready (this->dsi_);

      // get a stringified representation of the object reference created above
      str = this->orb_->object_to_string (this->dsi_, env);
      if (env.exception() != 0)
	{
	  ACE_DEBUG ((LM_ERROR, "cannot get IOR from DSI object" ));
	  //	  print_exception (env.exception(), "object_to_string", stdout);
	  return 1;
	}

      // save the IOR in persistent storage so that clients can read this and obtain
      // object references. We need to do this since we do not yet have some kind
      // of a naming service that will allow clients to "bind" to desired objects
      iorfile.open ("./ior.txt", ios::out);
      iorfile << ((char *)str) << endl;
      iorfile.close ();
      break;
    default:
      ACE_DEBUG ((LM_DEBUG, "bad policy\n"));
      return -1;
    }

  ACE_Reactor::run_event_loop (); // should be replaced by impl_is_ready
  return 0;
}

