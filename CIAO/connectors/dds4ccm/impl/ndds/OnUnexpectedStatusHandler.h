// $Id$
/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Eventhandler on orb thread, handling the
 * received data from DDS.
 */

#ifndef CIAO_ON_UNEXPECTEDSTATUS_HANDLER_T
#define CIAO_ON_UNEXPECTEDSTATUS_HANDLER_T

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "ace/Event_Handler.h"
#include "dds4ccm_ndds_impl_export.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_IMPL_Export On_Unexpected_Status_Handler :
      public ACE_Event_Handler
    {
      public:
        On_Unexpected_Status_Handler (
            ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
            ::DDS::Entity_ptr entity,
            ::DDS::StatusKind status_kind);
        virtual ~On_Unexpected_Status_Handler ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var error_listener_;
        ::DDS::Entity_var entity_;
        ::DDS::StatusKind status_kind_;
    };
    
    class DDS4CCM_NDDS_IMPL_Export OnRequestedOncompatibleQosHandler :
      public ACE_Event_Handler
    {
      public:
        OnRequestedOncompatibleQosHandler (
            ::CCM_DDS::ConnectorStatusListener_ptr csl,
            ::DDS::DataReader_ptr dr,
            const ::DDS::RequestedIncompatibleQosStatus &status);
        virtual ~OnRequestedOncompatibleQosHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var csl_;
        ::DDS::DataReader_var dr_;
        ::DDS::RequestedIncompatibleQosStatus const status_;
    };

    class DDS4CCM_NDDS_IMPL_Export OnSampleRejectedHandler :
      public ACE_Event_Handler
    {
      public:
        OnSampleRejectedHandler (
            ::CCM_DDS::ConnectorStatusListener_ptr csl,
            ::DDS::DataReader_ptr dr,
            const ::DDS::SampleRejectedStatus &status);
        virtual ~OnSampleRejectedHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var csl_;
        ::DDS::DataReader_var dr_;
        ::DDS::SampleRejectedStatus const status_;
    };

    class DDS4CCM_NDDS_IMPL_Export OnInconsistentTopicHandler :
      public ACE_Event_Handler
    {
      public:
        OnInconsistentTopicHandler (
            ::CCM_DDS::ConnectorStatusListener_ptr csl,
            ::DDS::Topic_ptr tp,
            const ::DDS::InconsistentTopicStatus &status);
        virtual ~OnInconsistentTopicHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var csl_;
        ::DDS::Topic_var tp_;
        ::DDS::InconsistentTopicStatus const status_;
    };
  }
}

#endif
