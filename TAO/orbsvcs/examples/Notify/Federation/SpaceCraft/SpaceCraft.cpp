// file      : SpaceCraft.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
#include "tao/corba.h"

#include "orbsvcs/CosNotificationC.h"
#include "orbsvcs/CosNotifyFilterC.h"
#include "orbsvcs/CosNotifyChannelAdminC.h"

// For in-process Notification Service.
#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Notify/CosNotify_Initializer.h" // NS static link helper.

#include "Gate/Gate.h"

using namespace CORBA;
using namespace CosNotifyFilter;
using namespace CosNotification;
using namespace CosNotifyChannelAdmin;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    ORB_var const orb (ORB_init (argc, argv));

    if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Usage: %s <space-craft-name>={a, b, c}\n"),
                  argv[0]));
      return 1;
    }

    // Activate the root POA.
    //
    CORBA::Object_var const obj (
      orb->resolve_initial_references ("RootPOA"));

    PortableServer::POA_var const root_poa (PortableServer::POA::_narrow (obj.in ()));

    PortableServer::POAManager_var const poa_manager (root_poa->the_POAManager ());

    poa_manager->activate ();
    // Initialize Notification Service.
    //
    TAO_Notify_Service *const ns = TAO_Notify_Service::load_default ();

    if (ns == nullptr)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Notification Service not found! check svc.conf\n")));
      return -1;
    }
    ns->init_service (orb.in ());

    // Create the channel factory.
    //
    EventChannelFactory_var const factory (ns->create (root_poa.in ()));

    if (is_nil (factory.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Unable to create channel factory\n")));
      return 1;
    }

    // Create the channel.
    //
    QoSProperties const qosp {};
    AdminProperties const ap {};
    ChannelID id {};

    EventChannel_var const channel (factory->create_channel (qosp, ap, id));

    // Create and install the filter. We want to reduce the amount
    // of discovery messages that are propagated between space crafts.
    //
    FilterFactory_var const filter_factory (channel->default_filter_factory ());
    Filter_var const filter (filter_factory->create_filter ("EXTENDED_TCL"));

    ConstraintExpSeq constraints (1);
    constraints.length (1);

    constraints[0].event_types.length (0);
    constraints[0].constraint_expr = string_dup (
      "$domain_name == 'Aerospace' and "
      "$type_name == 'AgentDiscovery' and "
      "($.counter - 3 * ($.counter / 3)) == 0"); // ETCL (or TAO) lacks %.

    filter->add_constraints (constraints);

    AdminID admin_id {};
    ConsumerAdmin_var const consumer_admin (
      channel->new_for_consumers (AND_OP, admin_id));

    consumer_admin->add_filter (filter.in ());

    // Find which space craft we are.
    //
    ACE_INET_Addr space_craft_addr;
    ACE_TCHAR const *const space_craft_name = argv[1];

    // Do a quick mapping to mcast addresses.
    //
    switch (space_craft_name[0])
    {
    case 'a':
      {
        space_craft_addr = ACE_INET_Addr ("224.1.0.1:10000");
        break;
      }
    case 'b':
      {
        space_craft_addr = ACE_INET_Addr ("224.1.0.2:10000");
        break;
      }
    case 'c':
      {
        space_craft_addr = ACE_INET_Addr ("224.1.0.3:10000");
        break;
      }
    default:
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Space craft name should be either 'a', 'b', or 'c'.\n")));
        return 1;
      }
    }

    // Create the SpaceCraft <=> Channel gate.
    //
    Gate space_craft_gate (space_craft_addr,
                           consumer_admin.in (),
                           channel->default_supplier_admin ());

    // Create the Channel <=> Constellation gate.
    //
    ACE_INET_Addr const constellation_addr ("224.1.1.1:10000");
    Gate constellation_gate (constellation_addr, channel.in ());

    orb->run ();

    return 0;
  }
  catch (CORBA::UserException const& ue)
  {
    ue._tao_print_exception ("User exception: ");
    return 1;
  }
  catch (CORBA::SystemException const& se)
  {
    se._tao_print_exception ("System exception: ");
    return 1;
  }

  return 1;
}
