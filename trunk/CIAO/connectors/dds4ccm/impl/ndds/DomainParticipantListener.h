/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_DOMAINPARTICIPANTLISTENER_H
#define CIAO_RTI_DOMAINPARTICIPANTLISTENER_H

#include "tao/LocalObject.h"

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "dds4ccm_ndds_impl_export.h"

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
# include "ndds/ndds_cpp.h"
#endif

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::DomainParticipantListener DDSDomainParticipantListener;
#endif

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      class DDS4CCM_NDDS_IMPL_Export RTI_DomainParticipantListener_i :
        public virtual ::DDSDomainParticipantListener
      {
      public:
        // Constructor
        RTI_DomainParticipantListener_i (::DDS::DomainParticipantListener_ptr p);

        // Destructor
        virtual ~RTI_DomainParticipantListener_i (void);

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
}

#endif
