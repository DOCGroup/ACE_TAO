/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 */
#ifndef DDS_BASE_CONNECTOR_T_H
#define DDS_BASE_CONNECTOR_T_H

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_Base_Connector_T
  : public virtual CCM_TYPE::base_type,
    public virtual ::CORBA::LocalObject,
    private virtual ACE_Copy_Disabled
{
public:
  DDS_Base_Connector_T (void);
  virtual ~DDS_Base_Connector_T (void);

  virtual ::DDS::DomainId_t domain_id (void);

  virtual void domain_id (::DDS::DomainId_t domain_id);

  virtual char *qos_profile (void);

  virtual void qos_profile (const char * qos_profile);

protected:
  DDS::DomainId_t domain_id_;
  CORBA::String_var qos_profile_;
};

#include "dds4ccm/impl/ndds/DDS_Base_Connector_T.cpp"

#endif /* CONNECTOR_T_H_ */
