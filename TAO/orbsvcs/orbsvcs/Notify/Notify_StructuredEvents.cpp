// $Id$
#include "Notify_StructuredEvents.h"
#include "Notify_QoSAdmin_i.h"
#include "tao/debug.h"

ACE_RCSID (Notify, Notify_Event, "$Id$")


TAO_Notify_StructuredEvents::TAO_Notify_StructuredEvents (
                          const CosNotification::EventBatch& notifications)
  : data_ (notifications),
    match_called_ (0)
{
  if (this->data_.length () > 0)
    {
      this->event_type_ = this->data_[0].header.fixed_header.event_type;
    }
  this->init_QoS ();
}

TAO_Notify_StructuredEvents::~TAO_Notify_StructuredEvents ()
{
}

void
TAO_Notify_StructuredEvents::init_QoS (void)
{
  if (this->data_.length () > 0)
    {
      CosNotification::PropertySeq& qos = this->data_[0].header.variable_header;

      for (CORBA::ULong index = 0; index < qos.length (); ++index)
        {
          ACE_CString property_name (qos[index].name);

          if (property_name.compare (CosNotification::Priority) == 0)
            {
              qos[index].value >>= this->priority_;
            }
          else if (property_name.compare (CosNotification::StartTime) == 0)
            {
              // qos[index].value >>= this->start_time_;
            }
          else if (property_name.compare (CosNotification::StopTime) == 0)
            {
              // qos[index].value >>= this->stop_time_;
            }
          else if (property_name.compare (CosNotification::Timeout) == 0)
            {
              qos[index].value >>= this->timeout_;
            }
        }
    }
}

TAO_Notify_Event*
TAO_Notify_StructuredEvents::clone (void)
{
  TAO_Notify_StructuredEvents* clone;

  ACE_NEW_RETURN (clone, TAO_Notify_StructuredEvents (this->data_), 0);

  return clone;
}

void
TAO_Notify_StructuredEvents::operator=(const TAO_Notify_StructuredEvents& structured_events)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "In TAO_Notify_StructuredEvents::operator=\n"));

  if (this != &structured_events)
    {
      this->data_ = structured_events.data_;
      this->matching_ = structured_events.matching_;
      this->event_type_ = structured_events.event_type_;
    }
}

CORBA::Boolean
TAO_Notify_StructuredEvents::is_special_event_type (void) const
{
  return this->event_type_.is_special ();
}

const TAO_Notify_EventType&
TAO_Notify_StructuredEvents::event_type (void) const
{
  return this->event_type_;
}

CORBA::Boolean
TAO_Notify_StructuredEvents::do_match (CosNotifyFilter::Filter_ptr filter
                                       TAO_ENV_ARG_DECL)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvents::do_match ()\n"));

  CORBA::Boolean status = 0;
  CORBA::ULong index = this->matching_.length ();
  CORBA::ULong length = this->data_.length ();

  for (CORBA::ULong i = 0; i < length; i++)
    {
      CORBA::Boolean matched = filter->match_structured (this->data_[i]
                                                         TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (matched)
        {
          // Return TRUE if any event in the sequence matches
          status = 1;

          // Keep track of the position within the data sequence
          this->matching_.length (index + 1);
          this->matching_[index++] = i;
        }
    }
  this->match_called_ = 1;

  return status;
}

void
TAO_Notify_StructuredEvents::do_push (CosEventComm::PushConsumer_ptr consumer
                                      TAO_ENV_ARG_DECL) const
{
  CORBA::ULong sending_length  = (this->match_called_ ?
                                         this->matching_.length () :
                                         this->data_.length ());
  for (CORBA::ULong i = 0; i < sending_length; i++)
    {
      CORBA::Any any;
      any <<= this->data_[(this->match_called_ ?
                                  this->matching_[i] : i)];
      consumer->push (any TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Notify_StructuredEvents::do_push (CosNotifyComm::StructuredPushConsumer_ptr consumer
                                      TAO_ENV_ARG_DECL) const
{
  CORBA::ULong sending_length  = (this->match_called_ ?
                                         this->matching_.length () :
                                         this->data_.length ());
  for (CORBA::ULong i = 0; i < sending_length; i++)
    {
      consumer->push_structured_event (this->data_[(this->match_called_ ?
                                          this->matching_[i] : i)]
                                       TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Notify_StructuredEvents::do_push (
                            CosNotifyComm::SequencePushConsumer_ptr consumer,
                            const TAO_Notify_QoSAdmin_i& qos_admin,
                            CosNotification::EventBatch& unsent,
                            int flush_queue
                            TAO_ENV_ARG_DECL) const
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                          "TAO_Notify_StructuredEvents::do_push with "
                          "flush_queue = %d\n", flush_queue));

  CORBA::ULong unsent_length      = unsent.length ();
  CORBA::ULong maximum_batch_size = qos_admin.maximum_batch_size ();
  CORBA::ULong sending_length     = (this->match_called_ ?
                                         this->matching_.length () :
                                         this->data_.length ());
  CosNotification::EventBatch matched (maximum_batch_size);

  // Deal with the unsent events first
  CORBA::ULong queue_size = (flush_queue &&
                             unsent_length < maximum_batch_size ?
                                     unsent_length : maximum_batch_size);

  while (unsent_length > 0 && unsent_length >= queue_size)
    {
      // We can only send queue_size at a time!
      matched.length (queue_size);

      // Pack 'em up and send 'em out.
      for (CORBA::ULong i = 0; i < queue_size; i++)
        {
          matched[i] = unsent[i];
        }
      consumer->push_structured_events (matched TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Shift the rest of the unsent messages to the front.
      // We are reusing 'matched' for this purpose.
      matched.length (unsent_length - queue_size);
      for (CORBA::ULong j = queue_size; j < unsent_length; j++)
        {
          matched[j - queue_size] = unsent[j];
        }
      unsent = matched;
      unsent_length -= queue_size;

      // If we are flushing the queue and we don't have enough to
      // fill upto queue_size, then we need to reset queue_size to
      // the size of the rest of the unsent messages.
      if (flush_queue && unsent_length < queue_size)
        {
          queue_size = unsent_length;
        }
    }

    if (sending_length > 0)
      {
        CORBA::ULong start = 0;
        if (unsent_length + sending_length >= maximum_batch_size)
          {
            matched = unsent;
            matched.length (maximum_batch_size);

            // Add to the matched sequence until we are full
            CORBA::ULong index = 0;
            for (CORBA::ULong i = unsent_length; i < maximum_batch_size; i++)
              {
                index = i - unsent_length;
                matched[i] = this->data_[(this->match_called_ ?
                                          this->matching_[index] : index)];
                start++;
              }
            consumer->push_structured_events (matched TAO_ENV_ARG_PARAMETER);
            ACE_CHECK;

            unsent.length (0);
            unsent_length = 0;

            // Send some more events out
            while (start < sending_length &&
                   sending_length - start >= maximum_batch_size)
              {
                for (CORBA::ULong i = 0; i < maximum_batch_size; i++)
                  {
                    matched[i] = this->data_[(this->match_called_ ?
                                              this->matching_[start] : start)];
                    start++;
                  }
                consumer->push_structured_events (matched TAO_ENV_ARG_PARAMETER);
                ACE_CHECK;
              }
          }

          // append the rest to the unsent sequence
          unsent.length (unsent_length + (sending_length - start));
          for (CORBA::ULong j = start; j < sending_length; j++)
            {
              unsent[unsent_length + (j - start)] =
                               this->data_[(this->match_called_ ?
                                            this->matching_[j] : j)];
            }
      }
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Unbounded_Sequence<CORBA::ULong>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Unbounded_Sequence<CORBA::ULong>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
