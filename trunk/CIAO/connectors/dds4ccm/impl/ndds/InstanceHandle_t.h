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
operator!= (const ::DDS::InstanceHandle_t & instancehandle1, const ::DDS::InstanceHandle_t & instancehandle2)
{
  return instancehandle1.length  != instancehandle2.length  ||
         instancehandle1.isValid != instancehandle2.isValid ||
         ACE_OS::memcmp (instancehandle1.value, instancehandle2.value, sizeof (instancehandle1.value)) != 0;
}

#endif /* CIAO_RTI_INSTANCEHANDLE_T_H */
