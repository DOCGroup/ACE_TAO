/**
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_USERDATAQOSPOLICY_H
#define CIAO_RTI_USERDATAQOSPOLICY_H

/// @todo

inline void
operator<<= (::DDS::UserDataQosPolicy &ddsqos, const ::DDS_UserDataQosPolicy & qos)
{
//  ddsqos.value <<= qos.value;
}

inline void
operator<<= (::DDS_UserDataQosPolicy &ddsqos, const ::DDS::UserDataQosPolicy & qos)
{
//  ddsqos.value <<= qos.value;
}

inline void
operator>>= (const ::DDS_UserDataQosPolicy &qos, ::DDS::UserDataQosPolicy & ddsqos)
{
//  ddsqos.value <<= qos.value;
}

inline void
operator>>= (const ::DDS::UserDataQosPolicy &qos, ::DDS_UserDataQosPolicy & ddsqos)
{
//  ddsqos.value <<= qos.value;
}

#endif /* CIAO_RTI_USERDATAQOSPOLICY_H */
