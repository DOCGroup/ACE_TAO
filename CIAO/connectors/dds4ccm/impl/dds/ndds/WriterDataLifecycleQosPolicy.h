/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_WRITERDATALIFECYCLEQOSPOLICY_H
#define DDS4CCM_WRITERDATALIFECYCLEQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::WriterDataLifecycleQosPolicy DDS_WriterDataLifecycleQosPolicy;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::WriterDataLifecycleQosPolicy &ddsqos, const ::DDS_WriterDataLifecycleQosPolicy & qos)
{
  ddsqos.autodispose_unregistered_instances = qos.autodispose_unregistered_instances;
}

inline void
operator<<= (::DDS_WriterDataLifecycleQosPolicy &ddsqos, const ::DDS::WriterDataLifecycleQosPolicy & qos)
{
  ddsqos.autodispose_unregistered_instances = qos.autodispose_unregistered_instances;
}

#endif

#endif /* DDS4CCM_WRITERDATALIFECYCLEQOSPOLICY_H */
