/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_WRITERDATALIFECYCLEQOSPOLICY_H
#define DDS4CCM_WRITERDATALIFECYCLEQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::WriterDataLifecycleQosPolicy DDS_WriterDataLifecycleQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
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
#endif

#endif /* DDS4CCM_WRITERDATALIFECYCLEQOSPOLICY_H */
