/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef DDS4CCM_USERDATAQOSPOLICY_H
#define DDS4CCM_USERDATAQOSPOLICY_H

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
typedef ::DDS::UserDataQosPolicy DDS_UserDataQosPolicy;
#endif

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::UserDataQosPolicy &ddsqos, const ::DDS_UserDataQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (ddsqos.value, qos.value, sizeof (qos.value));
}

inline void
operator<<= (::DDS_UserDataQosPolicy &ddsqos, const ::DDS::UserDataQosPolicy & qos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (ddsqos.value, qos.value, sizeof (qos.value));
}

inline void
operator>>= (const ::DDS_UserDataQosPolicy &qos, ::DDS::UserDataQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (ddsqos.value, qos.value, sizeof (qos.value));
}

inline void
operator>>= (const ::DDS::UserDataQosPolicy &qos, ::DDS_UserDataQosPolicy & ddsqos)
{
  ACE_UNUSED_ARG (ddsqos);
  ACE_UNUSED_ARG (qos);
//   ACE_OS::memcpy (qos.value, ddsqos.value, sizeof (ddsqos.value));
}
#endif

#endif /* DDS4CCM_USERDATAQOSPOLICY_H */
