/* -*- C++ -*- $Id$ */
#include "orbsvcs/orbsvcs/Notify/Notify_StructuredPushConsumer.h"
#include "orbsvcs/orbsvcs/Notify/Notify_StructuredPushSupplier.h"
#include "Filter.h"

// @@ Pradeep: local #includes should go first.

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"
#define CA_FILTER "threshold < 20"
#define SA_FILTER "threshold > 10"
// @@ Pradeep: it is 'GRAMMAR' not 'GRAMMER'
#define TCL_GRAMMER "TCL"

// @@ Pradeep: please don't put classes in the .cpp file...

class PushConsumer : public TAO_Notify_StructuredPushConsumer
{
private:
  ACE_CString myname_;

public:
  PushConsumer (const char* myname)
  {
    myname_ = myname;
  }

  virtual void push_structured_event (
        const CosNotification::StructuredEvent & notification,
        CORBA::Environment &/*ACE_TRY_ENV*/
        )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventComm::Disconnected
                     ))
  {
    CORBA::Long val;

    notification.remainder_of_body >>= val;

    // @@ Pradeep: for your tests try to make sure that you count the
    // number of expected and sent events to verify that things work
    // correctly in an automatic way...

    ACE_DEBUG ((LM_DEBUG,
                "%s received event %d\n", myname_.fast_rep (),
                val));
  }
};

class PushSupplier : public TAO_Notify_StructuredPushSupplier
{
private:
  ACE_CString myname_;

public:
  PushSupplier (const char* myname)
  {
    myname_ = myname;
  }

virtual void send_event (const CosNotification::StructuredEvent& event,
                           CORBA::Environment &ACE_TRY_ENV)
  {
    ACE_DEBUG ((LM_DEBUG,
                "%s is sending an event \n", myname_.fast_rep ()));
    // add you own constraints here.
    TAO_Notify_StructuredPushSupplier::send_event (event,
                                                   ACE_TRY_ENV);
    ACE_CHECK;
  }
};


FilterClient::FilterClient (void)
{
  // No-Op.
  ifgop_ = CosNotifyChannelAdmin::OR_OP;
}

FilterClient::~FilterClient ()
{
  // No-Op.
}

void
FilterClient::init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV)
{
  init_ORB (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;
  resolve_naming_service (ACE_TRY_ENV);
  ACE_CHECK;
  resolve_Notify_factory (ACE_TRY_ENV);
  ACE_CHECK;
  create_EC (ACE_TRY_ENV);
  ACE_CHECK;
  create_supplieradmin (ACE_TRY_ENV);
  ACE_CHECK;
  create_consumeradmin (ACE_TRY_ENV);
  ACE_CHECK;
  create_consumers (ACE_TRY_ENV);
  ACE_CHECK;
  create_suppliers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::run (CORBA::Environment &ACE_TRY_ENV)
{
  send_events (ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::init_ORB (int argc,
                      char *argv [],
                      CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (poa_object))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object,
                                  ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME,
                                            ACE_TRY_ENV);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::resolve_Notify_factory (CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name,
                                   ACE_TRY_ENV);
  ACE_CHECK;

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in (),
                                                         ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::create_EC (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id,
                                         ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
FilterClient::create_supplieradmin (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory (ACE_TRY_ENV);
  ACE_CHECK;

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var sa_filter =
    ffact->create_filter (TCL_GRAMMER, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (sa_filter.in ()));

  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (SA_FILTER);

  sa_filter->add_constraints (constraint_list, ACE_TRY_ENV);
  ACE_CHECK;

  supplier_admin_->add_filter (sa_filter.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient:: create_consumeradmin (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::AdminID adminid = 0;

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  CosNotifyFilter::FilterFactory_var ffact =
    ec_->default_filter_factory (ACE_TRY_ENV);
  ACE_CHECK;

  // setup a filter at the consumer admin
  CosNotifyFilter::Filter_var ca_filter =
    ffact->create_filter (TCL_GRAMMER, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ca_filter.in ()));

  /*  struct ConstraintExp {
                CosNotification::EventTypeSeq event_types;
                string constraint_expr;
                };
  */
  CosNotifyFilter::ConstraintExpSeq constraint_list (1);
  constraint_list.length (1);

  constraint_list[0].event_types.length (0);
  constraint_list[0].constraint_expr = CORBA::string_dup (CA_FILTER);

  ca_filter->add_constraints (constraint_list, ACE_TRY_ENV);
  ACE_CHECK;

  consumer_admin_->add_filter (ca_filter.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::create_consumers (CORBA::Environment &ACE_TRY_ENV)
{
  // startup the first consumer.
  ACE_NEW_THROW_EX (consumer_1,
                    PushConsumer ("consumer1"),
                    CORBA::NO_MEMORY ());

  consumer_1->init (this->root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  consumer_1->connect (consumer_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  // startup the second consumer.
  ACE_NEW_THROW_EX (consumer_2,
                    PushConsumer ("consumer1"),
                    CORBA::NO_MEMORY ());

  consumer_2->init (this->root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  consumer_2->connect (consumer_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::create_suppliers (CORBA::Environment &ACE_TRY_ENV)
{
  // startup the first supplier
  ACE_NEW_THROW_EX (supplier_1,
                    PushSupplier ("supplier1"),
                    CORBA::NO_MEMORY ());

  supplier_1->init (this->root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  supplier_1->connect (supplier_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  // startup the second supplier
  ACE_NEW_THROW_EX (supplier_2,
                    PushSupplier ("supplier2"),
                    CORBA::NO_MEMORY ());

  supplier_2->init (this->root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  supplier_2->connect (supplier_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
FilterClient::send_events (CORBA::Environment &ACE_TRY_ENV)
{
  // operations:
  CosNotification::StructuredEvent event;

  // EventHeader

  // FixedEventHeader
  // EventType
  // string
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("myevent");

  // OptionalHeaderFields
  // PropertySeq
  // sequence<Property>: string name, any value
  event.header.variable_header.length (1); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup("threshold");

  event.filterable_data[1].name = CORBA::string_dup("temperature");
  event.filterable_data[1].value <<= (CORBA::Long)70;

  event.filterable_data[2].name = CORBA::string_dup("pressure");
  event.filterable_data[2].value <<= (CORBA::Long)80;

  for (int i = 0; i < 30; i++)
    {
      event.filterable_data[0].value <<= (CORBA::Long)i;

      // any
      event.remainder_of_body <<= (CORBA::Long)i;

      supplier_1->send_event (event, ACE_TRY_ENV);
      ACE_CHECK;

      supplier_2->send_event (event, ACE_TRY_ENV);
      ACE_CHECK;
    }
}
