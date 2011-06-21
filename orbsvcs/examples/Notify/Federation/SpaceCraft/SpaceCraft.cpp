// file      : SpaceCraft.cpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

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

#include <iostream>
#include <sstream>

using std::cerr;
using std::endl;

using namespace CORBA;
using namespace CosNotifyComm;
using namespace CosNotifyFilter;
using namespace CosNotification;
using namespace CosNotifyChannelAdmin;

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    ORB_var orb (ORB_init (argc, argv));

    if (argc < 2)
    {
      ACE_ERROR ((LM_ERROR,
                  "Usage: %s <space-craft-name>={a, b, c}\n",
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

    // Create and install the filter. We want to reduce the amount
    // of dicovery messages that are propagated between space crafts.
    //
    FilterFactory_var filter_factory (channel->default_filter_factory ());
    Filter_var filter (filter_factory->create_filter ("EXTENDED_TCL"));

    ConstraintExpSeq constraints (1);
    constraints.length (1);

    constraints[0].event_types.length (0);
    constraints[0].constraint_expr = string_dup (
      "$domain_name == 'Aerospace' and "
      "$type_name == 'AgentDiscovery' and "
      "($.counter - 3 * ($.counter / 3)) == 0");// ETCL (or TAO) doesn't have %?

    filter->add_constraints (constraints);

    AdminID admin_id = 0;
    ConsumerAdmin_var consumer_admin (
      channel->new_for_consumers (AND_OP, admin_id));

    consumer_admin->add_filter (filter.in ());

    // Find which space craft we are.
    //
    ACE_INET_Addr space_craft_addr;
    const ACE_TCHAR *space_craft_name = argv[1];

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
                    "Space craft name should be either 'a', 'b', or 'c'.\n"));
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
    ACE_INET_Addr constellation_addr ("224.1.1.1:10000");
    Gate constellation_gate (constellation_addr, channel.in ());

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
