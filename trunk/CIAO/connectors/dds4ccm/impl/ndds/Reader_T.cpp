// $Id$

#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/SampleInfo.h"
#include "dds4ccm/impl/ndds/Subscriber.h"
#include "dds4ccm/impl/ndds/QueryCondition.h"
#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::Reader_T (void)
  : topic_ (0),
    library_name_ (""),
    profile_name_ (""),
    impl_ (0)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::Reader_T");
  #if defined DDS4CCM_USES_QUERY_CONDITION
    this->qc_ = DDS::ReadCondition::_nil ();
  #else
    this->cft_ = DDS::ContentFilteredTopic::_nil ();
  #endif
}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE>
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::~Reader_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::~Reader_T");
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename DDS_TYPE::data_reader *
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::impl (void)
{
  if (this->impl_)
    {
      return this->impl_;
    }
  else
    {
      throw ::CORBA::BAD_INV_ORDER ();
    }
}

//for the requirement : 'samples ordered by instances' the following settings are necessary:
// ordered_access -> true   and     DDS_INSTANCE_PRESENTATION_QOS (default) .
template <typename DDS_TYPE, typename CCM_TYPE>
CORBA::ULong
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::get_nr_valid_samples (
  const DDS_SampleInfoSeq & sample_info,
  bool determine_last)
{
  CORBA::ULong nr_of_samples = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if (determine_last)
        {
          if (sample_info[i].sample_rank == 0 &&
              sample_info[i].valid_data)
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_without_instance (
  typename DDS_TYPE::dds_seq_type & data,
  DDS_SampleInfoSeq & sample_info)
{
  DDS_ReturnCode_t retval = this->impl ()->read (
                                data,
                                sample_info,
                                DDS_LENGTH_UNLIMITED,
                                DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE,
                                DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                DDS_ALIVE_INSTANCE_STATE);

  if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
    {
      this->impl ()->return_loan(data, sample_info);
      CIAO_ERROR (1, (LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_without_instance - ")
                            ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}


template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_last (
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  //this function has to return the last sample of all instances
  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  read_without_instance (data, sample_info);

  typename CCM_TYPE::seq_type::_var_type  inst_seq = new typename CCM_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

  CORBA::ULong const nr_of_last_samples = get_nr_valid_samples (sample_info, true);

  CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_last - ")
                          ACE_TEXT ("total number of samples <%u> - ")
                          ACE_TEXT ("last number of samples <%u>\n"),
                            data.length(),
                            nr_of_last_samples));
  CORBA::ULong ix = 0;
  infoseq->length (nr_of_last_samples);
  inst_seq->length (nr_of_last_samples);
  // we need only the last sample of each instance
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
        {
          infoseq[ix] <<= sample_info[i];
          inst_seq[ix] = data[i];
          ++ix;
        }
    }
  //return the loan
  this->impl ()->return_loan(data,sample_info);
  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  //this function has to return all samples of all instances
  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->read_without_instance (data, sample_info);

  CORBA::ULong const nr_of_valid_samples = get_nr_valid_samples (sample_info);
  CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                          ACE_TEXT ("total number of samples <%u> - ")
                          ACE_TEXT ("valid number of samples <%u>\n"),
                            data.length (),
                            nr_of_valid_samples));

  typename CCM_TYPE::seq_type::_var_type  inst_seq = new typename CCM_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

  infoseq->length (nr_of_valid_samples);
  inst_seq->length (nr_of_valid_samples);

  CORBA::ULong ix = 0;

  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
        if(sample_info[i].valid_data)
          {
            infoseq[ix] <<= sample_info[i];
            inst_seq[ix] = data[i];
            ++ix;
          }
    }
  //return the loan
  this->impl ()->return_loan(data,sample_info);

  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_InstanceHandle_t
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::check_handle (
          const typename DDS_TYPE::value_type& an_instance,
          const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
  hnd <<= instance_handle;

  DDS_InstanceHandle_t lookup_hnd =
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

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_with_instance (
  typename DDS_TYPE::dds_seq_type & data,
  const ::DDS_InstanceHandle_t & lookup_hnd,
  DDS_SampleInfoSeq & sample_info)
{
  CIAO_DEBUG (6, (LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_with_instance - ")
                        ACE_TEXT ("Reading with instance.\n")));
  DDS_ReturnCode_t const retval = this->impl ()->read_instance (
                                      data,
                                      sample_info,
                                      DDS_LENGTH_UNLIMITED,
                                      lookup_hnd,
                                      DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                                      DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                                      DDS_ALIVE_INSTANCE_STATE);
  if (retval != DDS_RETCODE_OK && retval != DDS_RETCODE_NO_DATA)
    {
      this->impl ()->return_loan(data, sample_info);
      CIAO_ERROR (1, (LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_with_instance - ")
                            ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
      throw ::CCM_DDS::InternalError (retval, 0);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_one_last (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info,
          const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t lookup_hnd = check_handle (an_instance, instance_handle);

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;
  //for now, only read with instance...
  this->read_with_instance (data, lookup_hnd, sample_info);

  ::DDS_Long sample = data.length();
  CIAO_DEBUG (6, (LM_INFO,
              ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_last - ")
              ACE_TEXT ("total number of samples <%u>\n"),
              sample));
  while (sample >= 0 && !sample_info[sample-1].valid_data)
    --sample;
  if (sample >= 0)
    {
      if(sample_info[sample-1].valid_data)
        {
          an_instance = data[sample-1];
          info <<= sample_info[sample-1];
        }
    }
  // Return the loan
  this->impl ()->return_loan(data, sample_info);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_one_all (
  const typename DDS_TYPE::value_type& an_instance,
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t const lookup_hnd = check_handle (an_instance, instance_handle);

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  this->read_with_instance (data, lookup_hnd, sample_info);

  // Count the number of valid samples
  CORBA::ULong nr_of_valid_samples = this->get_nr_valid_samples (sample_info);
  CIAO_DEBUG (6, (LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                          ACE_TEXT ("total number of samples <%u> - ")
                          ACE_TEXT ("valid number of samples <%u>\n"),
                            data.length (),
                            nr_of_valid_samples));

  typename CCM_TYPE::seq_type::_var_type inst_seq = new typename CCM_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

  infoseq->length (nr_of_valid_samples);
  inst_seq->length (nr_of_valid_samples);

  // Copy the valid samples
  CORBA::ULong ix = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); ++i)
    {
      if(sample_info[i].valid_data)
        {
          infoseq[ix] <<= sample_info[i];
          inst_seq[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  this->impl ()->return_loan(data, sample_info);

  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::create_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::create_filter");
  ::DDS::Subscriber_var sub = this->reader_->get_subscriber ();
  if (CORBA::is_nil (sub))
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::create_filter - "
                    "Error: Unable to get Subscriber.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  ::DDS::DomainParticipant_var dp = sub->get_participant ();
  if (CORBA::is_nil (dp))
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::create_filter - "
                    "Error: Unable to get Participant.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
    }

  this->cft_ =
    dp->create_contentfilteredtopic (
                        "ActFunny",
                        this->topic_.in (),
                        filter.query,
                        filter.query_parameters);
  if (CORBA::is_nil (this->cft_))
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::create_filter - "
                    "Error: Unable to create ContentFilteredTopic.\n"));
      throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 1);
    }
//   this->set_expression_parameters (filter.query_parameters);
  ::DDS::DataReaderListener_var listener = this->reader_->get_listener ();
  this->reader_->set_listener (::DDS::DataReaderListener::_nil (), 0);
  ::DDS::ReturnCode_t retval = sub->delete_datareader (this->reader_);
  if (retval != ::DDS::RETCODE_OK)
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::create_filter - "
                    "Error: Unable to delete DataReader.\n"));
    }
  this->reader_ = ::DDS::CCM_DataReader::_nil ();

  ::DDS::DataReader_var reader = ::DDS::DataReader::_nil ();
  if (this->library_name_.length () > 0 &&
      this->profile_name_.length () > 0)
    {
      reader = sub->create_datareader_with_profile (
                        this->cft_,
                        this->library_name_.c_str (),
                        this->profile_name_.c_str (),
                        listener,
                        ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  else
    {
      ::DDS::DataReaderQos drqos;
      reader = sub->create_datareader (
                        this->cft_,
                        drqos,
                        listener,
                        ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  if (CORBA::is_nil(reader))
    {
      CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::create_filter - "
                    "Error: Unable to create a new DataReader.\n"));
    }
  this->reader_ = ::DDS::CCM_DataReader::_narrow (reader);
  this->set_impl (reader);
}

template <typename DDS_TYPE, typename CCM_TYPE>
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::filter (void)
{
  #if defined DDS4CCM_USES_QUERY_CONDITION
    if (CORBA::is_nil (this->qc_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::filter - "
                      "Error: No QueryCondition set yet. First set a filter.\n"));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
      }
    ::CCM_DDS::QueryFilter * filter = new ::CCM_DDS::QueryFilter();
    filter->query = this->qc_->get_query_expression ();
    this->qc_->get_query_parameters (filter->query_parameters);
    return filter;
  #else
    if (CORBA::is_nil (this->cft_))
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::filter - "
                      "Error: No ContentFilter set yet. First set a filter.\n"));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
      }
    ::CCM_DDS::QueryFilter * filter = new ::CCM_DDS::QueryFilter();
    filter->query = this->cft_->get_filter_expression ();
    ::DDS::ReturnCode_t retval = this->cft_->get_expression_parameters (
                                          filter->query_parameters);
    if (retval != DDS::RETCODE_OK)
      {
        CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::filter - "
                                  "Error getting expression_paramaters. "
                                  "Retval is %C\n",
                                  translate_retcode(retval)));
        throw CCM_DDS::InternalError (::DDS::RETCODE_ERROR, retval);
      }
    return filter;
  #endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::filter");
  #if defined DDS4CCM_USES_QUERY_CONDITION
    if (CORBA::is_nil (this->qc_))
      {
        this->qc_ = this->reader_->create_querycondition (
                        ::DDS::SampleStateMask sample_states,
                        ::DDS::ViewStateMask view_states,
                        ::DDS::InstanceStateMask instance_states,
                        const char * query_expression,
                        const ::DDS::StringSeq & query_parameters);
      }
    else
      {
        this->qc_p>set_query_parameters ();
      }
  #else
    if (CORBA::is_nil (this->cft_))
      {
        create_filter (filter);
      }
    else
      {
        this->cft_->set_expression_parameters (
          filter.query_parameters);
      }
  #endif
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::set_qos (
  ::DDS::Topic_ptr topic,
  const char * library_name,
  const char * profile_name)
{
  this->library_name_ = library_name;
  this->profile_name_ = profile_name;
  this->topic_ = topic;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::set_impl (
  ::DDS::DataReader_ptr reader)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::set_impl");

  if (::CORBA::is_nil (reader))
    {
      this->impl_ = 0;
    }
  else
    {
      this->reader_ = reader;
      RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (reader);

      if (!rdr)
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::data_reader - "
                       "Unable to cast provided DataReader to servant\n"));
          throw ::CORBA::INTERNAL ();
        }

      this->impl_ =  DDS_TYPE::data_reader::narrow (rdr->get_impl ());

      if (!this->impl ())
        {
          CIAO_ERROR (1, (LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::data_reader - "
                       "Unable to narrow the provided reader entity to the specific "
                       "type necessary to publish messages\n"));
          throw ::CORBA::INTERNAL ();
        }
    }
}

