/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#include "dds4ccm/idl/dds_rtf2_dcpsEC.h"
#include "dds4ccm/idl/dds4ccm_ConnectorStatusListenerC.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "ace/Copy_Disabled.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    template <typename DDS_TYPE, typename CCM_TYPE>
    class DomainParticipantListener_T :
      public virtual ::DDS::CCM_DomainParticipantListener,
      public virtual ::CORBA::LocalObject,
      private virtual ACE_Copy_Disabled
    {
    public:
      /// Constructor
      DomainParticipantListener_T (
        ::CCM_DDS::ConnectorStatusListener_ptr error_listener);

      /// Destructor
      virtual ~DomainParticipantListener_T (void);

      static ::DDS::StatusMask get_mask (void);

      virtual void
      on_inconsistent_topic (
        ::DDS::Topic_ptr,
        const ::DDS::InconsistentTopicStatus & );

      virtual void
      on_requested_deadline_missed (
        ::DDS::DataReader_ptr ,
        const ::DDS::RequestedDeadlineMissedStatus & );

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

      virtual void
      on_sample_lost (
        ::DDS::DataReader_ptr ,
        const ::DDS::SampleLostStatus & );

      virtual void
      on_offered_deadline_missed (
        ::DDS::DataWriter_ptr ,
        const ::DDS::OfferedDeadlineMissedStatus & );

      virtual void
      on_offered_incompatible_qos (
        ::DDS::DataWriter_ptr ,
        const ::DDS::OfferedIncompatibleQosStatus & );

      virtual void
      on_liveliness_lost (
        ::DDS::DataWriter_ptr ,
        const ::DDS::LivelinessLostStatus & );

      virtual void
      on_publication_matched (
        ::DDS::DataWriter_ptr ,
        const ::DDS::PublicationMatchedStatus & );

      virtual void
      on_data_on_readers (
        ::DDS::Subscriber_ptr );

#if (CIAO_DDS4CCM_NDDS==1)
      virtual void on_reliable_writer_cache_changed (
        ::DDS::DataWriter_ptr ,
        const ::DDS::ReliableWriterCacheChangedStatus & );

      virtual void on_reliable_reader_activity_changed (
        ::DDS::DataWriter_ptr ,
        const ::DDS::ReliableReaderActivityChangedStatus & );
#endif

    private:
      ::CCM_DDS::ConnectorStatusListener_var error_listener_;
    };
  }
}

#include "dds4ccm/impl/DomainParticipantListener_T.cpp"
