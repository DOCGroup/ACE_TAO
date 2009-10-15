// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"
#include "dds4ccm/impl/ndds/NDDS_Traits.h"

#include "ciao/Logger/Log_Macros.h"
// Implementation skeleton constructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::Getter_T (::DDS::DataReader_ptr reader)
: impl_ (0),
  condition_(0)
{
  printf("----in constructor getter -----\n");
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::Getter_T");

  RTI_DataReader_i *rdr = dynamic_cast <RTI_DataReader_i *> (reader);

  if (rdr == 0)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                   "Unable to cast provided DataWriter to servant\n"));
      throw CORBA::INTERNAL ();
    }

  this->impl_ =  NDDS_TYPE::data_reader::narrow (rdr->get_datareader ());

  if (!this->impl_)
    {
      CIAO_ERROR ((LM_ERROR, CLINFO "CIAO::DDS4CCM::RTI::Getter_T::Getter_T - "
                   "Unable to narrow the provided writer entity to the specific "
                   "type necessary to publish messages\n"));
      throw CORBA::INTERNAL ();
    }
}

// Implementation skeleton destructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::~Getter_T (void)
{
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::~Getter_T");
}

template <typename NDDS_TYPE, typename BASE >
bool
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::get_all (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
  return true;
}

template <typename NDDS_TYPE, typename BASE >
bool
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::get_all_history (
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
  return true;
}

template <typename NDDS_TYPE, typename BASE >
bool
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::get_one (
          typename NDDS_TYPE::value_type& an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
/*  DDS_SampleInfoSeq sample_info;
  DDS_ReturnCode_t retval;
  typename NDDS_TYPE::dds_seq_type data;
  if (this->condition_)
    {
      // retval =  impl_->read_w_condition (data, sample_info, 1, this->condition_);
    }
    else
      {
        retval = impl_->read (data,
                           sample_info,
                           1,
                           DDS_READ_SAMPLE_STATE | DDS_NOT_READ_SAMPLE_STATE ,
                           DDS_NEW_VIEW_STATE | DDS_NOT_NEW_VIEW_STATE,
                           DDS_ALIVE_INSTANCE_STATE);
       }
    printf("------- in read_one Reader_T of ndds 222222222 \n");
    if (retval == DDS_RETCODE_OK)
      {
        an_instance = data[0];
        //info.timestamp <<= sample_info[0].reception_timestamp;
      }
    else
      {
        printf ("failed retval is %d ---\n", retval);
        throw ::CCM_DDS::InternalError (retval, 0);
      }*/
  return true;
}

template <typename NDDS_TYPE, typename BASE >
bool
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::get_one_history (
          const typename NDDS_TYPE::value_type& an_instance,
          typename NDDS_TYPE::seq_type::_out_type instances,
          ::CCM_DDS::ReadInfoSeq_out infos)
{
  ACE_UNUSED_ARG (an_instance);
  ACE_UNUSED_ARG (instances);
  ACE_UNUSED_ARG (infos);
  return true;
}

template <typename NDDS_TYPE, typename BASE >
bool
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::get_next (
          typename NDDS_TYPE::value_type::_out_type  an_instance,
          ::CCM_DDS::ReadInfo_out info)
{
  ACE_UNUSED_ARG (an_instance);
  ACE_UNUSED_ARG (info);
  return true;
}

template <typename NDDS_TYPE, typename BASE >
 ::CCM_DDS::QueryFilter *
 CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::filter (void)
{
  return 0;
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::filter (const ::CCM_DDS::QueryFilter & filter)
{
  ACE_UNUSED_ARG (filter);
}

template <typename NDDS_TYPE, typename BASE >
::DDS::Duration_t
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::time_out (void)
{
  return ::DDS::Duration_t ();
}

template <typename NDDS_TYPE, typename BASE >
void
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::time_out (const ::DDS::Duration_t & time_out)
{
  ACE_UNUSED_ARG (time_out);
}
