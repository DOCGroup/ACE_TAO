// $Id$
/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Eventhandler on orb thread, handling the
 * received data from DDS.
 */

#ifndef CIAO_DDS_CALLBACK_STATUS_HANDLER
#define CIAO_DDS_CALLBACK_STATUS_HANDLER

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "dds4ccm/idl/dds4ccm_PortStatusListenerC.h"
#include "ace/Event_Handler.h"
#include "dds4ccm_dds_impl_export.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_DDS_IMPL_Export OnUnexpectedStatusHandler :
      public ACE_Event_Handler
    {
      public:
        OnUnexpectedStatusHandler (
            ::CCM_DDS::ConnectorStatusListener_ptr error_listener,
            ::DDS::Entity_ptr entity,
            ::DDS::StatusKind status_kind);
        virtual ~OnUnexpectedStatusHandler ();

        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var error_listener_;
        ::DDS::Entity_var entity_;
        ::DDS::StatusKind status_kind_;
    };

    class DDS4CCM_DDS_IMPL_Export OnRequestedOncompatibleQosHandler :
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

    class DDS4CCM_DDS_IMPL_Export OnSampleRejectedHandler :
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

    class DDS4CCM_DDS_IMPL_Export OnInconsistentTopicHandler :
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

    class DDS4CCM_DDS_IMPL_Export OnOfferedDeadlineMissedHandler :
      public ACE_Event_Handler
    {
      public:
        OnOfferedDeadlineMissedHandler (
            ::CCM_DDS::ConnectorStatusListener_ptr csl,
            ::DDS::DataWriter_ptr dw,
            const ::DDS::OfferedDeadlineMissedStatus &status);
        virtual ~OnOfferedDeadlineMissedHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var csl_;
        ::DDS::DataWriter_var dw_;
        ::DDS::OfferedDeadlineMissedStatus const status_;
    };

    class DDS4CCM_DDS_IMPL_Export OnOfferedIncompatibleQoSHandler :
      public ACE_Event_Handler
    {
      public:
        OnOfferedIncompatibleQoSHandler (
            ::CCM_DDS::ConnectorStatusListener_ptr csl,
            ::DDS::DataWriter_ptr dw,
            const ::DDS::OfferedIncompatibleQosStatus &status);
        virtual ~OnOfferedIncompatibleQoSHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::ConnectorStatusListener_var csl_;
        ::DDS::DataWriter_var dw_;
        ::DDS::OfferedIncompatibleQosStatus const status_;
    };

    class DDS4CCM_DDS_IMPL_Export OnRequestedDeadlineMissedHandler :
      public ACE_Event_Handler
    {
      public:
        OnRequestedDeadlineMissedHandler (
            ::CCM_DDS::PortStatusListener_ptr psl,
            ::DDS::DataReader_ptr dr,
            const ::DDS::RequestedDeadlineMissedStatus &status);
        virtual ~OnRequestedDeadlineMissedHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::PortStatusListener_var psl_;
        ::DDS::DataReader_var dr_;
        ::DDS::RequestedDeadlineMissedStatus const status_;
    };

    class DDS4CCM_DDS_IMPL_Export OnSampleLostHandler :
      public ACE_Event_Handler
    {
      public:
        OnSampleLostHandler (
            ::CCM_DDS::PortStatusListener_ptr psl,
            ::DDS::DataReader_ptr dr,
            const ::DDS::SampleLostStatus &status);
        virtual ~OnSampleLostHandler ();
        virtual int handle_exception (ACE_HANDLE fc = ACE_INVALID_HANDLE);
      private:
        ::CCM_DDS::PortStatusListener_var psl_;
        ::DDS::DataReader_var dr_;
        ::DDS::SampleLostStatus const status_;
    };
  }
}

#endif /* CIAO_DDS_CALLBACK_STATUS_HANDLER */
