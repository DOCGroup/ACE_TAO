/**
 * @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_TRANSPORTPRIOTITYQOSPOLICY_H
#define DDS4CCM_TRANSPORTPRIOTITYQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

inline void
operator<<= (::DDS::TransportPriorityQosPolicy &ddsqos, const ::DDS_TransportPriorityQosPolicy & qos)
{
  ddsqos.value = qos.value;
}

inline void
operator<<= (::DDS_TransportPriorityQosPolicy &ddsqos, const ::DDS::TransportPriorityQosPolicy & qos)
{
  ddsqos.value = qos.value;
}

#endif /* DDS4CCM_TRANSPORTPRIOTITYQOSPOLICY_H */
