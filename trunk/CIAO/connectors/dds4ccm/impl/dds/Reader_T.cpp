// $Id$

#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/DataReader_T.h"

#include "dds4ccm/impl/dds/ndds/SampleInfo.h"
#include "dds4ccm/impl/dds/ndds/StringSeq.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::Reader_T (void)
  : reader_ (0)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::Reader_T");
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::~Reader_T (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::~Reader_T");
}

// For the requirement : 'samples ordered by instances' the following settings 
// are necessary: ordered_access -> true and
// DDS_INSTANCE_PRESENTATION_QOS (default)
template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
CORBA::ULong
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::get_nr_valid_samples (
  const DDS_SampleInfoSeq& sample_info,
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

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::read_last (
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  // This function has to return the last sample of all instances
  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->impl ()->read_wo_instance (data, sample_info);

  typename CCM_TYPE::seq_type * inst_seq = 0;
  ACE_NEW_THROW_EX (inst_seq,
                    typename CCM_TYPE::seq_type,
                    CORBA::NO_MEMORY ());
  ::CCM_DDS::ReadInfoSeq * infoseq = 0;
  ACE_NEW_THROW_EX (infoseq,
                    ::CCM_DDS::ReadInfoSeq,
                    CORBA::NO_MEMORY ());

  CORBA::ULong const nr_of_last_samples =
    this->get_nr_valid_samples (sample_info, true);

  DDS4CCM_DEBUG (6, (LM_DEBUG,
                     ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_last - ")
                     ACE_TEXT ("total number of samples <%u> - ")
                     ACE_TEXT ("last number of samples <%u>\n"),
                     data.length(),
                     nr_of_last_samples));
  CORBA::ULong ix = 0;

  infoseq->length (nr_of_last_samples);
  inst_seq->length (nr_of_last_samples);

  // We need only the last sample of each instance
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
        {
          (*infoseq)[ix] <<= sample_info[i];
          (*inst_seq)[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DDS_CCM::Reader_T::read_last - "
        "Error returning loan to DDS - <%C>\n",
        translate_retcode (retval)));
    }
  infos = infoseq;
  instances = inst_seq;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  // This function has to return all samples of all instances
  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->impl ()->read_wo_instance (data, sample_info);

  CORBA::ULong const nr_of_valid_samples = this->get_nr_valid_samples (sample_info);
  DDS4CCM_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_all - ")
                            ACE_TEXT ("total number of samples <%u> - ")
                            ACE_TEXT ("valid number of samples <%u>\n"),
                            data.length (),
                            nr_of_valid_samples));

  typename CCM_TYPE::seq_type * inst_seq = 0;
  ACE_NEW_THROW_EX (inst_seq,
                    typename CCM_TYPE::seq_type,
                    CORBA::NO_MEMORY ());
  ::CCM_DDS::ReadInfoSeq * infoseq = 0;
  ACE_NEW_THROW_EX (infoseq,
                    ::CCM_DDS::ReadInfoSeq,
                    CORBA::NO_MEMORY ());

  infoseq->length (nr_of_valid_samples);
  inst_seq->length (nr_of_valid_samples);

  CORBA::ULong ix = 0;

  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
        if(sample_info[i].valid_data)
          {
            (*infoseq)[ix] <<= sample_info[i];
            (*inst_seq)[ix] = data[i];
            ++ix;
          }
    }

  // Return the loan
  DDS_ReturnCode_t const retval = this->impl ()->return_loan (data, sample_info);
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DDS_CCM::Reader_T::read_all - "
        "Error returning loan to DDS - <%C>\n",
        translate_retcode (retval)));
    }

  infos = infoseq;
  instances = inst_seq;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_InstanceHandle_t
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::check_handle (
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
      throw ::CCM_DDS::InternalError (0, 0);
    }
  if (DDS_InstanceHandle_equals (&lookup_hnd, &::DDS_HANDLE_NIL))
    {
      throw ::CCM_DDS::NonExistent ();
    }
  return lookup_hnd;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::read_one_last (
  typename DDS_TYPE::value_type& an_instance,
  ::CCM_DDS::ReadInfo_out info,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t const lookup_hnd =
    this->check_handle (an_instance, instance_handle);

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  // For now, only read with instance...
  this->impl ()->read_w_instance (data, lookup_hnd, sample_info);

  ::DDS_Long sample = data.length();
  DDS4CCM_DEBUG (6, (LM_INFO,
              ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_one_last - ")
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
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DDS_CCM::Reader_T::read_one_last - "
        "Error returning loan to DDS - <%C>\n",
        translate_retcode (retval)));
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::read_one_all (
  const typename DDS_TYPE::value_type& an_instance,
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t const lookup_hnd =
    this->check_handle (an_instance, instance_handle);

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->impl ()->read_w_instance (data, lookup_hnd, sample_info);

  // Count the number of valid samples
  CORBA::ULong const nr_of_valid_samples =
    this->get_nr_valid_samples (sample_info);
  DDS4CCM_DEBUG (6, (LM_DEBUG,
                     ACE_TEXT ("CIAO::DDS4CCM::DDS_CCM::Reader_T::read_all - ")
                     ACE_TEXT ("total number of samples <%u> - ")
                     ACE_TEXT ("valid number of samples <%u>\n"),
                     data.length (),
                     nr_of_valid_samples));

  typename CCM_TYPE::seq_type * inst_seq = 0;
  ACE_NEW_THROW_EX (inst_seq,
                    typename CCM_TYPE::seq_type (nr_of_valid_samples),
                    CORBA::NO_MEMORY ());
  ::CCM_DDS::ReadInfoSeq * infoseq = 0;
  ACE_NEW_THROW_EX (infoseq,
                    ::CCM_DDS::ReadInfoSeq (nr_of_valid_samples),
                    CORBA::NO_MEMORY ());

  infoseq->length (nr_of_valid_samples);
  inst_seq->length (nr_of_valid_samples);

  // Copy the valid samples
  CORBA::ULong ix = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if(sample_info[i].valid_data)
        {
          (*infoseq)[ix] <<= sample_info[i];
          (*inst_seq)[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  DDS_ReturnCode_t const retval =
    this->impl ()->return_loan (data, sample_info);
  if (retval != DDS_RETCODE_OK)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
        "CIAO::DDS4CCM::DDS_CCM::Reader_T::read_one_all - "
        "Error returning loan to DDS - <%C>\n",
        translate_retcode (retval)));
    }

  infos = infoseq;
  instances = inst_seq;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter *
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::filter (void)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::filter");
  return this->impl ()->filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::filter");
  return this->impl ()->filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::set_impl (
  DataReader_T<DDS_TYPE, CCM_TYPE> * dr)
{
  DDS4CCM_TRACE ("CIAO::DDS4CCM::DDS_CCM::Reader_T::set_impl");

  this->reader_ = dr;
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
CIAO::DDS4CCM::DataReader_T<DDS_TYPE, CCM_TYPE> *
CIAO::DDS4CCM::DDS_CCM::Reader_T<DDS_TYPE, CCM_TYPE, FIXED>::impl (void)
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
