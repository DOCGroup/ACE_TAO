/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DOMAINPARTICIPANTLISTENER_H_
#define DOMAINPARTICIPANTLISTENER_H_

#include "dds4ccm/idl/ndds/ndds_dcpsC.h"
#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
# include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace NDDS
  {
    class DDS4CCM_NDDS_Export DDS_DomainParticipantListener_i :
      public ::DDSDomainParticipantListener
    {
    public:
      /// Constructor
      DDS_DomainParticipantListener_i (::DDS::DomainParticipantListener_ptr p);

      /// Destructor
      virtual ~DDS_DomainParticipantListener_i (void);

      ::DDS::DomainParticipantListener_ptr get_domainparticipantlistener (void);

      virtual void
      on_inconsistent_topic (
        ::DDSTopic*,
        const ::DDS_InconsistentTopicStatus & );

      virtual void
      on_requested_deadline_missed (
        ::DDSDataReader* ,
        const ::DDS_RequestedDeadlineMissedStatus & );

      virtual void
      on_requested_incompatible_qos (
        ::DDSDataReader* ,
        const ::DDS_RequestedIncompatibleQosStatus & );

      virtual void
      on_sample_rejected (
        ::DDSDataReader*,
        const ::DDS_SampleRejectedStatus & );

      virtual void
      on_liveliness_changed (
        ::DDSDataReader* ,
        const ::DDS_LivelinessChangedStatus & );

      virtual void
      on_data_available (
        ::DDSDataReader* );

      virtual void
      on_subscription_matched (
        ::DDSDataReader* ,
        const ::DDS_SubscriptionMatchedStatus & );

      virtual void
      on_sample_lost (
        ::DDSDataReader* ,
        const ::DDS_SampleLostStatus & );

      virtual void
      on_offered_deadline_missed (
        ::DDSDataWriter* ,
        const ::DDS_OfferedDeadlineMissedStatus & );

      virtual void
      on_offered_incompatible_qos (
        ::DDSDataWriter* ,
        const ::DDS_OfferedIncompatibleQosStatus & );

      virtual void
      on_liveliness_lost (
        ::DDSDataWriter* ,
        const ::DDS_LivelinessLostStatus & );

      virtual void
      on_publication_matched (
        ::DDSDataWriter* ,
        const ::DDS_PublicationMatchedStatus & );

      virtual void
      on_data_on_readers (
        ::DDSSubscriber* );


      void set_dds_dp (::DDS::DomainParticipant_ptr dp);
    private:
      ::DDS::DomainParticipantListener_var impl_;
      ::DDS::DomainParticipant_var dp_;

      ::DDS::DataReader_ptr
      create_datareader (DDSDataReader * reader);

      ::DDS::DataWriter_ptr
      create_datawriter (DDSDataWriter * writer);

    };
  }
}

#endif /* DOMAINPARTICIPANTLISTENER_H_ */
