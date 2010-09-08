// $Id$

#include "BCast.h"
#include "Consumer.h"
#include "Supplier.h"
#include "orbsvcs/Event/ECG_Mcast_Gateway.h"
#include "tao/ORB_Core.h"
#include "ace/Arg_Shifter.h"
#include "ace/INET_Addr.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  EC_BCast driver;
  return driver.run (argc, argv);
}

// ****************************************************************

EC_BCast::EC_BCast (void)
  : bcast_address_ (ACE_TEXT("255.255.255.255")),
    bcast_port_ (12345)
{
}

int
EC_BCast::parse_args (int& argc, ACE_TCHAR* argv[])
{
  if (this->EC_Driver::parse_args (argc, argv) != 0)
    return -1;

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, ACE_TEXT("-port")) == 0)
        {
          arg_shifter.consume_arg ();
          this->bcast_port_ = ACE_OS::atoi (arg_shifter.get_current ());
        }

      else if (ACE_OS::strcmp (arg, ACE_TEXT("-address")) == 0)
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
EC_BCast::execute_test (void)
{
  // Subscription determining which EC events will get sent out on the
  // UDP socket.
  RtecEventChannelAdmin::ConsumerQOS sub;
  int shutdown_event_type;
  this->build_consumer_qos (0, sub, shutdown_event_type);

  // Obtain UDP address in the string format for Gateway initialization.
  ACE_TCHAR address_server_arg [256];
  ACE_INET_Addr udp_addr;
  if (udp_addr.set (this->bcast_port_, this->bcast_address_) == -1
      || udp_addr.addr_to_string (address_server_arg, 256) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "%N (%l): Problems with specified UDP address\n"));
      return;
    }

  // Set up UDP federation.
  TAO_ECG_Mcast_Gateway::Attributes lAttributes;
  lAttributes.address_server_type = TAO_ECG_Mcast_Gateway::ECG_ADDRESS_SERVER_BASIC;
  lAttributes.handler_type = TAO_ECG_Mcast_Gateway::ECG_HANDLER_UDP;
  lAttributes.service_type = TAO_ECG_Mcast_Gateway::ECG_MCAST_TWO_WAY;

  TAO_ECG_Mcast_Gateway gateway;
  if (gateway.init (sub,
                    ACE_TEXT_ALWAYS_CHAR(address_server_arg),
                    lAttributes)
      == -1)
    return;

  gateway.run (this->orb_.in (),
               this->event_channel_.in ());

  if (this->allocate_tasks () == -1)
    return;

  this->activate_tasks ();

  if (this->verbose ())
    ACE_DEBUG ((LM_DEBUG, "BCast (%P|%t) suppliers are active\n"));

  ACE_Time_Value tv (30, 0);
  this->orb_->run (tv);

  // Wait for the supplier threads...
  if (ACE_Thread_Manager::instance ()->wait () == -1)
    {
      ACE_ERROR ((LM_ERROR, "BCast (%P|%t) Thread_Manager wait failed\n"));
    }
}

void
EC_BCast::dump_results (void)
{
  this->EC_Driver::dump_results ();
}
