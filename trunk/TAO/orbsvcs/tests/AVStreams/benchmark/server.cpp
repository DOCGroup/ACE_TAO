// $Id$

#include "server.h"
#include "tao/Timeprobe.h"
#include "tao/debug.h"

ACE_RCSID(benchmark, server, "$Id$")




Server::Server (CORBA::ORB_ptr orb, PortableServer::POA_ptr poa)
  : orb_ (orb),
    poa_ (poa),
    process_strategy_ (&SERVER_GLOBALS::instance ()->process_options_),
    mmdevice_ (0)
{
  reactive_strategy_.init (orb, poa);
}


CORBA::ORB_ptr
Server::orb (void)
{
  return this->orb_.in ();
}

// Initializes the server
int
Server::init (int argc, char** argv, CORBA::Environment& ACE_TRY_ENV)
{
  ACE_TRY
    {

      //Activate POA Manager
      PortableServer::POAManager_var mgr
        = this->poa_->the_POAManager ();

      mgr->activate ();

      int result = SERVER_GLOBALS::instance ()->parse_args (argc,
                                                            argv);

      if (result == -1)
        ACE_ERROR_RETURN  ((LM_ERROR,"parse args failed\n"),-1);

      // Initialize the naming services
      if (my_name_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      switch (SERVER_GLOBALS::instance ()->strategy_)
        {
        case REACTIVE_STRATEGY:
          ACE_NEW_RETURN (this->mmdevice_,
                          TAO_MMDevice (&this->reactive_strategy_),
                          -1);
          break;
        case PROCESS_STRATEGY:
          ACE_NEW_RETURN (this->mmdevice_,
                          TAO_MMDevice (&this->process_strategy_),
                          -1);
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Invalid strategy\n"),
                            -1);
        }


      // Register the mmdevice with the naming service.
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Bench_Server_MMDevice");

      this->my_name_client_->rebind (server_mmdevice_name,
                                     this->mmdevice_->_this (),
                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Server::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}


// Runs the server
int
Server::run (CORBA::Environment& ACE_TRY_ENV)
{
  // Run the ORB event loop
  while (1)
    {
      this->orb_->run (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      if (errno== EINTR)
        continue;
      else
        break;
    }
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Server::run () "
              "came out of the (ORB) "
              "event loop %p\n",
              "run_event_loop"));
  return 0;

}

Server::~Server (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Server: Removing handlers from the Reactor\n"));

  if (this->mmdevice_ != 0)
    delete this->mmdevice_;

}


int
main (int argc, char **argv)
{

  //TAO_debug_level++;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv);

      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      Server server (orb.in (), poa.in ());

      if (server.init (argc, argv, ACE_TRY_ENV) == -1)
        return 1;

      server.run (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Bench_Server::Exception \n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_AV_Endpoint_Reactive_Strategy<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<Bench_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
