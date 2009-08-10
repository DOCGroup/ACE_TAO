// $Id$

#include "Reader.h"

// Implementation skeleton constructor
CCM_DDS_string_Reader_i::CCM_DDS_string_Reader_i (void)
{
}

// Implementation skeleton destructor
CCM_DDS_string_Reader_i::~CCM_DDS_string_Reader_i (void)
{
}

void CCM_DDS_string_Reader_i::read_all (
  ::CCM_DDS::string_Reader::stringSeq_out instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  // Add your implementation here
}

void CCM_DDS_string_Reader_i::read_all_history (
  ::CCM_DDS::string_Reader::stringSeq_out instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  // Add your implementation here
}

void CCM_DDS_string_Reader_i::read_one (
  char *& an_instance,
  ::CCM_DDS::ReadInfo_out info)
{
  // Add your implementation here
}

void CCM_DDS_string_Reader_i::read_one_history (
  const char * an_instance,
  ::CCM_DDS::string_Reader::stringSeq_out instances,
  ::CCM_DDS::ReadInfoSeq_out infos)
{
  // Add your implementation here
}

::CCM_DDS::QueryFilter * CCM_DDS_string_Reader_i::filter (void)
{
  // Add your implementation here
}

void CCM_DDS_string_Reader_i::filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  // Add your implementation here
}
