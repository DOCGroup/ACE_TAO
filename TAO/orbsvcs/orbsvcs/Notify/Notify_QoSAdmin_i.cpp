/* -*- C++ -*- $Id$ */

#include "Notify_QoSAdmin_i.h"
#include "Notify_Extensions.h"

#if !defined (__ACE_INLINE__)
#include "Notify_QoSAdmin_i.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, Notify_QoSAdmin_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_QoSAdmin_i::TAO_Notify_QoSAdmin_i (void)
  :event_reliability_ (CosNotification::BestEffort),
   event_reliability_set_ (0),
   connection_reliability_ (CosNotification::BestEffort),
   connection_reliability_set_ (0),
   priority_ (CosNotification::DefaultPriority),
   priority_set_ (0),
   timeout_ (0),
   timeout_set_ (0),
   start_time_supported_ (0),
   start_time_supported_set_ (0),
   stop_time_supported_ (0),
   stop_time_supported_set_ (0),
   max_events_per_consumer_ (0),
   max_events_per_consumer_set_ (0),
   order_policy_ (CosNotification::AnyOrder),
   order_policy_set_ (0),
   discard_policy_ (CosNotification::AnyOrder),
   discard_policy_set_ (0),
   maximum_batch_size_ (1),
   maximum_batch_size_set_ (0),
   pacing_interval_ (0),
   pacing_interval_set_ (0),
   blocking_timeout_ (0),
   blocking_timeout_set_ (0)
{
}

// Implementation skeleton destructor
TAO_Notify_QoSAdmin_i::~TAO_Notify_QoSAdmin_i (void)
{
}

CosNotification::QoSProperties * TAO_Notify_QoSAdmin_i::get_qos (
    TAO_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
       CORBA::SystemException
     ))
{
  static const unsigned int property_count = 12;
  CosNotification::QoSProperties_var properties;
  ACE_NEW_THROW_EX (properties,
                    CosNotification::AdminProperties (property_count),
                    CORBA::NO_MEMORY ());

  properties->length (property_count);
  unsigned int index = 0;

  if (this->event_reliability_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::EventReliability);
      (*properties)[index++].value <<= this->event_reliability_;
    }
  if (this->connection_reliability_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::ConnectionReliability);
      (*properties)[index++].value <<= this->connection_reliability_;
    }
  if (this->priority_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::Priority);
      (*properties)[index++].value <<= this->priority_;
    }
  if (this->timeout_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::Timeout);
      (*properties)[index++].value <<= this->timeout_;
    }
  if (this->blocking_timeout_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (TAO_Notify_Extensions::BlockingPolicy);
      (*properties)[index++].value <<= this->blocking_timeout_;
    }
  if (this->start_time_supported_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::StartTimeSupported);
      (*properties)[index++].value <<= CORBA::Any::from_boolean (
                                         this->start_time_supported_);
    }
  if (this->stop_time_supported_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::StopTimeSupported);
      (*properties)[index++].value <<= CORBA::Any::from_boolean (
                                         this->stop_time_supported_);
    }
  if (this->max_events_per_consumer_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::MaxEventsPerConsumer);
      (*properties)[index++].value <<= this->max_events_per_consumer_;
    }
  if (this->order_policy_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::OrderPolicy);
      (*properties)[index++].value <<= this->order_policy_;
    }
  if (this->discard_policy_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::DiscardPolicy);
      (*properties)[index++].value <<= this->discard_policy_;
    }
  if (this->maximum_batch_size_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::MaximumBatchSize);
      (*properties)[index++].value <<= this->maximum_batch_size_;
    }
  if (this->pacing_interval_set_)
    {
      (*properties)[index].name =
        CORBA::string_dup (CosNotification::PacingInterval);
      (*properties)[index++].value <<= this->pacing_interval_;
    }

  // Set the length
  properties->length (index);

  return properties._retn ();
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
      ACE_CString property_name (qos[index].name);

      if (property_name.compare (CosNotification::EventReliability) == 0)
        {
          CosNotification::PropertyErrorSeq err_seq (1);
          err_seq.length (1);

          err_seq[0].code = CosNotification::UNSUPPORTED_PROPERTY;
          err_seq[0].name = CORBA::string_dup (CosNotification::EventReliability);

          this->event_reliability_set_ = 1;
          ACE_THROW (CosNotification::UnsupportedQoS (err_seq));
        }
      else if (property_name.compare (CosNotification::ConnectionReliability) == 0)
        {
          CosNotification::PropertyErrorSeq err_seq (1);
          err_seq.length (1);

          err_seq[0].code = CosNotification::UNSUPPORTED_PROPERTY;
          err_seq[0].name = CORBA::string_dup (CosNotification::ConnectionReliability);

          this->connection_reliability_set_ = 1;
          ACE_THROW (CosNotification::UnsupportedQoS (err_seq));
        }
      else if (property_name.compare (CosNotification::Priority) == 0)
        {
          qos[index].value >>= this->priority_;
          this->priority_set_ = 1;
        }
      else if (property_name.compare (CosNotification::Timeout) == 0)
        {
          qos[index].value >>= this->timeout_;
          this->timeout_set_ = 1;
        }
      else if (property_name.compare (TAO_Notify_Extensions::BlockingPolicy) == 0)
        {
          qos[index].value >>= this->blocking_timeout_;
          this->blocking_timeout_set_ = 1;
        }
      else if (property_name.compare (CosNotification::StartTimeSupported) == 0)
        {
          qos[index].value >>= CORBA::Any::to_boolean (this->start_time_supported_);
          this->start_time_supported_set_ = 1;
        }
      else if (property_name.compare (CosNotification::StopTimeSupported) == 0)
        {
          qos[index].value >>= CORBA::Any::to_boolean (this->stop_time_supported_);
          this->stop_time_supported_set_ = 1;
        }
      else if (property_name.compare (CosNotification::MaxEventsPerConsumer) == 0)
        {
          qos[index].value >>= this->max_events_per_consumer_;
          this->max_events_per_consumer_set_ = 1;
        }
      else if (property_name.compare (CosNotification::OrderPolicy) == 0)
        {
          qos[index].value >>= this->order_policy_;
          this->order_policy_set_ = 1;
        }
      else if (property_name.compare (CosNotification::DiscardPolicy) == 0)
        {
          qos[index].value >>= this->discard_policy_;
          this->discard_policy_set_ = 1;
        }
      else if (property_name.compare (CosNotification::MaximumBatchSize) == 0)
        {
          qos[index].value >>= this->maximum_batch_size_;
          this->maximum_batch_size_set_ = 1;
        }
      else if (property_name.compare (CosNotification::PacingInterval) == 0)
        {
          qos[index].value >>= this->pacing_interval_;
          this->pacing_interval_set_ = 1;
        }
    }
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
