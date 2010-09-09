// $Id$

#include "orbsvcs/LoadBalancing/LB_CPU_Load_Average_Monitor.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"
#include "ace/os_include/sys/os_pstat.h"
#include "ace/os_include/sys/os_loadavg.h"
#if defined(__NetBSD__) || defined (__APPLE__)
#include <sys/sysctl.h>
#endif

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_CPU_Load_Average_Monitor::TAO_LB_CPU_Load_Average_Monitor (
  const ACE_TCHAR * location_id,
  const ACE_TCHAR * location_kind)
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
          CORBA::ULong t = static_cast<CORBA::ULong> (ACE_OS::time ());

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
      this->location_[0].id = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(location_id));

      if (location_kind != 0)
        this->location_[0].kind = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(location_kind));
    }
}

TAO_LB_CPU_Load_Average_Monitor::~TAO_LB_CPU_Load_Average_Monitor (void)
{
}

CosLoadBalancing::Location *
TAO_LB_CPU_Load_Average_Monitor::the_location (void)
{
  CosLoadBalancing::Location * location;
  ACE_NEW_THROW_EX (location,
                    CosLoadBalancing::Location (this->location_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return location;
}

CosLoadBalancing::LoadList *
TAO_LB_CPU_Load_Average_Monitor::loads (void)
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

#if 0

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
  FILE * s = ACE_OS::fopen ("/proc/loadavg", "r");

  if (s == 0)
    throw CORBA::NO_IMPLEMENT (
      CORBA::SystemException::_tao_minor_code (
        TAO::VMCID,
        errno),
      CORBA::COMPLETED_NO);

  fscanf (s, "%f", &loadavg[0]);

  (void) ACE_OS::fclose (s);

  const int samples = 1;

# else

#   if defined (ACE_LACKS_GETLOADAVG)

  const int samples = 0;

#   else

  const int samples = ::getloadavg (loadavg, 1);

#   endif /* ACE_LACKS_GETLOADAVG */

# endif  /* linux
            && ((__GLIBC__ == 2 && __GLIBC_MINOR__ < 2)
                || (!_BSD_SOURCE && !_GNU_SOURCE)) */

  if (samples == 1)
    {
      const long num_processors = ACE_OS::sysconf (_SC_NPROCESSORS_ONLN);

      ACE_ASSERT (num_processors > 0);

      if (num_processors > 0)
        load = loadavg[0] / num_processors;
      else
        throw CORBA::TRANSIENT ();  // Correct exception?
    }
  else
    throw CORBA::TRANSIENT ();  // Correct exception?

#elif defined (__NetBSD__) || defined (__APPLE__)

  double loadavg[1];

  const int samples = ::getloadavg (loadavg, 1);

  if (samples == 1)
    {
      int mib[2], num_processors;
      size_t len;

      mib[0] = CTL_HW;
      mib[1] = HW_NCPU;
      len = sizeof(num_processors);

      sysctl(mib, 2, &num_processors, &len, 0, 0);

      ACE_ASSERT (num_processors > 0);

      if (num_processors > 0)
        load = loadavg[0] / num_processors;
      else
        throw CORBA::TRANSIENT ();  // Correct exception?
    }
  else
    throw CORBA::TRANSIENT ();  // Correct exception?

#elif defined (__hpux)

  struct pst_dynamic psd;

  if (::pstat_getdynamic (&psd, sizeof (psd), (size_t) 1, 0) != -1)
    {
      const long & num_processors = psd.psd_proc_cnt;

      ACE_ASSERT (num_processors > 0);

      if (num_processors > 0)
        load = psd.psd_avg_1_min / num_processors;
      else
        throw CORBA::TRANSIENT ();  // Correct exception?
    }
  else
    throw CORBA::TRANSIENT ();  // Correct exception?

#endif

#if defined (linux) || defined (sun) || defined (__hpux) || defined(__NetBSD__) || defined (__APPLE__)

  CosLoadBalancing::LoadList * tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    CosLoadBalancing::LoadList (1),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  CosLoadBalancing::LoadList_var load_list = tmp;

  load_list->length (1);

  load_list[0].id = CosLoadBalancing::LoadAverage;
  load_list[0].value = load;

  return load_list._retn ();

#else

  ACE_UNUSED_ARG (load);
  throw CORBA::NO_IMPLEMENT ();

#endif  /* linux || sun || __hpux  || __NetBSD__ || __APPLE__ */

}

TAO_END_VERSIONED_NAMESPACE_DECL
