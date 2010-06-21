// $Id$

#if !defined DERIVED_CONNECTOR_T
#define DERIVED_CONNECTOR_T

#include "connectors/dds4ccm/impl/dds/DDS_Event_Connector_T.h"
#include "connectors/dds4ccm/impl/dds/DDS_State_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class MyEvent_T
  : public virtual DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>
{
};

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class MyState_T
  : public virtual DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>
{
};

#endif
