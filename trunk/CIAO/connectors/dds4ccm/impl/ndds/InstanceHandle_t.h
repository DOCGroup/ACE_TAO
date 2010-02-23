/**
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * @author Johnny Willemsen <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */

#ifndef CIAO_RTI_INSTANCEHANDLE_T_H
#define CIAO_RTI_INSTANCEHANDLE_T_H

#if defined (CIAO_DDS4CCM_NDDS) && (CIAO_DDS4CCM_NDDS==1)
inline void
operator<<= (::DDS::InstanceHandle_t &ddsinstancehandle, const ::DDS_InstanceHandle_t & instancehandle)
{
  ACE_OS::memcpy (ddsinstancehandle.value, instancehandle.keyHash.value, sizeof (instancehandle.keyHash.value));
  ddsinstancehandle.length = instancehandle.keyHash.length;
  ddsinstancehandle.isValid = instancehandle.isValid;
}

inline void
operator<<= (::DDS_InstanceHandle_t &ddsinstancehandle, const ::DDS::InstanceHandle_t & instancehandle)
{
  ACE_OS::memcpy (ddsinstancehandle.keyHash.value, instancehandle.value, sizeof (instancehandle.value));
  ddsinstancehandle.keyHash.length = instancehandle.length;
  ddsinstancehandle.isValid = instancehandle.isValid;
}

inline void
operator>>= (const ::DDS_InstanceHandle_t &instancehandle, ::DDS::InstanceHandle_t & ddsinstancehandle)
{
  ACE_OS::memcpy (ddsinstancehandle.value, instancehandle.keyHash.value, sizeof (instancehandle.keyHash.value));
  ddsinstancehandle.length = instancehandle.keyHash.length;
  ddsinstancehandle.isValid = instancehandle.isValid;
}

inline void
operator>>= (const ::DDS::InstanceHandle_t &instancehandle, ::DDS_InstanceHandle_t & ddsinstancehandle)
{
  ACE_OS::memcpy (ddsinstancehandle.keyHash.value, instancehandle.value, sizeof (instancehandle.value));
  ddsinstancehandle.keyHash.length = instancehandle.length;
  ddsinstancehandle.isValid = instancehandle.isValid;
}

inline bool
operator== (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  return instancehandle1.length  == instancehandle2.length  &&
         instancehandle1.isValid == instancehandle2.isValid &&
         ACE_OS::memcmp (instancehandle1.value, instancehandle2.value, sizeof (instancehandle1.value)) == 0;
}

inline bool
operator== (const DDS_InstanceHandle_t & dds_instancehandle, const ::DDS::InstanceHandle_t & ccm_instancehandle)
{
  return dds_instancehandle.keyHash.length  == ccm_instancehandle.length  &&
         static_cast<CORBA::ULong>(dds_instancehandle.isValid) == ccm_instancehandle.isValid &&
         ACE_OS::memcmp (dds_instancehandle.keyHash.value, ccm_instancehandle.value, sizeof (dds_instancehandle.keyHash.value)) == 0;
}

inline bool
operator== (const ::DDS::InstanceHandle_t & ccm_instancehandle, const DDS_InstanceHandle_t & dds_instancehandle)
{
  return ccm_instancehandle.length  == dds_instancehandle.keyHash.length  &&
         ccm_instancehandle.isValid == static_cast<CORBA::ULong>(dds_instancehandle.isValid) &&
         ACE_OS::memcmp (ccm_instancehandle.value, dds_instancehandle.keyHash.value, sizeof (ccm_instancehandle.value)) == 0;
}

inline bool
operator!= (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  return instancehandle1.length  != instancehandle2.length  ||
         instancehandle1.isValid != instancehandle2.isValid ||
         ACE_OS::memcmp (instancehandle1.value, instancehandle2.value, sizeof (instancehandle1.value)) != 0;
}

inline bool
operator!= (const DDS_InstanceHandle_t & dds_instancehandle, const ::DDS::InstanceHandle_t & ccm_instancehandle)
{
  return dds_instancehandle.keyHash.length != ccm_instancehandle.length  ||
         static_cast<CORBA::ULong>(dds_instancehandle.isValid) != ccm_instancehandle.isValid ||
         ACE_OS::memcmp (dds_instancehandle.keyHash.value, ccm_instancehandle.value, sizeof (dds_instancehandle.keyHash.value)) != 0;
}

inline bool
operator!= (const ::DDS::InstanceHandle_t & ccm_instancehandle, const DDS_InstanceHandle_t & dds_instancehandle)
{
  return ccm_instancehandle.length  != dds_instancehandle.keyHash.length  ||
         ccm_instancehandle.isValid != static_cast<CORBA::ULong>(dds_instancehandle.isValid) ||
         ACE_OS::memcmp (ccm_instancehandle.value, dds_instancehandle.keyHash.value, sizeof (ccm_instancehandle.value)) != 0;
}
#endif

#if defined (CIAO_DDS4CCM_OPENDDS) && (CIAO_DDS4CCM_OPENDDS==1)
inline void
operator<<= (::DDS::InstanceHandle_t &ddsinstancehandle, const ::DDS_InstanceHandle_t & instancehandle)
{
  ddsinstancehandle = instancehandle;
}
#endif

#endif /* CIAO_RTI_INSTANCEHANDLE_T_H */
