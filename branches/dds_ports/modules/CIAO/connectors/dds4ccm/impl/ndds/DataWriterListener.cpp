// $Id$

#include "DataWriterListener.h"
#include "DataWriter.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataWriterListener_i::RTI_DataWriterListener_i (DDSDataWriterListener *s)
        : drl_ (s)
      {
      }

      // Implementation skeleton destructor
      RTI_DataWriterListener_i::~RTI_DataWriterListener_i (void)
      {
      }

      void
      RTI_DataWriterListener_i::on_offered_deadline_missed (
        ::DDS::DataWriter_ptr the_writer,
        const ::DDS::OfferedDeadlineMissedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataWriterListener_i::on_offered_incompatible_qos (
        ::DDS::DataWriter_ptr the_writer,
        const ::DDS::OfferedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataWriterListener_i::on_liveliness_lost (
        ::DDS::DataWriter_ptr the_writer,
        const ::DDS::LivelinessLostStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataWriterListener_i::on_publication_matched (
        ::DDS::DataWriter_ptr the_writer,
        const ::DDS::PublicationMatchedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSDataWriterListener *
      RTI_DataWriterListener_i::get_datawriterlistener (void)
      {
        return this;//->drl_;
      }
    }
  }
}

