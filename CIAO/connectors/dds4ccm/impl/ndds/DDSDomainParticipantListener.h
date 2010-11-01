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

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"

#include "dds4ccm/impl/ndds/dds4ccm_ndds_export.h"
#include "dds4ccm/impl/dds4ccm_conf.h"

# include "ndds/ndds_cpp.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    class DDS4CCM_NDDS_Export DDS_DomainParticipantListener_i :
      public virtual ::DDSDomainParticipantListener
    {
    public:
      /// Constructor
      DDS_DomainParticipantListener_i (::DDS::DomainParticipantListener_ptr p);

      /// Destructor
      virtual ~DDS_DomainParticipantListener_i (void);

      ::DDS::DomainParticipantListener_ptr get_domainparticipantlistener (void);

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

    private:
      ::DDS::DomainParticipantListener_var impl_;
    };
  }
}

#endif /* DOMAINPARTICIPANTLISTENER_H_ */
