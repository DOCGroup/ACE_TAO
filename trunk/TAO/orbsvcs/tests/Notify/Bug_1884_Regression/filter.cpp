// $Id$

#include "orbsvcs/CosNotifyChannelAdminC.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

#include <iostream>


const char *ior = "file://ecf.ior";

CosNotifyChannelAdmin::EventChannel_var
get_event_channel(CORBA::ORB_ptr orb)

{
  CosNotifyChannelAdmin::EventChannel_var ec;
  CosNotifyChannelAdmin::ChannelID id;
  CosNotification::QoSProperties init_qos(0);
  CosNotification::AdminProperties init_admin(0);



  std::cout << "Get CosNotifyChannelAdmin::EventChannelFactory"  << std::endl;
  std::cout << "IorEventChannelFactory=" << ior << std::endl;
  CORBA::Object_var obj = orb->string_to_object(ior);

  if (CORBA::is_nil(obj.in ()))
  {
    std::cerr << "Bad ec_fact.ior " << std::endl;
    exit(1);
  }


  CosNotifyChannelAdmin::EventChannelFactory_var factory =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow(obj.in ());
  if (CORBA::is_nil(factory.in()))
  {
    std::cerr << "Could not _narrow object to type CosNotifyChannelAdmin::EventChannelFactory" << std::endl;
    exit(1);
  }

  //Get the first ec
  CosNotifyChannelAdmin::ChannelIDSeq_var channelIdSeq;
  try
  {
    channelIdSeq = factory->get_all_channels();
  }
  catch (CORBA::SystemException& se )
  {
    std::cerr << "System exception occurred during get_all_channels: "
      << se << std::endl;
    exit(1);
  }

  if( channelIdSeq->length() == 0 )
  {

    try
    {
      ec = factory->create_channel( init_qos, init_admin, id);
    }
    catch (CORBA::SystemException& se )
    {
      std::cerr << "System exception occurred during find_channel: "
        << se << std::endl;
      exit(1);
    }
  }
  else {
    try
    {
      ec = factory->get_event_channel(channelIdSeq[0]);
    }
    catch (CosNotifyChannelAdmin::ChannelNotFound&)
    {
      std::cerr << "ChannelNotFound: "
        << channelIdSeq[0] << std::endl;
      exit(1);
    }

    catch (CORBA::SystemException& se )
    {
      std::cerr << "System exception occurred during get_event_channel: "
        << se << std::endl;
      exit(1);
    }

  }

  return ec._retn();
}


void match_structure_test (const CosNotifyFilter::Filter_var& filter,
                           const CosNotification::StructuredEvent& event,
                           bool & expected_result)
{
  bool bResult = false;
  try{
    bResult = filter->match_structured(event);
  }
  catch(CosNotifyFilter::UnsupportedFilterableData)
  {
    std::cerr << "UnsupportedFilterableData!" << std::endl;
  }
  catch (CORBA::SystemException& se )
  {
    std::cerr << "System exception occurred during match_structured: "
      << se << std::endl;
    exit(1);
  }

  std::cout << "The result is:" << (bResult?"Match":"Unmatch") << std::endl;

  if (bResult != expected_result)
  {
    // The result should be unmatched as it's filtered with event type.
    std::cerr << "filter test failed." << std::endl;
    exit(1);
  }
}


void update_constraints (const CosNotifyFilter::Filter_var& filter,
                         const char* domain1, const char* type1,
                         const char* domain2, const char* type2)
{
    filter->remove_all_constraints ();

    CosNotification::EventTypeSeq event_types(2);
    event_types.length(2);

    event_types[0].domain_name = CORBA::string_dup(domain1);
    event_types[0].type_name = CORBA::string_dup(type1);

    event_types[1].domain_name = CORBA::string_dup(domain2);
    event_types[1].type_name = CORBA::string_dup(type2);

    CosNotifyFilter::ConstraintExpSeq constraints(1);
    constraints.length(1);

    constraints[0].event_types = event_types;
    constraints[0].constraint_expr = CORBA::string_dup("");

    CosNotifyFilter::ConstraintInfoSeq_var cons_info
      = filter->add_constraints(constraints);

    std::cout << "Constructing a filter..." << std::endl;

    for (CORBA::ULong i = 0; i < event_types.length(); ++i)
    {
      std::cout << "\tevent_types[" << i << "].domain_name="
        << event_types[i].domain_name
        << std::endl;
      std::cout << "\tevent_types[" << i << "].type_name="
        << event_types[i].type_name
        << std::endl;
    }

    std::cout << "\tIts constraint ="
      << constraints[0].constraint_expr.in ()
      << std::endl;
}


int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
  {
    PortableServer::POAManager_var poa_manager;

    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    CORBA::Object_var poa_obj = orb->resolve_initial_references("RootPOA");
    PortableServer::POA_var root_poa = PortableServer::POA::_narrow(poa_obj.in());

    poa_manager = root_poa->the_POAManager();

    poa_manager->activate();

    /*Get event_channel*/
    std::cout << "Get event_channel now"  << std::endl;
    CosNotifyChannelAdmin::EventChannel_var ec = get_event_channel(orb.in());

    //Attaching a filter to pps
    CosNotifyFilter::FilterFactory_var dff =
      ec->default_filter_factory();

    ACE_ASSERT(!CORBA::is_nil(dff.in()));

    std::cout << "Test CosNotifyFilter::Filter::match_structured" << std::endl;

    //Constructing a test event
    CosNotification::StructuredEvent event;

    event.header.fixed_header.event_type.domain_name =
      CORBA::string_dup("Test_domain");
    event.header.fixed_header.event_type.type_name =
      CORBA::string_dup("Test_type_name");

    event.header.variable_header.length(0);
    event.remainder_of_body <<= "";

    std::cout << "Constructing a test event" << std::endl;
    std::cout << "\tIts header.fixed_header.event_type.domain_name="
      << event.header.fixed_header.event_type.domain_name
      << std::endl;
    std::cout << "\tIts header.fixed_header.event_type.type_name="
      << event.header.fixed_header.event_type.type_name
      << std::endl;

    CosNotifyFilter::Filter_var filter = dff->create_filter("EXTENDED_TCL");
    update_constraints (filter, "DomainA", "TypeA", "DomainB", "TypeB");

    bool expected_result = false;
    match_structure_test (filter, event, expected_result);

    update_constraints (filter, "*", "%ALL", "DomainB", "TypeB");
    expected_result = true;
    match_structure_test (filter, event, expected_result);

    update_constraints (filter, "Test_domain", "%ALL", "DomainB", "TypeB");
    expected_result = true;
    match_structure_test (filter, event, expected_result);

    update_constraints (filter, "*", "Test_type_name", "DomainB", "TypeB");
    expected_result = true;
    match_structure_test (filter, event, expected_result);

    update_constraints (filter, "*", "*", "*", "*");
    expected_result = true;
    match_structure_test (filter, event, expected_result);

    orb->shutdown();

  }
  catch(...)
  {
    std::cerr << "Filter: Some exceptions was caught!" << std::endl;
    exit(1);
  }


  return 0;
}


