// $Id$

#include "ECG_Mcast_Gateway.h"

#include "EC_Lifetime_Utils_T.h"
#include "ECG_Simple_Address_Server.h"
#include "ECG_Complex_Address_Server.h"
#include "ECG_Simple_Mcast_EH.h"
#include "ECG_Mcast_EH.h"
#include "ECG_UDP_EH.h"

#include "orbsvcs/orbsvcs/Event_Utilities.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "tao/ORB_Core.h"

#if ! defined (__ACE_INLINE__)
#include "ECG_Mcast_Gateway.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, ECG_Mcast_Gateway, "$Id$")

typedef TAO_EC_Shutdown_Command<TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> >
UDP_Sender_Shutdown;

typedef TAO_EC_Shutdown_Command<TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> >
UDP_Receiver_Shutdown;


int
TAO_ECG_Mcast_Gateway::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_ECG_Mcast_Gateway);
}


int
TAO_ECG_Mcast_Gateway::fini (void)
{
  return 0;
}

int
TAO_ECG_Mcast_Gateway::init (int argc, char* argv[])
{
  int result = 0;

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, "-ECGService") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "receiver") == 0)
                this->service_type_ = ECG_MCAST_RECEIVER;
              else if (ACE_OS::strcasecmp (opt, "sender") == 0)
                this->service_type_ = ECG_MCAST_SENDER;
              else if (ACE_OS::strcasecmp (opt, "two_way") == 0)
                this->service_type_ = ECG_MCAST_TWO_WAY;
              else
                {
                  ACE_ERROR ((LM_ERROR,
                                         "Unsupported <-ECGService> option "
                                         "value: <%s>. Ignoring this option "
                                         "- using defaults instead.",
                            opt));
                  result = -1;
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECGAddressServer") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "basic") == 0)
                this->address_server_type_ = ECG_ADDRESS_SERVER_BASIC;
              else if (ACE_OS::strcasecmp (opt, "source") == 0)
                this->address_server_type_ = ECG_ADDRESS_SERVER_SOURCE;
              else if (ACE_OS::strcasecmp (opt, "type") == 0)
                this->address_server_type_ = ECG_ADDRESS_SERVER_TYPE;
              else
                {
                  ACE_ERROR ((LM_ERROR,
                                  "Unsupported <-ECGAddressServer> "
                                  "option value: <%s>. Ignoring this "
                                  "option - using defaults instead.",
                              opt));
                  result = -1;
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECGAddressServerArg") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->address_server_arg_.set (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }


      else if (ACE_OS::strcasecmp (arg, "-ECGHandler") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "basic") == 0)
                this->handler_type_ = ECG_HANDLER_BASIC;
              else if (ACE_OS::strcasecmp (opt, "complex") == 0)
                this->handler_type_ = ECG_HANDLER_COMPLEX;
              else if (ACE_OS::strcasecmp (opt, "udp") == 0)
                this->handler_type_ = ECG_HANDLER_UDP;
              else
                {
                  ACE_ERROR ((LM_ERROR,
                                  "Unsupported <-ECGHandler> "
                                  "option value: <%s>. Ignoring this "
                                  "option - using defaults instead.",
                              opt));
                  result = -1;
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECGTTL") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              this->ttl_value_ = ACE_OS::strtoul(opt, 0, 0) & 0xff;
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECGNIC") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->nic_.set (arg_shifter.get_current ());
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECGIPMULTICASTLOOP") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->ip_multicast_loop_ =
                (ACE_OS::atoi(arg_shifter.get_current()) != 0);
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECGNONBLOCKING") == 0)
      {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->non_blocking_ =
                (ACE_OS::atoi(arg_shifter.get_current()) != 0);
              arg_shifter.consume_arg ();
            }
        }

      else
        {
          arg_shifter.ignore_arg ();
          ACE_DEBUG ((LM_WARNING,
                             "Ignoring <%s> option "
                             "during initialization.",
                      arg));
          result = -1;
        }
    }

  if (this->validate_configuration () == -1)
    return -1;
  else
    return result;
}

int
TAO_ECG_Mcast_Gateway::init (const char * address_server_arg,
                             const Attributes & attr)
{
  this->address_server_arg_.set (address_server_arg);

  this->address_server_type_ = attr.address_server_type;
  this->handler_type_ = attr.handler_type;
  this->service_type_ = attr.service_type;
  this->ttl_value_ = attr.ttl_value;
  this->nic_.set (attr.nic.c_str ());
  this->ip_multicast_loop_ = attr.ip_multicast_loop;
  this->non_blocking_ = attr.non_blocking;

  return this->validate_configuration ();
}

int
TAO_ECG_Mcast_Gateway::init (
    const RtecEventChannelAdmin::ConsumerQOS & consumer_qos,
    const char * address_server_arg,
    const Attributes & attributes)
{
  this->consumer_qos_ = consumer_qos;
  return this->init (address_server_arg,
                     attributes);
}

int
TAO_ECG_Mcast_Gateway::validate_configuration (void)
{
  if ((this->handler_type_ == ECG_HANDLER_BASIC
       || this->handler_type_ == ECG_HANDLER_UDP)
      && this->service_type_ != ECG_MCAST_SENDER
      && this->address_server_type_ != ECG_ADDRESS_SERVER_BASIC)
    {
      ACE_DEBUG ((LM_ERROR,
                      "Configurations for mcast handler and "
                      "address server do not match."));
      return -1;
    }

  // Currently all Address Server implementations require an
  // initialization string.  If we ever add a new Address Server
  // implementation, which does not, we'll have to remove this check.
  if (this->address_server_arg_.length () == 0)
    {
      ACE_DEBUG ((LM_ERROR,
                      "Address server initializaton "
                      "argument not specified."));
      return -1;
    }

  if (this->ip_multicast_loop_ != 0
      && this->ip_multicast_loop_ != 1)
    {
      ACE_DEBUG ((LM_ERROR,
                  "IP MULTICAST LOOP option must have a boolean value."));
      return -1;
    }

  if (this->non_blocking_ != 0
      && this->non_blocking_ != 1)
    {
      ACE_DEBUG ((LM_ERROR,
                  "NON BLOCKING flag must have a boolean value."));
      return -1;
    }

  return 0;
}

TAO_ECG_Refcounted_Endpoint
TAO_ECG_Mcast_Gateway::init_endpoint (void)
{
  TAO_ECG_Refcounted_Endpoint endpoint;

  ACE_NEW_RETURN (endpoint,
                  TAO_ECG_UDP_Out_Endpoint,
                  endpoint);

  ACE_SOCK_Dgram& dgram = endpoint->dgram ();

  if (dgram.open (ACE_Addr::sap_any) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                             "Cannot open dgram "
                             "for sending mcast messages."));
      return TAO_ECG_Refcounted_Endpoint ();
    }

  if (this->nic_.length () != 0)
    {
      dgram.set_nic (this->nic_.c_str ());
    }

  if (this->ttl_value_ > 0)
    {
      if (dgram.ACE_SOCK::set_option (IPPROTO_IP,
                                      IP_MULTICAST_TTL,
                                      &this->ttl_value_,
                                      sizeof (this->ttl_value_))
          == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      "Error setting TTL option on dgram "
                      "for sending mcast messages."));
          return TAO_ECG_Refcounted_Endpoint ();
        }
    }

  if (dgram.ACE_SOCK::set_option (IPPROTO_IP,
                                  IP_MULTICAST_LOOP,
                                  &this->ip_multicast_loop_,
                                  sizeof (this->ip_multicast_loop_)) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error setting MULTICAST_LOOP option "
                  "on dgram for sending mcast messages."));
      return TAO_ECG_Refcounted_Endpoint ();
    }

  if (this->non_blocking_
      && dgram.enable(ACE_NONBLOCK) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "Error setting NON BLOCKING option."));
      return TAO_ECG_Refcounted_Endpoint ();
    }

  return endpoint;
}

PortableServer::ServantBase *
TAO_ECG_Mcast_Gateway::init_address_server (void)
{
  const char * address_server_arg =
    (this->address_server_arg_.length ())
    ? this->address_server_arg_.c_str () : 0;

  if (this->address_server_type_ == ECG_ADDRESS_SERVER_BASIC)
    {
      TAO_EC_Servant_Var<TAO_ECG_Simple_Address_Server> impl =
        TAO_ECG_Simple_Address_Server::create ();
      if (!impl.in ())
        return 0;

      if (impl->init (address_server_arg) == -1)
        {
          return 0;
        }
      return impl._retn ();
    }

  else if (this->address_server_type_ == ECG_ADDRESS_SERVER_SOURCE)
    {
      TAO_EC_Servant_Var<TAO_ECG_Complex_Address_Server> impl =
        TAO_ECG_Complex_Address_Server::create (1);
      if (!impl.in ())
        return 0;

      if (impl->init (address_server_arg) == -1)
        {
          return 0;
        }
      return impl._retn ();
    }

  else if (this->address_server_type_ == ECG_ADDRESS_SERVER_TYPE)
    {
      TAO_EC_Servant_Var<TAO_ECG_Complex_Address_Server> impl =
        TAO_ECG_Complex_Address_Server::create (0);
      if (!impl.in ())
        return 0;

      if (impl->init (address_server_arg) == -1)
        {
          return 0;
        }
      return impl._retn ();
    }

  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot create address server: "
                  "unknown address server type specified."));
      return 0;
    }
}

TAO_ECG_Refcounted_Handler
TAO_ECG_Mcast_Gateway::init_handler (TAO_ECG_Dgram_Handler *receiver,
                                     RtecEventChannelAdmin::EventChannel_ptr ec,
                                     ACE_Reactor *reactor
                                     ACE_ENV_ARG_DECL)
{
  TAO_ECG_Refcounted_Handler handler;

  const char * nic =
    (this->nic_.length ()) ? this->nic_.c_str () : 0;
  const char * address_server_arg =
    (this->address_server_arg_.length ())
    ? this->address_server_arg_.c_str () : 0;

  if (this->handler_type_ == ECG_HANDLER_BASIC)
    {
      TAO_ECG_Simple_Mcast_EH * h = 0;
      ACE_NEW_RETURN (h,
                      TAO_ECG_Simple_Mcast_EH (receiver),
                      handler);
      handler = h;

      h->reactor (reactor);
      if (h->open (address_server_arg, nic) != 0)
        return TAO_ECG_Refcounted_Handler ();
    }

  else if (this->handler_type_ == ECG_HANDLER_COMPLEX)
    {
      TAO_ECG_Mcast_EH * h = 0;
      ACE_NEW_RETURN (h,
                      TAO_ECG_Mcast_EH (receiver, nic),
                      handler);
      handler = h;

      h->reactor (reactor);

      h->open (ec ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_ECG_Refcounted_Handler ());
    }

  else if (this->handler_type_ == ECG_HANDLER_UDP)
    {
      TAO_ECG_UDP_EH * h = 0;
      ACE_NEW_RETURN (h,
                      TAO_ECG_UDP_EH (receiver),
                      handler);
      handler = h;
      h->reactor (reactor);

      ACE_INET_Addr ipaddr;
      if (ipaddr.set (address_server_arg) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR using address server argument "
                      "in ACE_INET_Addr.set ()."));
          return TAO_ECG_Refcounted_Handler ();
        }
      if (h->open (ipaddr) != 0)
        return TAO_ECG_Refcounted_Handler ();
    }

  else
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot create handler: unknown "
                  "handler type specified."));
      return handler;
    }

  return handler;
}

TAO_EC_Servant_Var<TAO_ECG_UDP_Sender>
TAO_ECG_Mcast_Gateway::init_sender (
                               RtecEventChannelAdmin::EventChannel_ptr ec,
                               RtecUDPAdmin::AddrServer_ptr address_server,
                               TAO_ECG_Refcounted_Endpoint endpoint_rptr
                               ACE_ENV_ARG_DECL)
{
  TAO_EC_Servant_Var<TAO_ECG_UDP_Sender>
    sender (TAO_ECG_UDP_Sender::create ());
  if (!sender.in ())
    return sender;

  sender->init (ec,
                address_server,
                endpoint_rptr
                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> ());

  TAO_EC_Auto_Command<UDP_Sender_Shutdown> sender_shutdown;
  sender_shutdown.set_command (UDP_Sender_Shutdown (sender));

  if (this->consumer_qos_.dependencies.length () > 0)
    {
      // Client supplied consumer qos.  Use it.
      this->consumer_qos_.is_gateway = 1;
      sender->connect (this->consumer_qos_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> ());
    }
  else
    {
      // Client did not specify anything - subscribe to all events.
      ACE_ConsumerQOS_Factory consumer_qos_factory;
      consumer_qos_factory.start_disjunction_group (1);
      consumer_qos_factory.insert (ACE_ES_EVENT_SOURCE_ANY,
                                   ACE_ES_EVENT_ANY,
                                   0);
      RtecEventChannelAdmin::ConsumerQOS & qos =
        ACE_const_cast (RtecEventChannelAdmin::ConsumerQOS &,
                        consumer_qos_factory.get_ConsumerQOS ());
      qos.is_gateway = 1;

      sender->connect (qos ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> ());
    }

  sender_shutdown.disallow_command ();
  return sender;
}

TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver>
TAO_ECG_Mcast_Gateway::init_receiver (
                               RtecEventChannelAdmin::EventChannel_ptr ec,
                               RtecUDPAdmin::AddrServer_ptr address_server,
                               TAO_ECG_Refcounted_Endpoint endpoint_rptr
                               ACE_ENV_ARG_DECL)
{
  TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver>
    receiver (TAO_ECG_UDP_Receiver::create ());
  if (!receiver.in ())
    return receiver;

  receiver->init (ec,
                  endpoint_rptr,
                  address_server
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> ());

  TAO_EC_Auto_Command<UDP_Receiver_Shutdown> receiver_shutdown;
  receiver_shutdown.set_command (UDP_Receiver_Shutdown (receiver));

  ACE_SupplierQOS_Factory supplier_qos_factory;
  supplier_qos_factory.insert (ACE_ES_EVENT_SOURCE_ANY,
                               ACE_ES_EVENT_ANY,
                               0, 1);
  RtecEventChannelAdmin::SupplierQOS & qos =
    ACE_const_cast (RtecEventChannelAdmin::SupplierQOS &,
                    supplier_qos_factory.get_SupplierQOS ());
  qos.is_gateway = 1;

  receiver->connect (qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> ());

  receiver_shutdown.disallow_command ();
  return receiver;
}

void
TAO_ECG_Mcast_Gateway::verify_args (CORBA::ORB_ptr orb,
                                    RtecEventChannelAdmin::EventChannel_ptr ec
                                    ACE_ENV_ARG_DECL)
{
  if (CORBA::is_nil (ec))
    {
      ACE_ERROR ((LM_ERROR,
                  "Nil event channel argument passed to "
                  "TAO_ECG_Mcast_Gateway::run()."));
      ACE_THROW (CORBA::INTERNAL ());
    }
  if (CORBA::is_nil (orb))
    {
      ACE_ERROR ((LM_ERROR,
                  "Nil orb argument passed to "
                  "TAO_ECG_Mcast_Gateway::run()."));
      ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_ECG_Mcast_Gateway::run (CORBA::ORB_ptr orb,
                            RtecEventChannelAdmin::EventChannel_ptr ec
                            ACE_ENV_ARG_DECL)
{
  // Verify args.
  this->verify_args (orb, ec ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Auto-cleanup objects.
  TAO_EC_Object_Deactivator address_server_deactivator;
  TAO_EC_Auto_Command<UDP_Sender_Shutdown> sender_shutdown;
  TAO_EC_Auto_Command<UDP_Receiver_Shutdown> receiver_shutdown;

  // Set up address server.
  PortableServer::ServantBase_var address_server_servant =
    this->init_address_server ();
  if (!address_server_servant.in ())
    {
      ACE_DEBUG ((LM_ERROR,
                  "Unable to create address server."));
      ACE_THROW (CORBA::INTERNAL ());
    }

  RtecUDPAdmin::AddrServer_var address_server;

  PortableServer::POA_var poa =
    address_server_servant->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  activate (address_server,
            poa.in (),
            address_server_servant.in (),
            address_server_deactivator
            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ECG_Refcounted_Endpoint endpoint_rptr;
  TAO_EC_Servant_Var<TAO_ECG_UDP_Sender> sender;

  // Set up event sender.
  if (this->service_type_ == ECG_MCAST_SENDER
      || this->service_type_ == ECG_MCAST_TWO_WAY)
    {
      endpoint_rptr = this->init_endpoint ();
      if (endpoint_rptr.get () == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }

      sender = this->init_sender (ec,
                                  address_server.in (),
                                  endpoint_rptr
                                  ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (!sender.in ())
        {
          ACE_THROW (CORBA::INTERNAL ());
        }

      sender_shutdown.set_command (UDP_Sender_Shutdown (sender));
    }

  // Set up event receiver.
  TAO_EC_Servant_Var<TAO_ECG_UDP_Receiver> receiver;
  if (this->service_type_ == ECG_MCAST_RECEIVER
      || this->service_type_ == ECG_MCAST_TWO_WAY)
    {
      ACE_Reactor *reactor = orb->orb_core ()->reactor ();

      receiver = this->init_receiver (ec,
                                      address_server.in (),
                                      endpoint_rptr
                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      if (!receiver.in ())
        {
          ACE_THROW (CORBA::INTERNAL ());
        }

      receiver_shutdown.set_command (UDP_Receiver_Shutdown (receiver));

      TAO_ECG_Refcounted_Handler
        handler_rptr (this->init_handler (receiver.in (),
                                          ec,
                                          reactor
                                          ACE_ENV_ARG_PARAMETER));
      ACE_CHECK;
      if (handler_rptr.get () == 0)
        {
          ACE_THROW (CORBA::INTERNAL ());
        }
      receiver->set_handler_shutdown (handler_rptr);
    }

  // Everything went ok - disable auto-cleanup.
  address_server_deactivator.disallow_deactivation ();
  receiver_shutdown.disallow_command ();
  sender_shutdown.disallow_command ();
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_ECG_Mcast_Gateway,
                       ACE_TEXT ("ECG_Mcast_Gateway"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_ECG_Mcast_Gateway),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTEvent, TAO_ECG_Mcast_Gateway)

// ****************************************************************


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_ECG_Mcast_Gateway>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_ECG_Mcast_Gateway>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
