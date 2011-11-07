// $Id$

#include "common.h"

namespace CosNotifyCommImpl{
  class StructuredPushSupplier: public virtual POA_CosNotifyComm:: StructuredPushSupplier
  {
  public:
    void disconnect_structured_push_supplier()
    {};
    void subscription_change( const CosNotification::EventTypeSeq&,
      const CosNotification::EventTypeSeq&)
    {};

  };
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
    std::cout << "Create event_channel now"  << std::endl;
    CosNotifyChannelAdmin::EventChannel_var ec = get_event_channel(orb.in());

    //Instanciating the Supplier
    CosNotifyComm::StructuredPushSupplier_var sps =
      CosNotifyComm::StructuredPushSupplier::_nil();


    CosNotifyCommImpl::StructuredPushSupplier *pImpl_sps = new CosNotifyCommImpl::StructuredPushSupplier;
    sps = pImpl_sps->_this();

    //Obtain a Supplier Admin
    CosNotifyChannelAdmin::SupplierAdmin_var sa = ec->default_supplier_admin();
    if( sa.in() == CosNotifyChannelAdmin::SupplierAdmin::_nil() ){
      std::cerr << "sa is nil!" << std::endl;
      return 1;
    }

    //Obtain a Proxy Consumer
    CosNotifyChannelAdmin::ProxyID proxy_id;
    CosNotifyChannelAdmin::ClientType ctype = CosNotifyChannelAdmin::STRUCTURED_EVENT;

    CosNotifyChannelAdmin::ProxyConsumer_var proxyCon_obj;
    try
    {
      proxyCon_obj = sa->obtain_notification_push_consumer(ctype, proxy_id);
    }
    catch(CosNotifyChannelAdmin::AdminLimitExceeded err)
    {
      std::cerr << "CosNotifyChannelAdmin::AdminLimitExceeded Exception!" << std::endl;
      throw;
    }

    CosNotifyChannelAdmin::StructuredProxyPushConsumer_var ppc =
      CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow(proxyCon_obj.in());

    //Connecting a Supplier to a Proxy Consumer
    try
    {
      ppc->connect_structured_push_supplier(sps.in());
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


    //Demo::demo_send_heart_beat(ppc);
    //Send a Demo Notification

    CosNotification::StructuredEvent event;

    event.header.fixed_header.event_type.domain_name =
      CORBA::string_dup("Test_domain");
    event.header.fixed_header.event_type.type_name =
      CORBA::string_dup("Test_type_name");

    event.header.variable_header.length(0);
    event.remainder_of_body <<= "";

    std::cout << "Sending a demo event...," << std::endl;
    std::cout << "event.header.fixed_header.event_type.domain_name = "
      << event.header.fixed_header.event_type.domain_name
      << std::endl;
    std::cout << "event.header.fixed_header.event_type.type_name = "
      << event.header.fixed_header.event_type.type_name
      << std::endl;


    try{
      ppc->push_structured_event(event);
    }
    catch (CORBA::SystemException& se)
    {
      std::cerr << "System exception occurred during push: "
        << se << std::endl;
      throw;
    }
    catch (CORBA::Exception&)
    {
      std::cerr << "Unknown exception occurred during push" << std::endl;
      throw;
    }

    try{
      ppc->disconnect_structured_push_consumer();
    }
    catch(CORBA::Exception&)
    {
      std::cerr << "Disconnect fail!" << std::endl;
    }

    orb->shutdown();
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Supplier done.\n")));
  }
  catch(...)
  {
    std::cerr << "Supplier: Some exceptions was caught!" << std::endl;
    return 1;
  }


  return 0;
}

