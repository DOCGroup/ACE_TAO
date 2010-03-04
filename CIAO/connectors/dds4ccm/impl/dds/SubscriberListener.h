/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_SUBSCRIBERLISTENER_H
#define DDS4CCM_SUBSCRIBERLISTENER_H

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds/dds4ccm_ndds_impl_export.h"

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::SubscriberListener DDSSubscriberListener;
typedef ::DDS::DataReader DDSDataReader;
typedef ::DDS::Subscriber DDSSubscriber;
typedef ::DDS::RequestedDeadlineMissedStatus DDS_RequestedDeadlineMissedStatus;
typedef ::DDS::LivelinessChangedStatus DDS_LivelinessChangedStatus;
typedef ::DDS::RequestedIncompatibleQosStatus DDS_RequestedIncompatibleQosStatus;
typedef ::DDS::SampleRejectedStatus DDS_SampleRejectedStatus;
typedef ::DDS::SubscriptionMatchedStatus DDS_SubscriptionMatchedStatus;
typedef ::DDS::SampleLostStatus DDS_SampleLostStatus;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_IMPL_Export CCM_DDS_SubscriberListener_i :
      public virtual ::DDSSubscriberListener
    {
    public:
      /// Constructor
      CCM_DDS_SubscriberListener_i ( ::DDS::SubscriberListener_ptr s);

      /// Destructor
      virtual ~CCM_DDS_SubscriberListener_i (void);

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
    };
  }
}

#endif

