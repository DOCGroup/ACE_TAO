/* -*- C++ -*- $Id$ */

#include "Notify_QoSAdmin_i.h"

ACE_RCSID(Notify, Notify_QoSAdmin_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_QoSAdmin_i::TAO_Notify_QoSAdmin_i (void)
  :event_reliability_ (CosNotification::BestEffort),
   connection_reliability_ (CosNotification::BestEffort),
   priority_ (CosNotification::DefaultPriority),
   timeout_ (0),
   start_time_supported_ (0),
   stop_time_supported_ (0),
   max_events_per_consumer_ (0),
   order_policy_ (CosNotification::AnyOrder),
   discard_policy_ (CosNotification::AnyOrder),
   maximum_batch_size_ (1),
   pacing_interval_ (0)
{
}

// Implementation skeleton destructor
TAO_Notify_QoSAdmin_i::~TAO_Notify_QoSAdmin_i (void)
{
}

CosNotification::QoSProperties * TAO_Notify_QoSAdmin_i::get_qos (
    TAO_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((
       CORBA::SystemException
     ))
{
  //Add your implementation here
  return 0;
}

void
TAO_Notify_QoSAdmin_i::set_qos (const CosNotification::QoSProperties & qos
    TAO_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ))
{
  for (CORBA::ULong index = 0; index < qos.length (); ++index)
    {
      ACE_CString property_name(qos[index].name);

      if (property_name.compare (CosNotification::EventReliability) == 0)
        {
          CosNotification::PropertyErrorSeq err_seq(1);
          err_seq.length (1);

          err_seq[0].code = CosNotification::UNSUPPORTED_PROPERTY;
          err_seq[0].name = CORBA::string_dup (CosNotification::EventReliability);

          ACE_THROW (CosNotification::UnsupportedQoS (err_seq));
        }
      else if (property_name.compare (CosNotification::ConnectionReliability) == 0)
        {
          CosNotification::PropertyErrorSeq err_seq(1);
          err_seq.length (1);

          err_seq[0].code = CosNotification::UNSUPPORTED_PROPERTY;
          err_seq[0].name = CORBA::string_dup (CosNotification::ConnectionReliability);

          ACE_THROW (CosNotification::UnsupportedQoS (err_seq));
        }
      else if (property_name.compare (CosNotification::Priority) == 0)
        {
          qos[index].value >>= this->priority_;
        }
      else if (property_name.compare (CosNotification::Timeout))
        {
          qos[index].value >>= this->timeout_;
        }
      else if (property_name.compare (CosNotification::StartTimeSupported))
        {
          qos[index].value >>= CORBA::Any::to_boolean (this->start_time_supported_);
        }
      else if (property_name.compare (CosNotification::StopTimeSupported))
        {
          qos[index].value >>= CORBA::Any::to_boolean (this->stop_time_supported_);
        }
      else if (property_name.compare (CosNotification::MaxEventsPerConsumer))
        {
          qos[index].value >>= this->max_events_per_consumer_;
        }
      else if (property_name.compare (CosNotification::OrderPolicy))
        {
          qos[index].value >>= this->order_policy_;
        }
      else if (property_name.compare (CosNotification::DiscardPolicy))
        {
          qos[index].value >>= this->discard_policy_;
        }
      else if (property_name.compare (CosNotification::MaximumBatchSize))
        {
          qos[index].value >>= this->maximum_batch_size_;
        }
      else if (property_name.compare (CosNotification::DiscardPolicy))
        {
          qos[index].value >>= this->pacing_interval_;
        }
    }
  return;
}

void
TAO_Notify_QoSAdmin_i::validate_qos (
    const CosNotification::QoSProperties & /*required_qos*/,
    CosNotification::NamedPropertyRangeSeq_out /*available_qos*/
    TAO_ENV_ARG_DECL_NOT_USED //TAO_ENV_SINGLE_ARG_PARAMETER
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ))
{
  //Add your implementation here
  return;
}
