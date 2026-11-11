#include "Notify_Supplier.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"
#include "ace/OS_main.h"
#include "ace/Log_Msg.h"
#include "tao/PortableServer/Servant_var.h"

constexpr char naming_service_name[] = "NameService";
constexpr char notify_tls_log_factory_name[] = "NotifyLogFactory";
CORBA::ULong const log_event_count {10};
constexpr char query_1[] = "id > 0";
constexpr char query_2[] = "id >= 0";
constexpr char query_lang[] = "TCL";
constexpr char sa_filter_constraint[] = "threshold > 10";
constexpr char tcl_grammar[] = "TCL";

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Supplier supplier;

  return supplier.run (argc, argv);
}

int
Supplier::run (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // ORB initialization boiler plate...
      this->orb_ =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var const naming_obj =
        this->orb_->resolve_initial_references (naming_service_name);

      // Need to check return value for errors.
      if (CORBA::is_nil (naming_obj.in ()))
        throw CORBA::UNKNOWN ();

      this->naming_context_ =
        CosNaming::NamingContext::_narrow (naming_obj.in ());

      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (notify_tls_log_factory_name);

      CORBA::Object_var const obj =
        this->naming_context_->resolve (name);

      this->notify_log_factory_ =
        DsNotifyLogAdmin::NotifyLogFactory::_narrow (obj.in ());

      ACE_ASSERT (!CORBA::is_nil (this->notify_log_factory_.in ()));

      // Create a log.
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nCalling NotifyLogFactory::create...\n")));

      DsLogAdmin::LogFullActionType const log_full_action {DsLogAdmin::halt};
      DsLogAdmin::CapacityAlarmThresholdList const threshold {};
      CORBA::ULongLong const max_size {}; // 0 means "infinite"

      DsLogAdmin::LogId logid {};
      CosNotification::QoSProperties const initial_qos {};
      CosNotification::AdminProperties const initial_admin {};

      this->notify_log_ = this->notify_log_factory_->create (log_full_action,
                                                             max_size,
                                                             threshold,
                                                             initial_qos,
                                                             initial_admin,
                                                             logid);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Create returned logid = %u\n"), logid));

      CosNotifyChannelAdmin::AdminID admin_id {};
      this->supplier_admin_ =
        this->notify_log_->new_for_suppliers (CosNotifyChannelAdmin::OR_OP, admin_id);
      ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

      CosNotifyFilter::FilterFactory_var const ffact =
        this->notify_log_->default_filter_factory ();

      // setup a filter at the consumer admin
      CosNotifyFilter::Filter_var const sa_filter =
        ffact->create_filter (tcl_grammar);

      ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup (sa_filter_constraint);

      sa_filter->add_constraints (constraint_list);

      this->supplier_admin_->add_filter (sa_filter.in ());

      // startup the first supplier
      // ACE_NEW_THROW_EX evaluates its target more than once, so pass a raw output pointer.
      Filter_StructuredPushSupplier *supplier_impl {};
      ACE_NEW_THROW_EX (supplier_impl,
                        Filter_StructuredPushSupplier (),
                        CORBA::NO_MEMORY ());
      PortableServer::Servant_var<Filter_StructuredPushSupplier> const supplier_1 (
        supplier_impl);

      supplier_1->connect (this->supplier_admin_.in ());

      // Event to send through the notification service.
      CosNotification::StructuredEvent event {};
      event.header.fixed_header.event_type.domain_name = CORBA::string_dup ("*");
      event.header.fixed_header.event_type.type_name = CORBA::string_dup ("*");
      event.header.fixed_header.event_name = CORBA::string_dup ("myevent");

      event.header.variable_header.length (1);

      event.filterable_data.length (3);
      event.filterable_data[0].name = CORBA::string_dup ("threshold");

      event.filterable_data[1].name = CORBA::string_dup ("temperature");
      event.filterable_data[1].value <<= static_cast<CORBA::Long> (70);

      event.filterable_data[2].name = CORBA::string_dup ("pressure");
      event.filterable_data[2].value <<= static_cast<CORBA::Long> (80);

      event.filterable_data[0].value <<= static_cast<CORBA::Long> (4);

      event.remainder_of_body <<= static_cast<CORBA::Long> (4);

      for (CORBA::ULong k {}; k < log_event_count; ++k)
        {
          event.filterable_data[0].value <<= static_cast<CORBA::Long> (k);

          event.remainder_of_body <<= static_cast<CORBA::Long> (k);

          supplier_1->send_event (event);
        }

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Calling NotifyLog get_n_records...\n")));

      CORBA::ULongLong const record_count = this->notify_log_->get_n_records ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Number of records in Log = %Q\n"), record_count));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Calling NotifyLog::get_current_size...\n")));
      CORBA::ULongLong const current_size = this->notify_log_->get_current_size ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Size of data in Log = %Q\n"), current_size));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Querying the Log: %C\n"), query_1));
      DsLogAdmin::Iterator_var iter_out;
      DsLogAdmin::RecordList_var const rec_list =
        this->notify_log_->query (query_lang, query_1, iter_out);

      for (CORBA::ULong j {}; j < rec_list->length (); ++j)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("id = %Q, time= %Q\n"),
                    rec_list[j].id, rec_list[j].time));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Deleting records...\n")));

      CORBA::ULong const deleted_record_count =
        this->notify_log_->delete_records (query_lang, query_2);
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Deleted %u records\n"), deleted_record_count));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Calling NotifyLog::get_n_records...\n")));
      CORBA::ULongLong const remaining_record_count =
        this->notify_log_->get_n_records ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Number of records in Log after delete = %Q\n"),
                  remaining_record_count));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Getting the current size again...\n")));
      CORBA::ULongLong const remaining_size = this->notify_log_->get_current_size ();

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Size of data in Log = %Q\n"), remaining_size));

      this->notify_log_->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Supplier::run");
      return 1;
    }
  return 0;
}


void
Filter_StructuredPushSupplier::connect (
  CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin)
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this ();

  CosNotifyChannelAdmin::ProxyConsumer_var proxy_consumer =
    supplier_admin->obtain_notification_push_consumer (
      CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_consumer_id_);

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxy_consumer.in ());

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  this->proxy_consumer_->connect_structured_push_supplier (objref.in ());
}

void
Filter_StructuredPushSupplier::disconnect ()
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer ();
}

void
Filter_StructuredPushSupplier::subscription_change (
  CosNotification::EventTypeSeq const & /*added*/,
  CosNotification::EventTypeSeq const & /*removed*/)
{
  // No-op.
}

void
Filter_StructuredPushSupplier::send_event (
  CosNotification::StructuredEvent const &event)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->push_structured_event (event);
}

void
Filter_StructuredPushSupplier::disconnect_structured_push_supplier ()
{
  // No-Op.
}

