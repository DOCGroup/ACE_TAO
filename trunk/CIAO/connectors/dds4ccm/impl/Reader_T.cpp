// $Id$

#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/ndds/DataReader_T.h"

#include "dds4ccm/impl/ndds/convertors/SampleInfo.h"
#include "dds4ccm/impl/ndds/convertors/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::Reader_T (void)
  : reader_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::Reader_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::~Reader_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::~Reader_T");
}

/// For the requirement : 'samples ordered by instances' the following settings
/// are necessary: ordered_access -> true and
/// DDS_INSTANCE_PRESENTATION_QOS (default)
template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
CORBA::ULong
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::get_nr_valid_samples (
  const typename DDS_TYPE::sampleinfo_seq_type& sample_info,
  bool determine_last)
{
  CORBA::ULong nr_of_samples = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if (determine_last)
        {
          if (sample_info[i].sample_rank == 0 && sample_info[i].valid_data)
            {
              ++nr_of_samples;
            }
        }
      else if (sample_info[i].valid_data)
        {
          ++nr_of_samples;
        }
    }
  return nr_of_samples;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::read_last (
  typename CCM_TYPE::seq_type& instances,
  ::CCM_DDS::ReadInfoSeq& infos)
{
  // This function has to return the last sample of all instances
  typename DDS_TYPE::sampleinfo_seq_type sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->impl ()->read_wo_instance (data,
                                   sample_info,
                                   this->condition_manager_->get_querycondition_reader ());

  CORBA::ULong const nr_of_last_samples =
    this->get_nr_valid_samples (sample_info, true);

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                     ACE_TEXT ("Reader_T::read_last - ")
                     ACE_TEXT ("total number of samples <%u> - ")
                     ACE_TEXT ("last number of samples <%u>\n"),
                     data.length(),
                     nr_of_last_samples));
  CORBA::ULong ix = 0;

  instances.length (nr_of_last_samples);
  infos.length (nr_of_last_samples);

  // We need only the last sample of each instance
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
        {
          (infos)[ix] <<= sample_info[i];
          (instances)[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Reader_T::read_last - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::read_all (
          typename CCM_TYPE::seq_type& instances,
          ::CCM_DDS::ReadInfoSeq& infos)
{
  // This function has to return all samples of all instances
  typename DDS_TYPE::sampleinfo_seq_type sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->impl ()->read_wo_instance (data,
                                   sample_info,
                                   this->condition_manager_->get_querycondition_reader ());

  CORBA::ULong const nr_of_valid_samples = this->get_nr_valid_samples (sample_info);
  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("Reader_T::read_all - ")
                ACE_TEXT ("total number of samples <%u> - ")
                ACE_TEXT ("valid number of samples <%u>\n"),
                data.length (),
                nr_of_valid_samples));

  instances.length (nr_of_valid_samples);
  infos.length (nr_of_valid_samples);

  CORBA::ULong ix = 0;

  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
        if(sample_info[i].valid_data)
          {
            (infos)[ix] <<= sample_info[i];
            (instances)[ix] = data[i];
            ++ix;
          }
    }

  // Return the loan
  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Reader_T::read_all - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
DDS_InstanceHandle_t
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::check_handle (
  const typename DDS_TYPE::value_type& an_instance,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
  hnd <<= instance_handle;

  ::DDS_InstanceHandle_t const lookup_hnd =
      this->impl ()->lookup_instance (an_instance);
  if (!DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL) &&
      !DDS_InstanceHandle_equals (&hnd, &lookup_hnd))
    {
      throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  if (DDS_InstanceHandle_equals (&lookup_hnd, &::DDS_HANDLE_NIL))
    {
      throw ::CCM_DDS::NonExistent ();
    }
  return lookup_hnd;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::read_one_last (
  typename DDS_TYPE::value_type& an_instance,
  ::CCM_DDS::ReadInfo_out info,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t const lookup_hnd =
    this->check_handle (an_instance, instance_handle);

  typename DDS_TYPE::sampleinfo_seq_type sample_info;
  typename DDS_TYPE::dds_seq_type data;

  // For now, only read with instance
  this->impl ()->read_w_instance (data, lookup_hnd, sample_info);

  ::DDS_Long sample = data.length();
  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_INFO, DDS4CCM_INFO
                ACE_TEXT ("Reader_T::read_one_last - ")
                ACE_TEXT ("total number of samples <%u>\n"),
                sample));
  while (sample >= 0 && !sample_info[sample-1].valid_data)
    {
      --sample;
    }
  if (sample >= 0)
    {
      if(sample_info[sample-1].valid_data)
        {
          an_instance = data[sample-1];
          info <<= sample_info[sample-1];
        }
    }
  // Return the loan
  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
  if (retval != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Reader_T::read_one_last - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::read_one_all (
  const typename DDS_TYPE::value_type& an_instance,
  typename CCM_TYPE::seq_type& instances,
  ::CCM_DDS::ReadInfoSeq& infos,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  ::DDS_InstanceHandle_t const lookup_hnd =
    this->check_handle (an_instance, instance_handle);

  typename DDS_TYPE::sampleinfo_seq_type sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->impl ()->read_w_instance (data, lookup_hnd, sample_info);

  // Count the number of valid samples
  CORBA::ULong const nr_of_valid_samples =
    this->get_nr_valid_samples (sample_info);
  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("Reader_T::read_one_all - ")
                ACE_TEXT ("total number of samples <%u> - ")
                ACE_TEXT ("valid number of samples <%u>\n"),
                data.length (),
                nr_of_valid_samples));

  instances.length (nr_of_valid_samples);
  infos.length (nr_of_valid_samples);

  // Copy the valid samples
  CORBA::ULong ix = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if (sample_info[i].valid_data)
        {
          (infos)[ix] <<= sample_info[i];
          (instances)[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  DDS_ReturnCode_t const retval =
    this->impl ()->return_loan (data, sample_info);
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "Reader_T::read_one_all - "
                    "Error returning loan to DDS - <%C>\n",
                    translate_retcode (retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
::CCM_DDS::QueryFilter *
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::query (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::query");
  return this->condition_manager_->query ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::query (
  const ::CCM_DDS::QueryFilter & query)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::query");
  return this->condition_manager_->query (query);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::set_impl (
  DataReader_type * dr,
  ConditionManager_type * condition_manager)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::set_impl");

  this->reader_ = dr;
  this->condition_manager_ = condition_manager;
  this->condition_manager_->set_impl (dr);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED, DDS4CCM_Vendor VENDOR_TYPE>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE> *
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED, VENDOR_TYPE>::impl (void)
{
  if (this->reader_)
    {
      return this->reader_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
}
