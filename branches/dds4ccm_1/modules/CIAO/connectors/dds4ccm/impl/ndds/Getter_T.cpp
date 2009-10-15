// $Id$
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/Utils.h"

#include "ciao/Logger/Log_Macros.h"
// Implementation skeleton constructor
template <typename NDDS_TYPE, typename BASE >
CIAO::DDS4CCM::RTI::Getter_T<NDDS_TYPE, BASE>::Getter_T (::DDS::DataReader_ptr reader)
: impl_ (reader),
  condition_(0)
{
  printf("----in constructor getter -----\n");
  CIAO_TRACE ("CIAO::DDS4CCM::RTI::Getter_T::Getter_T");
  ACE_UNUSED_ARG (reader);
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
  ACE_UNUSED_ARG (an_instance);
  ACE_UNUSED_ARG (info);
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
