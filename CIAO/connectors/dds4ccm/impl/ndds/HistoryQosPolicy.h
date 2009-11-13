/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_HISTORYQOSPOLICY_H
#define CIAO_RTI_HISTORYQOSPOLICY_H

inline void
operator<<= (::DDS::HistoryQosPolicy &ddsqos, const ::DDS_HistoryQosPolicy & qos)
{
  ddsqos.kind = qos.kind;
  ddsqos.depth = qos.depth;
}

inline void
operator<<= (::DDS_HistoryQosPolicy &ddsqos, const ::DDS::HistoryQosPolicy & qos)
{
  ddsqos.kind = qos.kind;
  ddsqos.depth = qos.depth;
}

inline void
operator>>= (const ::DDS_HistoryQosPolicy &qos, ::DDS::HistoryQosPolicy & ddsqos)
{
  ddsqos.kind = qos.kind;
  ddsqos.depth = qos.depth;
}

inline void
operator>>= (const ::DDS::HistoryQosPolicy &qos, ::DDS_HistoryQosPolicy & ddsqos)
{
  ddsqos.kind = qos.kind;
  ddsqos.depth = qos.depth;
}

#endif /* CIAO_RTI_HISTORYQOSPOLICY_H */
