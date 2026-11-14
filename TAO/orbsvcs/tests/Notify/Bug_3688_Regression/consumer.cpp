#include "common.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"

namespace CosNotifyCommImpl
{
  class StructuredPushConsumer : public virtual POA_CosNotifyComm::StructuredPushConsumer
  {
  public:
    StructuredPushConsumer () = default;
    StructuredPushConsumer (StructuredPushConsumer const&) = delete;
    StructuredPushConsumer (StructuredPushConsumer&&) = delete;
    StructuredPushConsumer& operator= (StructuredPushConsumer const&) = delete;
    StructuredPushConsumer& operator= (StructuredPushConsumer&&) = delete;
    ~StructuredPushConsumer () override = default;

    void push_structured_event (CosNotification::StructuredEvent const& event) override
    {
      this->received_ = true;
      std::cout << "Received an event...," << std::endl;
      std::cout << "event.header.fixed_header.event_type.domain_name = "
                << event.header.fixed_header.event_type.domain_name << std::endl;
      std::cout << "event.header.fixed_header.event_type.type_name = "
                << event.header.fixed_header.event_type.type_name << std::endl;
    }

    void disconnect_structured_push_consumer () override
    {
    }

    void offer_change (
      CosNotification::EventTypeSeq const&,
      CosNotification::EventTypeSeq const&) override
    {
    }

    bool received_events () const
    {
      return this->received_;
    }

  private:
    bool received_ {};
  };
}

static int
parse_args (int argc, ACE_TCHAR* argv[], int& runtime)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("t:"));
  int option {};

  while ((option = get_opts ()) != -1)
    {
      switch (option)
        {
        case 't':
          runtime = ACE_OS::atoi (get_opts.opt_arg ());
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -t <runtime>\n"),
                             argv[0]),
                            -1);
        }
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR* argv[])
{
  int runtime { 5 };

  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa = PortableServer::POA::_narrow (poa_object.in ());
      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv, runtime) != 0)
        return 1;

      poa_manager->activate ();
      std::cout << "Get event_channel now" << std::endl;
      CosNotifyChannelAdmin::EventChannel_var const event_channel = get_event_channel (orb.in ());

      CosNotifyCommImpl::StructuredPushConsumer* const consumer_impl =
        new CosNotifyCommImpl::StructuredPushConsumer;
      PortableServer::ServantBase_var consumer_servant (consumer_impl);
      CosNotifyComm::StructuredPushConsumer_var structured_consumer = consumer_impl->_this ();

      CosNotifyChannelAdmin::AdminID admin_id {};
      CosNotifyChannelAdmin::ConsumerAdmin_var const consumer_admin =
        event_channel->new_for_consumers (CosNotifyChannelAdmin::AND_OP, admin_id);
      if (consumer_admin.in () == CosNotifyChannelAdmin::ConsumerAdmin::_nil ())
        {
          std::cerr << "ca is nil!" << std::endl;
          return 1;
        }

      CosNotifyChannelAdmin::ProxyID proxy_id {};
      CosNotifyChannelAdmin::ProxySupplier_var proxy_supplier_object;
      try
        {
          proxy_supplier_object = consumer_admin->obtain_notification_push_supplier (
            CosNotifyChannelAdmin::STRUCTURED_EVENT,
            proxy_id);
        }
      catch (CosNotifyChannelAdmin::AdminLimitExceeded const&)
        {
          std::cerr << "CosNotifyChannelAdmin::AdminLimitExceeded Exception!" << std::endl;
          throw;
        }

      CosNotifyChannelAdmin::StructuredProxyPushSupplier_var const proxy_supplier =
        CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (proxy_supplier_object.in ());

      CosNotifyFilter::FilterFactory_var const filter_factory =
        event_channel->default_filter_factory ();
      ACE_ASSERT (!CORBA::is_nil (filter_factory.in ()));
      CosNotifyFilter::Filter_var const filter = filter_factory->create_filter ("EXTENDED_TCL");

      CosNotification::EventTypeSeq event_types (2);
      event_types.length (2);
      event_types[0].domain_name = "Test_domain";
      event_types[0].type_name = "Test_type_name";
      event_types[1].domain_name = "DomainB";
      event_types[1].type_name = "TypeB";

      CosNotifyFilter::ConstraintExpSeq constraints (1);
      constraints.length (1);
      constraints[0].event_types = event_types;
      constraints[0].constraint_expr = "$data == 1 or $data == 2 or $data == 4";
      filter->add_constraints (constraints);
      proxy_supplier->add_filter (filter.in ());

      std::cout << "Attached a filter to ProxyPushSupplier" << std::endl;
      std::cout << "The filter's event_types[0].domain_name = " << event_types[0].domain_name << std::endl;
      std::cout << "The filter's event_types[0].type_name = " << event_types[0].type_name << std::endl;
      std::cout << "The filter's event_types[1].domain_name = " << event_types[1].domain_name << std::endl;
      std::cout << "The filter's event_types[1].type_name = " << event_types[1].type_name << std::endl;
      std::cout << "The filter's constraint_expr=" << constraints[0].constraint_expr << std::endl;

      try
        {
          proxy_supplier->connect_structured_push_consumer (structured_consumer.in ());
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
      ACE_Time_Value timeout (runtime);
      orb->run (timeout);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Consumer done.\n")));

      if (consumer_impl->received_events ())
        {
          std::cerr << "Test failed - should not receive test events." << std::endl;
          return 1;
        }

      std::cout << "Test passed - did not receive test events as expected." << std::endl;
    }
  catch (CORBA::Exception const& ex)
    {
      ex._tao_print_exception ("Consumer: ");
      return 1;
    }
  catch (...)
    {
      std::cerr << "Consumer: Some exceptions was caught!" << std::endl;
      return 1;
    }

  return 0;
}
