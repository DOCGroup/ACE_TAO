/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_GROUPDATAQOSPOLICY_H
#define DDS4CCM_GROUPDATAQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::GroupDataQosPolicy DDS_GroupDataQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
// @todo here
inline void
operator<<= (::DDS::GroupDataQosPolicy &ddsqos, const ::DDS_GroupDataQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (ddsqos.value, qos.value, sizeof (qos.value));
}

inline void
operator<<= (::DDS_GroupDataQosPolicy &ddsqos, const ::DDS::GroupDataQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (ddsqos.value, qos.value, sizeof (qos.value));
}

inline void
operator>>= (const ::DDS_GroupDataQosPolicy &qos, ::DDS::GroupDataQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (ddsqos.value, qos.value, sizeof (qos.value));
}

inline void
operator>>= (const ::DDS::GroupDataQosPolicy &qos, ::DDS_GroupDataQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (qos.value, ddsqos.value, sizeof (ddsqos.value));
}
#endif

#endif /* DDS4CCM_GROUPDATAQOSPOLICY_H */
