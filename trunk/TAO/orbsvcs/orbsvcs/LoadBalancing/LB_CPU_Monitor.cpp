#include "LB_CPU_Monitor.h"


ACE_RCSID (LoadBalancing,
           LB_CPU_Monitor,
           "$Id$")

TAO_LB_CPU_Monitor::TAO_LB_CPU_Monitor (const char * location_id,
                                        const char * location_kind)
  : location_ (1)
{
  this->location_.length (1);

  if (location_id == 0)
    {
      char host[MAXHOSTNAMELEN + 1];
      if (ACE_OS::hostname (host, sizeof (host)) != 0)
        {
          // Couldn't determine hostname.  Use the current time
          // instead.
          CORBA::ULong t = ACE_static_cast (CORBA::ULong, ACE_OS::time ());

          // A 64 byte buffer is more than enough to contain the
          // string representation of a 32 bit unsigned integer.
          char buf[64] = { '\0' };
          ACE_OS::sprintf (buf, "%u", t);

          this->location_[0].id = CORBA::string_dup (buf);
          this->location_[0].kind = CORBA::string_dup ("Creation Time");
        }
      else
        {
          this->location_[0].id = CORBA::string_dup (host);
          this->location_[0].kind = CORBA::string_dup ("Hostname");
        }
    }
  else
    {
      this->location_[0].id = CORBA::string_dup (location_id);
      this->location_[0].kind =
        (location_kind == 0
         ? CORBA::string_dup ("User Specified")
         : CORBA::string_dup (location_kind));
    }
}

CosLoadBalancing::Location *
TAO_LB_CPU_Monitor::the_location (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosLoadBalancing::Location * location;
  ACE_NEW_THROW_EX (location,
                    CosLoadBalancing::Location (this->location_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  return location;
}

CosLoadBalancing::LoadList *
TAO_LB_CPU_Monitor::loads (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Float load = 0;

#ifdef WINDOWS
  load = ::GetLoadAvg ();
#else
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
#endif

  CosLoadBalancing::LoadList * tmp;
  ACE_NEW_THROW_EX (tmp,
                    CosLoadBalancing::LoadList (1),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  CosLoadBalancing::LoadList_var load_list = tmp;

  load_list->length (1);

  load_list[0].id = CosLoadBalancing::CPU;
  load_list[0].value = load;

  return load_list._retn ();
}
