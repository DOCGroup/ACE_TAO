// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "Time_t.h"

#include "ciao/Logger/Log_Macros.h"

// Implementation skeleton constructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::Reader_T (::DDS::DataReader_ptr reader)
: reader_ (reader),
  condition_(0)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::Reader_T");
  ACE_UNUSED_ARG (reader);
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::~Reader_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Reader_T::~Reader_T");
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::read_all (
  typename NDDS_TYPE::seq_type::_out_type instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
printf("------- in read_all Reader_T of ndds  ------------- \n");
//return last sample of all instances
  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (rdr == 0)
  {
	CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::Reader_T - "
                   "Unable to cast provided DataReader to servant\n"));
      throw CORBA::INTERNAL ();
  }

  typename NDDS_TYPE::data_reader*
	  impl =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());
   

  if (!impl)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::Reader_T - "
                   "Unable to narrow the provided reader entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }

    DDS_SampleInfoSeq sample_info;
    DDS_ReturnCode_t retval;
	
   
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
    switch(retval)
    {
      case DDS_RETCODE_OK:
        printf (" 11111111111Data: retval is %d , number of data = %d---\n", retval, data.length() );
 
        //DDS_SampleInfoSeq sample_info  convert to ::CCM_DDS::ReadInfoSeq_out infos
        //typename NDDS_TYPE::dds_seq_type data  convert to  typename NDDS_TYPE::seq_type::_out_type instances
        
      /*   infos = new ::CCM_DDS::ReadInfoSeq(sample_info.length ());
         printf (" 22222222222Data: retval is %d , number of data = %d---\n, sample_info_length = %d ==\n", retval, data.length(), sample_info.length());
         for (CORBA::ULong i = 0; i < (CORBA::ULong)sample_info.length(); ++i)
         { 
            printf("23232323232332sample_info[%d].reception_timestamp = %d\n",i,sample_info[i].reception_timestamp);
            //sample_info[i].reception_timestamp >>= (*infos)[i].timestamp;
         } 
         
         printf (" 3333333333333Data: retval is %d , number of data = %d---\n", retval, data.length() );
 
         instances = new typename NDDS_TYPE::seq_type(data.length());
         printf (" 44444444444Data: retval is %d , number of data = %d---\n", retval, data.length() );
         //printf("instances.size() = %d\n", instances.size());
         for (CORBA::ULong i = 0; i < (CORBA::ULong)data.length(); i++)
         { 
             printf("444444444444444444444data.symbol[%d] = %s\n",i, data[i].symbol);
             memcpy(instances[i],data[i]);

         }
         */
         printf (" 55555555555Data: retval is %d , number of data = %d---\n", retval, data.length() );
         break;
      case DDS_RETCODE_NO_DATA:
        printf ("No data : retval is %d ---\n", retval);
        break;
      default:
        printf ("failed retval is %d ---\n", retval);
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::read_all_history (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::read_one (
          typename NDDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
  printf("------- in read_one Reader_T of ndds ------------- \n");

  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (this->reader_);
  if (rdr == 0)
  {
	CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::Reader_T - "
                   "Unable to cast provided DataReader to servant\n"));
      throw CORBA::INTERNAL ();
  }

  typename NDDS_TYPE::data_reader*
	  impl =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());
  

  if (!impl)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Reader_T::Reader_T - "
                   "Unable to narrow the provided reader entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
    
    DDS_InstanceHandle_t hnd = impl->lookup_instance (an_instance);
    if (DDS_InstanceHandle_equals (&hnd, & ::DDS_HANDLE_NIL))
    {
        printf ("Reader->read_one: Instance not registered\n");
    }

    DDS_SampleInfoSeq sample_info;
    DDS_ReturnCode_t retval;
	
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
          printf(" ------ read_instance --------\n");
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
           printf(" ------ read  without an instance --------\n");
           retval = impl->read(data,
                            sample_info,
                            DDS_LENGTH_UNLIMITED,
                            DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                            DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                            DDS_ALIVE_INSTANCE_STATE);

      }
     }
    int number_of_instances = 0;
    switch(retval)
    {
      case DDS_RETCODE_OK:
        printf (" Data: retval is %d ---\n", retval);
        number_of_instances = data.length();
        printf("number_of_instances =%d\n", number_of_instances); 
        //get last instance
        an_instance = data[number_of_instances-1];
        sample_info[number_of_instances-1].reception_timestamp >>= info.timestamp;
        //what about the following attributes?
        //info.access_status     DDS_SampleStateKind  sample_state or   DDS_ViewStateKind view_state; ?
        //info.instance_status   DDS_InstanceStateKind instance_state;
        //info.instance_rank     DDS_Long sample_rank;   is always 0 with last sample 
        
         break;
      case DDS_RETCODE_NO_DATA:
        printf ("No data : retval is %d ---\n", retval);
        throw ::CCM_DDS::NonExistent(0);
         break;
      default:
        printf ("failed retval is %d ---\n", retval);
        throw ::CCM_DDS::InternalError (retval, 0);
        break;
    }
 }

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::read_one_history (
          const typename NDDS_TYPE::value_type& an_instance,
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (an_instance);
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
}

template <typename NDDS_TYPE, typename BASE >
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::filter (void)
{
  return 0;
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::filter (const ::CCM_DDS::QueryFilter & filter)
{
  ACE_UNUSED_ARG (filter);
}

