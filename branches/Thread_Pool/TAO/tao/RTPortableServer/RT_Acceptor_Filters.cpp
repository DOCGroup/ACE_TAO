// @(#) $Id$

#include "RT_Acceptor_Filters.h"
#include "tao/Pluggable.h"
#include "tao/MProfile.h"

#if ! defined (__ACE_INLINE__)
#include "RT_Acceptor_Filters.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, RT_Acceptor_Filter, "$Id$")

TAO_Server_Protocol_Acceptor_Filter::
TAO_Server_Protocol_Acceptor_Filter (RTCORBA::ProtocolList &protocols)
  :  protocols_ (protocols)
{
}

int
TAO_Server_Protocol_Acceptor_Filter::
fill_mprofile (const TAO_ObjectKey &object_key,
               TAO_MProfile &mprofile,
               TAO_Acceptor **acceptors_begin,
               TAO_Acceptor **acceptors_end)
{
  // RTCORBA 1.0, Section 4.15.1: ServerProtocolPolicy determines
  // which protocols get included into IOR and in what order.
  for (CORBA::ULong j = 0; j < this->protocols_.length (); ++j)
    {
      CORBA::ULong protocol_type = this->protocols_[j].protocol_type;

      for (TAO_Acceptor** acceptor = acceptors_begin;
           acceptor != acceptors_end;
           ++acceptor)
        if ((*acceptor)->tag () == protocol_type
            && this->validate_acceptor (*acceptor)
            && ((*acceptor)->create_mprofile (object_key,
                                              mprofile,
                                              1 /* >=1 endpoints per profile */)
                == -1))
          return -1;
    }

  return 0;
}

int
TAO_Server_Protocol_Acceptor_Filter::
encode_endpoints (TAO_MProfile &mprofile)
{
  // Encode endpoints.
  for (CORBA::ULong i = 0;
       i < mprofile.profile_count ();
       ++i)
    {
      TAO_Profile *profile = mprofile.get_profile (i);
      if (profile->encode_endpoints () == -1)
        return -1;
    }

  return 0;
}
int
TAO_Server_Protocol_Acceptor_Filter::
validate_acceptor (TAO_Acceptor * /*acceptor*/)
{
  return 1;
}

TAO_Priority_Acceptor_Filter::
TAO_Priority_Acceptor_Filter (RTCORBA::ProtocolList &protocols,
                              CORBA::Short priority)
  : TAO_Server_Protocol_Acceptor_Filter (protocols),
    priority_ (priority)
{
}

int
TAO_Priority_Acceptor_Filter::validate_acceptor (TAO_Acceptor *acceptor)
{
  return (this->priority_ == acceptor->priority ());
}

TAO_Bands_Acceptor_Filter::
TAO_Bands_Acceptor_Filter (RTCORBA::ProtocolList &protocols,
                           RTCORBA::PriorityBands &bands)
  : TAO_Server_Protocol_Acceptor_Filter (protocols),
    priority_bands_ (bands)
{
}

int
TAO_Bands_Acceptor_Filter::validate_acceptor (TAO_Acceptor *acceptor)
{
  for (CORBA::ULong i = 0;
       i < this->priority_bands_.length ();
       ++i)
    {
      if (this->priority_bands_[i].low <= acceptor->priority ()
          && this->priority_bands_[i].high >= acceptor->priority ())
        return 1;
    }
  return 0;
}
