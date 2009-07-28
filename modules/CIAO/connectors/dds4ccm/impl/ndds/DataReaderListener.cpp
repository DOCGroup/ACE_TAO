// $Id$

#include "DataReaderListener.h"
#include "DataReader.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataReaderListener_i::RTI_DataReaderListener_i (DDSDataReaderListener *s)
        : drl_ (s)
      {
      }

      // Implementation skeleton destructor
      RTI_DataReaderListener_i::~RTI_DataReaderListener_i (void)
      {
      }

      void
      RTI_DataReaderListener_i::on_requested_deadline_missed (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::RequestedDeadlineMissedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_requested_incompatible_qos (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::RequestedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_sample_rejected (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SampleRejectedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_liveliness_changed (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::LivelinessChangedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_data_available(DDSDataReader *reader)
      {
printf ("on data available\n");      
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (reader);
        this->on_data_available (dds_reader.in ());
      }

      void
      RTI_DataReaderListener_i::on_data_available (
        ::DDS::DataReader_ptr the_reader)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_subscription_matched (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SubscriptionMatchedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_sample_lost (
        ::DDS::DataReader_ptr the_reader,
        const ::DDS::SampleLostStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      DDSDataReaderListener *
      RTI_DataReaderListener_i::get_datareaderlistener (void)
      {
        return this;//->drl_;
      }
    }
  }
}

