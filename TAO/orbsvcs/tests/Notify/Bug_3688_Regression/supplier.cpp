#include "common.h"
#include "ace/OS_NS_unistd.h"

namespace CosNotifyCommImpl
{
  class StructuredPushSupplier : public virtual POA_CosNotifyComm::StructuredPushSupplier
  {
  public:
    StructuredPushSupplier () = default;
    StructuredPushSupplier (StructuredPushSupplier const&) = delete;
    StructuredPushSupplier (StructuredPushSupplier&&) = delete;
    StructuredPushSupplier& operator= (StructuredPushSupplier const&) = delete;
    StructuredPushSupplier& operator= (StructuredPushSupplier&&) = delete;
    ~StructuredPushSupplier () override = default;

    void disconnect_structured_push_supplier () override
    {
    }

    void subscription_change (
      CosNotification::EventTypeSeq const&,
      CosNotification::EventTypeSeq const&) override
    {
    }
  };
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();
      poa_manager->activate ();

      std::cout << "Create event_channel now" << std::endl;
      CosNotifyChannelAdmin::EventChannel_var const event_channel = get_event_channel (orb.in ());

      CosNotifyCommImpl::StructuredPushSupplier* const supplier_impl =
        new CosNotifyCommImpl::StructuredPushSupplier;
      PortableServer::ServantBase_var supplier_servant (supplier_impl);
      CosNotifyComm::StructuredPushSupplier_var supplier = supplier_impl->_this ();

      CosNotifyChannelAdmin::SupplierAdmin_var const supplier_admin =
        event_channel->default_supplier_admin ();
      if (supplier_admin.in () == CosNotifyChannelAdmin::SupplierAdmin::_nil ())
        {
          std::cerr << "sa is nil!" << std::endl;
          return 1;
        }

      CosNotifyChannelAdmin::ProxyID proxy_id {};
      CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer_object;
      try
        {
          proxy_consumer_object = supplier_admin->obtain_notification_push_consumer (
            CosNotifyChannelAdmin::STRUCTURED_EVENT,
            proxy_id);
        }
      catch (CosNotifyChannelAdmin::AdminLimitExceeded const&)
        {
          std::cerr << "CosNotifyChannelAdmin::AdminLimitExceeded Exception!" << std::endl;
          throw;
        }

      CosNotifyChannelAdmin::StructuredProxyPushConsumer_var const proxy_consumer =
        CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxy_consumer_object.in ());

      try
        {
          proxy_consumer->connect_structured_push_supplier (supplier.in ());
        }
      catch (CosEventChannelAdmin::AlreadyConnected const&)
        {
          std::cerr << "CosEventChannelAdmin::AlreadyConnected" << std::endl;
          throw;
        }
      catch (CORBA::SystemException const& ex)
        {
          std::cerr << "System exception occurred during connect: " << ex << std::endl;
          throw;
        }

      CosNotification::StructuredEvent event {};
      event.header.fixed_header.event_type.domain_name = "Test_domain";
      event.header.fixed_header.event_type.type_name = "Test_type_name";
      event.header.variable_header.length (0);

      CORBA::Long const data = 5;
      event.filterable_data.length (1);
      event.filterable_data[0].name = "data";
      event.filterable_data[0].value <<= data;
      event.remainder_of_body <<= data;

      std::cout << "Sending a demo event...," << std::endl;
      std::cout << "event.header.fixed_header.event_type.domain_name = "
                << event.header.fixed_header.event_type.domain_name << std::endl;
      std::cout << "event.header.fixed_header.event_type.type_name = "
                << event.header.fixed_header.event_type.type_name << std::endl;
      std::cout << "event.filterable_data: data=" << data << std::endl;

      try
        {
          proxy_consumer->push_structured_event (event);
        }
      catch (CORBA::SystemException const& ex)
        {
          std::cerr << "System exception occurred during push: " << ex << std::endl;
          throw;
        }
      catch (CORBA::Exception const&)
        {
          std::cerr << "Unknown exception occurred during push" << std::endl;
          throw;
        }

      ACE_OS::sleep (5);

      try
        {
          proxy_consumer->disconnect_structured_push_consumer ();
        }
      catch (CORBA::Exception const&)
        {
          std::cerr << "Disconnect fail!" << std::endl;
        }

      orb->shutdown ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Supplier done.\n")));
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Supplier: ");
      return 1;
    }
  catch (...)
    {
      std::cerr << "Supplier: Some exceptions was caught!" << std::endl;
      return 1;
    }

  return 0;
}
