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

#endif /* DDS4CCM_HISTORYQOSPOLICY_H */
