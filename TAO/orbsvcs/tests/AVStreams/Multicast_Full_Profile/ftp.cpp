#include "ftp.h"
#include "tao/debug.h"

FTP_Client_Callback::FTP_Client_Callback ()
  :count_ (0)
{
}

int
FTP_Client_Callback::handle_end_stream ()
{
  TAO_AV_CORE::instance ()->orb ()->shutdown ();
  return 0;
}

void
FTP_Client_Callback::get_timeout (ACE_Time_Value *&tv,
                                  void *&)
{
  ACE_Time_Value *timeout;
  ACE_NEW (timeout,
           ACE_Time_Value(2));
  tv = timeout;
}

int
FTP_Client_Callback::handle_timeout (void *)
{
  ACE_Message_Block mb (BUFSIZ);
  ACE_DEBUG ((LM_DEBUG,"FTP_Client_Callback::get_frame\n"));
  char *buf = mb.rd_ptr ();

  int n = ACE_OS::fread(buf,1,mb.size (),CLIENT::instance ()->file ());
  if (n < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread end of file\n"),-1);
    }
  if (n == 0)
    {
      if (feof (CLIENT::instance ()->file ()))
        {
          // wait for sometime for the data to be flushed to the other side.
          this->count_++;
          if (this->count_ == 2)
            {
              try
                {
                  ACE_DEBUG ((LM_DEBUG,"handle_timeout:End of file\n"));
                  AVStreams::flowSpec stop_spec (1);
                  ACE_DEBUG ((LM_DEBUG, "Just before Orb Shutdown\n"));
                  TAO_AV_CORE::instance ()->orb ()->shutdown (0);
                  return 0;
                }
              catch (const CORBA::Exception& ex)
                {
                  ex._tao_print_exception (
                    "FTP_Client_Callback::handle_timeout\n");
                  return -1;
                }
            }
          else
            return 0;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,"FTP_Client_Flow_Handler::fread error\n"),-1);
    }
  mb.wr_ptr (n);
  int result = this->protocol_object_->send_frame (&mb);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"send failed:%p","FTP_Client_Flow_Handler::send\n"),-1);
  ACE_DEBUG ((LM_DEBUG,"handle_timeout::buffer sent successfully\n"));
  return 0;
}

FTP_Client_Producer::FTP_Client_Producer ()
  :TAO_FlowProducer ("Data",CLIENT::instance ()->protocols (),CLIENT::instance ()->format ())
{
}

int
FTP_Client_Producer::set_protocol_object (const char *,
                                          TAO_AV_Protocol_Object *object)
{
  this->callback_->set_protocol_object (object);
  return 0;
}

int
FTP_Client_Producer::get_callback (const char *,
                                   TAO_AV_Callback *&callback)
{
   ACE_NEW_RETURN (this->callback_,
                   FTP_Client_Callback,
                   -1);
   callback = this->callback_;
   return 0;
}

int
Client::parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:a:p:sd"));

  this->use_sfp_ = 0;
  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = opts.opt_arg ();
          break;
        case 'a':
          this->address_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ()));
          break;
        case 's':
          this->use_sfp_ = 1;
          break;
        case 'd':
          TAO_debug_level++;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown option\n"));
          return -1;
        }
    }
  return 0;
}

FILE *
Client::file ()
{
  return this->fp_;
}

char*
Client::flowname ()
{
  return this->flowname_;
}

AVStreams::protocolSpec
Client::protocols ()
{
  AVStreams::protocolSpec protocols (1);
  protocols.length (1);
  char buf [BUFSIZ];
  ACE_OS::sprintf (buf, "%s=%s", this->protocol_, this->address_);
  protocols [0] = CORBA::string_dup (buf);
  return protocols;
}

const char *
Client::format ()
{
  return "UNS:ftp";
}

const char *
Client::address ()

{
  return this->address_;
}

TAO_StreamCtrl*
Client::streamctrl ()
{
  return &this->streamctrl_;
}

Client::Client ()
  : client_mmdevice_ (&endpoint_strategy_),
    fdev_ (0),
    address_ (ACE_OS::strdup ("224.9.9.2:10002")),
    fp_ (0),
    protocol_ (ACE_OS::strdup ("UDP"))
{
  endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (), TAO_AV_CORE::instance ()->poa ());
}


int
Client::bind_to_server (const char *name)
{
  try
    {
      // Initialize the naming services
      if (my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      CosNaming::Name server_mmdevice_name (1);
      server_mmdevice_name.length (1);
      server_mmdevice_name [0].id = name;
      CORBA::Object_var server_mmdevice_obj =
        my_naming_client_->resolve (server_mmdevice_name);

      this->server_mmdevice_ =
        AVStreams::MMDevice::_narrow (server_mmdevice_obj.in ());

      if (CORBA::is_nil (this->server_mmdevice_.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " could not resolve Server_Mmdevice in Naming service <%C>\n",
                           name),
                          -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
        "Command_Handler::resolve_reference");
      return -1;
    }
  return 0;
}

int
Client::init (int argc, ACE_TCHAR *argv[])
{
  this->argc_ = argc;
  this->argv_ = argv;

  try
    {
      PortableServer::POAManager_var mgr
        = TAO_AV_CORE::instance ()->poa ()->the_POAManager ();

      mgr->activate ();

      this->parse_args (this->argc_, this->argv_);

      ACE_DEBUG ((LM_DEBUG, "(%N,%l) Parsed Address  %C\n", this->address_));

      ACE_NEW_RETURN (this->fdev_,
                      FTP_Client_FDev,
                      -1);

      ACE_NEW_RETURN (this->flowname_,
                      char [BUFSIZ],
                      0);
      ACE_OS::sprintf (this->flowname_,
                       "Data");
      this->fdev_->flowname (this->flowname ());
      AVStreams::MMDevice_var mmdevice = this->client_mmdevice_._this ();
      AVStreams::FDev_var fdev = this->fdev_->_this ();
      mmdevice->add_fdev (fdev.in ());

      if (this->my_naming_client_.init (TAO_AV_CORE::instance ()->orb ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize "
                           "the TAO_Naming_Client.\n"),
                          -1);

      this->fp_ = ACE_OS::fopen (this->filename_, "r");
      if (this->fp_ != 0)
        {
          ACE_DEBUG ((LM_DEBUG,"file opened successfully\n"));
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client::init");
      return -1;
    }
  return 0;
}

int
Client::run ()
{
  try
    {
      char flow_protocol_str [BUFSIZ];
      if (this->use_sfp_)
        ACE_OS::strcpy (flow_protocol_str, "sfp:1.0");
      else
        ACE_OS::strcpy (flow_protocol_str, "");
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      AVStreams::flowSpec flow_spec (1);
      // Bind the client and server mmdevices.
      ACE_DEBUG ((LM_DEBUG, "(%N,%l) Parsed Address %C\n", this->address_));
      ACE_INET_Addr *addr = new ACE_INET_Addr(this->address_);
      TAO_Forward_FlowSpec_Entry entry (this->flowname_,
                                        "IN",
                                        "USER_DEFINED",
                                        flow_protocol_str,
                                        this->protocol_,
                                        addr);
      ACE_DEBUG ((LM_DEBUG, "(%N,%l) flowspec: %C\n", entry.entry_to_string() ));
      flow_spec.length (1);
      flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

      AVStreams::MMDevice_var client_mmdevice =
        this->client_mmdevice_._this ();

      CORBA::Boolean result =
        this->streamctrl_.bind_devs (client_mmdevice.in (),
                                     AVStreams::MMDevice::_nil (),
                                     the_qos.inout (),
                                     flow_spec);

      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind_devs for client_mmdevice failed\n"),-1);
      if (this->bind_to_server ("Server_MMDevice1") == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
      result = this->streamctrl_.bind_devs (AVStreams::MMDevice::_nil (),
                                            this->server_mmdevice_.in (),
                                            the_qos.inout (),
                                            flow_spec);

      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"(%N,%l) streamctrl::bind_devs for mmdevice 1 failed\n"),-1);
      if (this->bind_to_server ("Server_MMDevice2") == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Error binding to the naming service\n"),
                          -1);
      result = this->streamctrl_.bind_devs (AVStreams::MMDevice::_nil (),
                                            this->server_mmdevice_.in (),
                                            the_qos.inout (),
                                            flow_spec);

      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"(%N,%l) streamctrl::bind_devs for mmdevice 2 failed\n"),-1);
      AVStreams::flowSpec start_spec (1);
      start_spec.length (1);
      start_spec [0] = CORBA::string_dup (this->flowname_);
      this->streamctrl_.start (start_spec);
      // Schedule a timer for the for the flow handler.
      //TAO_AV_CORE::instance ()->run ();

      ACE_Time_Value tv (10000,0);
      TAO_AV_CORE::instance ()->orb ()->run (tv);

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      ACE_DEBUG ((LM_DEBUG, "Exited the TAO_AV_Core::run\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Client::run");
      return -1;
    }
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

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

  int result = 0;
  result = CLIENT::instance ()->init (argc, argv);
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"client::init failed\n"),1);
  result = CLIENT::instance ()->run ();
  if (result < 0)
    ACE_ERROR_RETURN ((LM_ERROR,"client::run failed\n"),1);

  CLIENT::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return result;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
