/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS_SUBSCRIBERLISTENER_T_H_
#define DDS_SUBSCRIBERLISTENER_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class SubscriberListener_T :
      public ::DDS::SubscriberListener,
      private ACE_Copy_Disabled
    {
    public:
      /// Constructor
      SubscriberListener_T (
        typename CCM_TYPE::context_type::_ptr_type context,
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

      /// Destructor
      virtual ~SubscriberListener_T (void);

      virtual void on_requested_incompatible_qos (
        ::DDS::DataReader_ptr the_reader,
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

      static ::DDS::StatusMask get_mask (void);

    private:
    typename CCM_TYPE::context_type::_var_type context_;
      ::CCM_DDS::ConnectorStatusListener_var error_listener_;
    };
  }
}

#include "dds4ccm/impl/ndds/SubscriberListener_T.cpp"

#endif /* DDS_SUBSCRIBERLISTENER_T_H_ */
