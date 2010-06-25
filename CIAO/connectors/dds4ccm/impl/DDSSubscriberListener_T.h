/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDSSUBSCRIBERLISTENER_T_H_
#define DDSSUBSCRIBERLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_NDDS==1)
#include "ndds/ndds_cpp.h"
#endif

#if (CIAO_DDS4CCM_OPENDDS==1)
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
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DataReader_T;

    template <typename DDS_TYPE, typename CCM_TYPE>
    class CCM_DDS_SubscriberListener_T :
      public virtual ::DDSSubscriberListener
    {
    typedef CCM_DDS_Subscriber_T<DDS_TYPE, CCM_TYPE> Subscriber_type;
    typedef DataReader_T<DDS_TYPE, CCM_TYPE> DataReader_type;
    public:
      /// Constructor
      CCM_DDS_SubscriberListener_T ( ::DDS::SubscriberListener_ptr s);

      /// Destructor
      virtual ~CCM_DDS_SubscriberListener_T (void);

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

    ::DDS::CCM_DataReader_ptr
      get_datareader_proxy (::DDSDataReader * the_reader);
    };
  }
}

#include "dds4ccm/impl/DDSSubscriberListener_T.cpp"

#endif /* DDSSUBSCRIBERLISTENER_T_H_ */

