/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_HISTORYQOSPOLICY_H
#define DDS4CCM_HISTORYQOSPOLICY_H

#include "dds4ccm/impl/dds4ccm_conf.h"

#if (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::HistoryQosPolicy DDS_HistoryQosPolicy;
#endif

#if (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::HistoryQosPolicy &ddsqos, const ::DDS_HistoryQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS::HistoryQosPolicyKind> (qos.kind);
  ddsqos.depth = qos.depth;
}

inline void
operator<<= (::DDS_HistoryQosPolicy &ddsqos, const ::DDS::HistoryQosPolicy & qos)
{
  ddsqos.kind = static_cast < ::DDS_HistoryQosPolicyKind> (qos.kind);
  ddsqos.depth = qos.depth;
}

#endif

#endif /* DDS4CCM_HISTORYQOSPOLICY_H */
