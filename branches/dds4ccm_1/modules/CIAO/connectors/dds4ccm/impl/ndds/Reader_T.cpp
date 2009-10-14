// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "ciao/Logger/Log_Macros.h"
// Implementation skeleton constructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE, BASE>::Reader_T (::DDS::DataReader_ptr reader)
: reader_ (reader),
  condition_(0)
{
  
  printf("----in constructor reader -----\n");
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
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
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
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::Writer_T - "
                   "Unable to cast provided DataWriter to servant\n"));
      throw CORBA::INTERNAL ();
    }

  NDDS_TYPE::data_reader*
	  impl_ =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());

  if (!impl_)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Writer_T::Writer_T - "
                   "Unable to narrow the provided writer entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }

    DDS_SampleInfoSeq sample_info;
    DDS_ReturnCode_t retval;
	
  //impl->read (
 // This has to be reworked using a reader template and traits. 
    DDS_StringSeq data;
    if (this->condition_)
      {
//        retval = (DDS_StringDataReader)this->reader_->read_w_condition (data, sample_info, 1, this->condition_);
      }
    else
      {
    
/*	retval = this->reader_->read (data,
                           sample_info,
                           1,
                           DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                           DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                           DDS_ALIVE_INSTANCE_STATE);*/
       }
	printf("------- in read_one Reader_T of ndds 222222222 \n");
    if (retval == DDS_RETCODE_OK)
      {
//        an_instance = data[0];
        //info.timestamp <<= sample_info[0].reception_timestamp;
      }
    else
      {
        //printf ("failed %d", retval);
        throw ::CCM_DDS::InternalError (retval, 0);
      }
printf("------- in read_one Reader_T of ndds 333333333333333\n");
//  ACE_UNUSED_ARG (an_instance);
//  ACE_UNUSED_ARG (info);
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

