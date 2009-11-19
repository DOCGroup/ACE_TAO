// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_TopicBase_Connector_T (void) :
    DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>()
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (void)
{
  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (
  const char * topic_name)
{
  this->topic_name_ = topic_name;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (const ::DDS::StringSeq & key_fields)
{
  ACE_UNUSED_ARG (key_fields);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (void)
{
  ::DDS::StringSeq *retval =
    new ::DDS::StringSeq (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    {
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);
    }
  return retval;
}

