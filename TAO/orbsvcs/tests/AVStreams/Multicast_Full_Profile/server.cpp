#include "server.h"

FTP_Server_FlowEndPoint::FTP_Server_FlowEndPoint ()
  : TAO_FlowConsumer ("Data",
                      FTP_SERVER::instance ()->protocols (),
                      FTP_SERVER::instance ()->format ())
{
  AVStreams::protocolSpec protocols (2);
  protocols.length (2);
  protocols [0] = CORBA::string_dup ("TCP");
  protocols [1] = CORBA::string_dup ("UDP");
  try
    {
      this->set_protocol_restriction (protocols);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "FTP_Server_FlowEndPoint::FTP_Server_FlowEndPoint");
    }
}

FTP_Server_FlowEndPoint::~FTP_Server_FlowEndPoint () = default;

int
FTP_Server_FlowEndPoint::get_callback (char const *,
                                       TAO_AV_Callback *&callback)
{
  ACE_DEBUG ((LM_DEBUG, "FTP_Server_FlowEndPoint::get_callback\n"));
  if (!this->callback_)
    this->callback_ = std::make_unique<FTP_Server_Callback> ();
  callback = this->callback_.get ();
  return 0;
}

int
FTP_Server_Callback::handle_stop ()
{
  ACE_DEBUG ((LM_DEBUG, "FTP_Server_Callback::stop\n"));
  FTP_SERVER::instance ()->close_file ();
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}

int
FTP_Server_Callback::receive_frame (ACE_Message_Block *frame,
                                    TAO_AV_frame_info *,
                                    ACE_Addr const &)
{
  ACE_DEBUG ((LM_DEBUG,"FTP_Server_Callback::receive_frame\n"));
  while (frame != nullptr)
    {
      std::size_t const written = ACE_OS::fwrite (frame->rd_ptr (),
                                                  frame->length (),
                                                  1,
                                                  FTP_SERVER::instance ()->file ());
      if (written != 1)
        ACE_ERROR_RETURN ((LM_ERROR, "FTP_Server_Flow_Handler::fwrite failed\n"), -1);
      frame = frame->cont ();
    }
  return 0;
}

int
FTP_Server_Callback::handle_end_stream ()
{
  ACE_DEBUG ((LM_DEBUG,"FTP_SFP_Callback::end_stream\n"));
  CORBA::ORB_var orb = TAO_AV_CORE::instance ()->orb ();
  orb->shutdown ();
  return 0;
}

Server::Server () = default;

AVStreams::protocolSpec
Server::protocols ()
{
  AVStreams::protocolSpec protocols (2);
  protocols.length (2);
  protocols [0] = CORBA::string_dup ("TCP");
  protocols [1] = CORBA::string_dup ("UDP");
  return protocols;
}

char const *
Server::format ()
{
  return "UNS:ftp";
}

int
Server::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableServer::POAManager_var mgr
        = TAO_AV_CORE::instance ()->poa ()->the_POAManager ();

      mgr->activate ();

      int result = this->parse_args (argc, argv);
      if (result == -1)
        ACE_ERROR_RETURN  ((LM_ERROR,"parse args failed\n"),-1);

      this->fp_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "w"));
      if (!this->fp_)
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to open output file\n"), -1);

      result = this->reactive_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                                              TAO_AV_CORE::instance ()->poa ());
      if (result != 0)
        return result;

      // Initialize the naming services
      if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      // Register the video mmdevice object with the ORB
      TAO_MMDevice *mmdevice {};
      ACE_NEW_RETURN (mmdevice,
                      TAO_MMDevice (&this->reactive_strategy_),
                      -1);
      PortableServer::ServantBase_var safe_mmdevice = mmdevice;

      FTP_Server_FDev *fdev {};
      ACE_NEW_RETURN (fdev,
                      FTP_Server_FDev,
                      -1);
      PortableServer::ServantBase_var safe_fdev = fdev;
      fdev->flowname ("Data");
      AVStreams::MMDevice_var mmdevice_ref = mmdevice->_this ();
      AVStreams::FDev_var fdev_ref = fdev->_this ();
      mmdevice_ref->add_fdev (fdev_ref.in ());

      // Register the mmdevice with the naming service.
      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice1");
      try
        {
          // Register the video control object with the naming server.
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice_ref.in ());
        }
      catch (CosNaming::NamingContext::AlreadyBound const &)
        {
          server_mmdevice_name [0].id = CORBA::string_dup ("Server_MMDevice2");
          this->my_naming_client_->bind (server_mmdevice_name,
                                         mmdevice_ref.in ());
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server::init");
      return -1;
    }
  return 0;
}

int
Server::run ()
{
  try
    {
      TAO_AV_CORE::instance ()->orb ()->run ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server::init");
      return -1;
    }
  return 0;
}

int
Server::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT ("f:"));

  int c {};
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->options_.filename = opts.opt_arg ();
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,"Usage: server -f filename"),-1);
        }
    }
  return 0;
}

FILE *
Server::file ()
{
  return this->fp_.get ();
}

void
Server::close_file ()
{
  this->fp_.reset ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

  try
    {
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      TAO_AV_CORE::instance ()->init (orb.in (),
                                      poa.in ());
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server::init");
      return -1;
    }

  int result = FTP_SERVER::instance ()->init (argc, argv);
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
