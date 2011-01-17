/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef SUBSCRIBERLISTENER_T_H_
#define SUBSCRIBERLISTENER_T_H_

#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export SubscriberListener :
      public ::DDS::SubscriberListener
    {
    public:
      /// Constructor
      SubscriberListener (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
        ACE_Reactor* reactor);

      /// Destructor
      virtual ~SubscriberListener (void);

      virtual void on_requested_incompatible_qos (
        ::DDS::DataReader_ptr reader,
        const ::DDS::RequestedIncompatibleQosStatus & status);

      virtual void on_liveliness_changed(
        ::DDS::DataReader* reader,
        const ::DDS::LivelinessChangedStatus& status);

      virtual void on_sample_rejected(
        ::DDS::DataReader* reader,
        const ::DDS::SampleRejectedStatus& status);

      virtual void on_subscription_matched(
        ::DDS::DataReader* reader,
        const ::DDS::SubscriptionMatchedStatus& status);

      virtual void on_data_on_readers(
        ::DDS::Subscriber* subscriber);

      virtual void on_requested_deadline_missed (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedDeadlineMissedStatus & );

      virtual void on_data_available (
        ::DDS::DataReader_ptr );

      virtual void on_sample_lost (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleLostStatus & );

      static ::DDS::StatusMask get_mask (
        ::CCM_DDS::ConnectorStatusListener_ptr csl);

    private:
      virtual void on_unexpected_status(
        ::DDS::Entity_ptr entity,
        ::DDS::StatusKind status_kind);

      ::CCM_DDS::ConnectorStatusListener_var error_listener_;

      ACE_Reactor* reactor_;
    };
  }
}

#endif /* SUBSCRIBERLISTENER_T_H_ */
