// $Id$

#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/SampleInfo.h"
#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::Reader_T (::DDS::DataReader_ptr reader)
  : impl_ (0),
    reader_ (reader)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::Reader_T");

  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (!rdr)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::Reader_T - ")
                   ACE_TEXT ("Unable to cast provided DataReader to servant\n")));
      throw CORBA::INTERNAL ();
    }

  this->impl_ =  DDS_TYPE::data_reader::narrow (rdr->get_datareader ());

  if (!this->impl_)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::Reader_T - ")
                    ACE_TEXT ("Unable to narrow the provided reader entity to the specific ")
                    ACE_TEXT ("type necessary to publish messages\n")));
      throw CORBA::INTERNAL ();
    }

}

// Implementation skeleton destructor
template <typename DDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::~Reader_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::~Reader_T");
}


//for the requirement : 'samples ordered by instances' the following settings are necessary:
// ordered_access -> true   and     DDS_INSTANCE_PRESENTATION_QOS (default) .

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_last (
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  //this function has to return the last sample of all instances
  typename CCM_TYPE::seq_type::_var_type inst_seq = new typename CCM_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;
  DDS_ReturnCode_t retval = this->impl_->read ( data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);

  CORBA::ULong ix = 0;
  CORBA::ULong nr_of_last_samples = 0;
  switch(retval)
    {
      case DDS_RETCODE_OK:
        ix = 0;
        nr_of_last_samples = 0;
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_last - ")
                                ACE_TEXT ("number_of_samples <%d>\n"), data.length() ));
        //infoseq <<= sample_info; ??

        // count the last samples of all instances
        for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
          {
            if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
              {
                ++nr_of_last_samples;
              }
          }
        infoseq->length(nr_of_last_samples);
        inst_seq->length(nr_of_last_samples);
        // we need only the last sample of each instance
        for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
          {
            if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
              {
                sample_info[i].source_timestamp >>= infoseq[ix].source_timestamp;
                inst_seq[ix] = data[i];
                ++ix;
              }
          }
        break;
      case DDS_RETCODE_NO_DATA:
        CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_last - No data")));
        break;
      default:
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_last - ")
                                ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
  //return the loan
  this->impl_->return_loan(data,sample_info);
  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_all (
          typename CCM_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  //this function has to return all samples of all instances
  typename CCM_TYPE::seq_type::_var_type  inst_seq = new typename CCM_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

  DDS_SampleInfoSeq sample_info;
  typename DDS_TYPE::dds_seq_type data;

  DDS_ReturnCode_t retval = this->impl_->read ( data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);
    CORBA::ULong nr_of_samples = 0;
    CORBA::ULong ix = 0;
    switch(retval)
      {
        case DDS_RETCODE_OK:
          CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                                 ACE_TEXT ("number_of_samples <%d>\n"), data.length ()));
          //count the number of valid data
          for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
            {
              if(sample_info[i].valid_data)
                {
                  ++nr_of_samples;
                }
            }
          //infoseq->length(sample_info.length ());
          infoseq->length(nr_of_samples);
          inst_seq->length(nr_of_samples);
          for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
            {
                if(sample_info[i].valid_data)
                  {
                    sample_info[i].source_timestamp >>= infoseq[ix].source_timestamp;
                    inst_seq[ix] = data[i];
                    ++ix;
                  }
            }
          break;
        case DDS_RETCODE_NO_DATA:
          CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - No data")));
          break;
        default:
          CIAO_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                                ACE_TEXT ("retval is %C\n"), translate_retcode(retval)));
          throw ::CCM_DDS::InternalError (retval, 0);
          break;
      }
  //return the loan
  this->impl_->return_loan(data,sample_info);
  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_one_last (
          typename DDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info,
          const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
  hnd <<= instance_handle;
  if (DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL))
    {
      hnd = this->impl_->lookup_instance (an_instance);
    }
  DDS_SampleInfoSeq sample_info;
  DDS_ReturnCode_t retval = DDS_RETCODE_NO_DATA;
  typename DDS_TYPE::dds_seq_type data;

  // if initial instance has a registered key, pass back instance with this key,
  // else return last instance regardless of key
  if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
    {
      retval = this->impl_->read_instance(data,
                            sample_info,
                            1,
                            hnd,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);
    }
  else
    {
      CIAO_DEBUG ((LM_INFO,
                  ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_last - ")
                  ACE_TEXT ("No instance found.\n")));
      retval = this->impl_->read(data,
                            sample_info,
                            1,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);

    }
  switch(retval)
    {
      case DDS_RETCODE_OK:
        {
          ::DDS_Long const number_of_samples = data.length();
          CIAO_DEBUG ((LM_INFO,
                      ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_last - ")
                      ACE_TEXT ("number_of_samples = %d\n"),
                      number_of_samples));

          // Get last instance
          if(sample_info[number_of_samples-1].valid_data)
            {
              an_instance = data[number_of_samples-1];
              info <<= sample_info[number_of_samples-1];
            }
          this->impl_->return_loan(data,sample_info);
          }
        break;
      case DDS_RETCODE_NO_DATA:
        CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_last - ")
                              ACE_TEXT ("No data\n")));
        this->impl_->return_loan(data,sample_info);
        //only if a key and no instance for that key throw NonExistent exception
        if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
          {
            throw ::CCM_DDS::NonExistent(0);
          }
        break;
      default:
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_last - ")
                               ACE_TEXT ("retval <%C>\n"), translate_retcode(retval)));
        this->impl_->return_loan(data,sample_info);
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
 }

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::read_one_all (
  const typename DDS_TYPE::value_type& an_instance,
  typename CCM_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos,
  const ::DDS::InstanceHandle_t & instance_handle)
{
  DDS_InstanceHandle_t hnd = ::DDS_HANDLE_NIL;
  hnd <<= instance_handle;
  if (DDS_InstanceHandle_equals (&hnd, &::DDS_HANDLE_NIL))
    {
      hnd = this->impl_->lookup_instance (an_instance);
    }

  DDS_SampleInfoSeq sample_info;
  DDS_ReturnCode_t retval = DDS_RETCODE_NO_DATA;
  typename DDS_TYPE::dds_seq_type data;

  if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
    {
      CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_all - ")
                            ACE_TEXT ("Reading with instance.\n")));
      retval = this->impl_->read_instance (
                  data,
                  sample_info,
                  DDS_LENGTH_UNLIMITED,
                  hnd,
                  DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                  DDS_ALIVE_INSTANCE_STATE);
    }
  else
    {
      CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_all - ")
                            ACE_TEXT ("Reading without instance.\n")));
      retval = this->impl_->read (
                  data,
                  sample_info,
                  DDS_LENGTH_UNLIMITED,
                  DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                  DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                  DDS_ALIVE_INSTANCE_STATE);
    }

  if (retval != DDS_RETCODE_OK)
    {
      throw ::CCM_DDS::InternalError (retval, 0);
    }

  // Count the number of valid samples
  CORBA::ULong nr_of_samples = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
    {
      if(sample_info[i].valid_data)
        {
          ++nr_of_samples;
        }
    }

  typename CCM_TYPE::seq_type::_var_type inst_seq = new typename CCM_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;
  infoseq->length(nr_of_samples);
  inst_seq->length(nr_of_samples);

  // Copy the valid samples
  CORBA::ULong ix = 0;
  for (::DDS_Long i = 0 ; i < sample_info.length(); i++)
    {
      if(sample_info[i].valid_data)
        {
          infoseq[ix] <<= sample_info[i];
          inst_seq[ix] = data[i];
          ++ix;
        }
    }

  // Return the loan
  this->impl_->return_loan(data, sample_info);

  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename DDS_TYPE, typename CCM_TYPE >
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::filter (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE>::filter (const ::CCM_DDS::QueryFilter & filter)
{
  ACE_UNUSED_ARG (filter);
}

