// $Id$

#include "server.h"

FTP_Server_StreamEndPoint::FTP_Server_StreamEndPoint (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::FTP_Server_StreamEndPoint"));
}

int
FTP_Server_StreamEndPoint::make_udp_flow_handler (TAO_AV_UDP_Flow_Handler *&handler)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::make_udp_flow_handler"));
  ACE_NEW_RETURN (handler,
                  FTP_Server_UDP_Flow_Handler,
                  -1);
}

int
FTP_Server_StreamEndPoint::make_tcp_flow_handler (TAO_AV_TCP_Flow_Handler *&handler)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::make_tcp_flow_handler"));
  ACE_NEW_RETURN (handler,
                  FTP_Server_TCP_Flow_Handler,
                  -1);
}

int
FTP_Server_StreamEndPoint::make_dgram_mcast_flow_handler (TAO_AV_UDP_MCast_Flow_Handler *&handler)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::make_udp_flow_handler"));
  ACE_NEW_RETURN (handler,
                  FTP_Server_UDP_MCast_Flow_Handler,
                  -1);
}


int
FTP_Server_StreamEndPoint::get_sfp_callback (const char *flowname,
                                             TAO_SFP_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::get_sfp_callback\n"));
  ACE_NEW_RETURN (callback,
                  FTP_SFP_Callback,
                  -1);
  return 0;
}


FTP_Server_Flow_Handler::FTP_Server_Flow_Handler (void)
{
}

int
FTP_Server_Flow_Handler::start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Flow_Handler::start"));
  return 0;
}

int
FTP_Server_Flow_Handler::stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::stop"));
  ACE_OS::fclose (SERVER::instance ()->file ());
  CORBA::ORB_var orb = TAO_AV_CORE::instance ()->orb_manager ()->orb ();
  orb->shutdown ();
  return 0;
}

int
FTP_Server_Flow_Handler::input (ACE_HANDLE fd)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::handle_input"));
  char buf[BUFSIZ];
  int result = this->transport_->recv (buf,
                                       BUFSIZ);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_Server_Flow_Handler::recv failed\n"),-1);
  result = ACE_OS::fwrite (buf,BUFSIZ,1,SERVER::instance ()->file ());
  if (result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_Server_Flow_Handler::fwrite failed\n"),-1);
  return 0;
}

int
FTP_Server_UDP_Flow_Handler::handle_input (ACE_HANDLE fd)
{
  return FTP_Server_Flow_Handler::input (fd);
}

int
FTP_Server_TCP_Flow_Handler::handle_input (ACE_HANDLE fd)
{
  return FTP_Server_Flow_Handler::input (fd);
}

int
FTP_Server_UDP_MCast_Flow_Handler::handle_input (ACE_HANDLE fd)
{
  return FTP_Server_Flow_Handler::input (fd);
}

int
FTP_Server_TCP_Flow_Handler::get_handle (void) const
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_TCP_Flow_Handler::get_handle\n"));
  return this->peer ().get_handle ();
}

int
FTP_Server_TCP_Flow_Handler::start (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_TCP_Flow_Handler::start "));
  ACE_HANDLE handle = this->peer ().get_handle ();
  ACE_Event_Handler *handler = 0;
  int result = TAO_AV_CORE::instance ()->orb_manager ()->orb ()->orb_core ()->reactor ()->handler (handle,
                                                                                                   ACE_Event_Handler::READ_MASK,
                                                                                                   &handler);
  if (handler == 0)
    ACE_DEBUG ((LM_DEBUG,"FTP_Server_Flow_Handler::start:handler is null\n"));
  return 0;
}


int
FTP_SFP_Callback::start_failed (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_SFP_Callback::start_failed\n"));
  return 0;
}

int
FTP_SFP_Callback::stream_established (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_SFP_Callback::stream_established\n"));
  return 0;
}

int
FTP_SFP_Callback::receive_frame (ACE_Message_Block *frame)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_SFP_Callback::receive_frame\n"));
  while (frame != 0)
    {
      int result = ACE_OS::fwrite (frame->rd_ptr (),
                                   frame->length (),
                                   1,
                                   SERVER::instance ()->file ());
      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Server_Flow_Handler::fwrite failed\n"),-1);
      frame = frame->cont ();
    }
  return 0;
}

void
FTP_SFP_Callback::end_stream (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_SFP_Callback::end_stream\n"));
  ACE_OS::fclose (SERVER::instance ()->file ());
  CORBA::ORB_var orb = TAO_AV_CORE::instance ()->orb_manager ()->orb ();
  orb->shutdown ();
}

Server::Server (void)
  :orb_manager_ (TAO_AV_CORE::instance ()->orb_manager ()),
   reactive_strategy_ (orb_manager_)
{
}

int
Server::init (int argc,
              char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_AV_CORE::instance ()->init (argc,
                                      argv,
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;
      this->orb_manager_ = 
        TAO_AV_CORE::instance ()->orb_manager ();

      // Initialize the orb_manager
      this->orb_manager_->init_child_poa (argc,
                                         argv,
                                         "child_poa",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ORB_var orb =
        this->orb_manager_->orb ();

      PortableServer::POA_var child_poa =
        this->orb_manager_->child_poa ();


      int result = this->parse_args (argc,argv);
      if (result == -1)
        ACE_ERROR_RETURN  ((LM_ERROR,"parse args failed\n"),-1);
      // Initialize the naming services

      if (my_naming_client_.init (orb.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client. \n"),
                          -1);

      // Register the video mmdevice object with the ORB
      ACE_NEW_RETURN (this->mmdevice_,
                      TAO_MMDevice (&this->reactive_strategy_),
                      -1);
      // create the video server mmdevice with the naming service pointer.
      this->orb_manager_->activate_under_child_poa ("Server_MMDevice",
                                                   this->mmdevice_,
                                                   ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Register the mmdevice with the naming service.
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice");

      // Register the video control object with the naming server.
      this->my_naming_client_->rebind (server_mmdevice_name,
                                     this->mmdevice_->_this (ACE_TRY_ENV),
                                     ACE_TRY_ENV);
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
Server::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_manager_->run (ACE_TRY_ENV);
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

  char c;
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->fp_ = ACE_OS::fopen (opts.optarg,"w");
          if (this->fp_ != 0)
            {
              ACE_DEBUG ((LM_DEBUG,"file opened successfully\n"));
            }
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (opts.optarg);
        default:
          ACE_ERROR_RETURN ((LM_ERROR,"Usage: server -f filename"),-1);
          break;
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
  int result = 0;
  result = SERVER::instance ()->init (argc,argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"SERVER::init failed\n"),1);
  result = SERVER::instance ()->run ();
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"SERVER::run failed\n"),1);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Server,ACE_Null_Mutex>;
template class TAO_AV_Endpoint_Reactive_Strategy_B <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Server,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_B <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy <FTP_Server_StreamEndPoint,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
