#include "LB_CPU_Monitor.h"

#if defined (ACE_HAS_SYS_LOADAVG_H)
# include <sys/loadavg.h>
#endif /* ACE_HAS_SYS_LOADAVG_H */


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

TAO_LB_CPU_Monitor::~TAO_LB_CPU_Monitor (void)
{
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

  // Obtain the number of processors each time a load is computed.
  // The idea is to take into account the processor failures that can
  // occur on platforms that continue running with the remaining
  // processors.
  //
  // @@ Is this a realistic scenario?  Perhaps we should just cache
  //    the number of processors and assume that any processor failure
  //    is a catastrophic one.

#if defined (WINDOWS)

  SYSTEM_INFO sys_info;
  ::GetSystemInfo (&sys_info);

  ACE_ASSERT (sys_info.dwNumberOfProcessors > 0);

  load = ::GetLoadAvg () / sys_info.dwNumberOfProcessors;

#elif defined (linux) || defined (sun)

  // Only bother getting the load average over the last minute.
  //
  // @todo Make this configurable so that the load average over the
  //       last 5 and 15 minutes can be used instead.
  double loadavg[1];

# if defined (linux) \
     && ((defined (__GLIBC__) && defined (__GLIBC_MINOR__) \
          && __GLIBC__ == 2 && __GLIBC_MINOR__ < 2) \
         || (!defined (_BSD_SOURCE) && !defined (_GNU_SOURCE)))

  // GLibc < 2.2 does not implement getloadavg().  Furthermore,
  // getloadavg() is only "visible" if _BSD_SOURCE or _GNU_SOURCE is
  // defined.

  // Obtain the load average directly from the `/proc' filesystem.
  FILE * s = ::fopen ("/proc/loadavg", "r");

  if (s == 0)
    ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_DEFAULT_MINOR_CODE,
                          errno),
                        CORBA::COMPLETED_NO),
                      0);

  fscanf (s, "%f", &loadavg[0]);

  (void) fclose (s);

  const int samples = 1;

# else

#   if defined (ACE_HAS_SYS_LOADAVG_H)
  const int samples = ::getloadavg (loadavg, 1);
#   else
  const int samples = 0;
#   endif /* ACE_HAS_SYS_LOADAVG_H */

# endif  /* linux
            && ((__GLIBC__ == 2 && __GLIBC_MINOR__ < 2)
                || (!_BSD_SOURCE && !_GNU_SOURCE)) */

  if (samples == 1)
    {
      const long num_processors = ::sysconf (_SC_NPROCESSORS_ONLN);

      ACE_ASSERT (num_processors > 0);

      if (num_processors > 0)
        load = loadavg[0] / num_processors;
      else
        ACE_THROW_RETURN (CORBA::TRANSIENT (), 0);  // Correct exception?
    }
  else
    ACE_THROW_RETURN (CORBA::TRANSIENT (), 0);  // Correct exception?

#elif defined (hpux)

  struct pst_dynamic psd;

  if (::pstat_getdynamic (&psd, sizeof (psd), (size_t) 1, 0) != -1)
    {
      const long & num_processors = psd.psd_proc_cnt;

      ACE_ASSERT (num_processors > 0);

      if (num_processors > 0)
        load = psd.psd_avg_1_min / num_processors;
      else
        ACE_THROW_RETURN (CORBA::TRANSIENT (), 0);  // Correct exception?
    }
  else
    ACE_THROW_RETURN (CORBA::TRANSIENT (), 0);  // Correct exception?

#endif

#if defined (WINDOWS) || defined (linux) || defined (sun) || defined (hpux)

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

#else

  ACE_UNUSED_ARG (load);
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);

#endif  /* WINDOWS || linux || sun || hpux */

}
