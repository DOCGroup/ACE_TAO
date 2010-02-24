/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_WRITERDATALIFECYCLEQOSPOLICY_H
#define CIAO_RTI_WRITERDATALIFECYCLEQOSPOLICY_H

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

inline void
operator>>= (const ::DDS_WriterDataLifecycleQosPolicy &qos, ::DDS::WriterDataLifecycleQosPolicy & ddsqos)
{
  ddsqos.autodispose_unregistered_instances = qos.autodispose_unregistered_instances;
}

inline void
operator>>= (const ::DDS::WriterDataLifecycleQosPolicy &qos, ::DDS_WriterDataLifecycleQosPolicy & ddsqos)
{
  ddsqos.autodispose_unregistered_instances = qos.autodispose_unregistered_instances;
}

#endif /* CIAO_RTI_WRITERDATALIFECYCLEQOSPOLICY_H */
