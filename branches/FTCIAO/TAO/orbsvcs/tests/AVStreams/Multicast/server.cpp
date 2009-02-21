// $Id$

#include "server.h"

int done = 0;

FTP_Server_StreamEndPoint::FTP_Server_StreamEndPoint (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::FTP_Server_StreamEndPoint\n"));
}

int
FTP_Server_StreamEndPoint::get_callback (const char *,
                                         TAO_AV_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_StreamEndPoint::get_callback\n"));
  ACE_NEW_RETURN (callback,
                  FTP_Server_Callback,
                  -1);
  return 0;
}

int
FTP_Server_Callback::handle_stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Callback::stop\n"));
  ACE_OS::fclose (FTP_SERVER::instance ()->file ());
  done = 1;
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
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Callback::end_stream\n"));
  done = 1;
  return 0;
}

Server::Server (void)
{
  reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                      TAO_AV_CORE::instance ()->poa ());
}

int
Server::init (int argc,
              ACE_TCHAR *argv[])
{
  try
    {

      PortableServer::POAManager_var mgr
        = TAO_AV_CORE::instance ()->poa ()->the_POAManager ();

      mgr->activate ();

      int result = this->parse_args (argc, argv);
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

      // Register the mmdevice with the naming service.
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice1");
      AVStreams::MMDevice_var mmdevice = this->mmdevice_->_this ();

      try
        {
          // Register the video control object with the naming server.
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice.in ());
        }
      catch (const CosNaming::NamingContext::AlreadyBound&)
        {
          server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice2");
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice.in ());
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server::init");
      return -1;
    }
  return 0;
}

int
Server::run (void)
{
  CORBA::ORB_ptr orb = TAO_AV_CORE::instance ()->orb();
  try
    {

      while( !done )
      {
        CORBA::Boolean wp = orb->work_pending ();
        if (wp)
         {
              orb->perform_work( );
         }
      }

      orb->shutdown( 1 );
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server::init");
      return -1;
    }


  return 0;
}

int
Server::parse_args (int argc, ACE_TCHAR *argv[])
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
          this->protocol_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv);


  try
    {
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      TAO_AV_CORE::instance ()->init (orb.in (),
                                      poa.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server::init");
      return -1;
    }

  result = FTP_SERVER::instance ()->init (argc, argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_SERVER::init failed\n"),1);
  result = FTP_SERVER::instance ()->run ();
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"FTP_SERVER::run failed\n"),1);

  FTP_SERVER::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
