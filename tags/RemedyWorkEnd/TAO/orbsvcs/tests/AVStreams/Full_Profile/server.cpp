// $Id$

#include "server.h"

FTP_Server_FlowEndPoint::FTP_Server_FlowEndPoint (void)
  :TAO_FlowConsumer ("Data",FTP_SERVER::instance ()->protocols (), FTP_SERVER::instance ()->format ())
{
  AVStreams::protocolSpec protocols (2);
  protocols.length (3);
  protocols [0] = CORBA::string_dup ("TCP");
  protocols [1] = CORBA::string_dup ("UDP");
  protocols [2] = CORBA::string_dup ("RTP/UDP");
  try
    {
      this->set_protocol_restriction (protocols);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "FTP_Server_FlowEndPoint::FTP_Server_FlowEndPoint\n");
    }
}

int
FTP_Server_FlowEndPoint::get_callback (const char *,
                                       TAO_AV_Callback *&callback)
{
  ACE_NEW_RETURN (callback,
                  FTP_Server_Callback,
                  -1);
  return 0;
}

FTP_Server_Callback::FTP_Server_Callback (void)
{
}

int
FTP_Server_Callback::handle_stop (void)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Callback::stop\n"));
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
  CORBA::ORB_var orb = TAO_AV_CORE::instance ()->orb ();
  orb->shutdown ();
  return 0;
}

Server::Server (void)
  :orb_ (TAO_AV_CORE::instance ()->orb ()),
   poa_ (TAO_AV_CORE::instance ()->poa ())
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
              ACE_TCHAR *argv[])
{
  try
    {
      PortableServer::POAManager_var mgr
        = this->poa_->the_POAManager ();

      mgr->activate ();

      int result = this->parse_args (argc, argv);
      if (result == -1)
        ACE_ERROR_RETURN  ((LM_ERROR,"parse args failed\n"),-1);

      // Initialize the naming services
      if (my_naming_client_.init (this->orb_.in ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      ACE_NEW_RETURN (this->streamendpoint_b_, TAO_StreamEndPoint_B, -1);

      ACE_NEW_RETURN (this->fep_b_, FTP_Server_FlowEndPoint, -1);

      sep_b_ = this->streamendpoint_b_->_this ();

      fep_b_obj_ = this->fep_b_->_this ();

      CORBA::String_var s1 = sep_b_->add_fep( fep_b_obj_.in() );

     ACE_DEBUG ((LM_DEBUG, "(%N,%l) Added flowendpoint named: %C\n", s1.in() ));


      // Register the mmdevice with the naming service.
      CosNaming::Name server_sep_b_name (1);
      server_sep_b_name.length (1);
      server_sep_b_name [0].id = CORBA::string_dup ("Server_StreamEndPoint_b");

      // Register the video control object with the naming server.
      this->my_naming_client_->rebind (server_sep_b_name,
                                       sep_b_.in ());


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
  try
    {
      this->orb_->run ();
    }
    catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server::run\n");
      return -1;
    }
  return 0;
}

int
Server::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:p:"));

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
          ACE_ERROR_RETURN ((LM_ERROR,"Usage: server -f filename\n"),-1);
        }
    }

    if( ! this->fp_ )
    {
       ACE_ERROR_RETURN ((LM_ERROR, "Invalid file!\nUsage: server -f filename\n"),-1);
    }

  return 0;
}

FILE*
Server::file (void)
{
  return this->fp_;
}

int
ACE_TMAIN (int argc,
      ACE_TCHAR *argv[])
{
  int result = 0;

  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                        argv);
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
    ACE_ERROR_RETURN ((LM_ERROR,"SERVER::init failed\n"),1);

  result = FTP_SERVER::instance ()->run ();

  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"SERVER::run failed\n"),1);

  FTP_SERVER::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Server, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
