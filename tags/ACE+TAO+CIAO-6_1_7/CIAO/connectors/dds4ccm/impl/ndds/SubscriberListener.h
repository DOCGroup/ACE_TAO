/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSSUBSCRIBERLISTENER_T_H_
#define DDSSUBSCRIBERLISTENER_T_H_

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_SubscriberListener_i :
      public ::DDSSubscriberListener
    {
    public:
      /// Constructor
      DDS_SubscriberListener_i (
        ::DDS::SubscriberListener_ptr s,
        ::DDS::DomainParticipant_ptr dp);

      /// Destructor
      virtual ~DDS_SubscriberListener_i (void);

      virtual void
      on_requested_deadline_missed (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedDeadlineMissedStatus & status);

      virtual void
      on_requested_incompatible_qos (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedIncompatibleQosStatus & status);

      virtual void
      on_sample_rejected (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleRejectedStatus & status);

      virtual void
      on_liveliness_changed (
        ::DDSDataReader* the_reader,
        const ::DDS_LivelinessChangedStatus & status);

      virtual void
      on_data_available (
        ::DDSDataReader* the_reader);

      virtual void
      on_subscription_matched (
        ::DDSDataReader* the_reader,
        const ::DDS_SubscriptionMatchedStatus & status);

      virtual void
      on_sample_lost (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleLostStatus & status);

      virtual void
      on_data_on_readers (
        ::DDSSubscriber* the_subscriber);

      ::DDS::SubscriberListener_ptr get_subscriber_listener (void);
    private:
      ::DDS::SubscriberListener_var impl_;
      ::DDS::DomainParticipant_var dp_;
    };
  }
}

#endif /* DDSSUBSCRIBERLISTENER_T_H_ */

