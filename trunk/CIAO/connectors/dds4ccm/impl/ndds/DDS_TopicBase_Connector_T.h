/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_TOPIC_BASE_CONNECTOR_H
#define DDS_TOPIC_BASE_CONNECTOR_H

#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/ndds/DDS_Base_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_TopicBase_Connector_T
  : public virtual DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>
{
public:
  DDS_TopicBase_Connector_T (void);
  virtual ~DDS_TopicBase_Connector_T (void);

  virtual char *topic_name (void);

  virtual void topic_name (const char * topic_name);

  virtual ::DDS::StringSeq *key_fields (void);

  virtual void key_fields (const ::DDS::StringSeq & key_fields);

protected:
  CORBA::String_var topic_name_;
  DDS::StringSeq key_fields_;
};

#include "dds4ccm/impl/ndds/DDS_TopicBase_Connector_T.cpp"

#endif /* DDS_TOPIC_BASE_CONNECTOR_H */
