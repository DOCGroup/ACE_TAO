/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_READERDATALIFECYCLEQOSPOLICY_H
#define DDS4CCM_READERDATALIFECYCLEQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#include "Duration_t.h"

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

#endif /* DDS4CCM_READERDATALIFECYCLEQOSPOLICY_H */
