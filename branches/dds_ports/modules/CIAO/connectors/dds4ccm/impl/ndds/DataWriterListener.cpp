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
      RTI_DataWriterListener_i::RTI_DataWriterListener_i (::DDS::DataWriterListener_ptr s)
        : impl_ (::DDS::DataWriterListener::_duplicate (s))
      {
      }

      // Implementation skeleton destructor
      RTI_DataWriterListener_i::~RTI_DataWriterListener_i (void)
      {
      }

      void
      RTI_DataWriterListener_i::on_offered_deadline_missed (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedDeadlineMissedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataWriterListener_i::on_offered_incompatible_qos (
        ::DDSDataWriter *the_writer,
        const ::DDS_OfferedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataWriterListener_i::on_liveliness_lost (
        ::DDSDataWriter *the_writer,
        const ::DDS_LivelinessLostStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataWriterListener_i::on_publication_matched (
        ::DDSDataWriter *the_writer,
        const ::DDS_PublicationMatchedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      ::DDS::DataWriterListener_ptr
      RTI_DataWriterListener_i::get_datawriterlistener (void)
      {
        return ::DDS::DataWriterListener::_duplicate (this->impl_.in ());
      }
    }
  }
}

