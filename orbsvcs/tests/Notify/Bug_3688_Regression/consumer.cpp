// $Id$

#include "common.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

int runtime = 5; //sec


namespace CosNotifyCommImpl{
  class StructuredPushConsumer: public virtual POA_CosNotifyComm:: StructuredPushConsumer
  {
  public:
    StructuredPushConsumer () : received_ (false) {};
    virtual ~ StructuredPushConsumer () {};

    void push_structured_event(
      const CosNotification::StructuredEvent &event)
    {
      received_ = true;
      std::cout << "Received an event...," << std::endl;
      std::cout << "event.header.fixed_header.event_type.domain_name = "
        << event.header.fixed_header.event_type.domain_name
        << std::endl;
      std::cout << "event.header.fixed_header.event_type.type_name = "
        << event.header.fixed_header.event_type.type_name
        << std::endl;
    };

    void disconnect_structured_push_consumer()
    {};

    void offer_change (
      const CosNotification::EventTypeSeq &,
      const CosNotification::EventTypeSeq &)
    {};

    bool received_events ()
    {
      return this->received_;
    };

  private:
    bool received_;
  };
}

int parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "t:");
  int x;

  while ((x = get_opts ()) != -1)
    switch (x)
  {
    case 't':
      runtime = ACE_OS::atoi (get_opts.optarg);
      break;

    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage:  %s -t <runtime>\n"),
                         argv [0]),
                        -1);
  }

  return 0; // successful parse
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

    if (parse_args (argc, argv) != 0)
      return 1;

    poa_manager->activate();

    /*Get event_channel*/
    std::cout << "Get event_channel now"  << std::endl;
    CosNotifyChannelAdmin::EventChannel_var ec = get_event_channel(orb.in());

    //Instanciating the Consumer
    CosNotifyComm::StructuredPushConsumer_var spc =
      CosNotifyComm::StructuredPushConsumer::_nil();

    CosNotifyCommImpl::StructuredPushConsumer *pImpl_spc = new CosNotifyCommImpl::StructuredPushConsumer;
    spc = pImpl_spc->_this();

    //Obtain a Consumer Admin
    CosNotifyChannelAdmin::AdminID adminid = 0;
    CosNotifyChannelAdmin::ConsumerAdmin_var ca =
      ec->new_for_consumers (CosNotifyChannelAdmin::AND_OP, adminid);

    if( ca.in() == CosNotifyChannelAdmin::ConsumerAdmin::_nil() ){
      std::cerr << "ca is nil!" << std::endl;
      return 1;
    }

    //Obtain a Proxy Consumer
    CosNotifyChannelAdmin::ProxyID proxy_id;
    CosNotifyChannelAdmin::ClientType ctype = CosNotifyChannelAdmin::STRUCTURED_EVENT;

    CosNotifyChannelAdmin::ProxySupplier_var proxySupplier_obj;
    try
    {
      proxySupplier_obj = ca->obtain_notification_push_supplier(ctype, proxy_id);
    }
    catch(CosNotifyChannelAdmin::AdminLimitExceeded err)
    {
      std::cerr << "CosNotifyChannelAdmin::AdminLimitExceeded Exception!" << std::endl;
      throw;
    }

    CosNotifyChannelAdmin::StructuredProxyPushSupplier_var pps =
      CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow(proxySupplier_obj.in());


    //Attaching a filter to pps
    CosNotifyFilter::FilterFactory_var dff =
      ec->default_filter_factory();

    ACE_ASSERT(!CORBA::is_nil(dff.in()));

    CosNotifyFilter::Filter_var filter = dff->create_filter("EXTENDED_TCL");

    CosNotification::EventTypeSeq event_types(1);
    event_types.length(2);

    event_types[0].domain_name = CORBA::string_dup("Test_domain");
    event_types[0].type_name = CORBA::string_dup("Test_type_name");
    event_types[1].domain_name = CORBA::string_dup("DomainB");
    event_types[1].type_name = CORBA::string_dup("TypeB");

    CosNotifyFilter::ConstraintExpSeq constraints(1);
    constraints.length(1);

    constraints[0].event_types = event_types;
    constraints[0].constraint_expr = CORBA::string_dup(
      "$data == 1 or $data == 2 or $data == 4");

    filter->add_constraints(constraints);

    pps->add_filter(filter.in());

    std::cout << "Attached a filter to ProxyPushSupplier" << std::endl;
    std::cout << "The filter's event_types[0].domain_name = " <<  event_types[0].domain_name << std::endl;
    std::cout << "The filter's event_types[0].type_name = " <<  event_types[0].type_name << std::endl;
    std::cout << "The filter's event_types[1].domain_name = " <<  event_types[1].domain_name << std::endl;
    std::cout << "The filter's event_types[1].type_name = " <<  event_types[1].type_name << std::endl;
    std::cout << "The filter's constraint_expr=" << constraints[0].constraint_expr << std::endl;

    //Connecting a Supplier to a Proxy Consumer
    try
    {
      pps->connect_structured_push_consumer(spc.in());
    }
    catch (CosEventChannelAdmin::AlreadyConnected ac)
    {
      std::cerr << "CosEventChannelAdmin::AlreadyConnected" << std::endl;
      throw;
    }
    catch (CORBA::SystemException& se)
    {
      std::cerr << "System exception occurred during connect: " <<
        se << std::endl;
      throw;
    }


    ACE_Time_Value tv (runtime);
    orb->run (tv);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Consumer done.\n")));

    if (pImpl_spc->received_events ())
    {
      //Consumer should not receive any events as the user defined constraint takes
      //effect.
      std::cerr << "Test failed - should not receive test events." << std::endl;
      return 1;
    }
    else
    {
      std::cout << "Test passed - did not receive test events as expected." << std::endl;
    }
  }
  catch(...)
  {
    std::cerr << "Consumer: Some exceptions was caught!" << std::endl;
    return 1;
  }


  return 0;
}

