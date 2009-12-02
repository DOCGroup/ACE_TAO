// $Id$

#include "SubscriberListener.h"
#include "Subscriber.h"
#include "DataReader.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "RequestedDeadlineMissedStatus.h"
#include "SampleRejectedStatus.h"
#include "LivelinessChangedStatus.h"
#include "RequestedIncompatibleQosStatus.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_SubscriberListener_i::RTI_SubscriberListener_i (::DDS::SubscriberListener_ptr p)
        : impl_ ( ::DDS::SubscriberListener::_duplicate (p))
      {
      }

      // Implementation skeleton destructor
      RTI_SubscriberListener_i::~RTI_SubscriberListener_i (void)
      {
      }

      ::DDS::SubscriberListener_ptr
      RTI_SubscriberListener_i::get_subscriber_listener (void)
      {
        return ::DDS::SubscriberListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

