// $Id$

// ============================================================================
//
// = LIBRARY
//    CORBA Orbix marshal benchmark
// 
// = FILENAME
//    orbix_marshal_server.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
// 
// ============================================================================

//#define USE_INIT

#include "orbix_marshal_server.h"

#if !defined (__ACE_INLINE__)
#include "orbix_marshal_server.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Orbix, orbix_marshal_server, "$Id$")

int
Orbix_Marshal_Server_Proxy::run (CORBA_Marshal_Proxy::OPTIONS &options,
				CORBA_Marshal_Proxy::RESULTS &results)
{
  char **argv;
  int argc;

#if 0
  // get the argument list to be passed on to the ORB_init method
  argv = options.orb_init_args (argc);

  // get the orb. Pass any ORB_init arguments
  this->orb_ = CORBA::ORB_init (argc, (char *const *)argv, "Orbix");
  if (this->orb_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ORB_init failed\n"));
      return -1;
    }

  // get the argument list to be passed on to the BOA_init method
  argv = options.orb_init_args (argc);

  // get the orb. Pass any ORB_init arguments
  this->boa_ = this->orb_->BOA_init (argc, (char *const *)argv, "Orbix_BOA");
  if (this->boa_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ORB_init failed\n"));
      return -1;
    }
#endif
  // tell the "results" object what file it must store the results into
  results.filename (options.filename ());

  // use SII or DII accordingly
  switch (options.policy ())
    {
    case CORBA_Marshal_Options::STATIC : // use SSI
      this->ssi_ = new Marshal_SSI_Impl (options.object_name ());
      break;
    case CORBA_Marshal_Options::DYNAMIC : // use DSI
      //      this->dsi_ = new Marshal_DSI_Impl (this->orb_, ACE_OS::strdup (options.object_name ()));
      this->dsi_ = new Marshal_DSI_Impl (&CORBA::Orbix, ACE_OS::strdup (options.object_name ()));
      // register with the BOA
      try {
	// we need to do a setImpl
	//this->boa_->setImpl ("Marshal", *this->dsi_);
	CORBA::Orbix.setImpl ("Marshal", *this->dsi_);
      }
      catch (CORBA::SystemException &se)
	{
	  cerr << "system exception in obj_is_ready" << se << endl;
	}
      break;
    default:
      ACE_DEBUG ((LM_DEBUG, "bad policy\n"));
      return -1;
    }
  
  ACE_DEBUG ((LM_DEBUG, "Calling impl is ready\n"));
  try {
    // call impl_is_ready
    //    this->boa_->impl_is_ready ((char *)options.service_name ());
    CORBA::Orbix.impl_is_ready ("marshalDSI");
  }
  catch (CORBA::SystemException &se)
    {
      cerr << "system exception: " << se << endl;
    }

  return 0;
}

