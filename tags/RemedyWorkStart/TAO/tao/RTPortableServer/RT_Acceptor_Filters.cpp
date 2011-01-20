// $Id$

#include "tao/RTPortableServer/RT_Acceptor_Filters.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/Transport_Acceptor.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Server_Protocol_Acceptor_Filter::TAO_Server_Protocol_Acceptor_Filter (
  RTCORBA::ProtocolList &protocols)
  : protocols_ (protocols)
{
}

int
TAO_Server_Protocol_Acceptor_Filter::fill_profile (
  const TAO::ObjectKey &object_key,
  TAO_MProfile &mprofile,
  TAO_Acceptor **acceptors_begin,
  TAO_Acceptor **acceptors_end,
  CORBA::Short priority)
{
  // RTCORBA 1.0, Section 4.15.1: ServerProtocolPolicy determines
  // which protocols get included into IOR and in what order.
  for (CORBA::ULong j = 0; j < this->protocols_.length (); ++j)
    {
      CORBA::ULong const protocol_type = this->protocols_[j].protocol_type;

      for (TAO_Acceptor** acceptor = acceptors_begin;
           acceptor != acceptors_end;
           ++acceptor)
        if ((*acceptor)->tag () == protocol_type &&
            ((*acceptor)->create_profile (object_key,
                                          mprofile,
                                          priority) == -1))
          return -1;
    }

  return 0;
}

int
TAO_Server_Protocol_Acceptor_Filter::encode_endpoints (TAO_MProfile &mprofile)
{
  // Encode endpoints.
  for (CORBA::ULong i = 0; i < mprofile.profile_count (); ++i)
    {
      TAO_Profile *profile = mprofile.get_profile (i);
      if (profile->encode_endpoints () == -1)
        return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
