// $Id$

#include "BCast.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Gateway_UDP.h"
#include "orbsvcs/Event/ECG_UDP_Sender.h"
#include "tao/ORB_Core.h"
#include "ace/Arg_Shifter.h"

ACE_RCSID(EC_Tests_Basic, BCast, "$Id$")

int
main (int argc, char *argv [])
{
  EC_BCast driver;
  return driver.run (argc, argv);
}

// ****************************************************************

EC_BCast::EC_BCast (void)
  : bcast_address_ ("255.255.255.255"),
    bcast_port_ (12345)
{
}

int
EC_BCast::parse_args (int& argc, char* argv[])
{
  if (this->EC_Driver::parse_args (argc, argv) != 0)
    return -1;

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-port") == 0)
        {
          arg_shifter.consume_arg ();
          this->bcast_port_ = ACE_OS::atoi (arg_shifter.get_current ());
        }

      else if (ACE_OS::strcmp (arg, "-address") == 0)
        {
          arg_shifter.consume_arg ();
          this->bcast_address_ = arg_shifter.get_current ();
        }

      arg_shifter.ignore_arg ();
    }

  return 0;
}

void
EC_BCast::print_args (void) const
{
  this->EC_Driver::print_args ();
}

void
EC_BCast::print_usage (void)
{
  this->EC_Driver::print_usage ();
}

void
EC_BCast::modify_attributes (TAO_EC_Event_Channel_Attributes&)
{
}

void
EC_BCast::execute_test (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_ECG_UDP_Sender sender;
  TAO_ECG_UDP_Out_Endpoint endpoint;

  ACE_INET_Addr udp_addr (this->bcast_port_, this->bcast_address_);

  Simple_Address_Server address_server_impl (udp_addr);
  RtecUDPAdmin::AddrServer_var address_server =
    address_server_impl._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (endpoint.dgram ().open (ACE_Addr::sap_any) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot open send endpoint\n"));
      return;
    }

  sender.init (this->event_channel_.in (),
               address_server.in (),
               &endpoint
               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::ConsumerQOS sub;
  int shutdown_event_type;
  this->build_consumer_qos (0, sub, shutdown_event_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  sender.open (sub ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  TAO_ECG_UDP_Receiver receiver;

  TAO_ECG_UDP_EH udp_eh (&receiver);

  ACE_Time_Value expire (0, 50000);
  receiver.init (this->event_channel_.in (),
                 &endpoint,
                 address_server.in (),
                 this->orb_->orb_core ()->reactor (),
                 expire, 5
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  udp_eh.reactor (this->orb_->orb_core ()->reactor ());
  ACE_INET_Addr local_addr (this->bcast_port_);
  if (udp_eh.open (local_addr) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot open EH %p\n"));
    }

  RtecEventChannelAdmin::SupplierQOS pub;
  this->build_supplier_qos (0, pub, shutdown_event_type ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  receiver.open (pub ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (this->allocate_tasks () == -1)
    return;

  this->activate_tasks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "BCast (%P|%t) suppliers are active\n"));

  ACE_Time_Value tv (30, 0);
  this->orb_->run (tv);

  // Wait for the supplier threads...
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    {
      ACE_ERROR ((LM_ERROR, "BCast (%P|%t) Thread_Manager wait failed\n"));
    }

  if (udp_eh.close () == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "Cannot close EH %p\n"));
    }

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "BCast (%P|%t) suppliers finished\n"));

  receiver.close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  sender.close (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // @@ Deactivate all the objects...!

  {
    // Deactivate the Address Server
    PortableServer::POA_var poa =
      address_server_impl._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;
    PortableServer::ObjectId_var id =
      poa->servant_to_id (&address_server_impl ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;
    poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (this->verbose ())
      ACE_DEBUG ((LM_DEBUG, "EC_BCast (%P|%t) address server deactivated\n"));
  }
}

void
EC_BCast::dump_results (void)
{
  this->EC_Driver::dump_results ();
}

// ****************************************************************

Simple_Address_Server::
Simple_Address_Server (const ACE_INET_Addr& address)
{
  this->address_.ipaddr = address.get_ip_address ();
  this->address_.port   = address.get_port_number ();
}

void
Simple_Address_Server::get_addr (const RtecEventComm::EventHeader&,
                                 RtecUDPAdmin::UDP_Addr& address
                                 ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  address = this->address_;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
