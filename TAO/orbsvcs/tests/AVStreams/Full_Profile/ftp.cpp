// $Id$

#include "ftp.h"


FTP_Client_Callback::FTP_Client_Callback (void)
  :count_ (0)
{
}

int
FTP_Client_Callback::handle_end_stream (void)
{
  TAO_AV_CORE::instance ()->stop_run ();
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
  ACE_TRY_NEW_ENV
    {
      ACE_Message_Block mb (BUFSIZ);
      ACE_DEBUG ((LM_DEBUG,"FTP_Client_Callback::handle_timeout\n"));
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
                  ACE_DEBUG ((LM_DEBUG,"handle_timeout:End of file\n"));
                  AVStreams::flowSpec stop_spec (1);
                  CLIENT::instance ()->streamctrl ()->stop (stop_spec ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                  //CLIENT::instance ()->streamctrl ()->destroy (stop_spec ACE_ENV_ARG_PARAMETER);
                  TAO_AV_CORE::instance ()->orb ()->shutdown (0);
                  ACE_TRY_CHECK;
                  return 0;
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
        ACE_ERROR_RETURN ((LM_ERROR,"send failed:%p","FTP_Client_Flow_Handler::send \n"),-1);
      ACE_DEBUG ((LM_DEBUG,"handle_timeout::buffer sent succesfully\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"FTP_Client_Callback::handle_timeout Failed");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

FTP_Client_Producer::FTP_Client_Producer (void)
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
Client::parse_args (int argc,
                    char **argv)
{
  ACE_Get_Opt opts (argc,argv,"f:a:p:s");

  this->use_sfp_ = 0;
  int c;
  while ((c= opts ()) != -1)
    {
      switch (c)
        {
        case 'f':
          this->filename_ = ACE_OS::strdup (opts.opt_arg ());
          break;
        case 'a':
          this->address_ = ACE_OS::strdup (opts.opt_arg ());
          break;
        case 'p':
          this->protocol_ = ACE_OS::strdup (opts.opt_arg ());
          break;
        case 's':
          this->use_sfp_ = 1;
          break;
        default:
          ACE_DEBUG ((LM_DEBUG,"Unknown option\n"));
          return -1;
        }
    }
  return 0;
}

FILE *
Client::file (void)
{
  return this->fp_;
}

const char*
Client::flowname (void)
{
  return this->flowname_.c_str();
}

AVStreams::protocolSpec
Client::protocols (void)
{
  AVStreams::protocolSpec protocols (1);
  protocols.length (1);
  char buf [BUFSIZ];
  ACE_OS::sprintf (buf,"%s=%s",this->protocol_,this->address_);
  protocols [0] = CORBA::string_dup (buf);
  return protocols;
}

const char *
Client::format (void)
{
  return "UNS:ftp";
}

const char *
Client::address (void)

{
  return this->address_;
}

TAO_StreamCtrl*
Client::streamctrl (void)
{
  return &this->streamctrl_;
}

Client::Client (void)
  : address_ (ACE_OS::strdup ("127.0.0.1:12345")),
    fp_ (0),
    protocol_ (ACE_OS::strdup ("UDP")),
    orb_ (TAO_AV_CORE::instance ()->orb ()),
    poa_ (TAO_AV_CORE::instance ()->poa ())
{
  endpoint_strategy_.init (TAO_AV_CORE::instance ()->orb (),
                           TAO_AV_CORE::instance ()->poa ());
}


int
Client::init (int argc,char **argv)
{
  this->argc_ = argc;
  this->argv_ = argv;

  CORBA::String_var ior;
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {

      PortableServer::POAManager_var mgr
        = TAO_AV_CORE::instance ()->poa ()->the_POAManager ();

      mgr->activate ();

      this->parse_args (this->argc_, this->argv_);

      ACE_NEW_RETURN ( this->streamendpoint_a_,
		      TAO_StreamEndPoint_A, -1 );

      ACE_NEW_RETURN ( this->fep_a_, FTP_Client_Producer, -1 );
      this->flowname_ = "Data";

      sep_a_ = this->streamendpoint_a_->_this( ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      fep_a_obj_ = this->fep_a_->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var s1 = sep_a_->add_fep( fep_a_obj_.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "(%N,%l) Added flowendpoint named: %s\n", s1.in() ));

      this->fp_ = ACE_OS::fopen (this->filename_,"r");

      if (this->fp_ != 0)
        {
          ACE_DEBUG ((LM_DEBUG,"file opened successfully\n"));
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Client::init\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
Client::run (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      char flow_protocol_str [BUFSIZ];
      if (this->use_sfp_)
        ACE_OS::strcpy (flow_protocol_str,"sfp:1.0");
      else
        ACE_OS::strcpy (flow_protocol_str,"");
      AVStreams::streamQoS_var the_qos (new AVStreams::streamQoS);
      AVStreams::flowSpec flow_spec (1);
      flow_spec.length (1);

      ACE_INET_Addr addr (this->address_);
      TAO_Forward_FlowSpec_Entry entry (this->flowname_.c_str(),
                                        "IN",
                                        "USER_DEFINED",
                                        flow_protocol_str,
                                        this->protocol_,
                                        &addr);
      flow_spec [0] = CORBA::string_dup (entry.entry_to_string ());

      ACE_High_Res_Timer timer;
      ACE_Time_Value elapsed;
      timer.start ();


      CORBA::Object_var obj_b = this->orb_->string_to_object("corbaname:rir:#Server_StreamEndPoint_b");
      AVStreams::StreamEndPoint_B_var sep_b = AVStreams::StreamEndPoint_B::_narrow ( obj_b.in() );

      CORBA::Boolean result =
        this->streamctrl_.bind ( sep_a_.in(), sep_b.in(), 
			         the_qos.inout(), flow_spec 
				 ACE_ENV_ARG_PARAMETER );

      timer.stop ();
      timer.elapsed_time (elapsed);
      elapsed.dump ();
      ACE_TRY_CHECK;
      if (result == 0)
        ACE_ERROR_RETURN ((LM_ERROR,"streamctrl::bind failed\n"),-1);

      AVStreams::flowSpec start_spec (1);
      this->streamctrl_.start (start_spec ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Schedule a timer for the for the flow handler.
      ACE_Time_Value tv (10000,0);
      this->orb_->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Client::run\n");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
main (int argc,
      char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      TAO_AV_CORE::instance ()->init (orb.in (),
                                      poa.in ()
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;


      int result = 0;
      result = CLIENT::instance ()->init (argc,argv);
      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"client::init failed\n"),1);
      result = CLIENT::instance ()->run ();

      poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (result < 0)
        ACE_ERROR_RETURN ((LM_ERROR,"client::run failed\n"),1);

    }
  ACE_CATCHANY

  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,"Client Failed\n");
    return -1;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton <Client,ACE_Null_Mutex>;
template class TAO_AV_Endpoint_Reactive_Strategy_A<TAO_StreamEndPoint_A,TAO_VDev,AV_Null_MediaCtrl>;
template class TAO_AV_Endpoint_Reactive_Strategy<TAO_StreamEndPoint_A,TAO_VDev,AV_Null_MediaCtrl>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton <Client,ACE_Null_Mutex>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy_A<TAO_StreamEndPoint_A,TAO_VDev,AV_Null_MediaCtrl>
#pragma instantiate TAO_AV_Endpoint_Reactive_Strategy<TAO_StreamEndPoint_A,TAO_VDev,AV_Null_MediaCtrl>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
