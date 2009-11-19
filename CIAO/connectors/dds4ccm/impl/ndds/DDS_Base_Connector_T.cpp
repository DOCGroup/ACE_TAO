// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_Base_Connector_T (void)
  : domain_id_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_Base_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::DomainId_t
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (void)
{
  return this->domain_id_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  this->domain_id_ = domain_id;
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (void)
{
  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (
  const char * qos_profile)
{
  this->qos_profile_ = qos_profile;
}

