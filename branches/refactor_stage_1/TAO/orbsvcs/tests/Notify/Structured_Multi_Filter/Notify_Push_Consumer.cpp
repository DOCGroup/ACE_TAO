// $Id$
#include "Notify_Push_Consumer.h"
#include "tao/debug.h"

CORBA::Short Notify_Push_Consumer::event_count = 0;

Notify_Push_Consumer::Notify_Push_Consumer (const char* name)
 : name_ (name)
{
}

void
Notify_Push_Consumer::connect (
       CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin,
       CosNotifyChannelAdmin::EventChannel_ptr notify_channel,
       CORBA::Boolean isFilter
       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (
      CosNotifyChannelAdmin::STRUCTURED_EVENT,
      proxy_supplier_id_
      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //add filter
  if (isFilter)
    {
      CosNotifyFilter::FilterFactory_var ffact =
        notify_channel->default_filter_factory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CosNotifyFilter::Filter_var filter =
        ffact->create_filter ("TCL" ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      if (CORBA::is_nil (filter.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      " (%P|%t) Consumer unable to initialize filter.\n"));
          exit (1);
        }

      CosNotifyFilter::ConstraintExpSeq constraint_list (1);
      constraint_list.length (1);

      constraint_list[0].event_types.length (0);
      constraint_list[0].constraint_expr = CORBA::string_dup ("Number != 100");

      filter->add_constraints (constraint_list ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // apply filter
      proxysupplier->add_filter (filter.in ());
    }


  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::_narrow (
      proxysupplier.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_supplier_->connect_structured_push_consumer (objref.in ()
                                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // give ownership to POA
  this->_remove_ref ();
}



CORBA::Short
Notify_Push_Consumer::get_count ()
{
  return event_count;
}



static const char*
Any_String (const CORBA::Any& any)
{
  static char out[256] = "";
  CORBA::Short s;
  CORBA::UShort us;
  CORBA::Long l;
  CORBA::ULong ul;
  CORBA::ULongLong ull;
  const char* str;

  if (any >>= s)
    {
      ACE_OS::sprintf (out, "%d", s);
    }
  else if (any >>= us)
    {
      ACE_OS::sprintf (out, "%u", us);
    }
  else if (any >>= l)
    {
      ACE_OS::sprintf (out, "%d", l);
    }
  else if (any >>= ul)
    {
      ACE_OS::sprintf (out, "%u", ul);
    }
  else if (any >>= str)
    {
      ACE_OS_String::strcpy (out, str);
    }
  else if (any >>= ull)
    {
#if defined (ACE_LACKS_LONGLONG_T)
      ACE_OS_String::strcpy (out, ull.as_string (out));
#else
      double temp =
# if defined (ACE_CONFIG_WIN32_H)
      ACE_static_cast(double, ACE_static_cast (CORBA::LongLong, ull));
# else
              ull;
# endif /* ACE_CONFIG_WIN32_H */

      ACE_OS::sprintf (out, "%.0f", temp);
#endif /* ACE_LACKS_LONGLONG_T */
    }
  else
    {
      ACE_OS_String::strcpy (out, "Unsupported Any Type");
    }

  return out;
}


void
Notify_Push_Consumer::push_structured_event (
                          const CosNotification::StructuredEvent& event
                          ACE_ENV_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "%s %d (sent recv) %s %s\n",
                Any_String (event.filterable_data[1].value),
                event_count,
                (const char*)event.filterable_data[0].name,
                Any_String (event.filterable_data[0].value)));

  event_count ++;
}
