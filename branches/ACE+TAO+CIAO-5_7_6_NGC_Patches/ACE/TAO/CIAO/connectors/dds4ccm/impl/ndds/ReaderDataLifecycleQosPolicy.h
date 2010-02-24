/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_READERDATALIFECYCLEQOSPOLICY_H
#define CIAO_RTI_READERDATALIFECYCLEQOSPOLICY_H

inline void
operator<<= (::DDS::ReaderDataLifecycleQosPolicy &ddsqos, const ::DDS_ReaderDataLifecycleQosPolicy & qos)
{
  ddsqos.autopurge_nowriter_samples_delay <<= qos.autopurge_nowriter_samples_delay;
  ddsqos.autopurge_disposed_samples_delay <<= qos.autopurge_disposed_samples_delay;
}

inline void
operator<<= (::DDS_ReaderDataLifecycleQosPolicy &ddsqos, const ::DDS::ReaderDataLifecycleQosPolicy & qos)
{
  ddsqos.autopurge_nowriter_samples_delay <<= qos.autopurge_nowriter_samples_delay;
  ddsqos.autopurge_disposed_samples_delay <<= qos.autopurge_disposed_samples_delay;
}

inline void
operator>>= (const ::DDS_ReaderDataLifecycleQosPolicy &qos, ::DDS::ReaderDataLifecycleQosPolicy & ddsqos)
{
  ddsqos.autopurge_nowriter_samples_delay <<= qos.autopurge_nowriter_samples_delay;
  ddsqos.autopurge_disposed_samples_delay <<= qos.autopurge_disposed_samples_delay;
}

inline void
operator>>= (const ::DDS::ReaderDataLifecycleQosPolicy &qos, ::DDS_ReaderDataLifecycleQosPolicy & ddsqos)
{
  ddsqos.autopurge_nowriter_samples_delay <<= qos.autopurge_nowriter_samples_delay;
  ddsqos.autopurge_disposed_samples_delay <<= qos.autopurge_disposed_samples_delay;
}

#endif /* CIAO_RTI_READERDATALIFECYCLEQOSPOLICY_H */
