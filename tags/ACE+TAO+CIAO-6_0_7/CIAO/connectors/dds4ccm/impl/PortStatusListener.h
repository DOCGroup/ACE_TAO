/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef PORTSTATUSLISTENER_T_H_
#define PORTSTATUSLISTENER_T_H_

#include "dds4ccm/idl/dds4ccm_PortStatusListenerC.h"
#include "dds4ccm/impl/dds4ccm_dds_impl_export.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Reactor;
ACE_END_VERSIONED_NAMESPACE_DECL

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export PortStatusListener :
      public ::DDS::DataReaderListener
    {
    public:
      /// Constructor
      PortStatusListener (
        ::CCM_DDS::PortStatusListener_ptr port_status_listener,
        ACE_Reactor* reactor);

      /// Destructor
      virtual ~PortStatusListener (void);

      virtual void on_requested_deadline_missed (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::RequestedDeadlineMissedStatus & status);

      virtual void on_sample_lost (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SampleLostStatus & status);

      virtual void
      on_requested_incompatible_qos (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedIncompatibleQosStatus & );

      virtual void
      on_sample_rejected (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleRejectedStatus & );

      virtual void
      on_liveliness_changed (
        ::DDS::DataReader_ptr ,
        const ::DDS::LivelinessChangedStatus & );

      virtual void
      on_data_available (
        ::DDS::DataReader_ptr );

      virtual void
      on_subscription_matched (
        ::DDS::DataReader_ptr ,
        const ::DDS::SubscriptionMatchedStatus & );

      static ::DDS::StatusMask get_mask (
        ::CCM_DDS::PortStatusListener_ptr psl);

    private:
      ::CCM_DDS::PortStatusListener_var port_status_listener_;
    protected:
      ACE_Reactor* reactor_;
    };
  }
}

#endif /* PORTSTATUSLISTENER_T_H_ */
