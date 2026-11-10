#include "ftp.h"
#include <cstdio>

void
FTP_Client_Callback::get_timeout (ACE_Time_Value *&tv,
                                  void *&)
{
  tv = &this->timeout_;
}

int
FTP_Client_Callback::handle_timeout (void *)
{
  try
    {
      ACE_Message_Block mb (BUFSIZ);
      ACE_DEBUG ((LM_DEBUG, "FTP_Client_Callback::handle_timeout\n"));
      std::size_t const bytes_read =
        ACE_OS::fread (mb.wr_ptr (), 1, mb.size (), CLIENT::instance ()->file ());

      if (bytes_read == 0)
        {
          if (std::feof (CLIENT::instance ()->file ()))
            {
              // wait for sometime for the data to be flushed to the other side.
              this->count_++;
              if (this->count_ == 2)
                {
                  ACE_DEBUG ((LM_DEBUG, "handle_timeout:End of file\n"));
                  AVStreams::flowSpec stop_spec (1);
                  CLIENT::instance ()->streamctrl ()->stop (stop_spec);
                  TAO_AV_CORE::instance ()->orb ()->shutdown (0);
                  return 0;
                }
              else
                return 0;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR, "FTP_Client_Flow_Handler::fread error\n"), -1);
        }
      mb.wr_ptr (bytes_read);
      int const result = this->protocol_object_->send_frame (&mb);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "FTP_Client_Flow_Handler::send failed\n"), -1);
      ACE_DEBUG ((LM_DEBUG, "handle_timeout::buffer sent successfully\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception (
        "FTP_Client_Callback::handle_timeout Failed");
      return -1;
    }
  return 0;
}

FTP_Client_Producer::FTP_Client_Producer ()
  : TAO_FlowProducer ("Data", CLIENT::instance ()->protocols (), CLIENT::instance ()->format ())
{
}

int
FTP_Client_Producer::get_callback (char const *,
                                   TAO_AV_Callback *&callback)
{
  callback = &CLIENT::instance ()->callback ();
  return 0;
}

int
Client::parse_args (int argc,
                    ACE_TCHAR *argv[])
{
  ACE_Get_Opt opts (argc, argv, ACE_TEXT("f:l:a:p:s"));

  int c {};
  bool peer_address_set {};
  bool local_address_set {};
  while ((c = opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->options_.filename = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 'l':
          this->options_.address = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          local_address_set = true;
          break;
        case 'a':
          this->options_.peer_addr = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          peer_address_set = true;
          break;
        case 'p':
          this->options_.protocol = ACE_TEXT_ALWAYS_CHAR (opts.opt_arg ());
          break;
        case 's':
          this->options_.use_sfp = true;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG, "Unknown option\n"));
          return -1;
        }
    }
  if (!local_address_set)
    {
      char buf [BUFSIZ] {};
      ACE_OS::hostname (buf,
                        BUFSIZ);
      this->options_.address = buf;
      this->options_.address += ":5000";
    }

  if (!peer_address_set)
    {
      char buf [BUFSIZ] {};
      ACE_OS::hostname (buf,
                        BUFSIZ);
      this->options_.peer_addr = buf;
      this->options_.peer_addr += ":5050";
    }

  return 0;
}

FILE *
Client::file () const
{
  return this->file_.get ();
}

FTP_Client_Callback &
Client::callback ()
{
  return this->callback_;
}

AVStreams::protocolSpec
Client::protocols () const
{
  AVStreams::protocolSpec protocols (1);
  protocols.length (1);
  std::string const protocol = this->options_.protocol + "=" + this->options_.address;
  protocols [0] = CORBA::string_dup (protocol.c_str ());
  return protocols;
}

char const *
Client::format () const
{
  return "UNS:ftp";
}

TAO_StreamCtrl *
Client::streamctrl ()
{
  return &this->streamctrl_;
}

Client::Client ()
  : file_ (nullptr, ACE_OS::fclose),
    orb_ (CORBA::ORB::_duplicate (TAO_AV_CORE::instance ()->orb ()))
{
  endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                           TAO_AV_CORE::instance ()->poa ());
}

int
Client::init (int argc, ACE_TCHAR *argv[])
{
  try
    {
      PortableServer::POAManager_var mgr
        = TAO_AV_CORE::instance ()->poa ()->the_POAManager ();

      mgr->activate ();

      if (this->parse_args (argc, argv) != 0)
        return -1;

      TAO_StreamEndPoint_A *streamendpoint_a {};
      ACE_NEW_RETURN (streamendpoint_a, TAO_StreamEndPoint_A, -1);
      PortableServer::ServantBase_var safe_streamendpoint_a = streamendpoint_a;

      FTP_Client_Producer *fep_a {};
      ACE_NEW_RETURN (fep_a, FTP_Client_Producer, -1);
      PortableServer::ServantBase_var safe_fep_a = fep_a;

      this->sep_a_ = streamendpoint_a->_this ();

      this->fep_a_obj_ = fep_a->_this ();

      CORBA::String_var s1 = this->sep_a_->add_fep (this->fep_a_obj_.in ());

      ACE_DEBUG ((LM_DEBUG, "(%N,%l) Added flowendpoint named: %C\n", s1.in ()));

      this->file_.reset (ACE_OS::fopen (this->options_.filename.c_str (), "r"));

      if (!this->file_)
        ACE_ERROR_RETURN ((LM_ERROR, "ERROR: file %C could not be opened\n",
                           this->options_.filename.c_str ()), -1);

      ACE_DEBUG ((LM_DEBUG, "file opened successfully\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client::init\n");
      return -1;
    }
  return 0;
}

int
Client::run ()
{
  try
    {
      char const *flow_protocol = this->options_.use_sfp ? "sfp:1.0" : "";
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      AVStreams::flowSpec flow_spec (1);
      flow_spec.length (1);

      ACE_INET_Addr addr (this->options_.address.c_str ());

      TAO_Forward_FlowSpec_Entry entry ("Data",
                                        "IN",
                                        "USER_DEFINED",
                                        flow_protocol,
                                        this->options_.protocol.c_str (),
                                        &addr);

      ACE_INET_Addr peer_addr (this->options_.peer_addr.c_str ());
      entry.set_peer_addr (&peer_addr);

      flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

      ACE_High_Res_Timer timer;
      ACE_Time_Value elapsed {};
      timer.start ();

      CORBA::Object_var obj_b =
        this->orb_->string_to_object ("corbaname:rir:#Server_StreamEndPoint_b");
      AVStreams::StreamEndPoint_B_var sep_b =
        AVStreams::StreamEndPoint_B::_narrow (obj_b.in ());

      CORBA::Boolean const result =
        this->streamctrl_.bind (this->sep_a_.in (),
                                sep_b.in (),
                                the_qos.inout (),
                                flow_spec);

      timer.stop ();
      timer.elapsed_time (elapsed);
      elapsed.dump ();
      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "streamctrl::bind failed\n"), -1);

      AVStreams::flowSpec start_spec (1);
      this->streamctrl_.start (start_spec);

      // Schedule a timer for the for the flow handler.
      ACE_Time_Value tv (10000, 0);
      this->orb_->run (tv);

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client::run\n");
      return -1;
    }
  return 0;
}

int
ACE_TMAIN (int argc,
           ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      TAO_AV_CORE::instance ()->init (orb.in (), poa.in ());

      int result = CLIENT::instance ()->init (argc, argv);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "client::init failed\n"), 1);
      result = CLIENT::instance ()->run ();

      poa->destroy (true, true);

      orb->destroy ();

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR, "client::run failed\n"), 1);
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Client Failed\n");
      return -1;
    }

  CLIENT::close ();  // Explicitly finalize the Unmanaged_Singleton.

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex> *ACE_Unmanaged_Singleton<Client, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
