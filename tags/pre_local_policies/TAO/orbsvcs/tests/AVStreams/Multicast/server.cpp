// $Id$

#include "server.h"

FTP_Server_StreamEndPoint::FTP_Server_StreamEndPoint (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::FTP_Server_StreamEndPoint"));
}

int
FTP_Server_StreamEndPoint::get_callback (const char *,
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
  CORBA::ORB_var orb = TAO_AV_CORE::instance ()->orb_manager ()->orb ();
  orb->shutdown ();
  return 0;
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
      server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice1");
          AVStreams::MMDevice_var mmdevice = this->mmdevice_->_this (ACE_TRY_ENV);
      ACE_TRY_EX (bind)
        {
          // Register the video control object with the naming server.
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice.in (),
                                         ACE_TRY_ENV);
          ACE_TRY_CHECK_EX (bind);
        }
      ACE_CATCH (CosNaming::NamingContext::AlreadyBound,al_ex)
        {
          server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice2");
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice.in (),
                                         ACE_TRY_ENV);
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
  result = FTP_SERVER::instance ()->init (argc,argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_SERVER::init failed\n"),1);
  result = FTP_SERVER::instance ()->run ();
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_SERVER::run failed\n"),1);
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
