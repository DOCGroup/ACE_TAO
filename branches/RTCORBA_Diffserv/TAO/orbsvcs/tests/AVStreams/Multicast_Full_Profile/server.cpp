// $Id$

#include "server.h"

FTP_Server_FlowEndPoint::FTP_Server_FlowEndPoint (void)
  :TAO_FlowConsumer ("Data",FTP_SERVER::instance ()->protocols (),FTP_SERVER::instance ()->format ())
{
  AVStreams::protocolSpec protocols (2);
  protocols.length (2);
  protocols [0] = CORBA::string_dup ("TCP");
  protocols [1] = CORBA::string_dup ("UDP");
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->set_protocol_restriction (protocols
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"FTP_Server_FlowEndPoint::FTP_Server_FlowEndPoint");
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

int
FTP_Server_FlowEndPoint::get_callback (const char *,
                                       TAO_AV_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::get_sfp_callback\n"));
  ACE_NEW_RETURN (callback,
                  FTP_Server_Callback,
                  -1);
  return 0;
}

int
FTP_Server_Callback::handle_stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Callback::stop"));
  ACE_OS::fclose (FTP_SERVER::instance ()->file ());
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}

int
FTP_Server_Callback::receive_frame (ACE_Message_Block *frame,
                                    TAO_AV_frame_info *,
                                    const ACE_Addr &)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Callback::receive_frame\n"));
  while (frame != 0)
    {
      int result = ACE_OS::fwrite (frame->rd_ptr (),
                                   frame->length (),
                                   1,
                                   FTP_SERVER::instance ()->file ());
      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Server_Flow_Handler::fwrite failed\n"),-1);
      frame = frame->cont ();
    }
  return 0;
}

int
FTP_Server_Callback::handle_end_stream (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_SFP_Callback::end_stream\n"));
  CORBA::ORB_var orb = TAO_AV_CORE::instance ()->orb ();
  orb->shutdown ();
  return 0;
}

// FTP_Server_FDev::FTP_Server_FDev (void)
//   :TAO_FDev ("Data")
// {
// }

// AVStreams::FlowConsumer_ptr
// FTP_Server_FDev::make_consumer (AVStreams::FlowConnection_ptr the_requester,
//                                 AVStreams::QoS & the_qos,
//                                 CORBA::Boolean_out met_qos,
//                                 char *& named_fdev,
//                                 ACE_ENV_SINGLE_ARG_DECL)
// {
//   ACE_DEBUG ((LM_DEBUG,"FTP_Server_FDev::make_consumer"));
//   FTP_Server_FlowEndPoint *endpoint;
//   ACE_NEW_RETURN (endpoint,
//                   FTP_Server_FlowEndPoint,
//                   0);
//   return endpoint->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
//   ACE_CHECK_RETURN (0);
// }

Server::Server (void)
{
  reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                           TAO_AV_CORE::instance ()->poa ());
}

AVStreams::protocolSpec
Server::protocols (void)
{
  AVStreams::protocolSpec protocols (2);
  protocols.length (2);
  protocols [0] = CORBA::string_dup ("TCP");
  protocols [1] = CORBA::string_dup ("UDP");
  return protocols;
}

const char*
Server::format (void)
{
  return "UNS:ftp";
}

int
Server::init (int argc,
              char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      PortableServer::POAManager_var mgr
        = TAO_AV_CORE::instance ()->poa ()->the_POAManager ();

      mgr->activate ();

      int result = this->parse_args (argc,argv);
      if (result == -1)
        ACE_ERROR_RETURN  ((LM_ERROR,"parse args failed\n"),-1);
      // Initialize the naming services

      if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      // Register the video mmdevice object with the ORB
      ACE_NEW_RETURN (this->mmdevice_,
                      TAO_MMDevice (&this->reactive_strategy_),
                      -1);

      ACE_NEW_RETURN (this->fdev_,
                      FTP_Server_FDev,
                      -1);
      this->fdev_->flowname ("Data");
      AVStreams::MMDevice_var mmdevice = this->mmdevice_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      AVStreams::FDev_var fdev = this->fdev_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      mmdevice->add_fdev (fdev.in ()
                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Register the mmdevice with the naming service.
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice1");
      ACE_TRY_EX (bind)
        {
          // Register the video control object with the naming server.
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (bind);
        }
      ACE_CATCH (CosNaming::NamingContext::AlreadyBound,al_ex)
        {
          server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice2");
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (-1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"server::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
Server::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_AV_CORE::instance ()->orb ()->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"server::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
Server::parse_args (int argc,char **argv)
{
  ACE_Get_Opt opts (argc,argv,"f:p:");

  int c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->fp_ = ACE_OS::fopen (opts.opt_arg (),"w");
          if (this->fp_ != 0)
            {
              ACE_DEBUG ((LM_DEBUG,"file opened successfully\n"));
            }
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,"Usage: server -f filename"),-1);

        }
    }
  return 0;
}

FILE*
Server::file (void)
{
  return this->fp_;
}

int
main (int argc,
      char **argv)
{

  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv);
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      TAO_AV_CORE::instance ()->init (orb.in (),
                                      poa.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"server::init");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  int result = 0;
  result = FTP_SERVER::instance ()->init (argc,argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_SERVER::init failed\n"),1);
  result = FTP_SERVER::instance ()->run ();
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_SERVER::run failed\n"),1);

  return result;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Server,ACE_Null_Mutex>;
template class TAO_AV_Endpoint_Reactive_Strategy_B<TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl>;
template class TAO_FDev<TAO_FlowProducer, FTP_Server_FlowEndPoint>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Server,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B<TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<TAO_StreamEndPoint_B, TAO_VDev, AV_Null_MediaCtrl>
#pragma instantiate TAO_FDev<TAO_FlowProducer, FTP_Server_FlowEndPoint>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
