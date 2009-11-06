// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/SampleInfo.h"



#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename NDDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::Reader_T (::DDS::DataReader_ptr reader)
: reader_ (reader),
  condition_(0)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::Reader_T");
  ACE_UNUSED_ARG (reader);
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename CCM_TYPE >
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::~Reader_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::~Reader_T");
}


//for the requirement : 'samples ordered by instances' the following settings are necessary:
// ordered_access -> true   and     DDS_INSTANCE_PRESENTATION_QOS (default) .

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::read_all (
  typename NDDS_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  //this function has to return the last sample of all instances
  typename NDDS_TYPE::seq_type::_var_type  inst_seq = new typename NDDS_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;

 
  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (rdr == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                   ACE_TEXT ("Unable to cast provided DataReader to servant\n")));
      throw CORBA::INTERNAL ();
    }

  typename NDDS_TYPE::data_reader*
      impl =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());

  if (!impl)
  {
    CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                   ACE_TEXT ("Unable to narrow the provided reader entity to the specific ")
                   ACE_TEXT ("type necessary to publish messages\n")));
    throw CORBA::INTERNAL ();
  }

  DDS_SampleInfoSeq sample_info;
  DDS_ReturnCode_t retval = DDS_RETCODE_NO_DATA;
  typename NDDS_TYPE::dds_seq_type data;
    
  // NDDS_TYPE::dds_seq_type = dds sequence
  // NDDS_TYPE::seq_type = ccm sequence
  if (this->condition_)
  {
      // retval =  impl->read_w_condition (data, sample_info, 1, this->condition_);
  }
  else
  {
    retval = impl->read ( data,
                          sample_info,
                          DDS_LENGTH_UNLIMITED,
                          DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                          DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                          DDS_ALIVE_INSTANCE_STATE);
  }

  CORBA::ULong ix = 0;
  CORBA::ULong nr_of_last_samples = 0;
  switch(retval)
  {
    
    case DDS_RETCODE_OK:
      ix = 0;
      nr_of_last_samples = 0;
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                              ACE_TEXT ("number_of_samples <%d>\n"), data.length() ));
      //infoseq <<= sample_info; ??

       // count the last samples of all instances
      for (CORBA::ULong i = 0 ; i < (CORBA::ULong)sample_info.length(); i++)
      { 
        if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
        { 
            nr_of_last_samples++;
        }
      }
      infoseq->length(nr_of_last_samples);
      inst_seq->length(nr_of_last_samples);
      // we need only the last sample of each instance
   
      for (CORBA::ULong i = 0 ; i < (CORBA::ULong)sample_info.length(); i++)
      { 
        if((sample_info[i].sample_rank == 0) && (sample_info[i].valid_data))
        { 
          sample_info[i].reception_timestamp >>= infoseq[ix].timestamp;

         inst_seq[ix] = data[i];
          ix++;
        }
      } 
    
      break;
    case DDS_RETCODE_NO_DATA:
      CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - No data")));
      break;
    default:
      CIAO_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all - ")
                              ACE_TEXT ("retval is %d\n"), retval));
      throw ::CCM_DDS::InternalError (retval, 0);
      break;
  }
  //return the loan 
  impl->return_loan(data,sample_info);
  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::read_all_history (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  //this function has to return all samples of all instances
  typename NDDS_TYPE::seq_type::_var_type  inst_seq = new typename NDDS_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;


  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (rdr == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::read_all_history - "
                    "Unable to cast provided DataReader to servant\n"));
      throw CORBA::INTERNAL ();
    }

  typename NDDS_TYPE::data_reader*
      impl =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());
   

  if (!impl)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::read_all_history - "
                   "Unable to narrow the provided reader entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }

  DDS_SampleInfoSeq sample_info;
  DDS_ReturnCode_t retval = DDS_RETCODE_NO_DATA;
  typename NDDS_TYPE::dds_seq_type data;

  // NDDS_TYPE::dds_seq_type = dds sequence
  // NDDS_TYPE::seq_type = ccm sequence
  if (this->condition_)
    {
      // retval =  impl->read_w_condition (data, sample_info, 1, this->condition_);
    }
  else
    {
      retval = impl->read ( data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);
     }
    CORBA::ULong nr_of_samples = 0;
    CORBA::ULong ix = 0;
    switch(retval)
    {
      case DDS_RETCODE_OK:
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all_history - ")
                               ACE_TEXT ("number_of_samples <%d>\n"), data.length() ));

        //count the number of valid data
        for (CORBA::ULong i = 0 ; i < (CORBA::ULong)sample_info.length(); i++)
        { 
            if(sample_info[i].valid_data)
                nr_of_samples++;
        } 
        //infoseq->length(sample_info.length ());
        infoseq->length(nr_of_samples);
        inst_seq->length(nr_of_samples);
        for (CORBA::ULong i = 0 ; i <(CORBA::ULong)sample_info.length(); i++)
        { 
            if(sample_info[i].valid_data)
            {
                sample_info[i].reception_timestamp >>= infoseq[ix].timestamp;
                inst_seq[ix] = data[i];
                ix++;
            }
        } 
        break;
      case DDS_RETCODE_NO_DATA:
        CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all_history - No data")));
        break;
      default:
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_all_history - ")
                               ACE_TEXT ("retval is %d\n"), retval));
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
  //return the loan 
  impl->return_loan(data,sample_info);
  infos = infoseq._retn ();
  instances = inst_seq._retn();
}

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::read_one (
          typename NDDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (rdr == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::read_one - "
                    "Unable to cast provided DataReader to servant\n"));
      throw CORBA::INTERNAL ();
    }

  typename NDDS_TYPE::data_reader*
      impl =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());


  if (!impl)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::read_one - "
                   "Unable to narrow the provided reader entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
    

  DDS_InstanceHandle_t hnd = impl->lookup_instance (an_instance);
  DDS_SampleInfoSeq sample_info;
  DDS_ReturnCode_t retval = DDS_RETCODE_NO_DATA;

  typename NDDS_TYPE::dds_seq_type data;

  // NDDS_TYPE::dds_seq_type = dds sequence
  // NDDS_TYPE::seq_type = ccm sequence
  if (this->condition_)
    {
        // retval =  impl->read_w_condition (data, sample_info, 1, this->condition_);
    }
  else
    { 
       // if initial instance has a registered key, pass back instance with this key,
       // else return last instance regardless of key
      if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
        {
          retval = impl->read_instance(data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            hnd,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);
        }
      else
        {
          CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_history - ")
                                ACE_TEXT ("No instance found.\n")));
          retval = impl->read(data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);

      }
     }
    int number_of_samples = 0;
    switch(retval)
    {
      case DDS_RETCODE_OK:
        CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one - ")
                              ACE_TEXT ("number_of_samples =%d\n"), number_of_samples));
        number_of_samples = data.length();
        //get last instance
        if(sample_info[number_of_samples-1].valid_data)
        {  an_instance = data[number_of_samples-1];
        //info <<= sample_info; 
           sample_info[number_of_samples-1].reception_timestamp >>= info.timestamp;
        }
        //else   ?? What to do ?

        //what about the following attributes?
        //info.access_status     DDS_SampleStateKind  sample_state or   DDS_ViewStateKind view_state; ?
        //info.instance_status   DDS_InstanceStateKind instance_state;
        //info.instance_rank     DDS_Long sample_rank;   is always 0 with last sample 
         //return the loan 
         impl->return_loan(data,sample_info);
         break;
      case DDS_RETCODE_NO_DATA:
        CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one - ")
                              ACE_TEXT ("No data\n")));
        impl->return_loan(data,sample_info);
        //only if a key and no instance for that key throw NonExistent exception  
        if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
          throw ::CCM_DDS::NonExistent(0);
        break;
      default:
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one - ")
                               ACE_TEXT ("retval <%d>\n"), retval));
        impl->return_loan(data,sample_info);
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
 }

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::read_one_history (
          const typename NDDS_TYPE::value_type& an_instance,
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{

//this function has to return all samples of all instances

  typename NDDS_TYPE::seq_type::_var_type  inst_seq = new typename NDDS_TYPE::seq_type;
  ::CCM_DDS::ReadInfoSeq_var infoseq = new ::CCM_DDS::ReadInfoSeq;


  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (rdr == 0)
  {
      CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_history - ")
                                    ACE_TEXT ("Unable to cast provided DataReader to servant\n")));
      throw CORBA::INTERNAL ();
  }

  typename NDDS_TYPE::data_reader*
      impl =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());

  if (!impl)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::read_one_history - "
                   "Unable to narrow the provided reader entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
    DDS_InstanceHandle_t hnd = impl->lookup_instance (an_instance);
    DDS_SampleInfoSeq sample_info;
    DDS_ReturnCode_t retval = DDS_RETCODE_NO_DATA;
    typename NDDS_TYPE::dds_seq_type data;
    
    // NDDS_TYPE::dds_seq_type = dds sequence
    // NDDS_TYPE::seq_type = ccm sequence
    if (this->condition_)
    {
        // retval =  impl->read_w_condition (data, sample_info, 1, this->condition_);
    }
    else
    {
      if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
      {
          retval = impl->read_instance(data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            hnd,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);
      }
      else
      {
          CIAO_DEBUG ((LM_INFO, ACE_TEXT ("CIAO::DDS4CCM::RTI::Reader_T::read_one_history - ")
                                ACE_TEXT ("No instance found.\n")));
          retval = impl->read(data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);

      }
     }
    CORBA::ULong ix = 0;
    CORBA::ULong nr_of_samples = 0;
    switch(retval)
    {
      case DDS_RETCODE_OK:
         //count the number of valid data
        for (CORBA::ULong i = 0 ; i < (CORBA::ULong)sample_info.length(); i++)
        { 
            if(sample_info[i].valid_data)
                nr_of_samples++;
        } 
        infoseq->length(nr_of_samples);
        inst_seq->length(nr_of_samples);

        for (CORBA::ULong i = 0 ; i < (CORBA::ULong)sample_info.length(); i++)
        { 
            sample_info[i].reception_timestamp >>= infoseq[ix].timestamp;
            inst_seq[ix] = data[i];
            ix++;
        } 
        break;
      case DDS_RETCODE_NO_DATA:
        CIAO_DEBUG ((LM_INFO, ACE_TEXT ("Reader_T: read_all_history No data : retval is %d ---\n"), retval));
        if (!DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
        {    impl->return_loan(data,sample_info);
          throw ::CCM_DDS::NonExistent(0);
        }
        break;
        break;
      default:
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Reader_T: read_all_history Failed retval is %d ---\n"), retval));
        impl->return_loan(data,sample_info);
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
    //return the loan 
    impl->return_loan(data,sample_info);
    infos = infoseq._retn ();
    instances = inst_seq._retn();
}

template <typename NDDS_TYPE, typename CCM_TYPE >
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::filter (void)
{
  return 0;
}

template <typename NDDS_TYPE, typename CCM_TYPE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, CCM_TYPE>::filter (const ::CCM_DDS::QueryFilter & filter)
{
  ACE_UNUSED_ARG (filter);
}

