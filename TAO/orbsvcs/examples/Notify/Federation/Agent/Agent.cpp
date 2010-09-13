// file      : Agent.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#include <ace/streams.h>
#if defined(ACE_USES_OLD_IOSTREAMS)
#  if defined(_MSC_VER)
#    include <strstrea.h>
#  else
#    include <strstream.h>
#  endif
#else
# include <sstream>
#endif

#include "tao/corba.h"

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/CosNotifyCommS.h"

// For in-process Notification Service.
//
#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Notify/CosNotify_Initializer.h" // NS static link helper.


#include "Gate/Gate.h"

using namespace CORBA;
using namespace CosNotifyComm;
using namespace CosNotification;
using namespace CosNotifyChannelAdmin;

class Agent : public POA_CosNotifyComm::StructuredPushConsumer
{
public:
  Agent (char const* space_craft_name,
         char const* agent_name,
         EventChannel_ptr ch)
      : space_craft_name_ (space_craft_name),
        agent_name_ (agent_name),
        counter_ (0)
  {
    // Obtain a proxy consumer.
    //
    ProxyConsumer_var pc (
      ch->default_supplier_admin ()->obtain_notification_push_consumer (
        STRUCTURED_EVENT, consumer_id_));

    consumer_ = StructuredProxyPushConsumer::_narrow (pc.in ());

    consumer_->connect_structured_push_supplier (
      StructuredPushSupplier::_nil ());

    // Register as a consumer.
    //
    StructuredPushConsumer_var ref (_this ()); // Activate on the default POA.

    ProxySupplier_var ps (
      ch->default_consumer_admin ()->obtain_notification_push_supplier (
        STRUCTURED_EVENT, supplier_id_));

    supplier_ = StructuredProxyPushSupplier::_narrow (ps.in ());

    supplier_->connect_structured_push_consumer (ref.in ());

    // Start tracker thread.
    //
    if (ACE_OS::thr_create (&tracker_thunk,
                            this,
                            THR_JOINABLE,
                            &thread_) != 0)
      ACE_OS::abort ();
  }

private:
  static ACE_THR_FUNC_RETURN
  tracker_thunk (void* arg)
  {
    Agent* a = reinterpret_cast<Agent*> (arg);
    a->tracker ();
    return 0;
  }

  void
  tracker ()
  {
    while (true)
    {
      StructuredEvent e;

      // Header.
      //
      e.header.fixed_header.event_type.domain_name = string_dup ("Aerospace");
      e.header.fixed_header.event_type.type_name = string_dup ("AgentDiscovery");

      // Make a unique "event id" by combining space_craft_name, agent_name,
      // and counter. This can be handy for debugging.
      //
#if defined(ACE_USES_OLD_IOSTREAMS)
      ostrstream ostr;
#else
      std::ostringstream ostr;
#endif
      ostr << space_craft_name_ << ":" << agent_name_ << ":" << counter_++;

#if defined(ACE_USES_OLD_IOSTREAMS)
      e.header.fixed_header.event_name = ostr.str ();
#else
      e.header.fixed_header.event_name = ostr.str ().c_str ();
#endif

      // Also add space_craft_name and agent_name fields separately
      // into variable_header. This will make filtering easier.
      //
      e.header.variable_header.length (2);

      e.header.variable_header[0].name = string_dup ("space_craft_name");
      e.header.variable_header[0].value <<= string_dup (space_craft_name_.in ());

      e.header.variable_header[1].name = string_dup ("agent_name");
      e.header.variable_header[1].value <<= string_dup (agent_name_.in ());

      // Add the counter value into filterable_data section of the event.
      //
      e.filterable_data.length (1);

      e.filterable_data[0].name = string_dup ("counter");
      e.filterable_data[0].value <<= counter_;


      consumer_->push_structured_event (e);

      ACE_OS::sleep (ACE_Time_Value (3, 0));
    }
  }

private:
  // NotifyPublish interface.
  //
  virtual void
  offer_change (EventTypeSeq const&,
                EventTypeSeq const&)
  {
    // We don't care.
  }

  // StructuredPushSupplier interface.
  //
  virtual void
  push_structured_event (StructuredEvent const& e)
  {
    // Extract space_craft_name and agent_name.
    //
    Char const* space_craft_name = 0;
    Char const* agent_name = 0;

    e.header.variable_header[0].value >>= space_craft_name;
    e.header.variable_header[1].value >>= agent_name;

    // Extract the counter value.
    //
    CORBA::ULong counter;
    e.filterable_data[0].value >>= counter;

    cerr << e.header.fixed_header.event_type.domain_name << "::"
         << e.header.fixed_header.event_type.type_name << " "
         << "id=" << e.header.fixed_header.event_name << " from "
         << "(" << space_craft_name << ", " << agent_name << ")"
         << ": " << counter << endl;
  }


  virtual void
  disconnect_structured_push_consumer (void)
  {
    // We don't care.
  }

private:
  String_var space_craft_name_;
  String_var agent_name_;
  ULong counter_;

  ACE_thread_t thread_;

  ProxyID consumer_id_;
  StructuredProxyPushConsumer_var consumer_;

  ProxyID supplier_id_;
  StructuredProxyPushSupplier_var supplier_;

};

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    ORB_var orb (ORB_init (argc, argv));

    if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR,
                  "Usage: %s <agent-name> [<space-craft-name>={a, b, c}]\n",
                  argv[0]));
      return 1;
    }


    // Activate the root POA.
    //
    CORBA::Object_var obj (
      orb->resolve_initial_references ("RootPOA"));

    PortableServer::POA_var root_poa (PortableServer::POA::_narrow (obj.in ()));

    PortableServer::POAManager_var poa_manager (root_poa->the_POAManager ());

    poa_manager->activate ();


    // Initialize Notification Service.
    //
    TAO_Notify_Service* ns = TAO_Notify_Service::load_default ();

    if (ns == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Notification Service not found! check svc.conf\n"));
      return -1;
    }


    ns->init_service (orb.in ());

    // Create the channel factory.
    //
    EventChannelFactory_var factory (ns->create (root_poa.in ()));

    if (is_nil (factory.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  "Unable to create channel factory\n"));
      return 1;
    }


    // Create the channel.
    //
    QoSProperties qosp;
    AdminProperties ap;
    ChannelID id;

    EventChannel_var channel (factory->create_channel (qosp, ap, id));

    // Find which space craft we are on.
    //
    ACE_INET_Addr addr;
    const ACE_TCHAR *space_craft_name = 0;

    if (argc < 3)
      space_craft_name = ACE_TEXT("a");  // Default to spacecraft "a".
    else
      space_craft_name = argv[2];

    // Do a quick mapping to mcast addresses.
    //
    switch (space_craft_name[0])
    {
    case 'a':
      {
        addr = ACE_INET_Addr ("224.1.0.1:10000");
        break;
      }
    case 'b':
      {
        addr = ACE_INET_Addr ("224.1.0.2:10000");
        break;
      }
    case 'c':
      {
        addr = ACE_INET_Addr ("224.1.0.3:10000");
        break;
      }
    default:
      {
        ACE_ERROR ((LM_ERROR,
                    "Space craft name should be either 'a', 'b', or 'c'.\n"));
        return 1;
      }
    }

    // Create the gate.
    //
    Gate gate (addr, channel.in ());

    // Start the agent.
    //
    Agent agent (ACE_TEXT_ALWAYS_CHAR(space_craft_name),
                 ACE_TEXT_ALWAYS_CHAR(argv[1]),
                 channel.in ());

    orb->run ();

    return 0;
  }
  catch (const CORBA::UserException& ue)
  {
    ue._tao_print_exception ("User exception: ");
    return 1;
  }
  catch (const CORBA::SystemException& se)
  {
    se._tao_print_exception ("System exception: ");
    return 1;
  }

  return 1;
}
