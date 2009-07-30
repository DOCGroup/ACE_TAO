// $Id$

#include "DataReaderListener.h"
#include "DataReader.h"
#include "SampleLostStatus.h"
#include "SubscriptionMatchedStatus.h"
#include "SampleRejectedStatus.h"

#include "dds4ccm/idl/dds4ccm_BaseC.h"

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  namespace DDS4CCM
  {
    namespace RTI
    {
      // Implementation skeleton constructor
      RTI_DataReaderListener_i::RTI_DataReaderListener_i (::DDS::DataReaderListener_ptr p)
        : drl_ (::DDS::DataReaderListener::_duplicate (p))
      {
      }

      // Implementation skeleton destructor
      RTI_DataReaderListener_i::~RTI_DataReaderListener_i (void)
      {
      }

      void
      RTI_DataReaderListener_i::on_requested_deadline_missed (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedDeadlineMissedStatus & status)
      {
// the cast doesn't work :-(
//        RTI_DataReader_i *dr = dynamic_cast< RTI_DataReader_i *> (the_reader);
        //this->drl_->on_requested_deadline_missed (dr, status);
      }

      void
      RTI_DataReaderListener_i::on_requested_incompatible_qos (
        ::DDSDataReader* the_reader,
        const ::DDS_RequestedIncompatibleQosStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_sample_rejected (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleRejectedStatus & status)
      {
        ::DDS::SampleRejectedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->drl_->on_sample_rejected (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_liveliness_changed (
        ::DDSDataReader* the_reader,
        const ::DDS_LivelinessChangedStatus & status)
      {
        throw CORBA::NO_IMPLEMENT ();
      }

      void
      RTI_DataReaderListener_i::on_data_available(::DDSDataReader *reader)
      {
        //RTI_DataReader_i *dr = dynamic_cast< RTI_DataReader_i *> (reader);
        //if (!dr) printf ("cast failed %s\n", typeid (*reader).name ());
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (reader);
        this->drl_->on_data_available (dds_reader.in ());
      }

      void
      RTI_DataReaderListener_i::on_subscription_matched (
        ::DDSDataReader* the_reader,
        const ::DDS_SubscriptionMatchedStatus & status)
      {
        ::DDS::SubscriptionMatchedStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->drl_->on_subscription_matched (dds_reader.in (), ddsstatus);
      }

      void
      RTI_DataReaderListener_i::on_sample_lost (
        ::DDSDataReader* the_reader,
        const ::DDS_SampleLostStatus & status)
      {
        ::DDS::SampleLostStatus ddsstatus;
        ddsstatus <<= status;
        ::DDS::DataReader_var dds_reader = new RTI_DataReader_i (the_reader);
        this->drl_->on_sample_lost (dds_reader.in (), ddsstatus);
      }

      ::DDS::DataReaderListener_ptr
      RTI_DataReaderListener_i::get_datareaderlistener (void)
      {
        return ::DDS::DataReaderListener::_duplicate (this->drl_.in ());
      }
    }
  }
}

