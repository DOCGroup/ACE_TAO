// $Id$

#include "PublisherListener.h"
#include "Publisher.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_PublisherListener_i::RTI_PublisherListener_i (DDSPublisherListener *p)
        : pub_list_ (p)
      {
      }

      // Implementation skeleton destructor
      RTI_PublisherListener_i::~RTI_PublisherListener_i (void)
      {
      }

      void
      RTI_PublisherListener_i::on_offered_deadline_missed (
        ::DDS::DataWriter_ptr writer,
        const ::DDS::OfferedDeadlineMissedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_PublisherListener_i::on_offered_incompatible_qos (
        ::DDS::DataWriter_ptr writer,
        const ::DDS::OfferedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_PublisherListener_i::on_liveliness_lost (
        ::DDS::DataWriter_ptr writer,
        const ::DDS::LivelinessLostStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_PublisherListener_i::on_publication_matched (
        ::DDS::DataWriter_ptr writer,
        const ::DDS::PublicationMatchedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSPublisherListener *
      RTI_PublisherListener_i::get_publisher_listener (void)
      {
        return this->pub_list_;
      }
    }
  }
}

