// $Id$
#include "dds4ccm/impl/ndds/Utils.h"

#include "dds4ccm/impl/ndds/DataReader.h"
#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/DataReaderHandler_T.h"
#include "tao/ORB_Core.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::DataReaderListener_T (
      typename CCM_TYPE::context_type::_ptr_type context,
      ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> &enabled)
      : context_ (CCM_TYPE::context_type::_duplicate (context)),
        enabled_ (enabled)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::DataReaderListener_T");
  printf ("CIAO::DDS4CCM::RTI::DataReaderListener_T::DataReaderListener_T");
  this->info_out_portstatus_ = this->context_->get_connection_info_out_status ();
  this->info_out_connector_status_ = this->context_->get_connection_error_listener ();
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::~DataReaderListener_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::DataReaderListener_T::~DataReaderListener_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_data_available(::DDS::DataReader *rdr)
{
  if (!this->enabled_.value ())
    return;

  ::CIAO::DDS4CCM::RTI::RTI_DataReader_i* rd =
      dynamic_cast < ::CIAO::DDS4CCM::RTI::RTI_DataReader_i*>(rdr);
  typename DDS_TYPE::data_reader * reader =
      dynamic_cast< typename DDS_TYPE::data_reader * > ((rd->get_datareader ()));

  if (!reader)
    {
      /* In this specific case, this will never fail */
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("DataReaderListener_T::narrow failed.\n")));
      return;
    }
  ::CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>* rh =
      new  ::CIAO::DDS4CCM::RTI::DataReaderHandler_T<DDS_TYPE, CCM_TYPE>(this->context_, reader);
  this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_requested_deadline_missed (
                                              ::DDS::DataReader_ptr the_reader,
                                               const ::DDS::RequestedDeadlineMissedStatus & status)
{
  if (!CORBA::is_nil (this->info_out_portstatus_))
    {
      this->info_out_portstatus_->on_requested_deadline_missed (the_reader, status);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_sample_lost (
                                  ::DDS::DataReader_ptr the_reader,
                                  const ::DDS::SampleLostStatus & status)
{
  if (!CORBA::is_nil (this->info_out_portstatus_))
    {
      this->info_out_portstatus_->on_sample_lost (the_reader, status);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::on_requested_incompatible_qos (
                                ::DDS::DataReader_ptr the_reader,
                                const ::DDS::RequestedIncompatibleQosStatus & status)
{
  if (!CORBA::is_nil (this->info_out_connector_status_))
    {
      this->info_out_connector_status_->on_requested_incompatible_qos (the_reader, status);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
bool
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::enabled (void) const
{
  return this->enabled_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::DataReaderListener_T<DDS_TYPE, CCM_TYPE>::enabled (bool enabled)
{
  this->enabled_ = enabled;
}

