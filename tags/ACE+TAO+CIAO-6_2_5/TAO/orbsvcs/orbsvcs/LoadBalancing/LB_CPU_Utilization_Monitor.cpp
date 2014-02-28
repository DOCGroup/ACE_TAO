// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/LoadBalancing/LB_CPU_Utilization_Monitor.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"
#include "ace/os_include/sys/os_loadavg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

double calc_cpu_loading (void)
{
  static char buf[1024];
  static unsigned long prev_idle = 0;
  static double prev_total = 0.0;

  FILE *file_ptr = 0;
  char *item = 0;
  char *arg = 0;
  unsigned long delta_idle = 0;
  unsigned long user = 0;
  unsigned long nice = 0;
  unsigned long idle = 0;
  unsigned long sys = 0;

  double percent_cpu_load = 0.0;

  if ((file_ptr = ACE_OS::fopen("/proc/stat", "r")) == 0)
          return percent_cpu_load;

  while ((ACE_OS::fgets (buf, sizeof (buf), file_ptr)) != 0)
  {
    item = ACE_OS::strtok (buf, " \t\n");
    arg = ACE_OS::strtok (0, "\n");

    if (item == 0 || arg == 0)
            continue;
    if (item[0] == 'c' && ACE_OS::strlen (item) == 3)
    {
      sscanf (arg, "%lu %lu %lu %lu", &user, &nice, &sys, &idle);
      break;
    }


  }

  ACE_OS::fclose (file_ptr);

  delta_idle = idle - prev_idle;
  double total;
  double time_passed;
  total = (double) (user + nice + sys + idle);
  time_passed = total - prev_total;

  percent_cpu_load = 100.0 - (delta_idle / time_passed * 100.0);

  prev_idle = idle;
  prev_total = total;

  return percent_cpu_load;
}

TAO_LB_CPU_Utilization_Monitor::TAO_LB_CPU_Utilization_Monitor (const char * location_id,
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
      this->location_[0].id = CORBA::string_dup (location_id);

      if (location_kind != 0)
        this->location_[0].kind = CORBA::string_dup (location_kind);
    }
}

TAO_LB_CPU_Utilization_Monitor::~TAO_LB_CPU_Utilization_Monitor (void)
{
}

CosLoadBalancing::Location *
TAO_LB_CPU_Utilization_Monitor::the_location (void)
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
TAO_LB_CPU_Utilization_Monitor::loads (void)
{
  CORBA::Float load = 0;

#if defined (linux) || defined (sun)

  double load_double = calc_cpu_loading ();
  load = load_double;

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

  ORBSVCS_DEBUG ((LM_DEBUG, "%2f\n", load_list[0].value));

  return load_list._retn ();

#else

  ACE_UNUSED_ARG (load);
  throw CORBA::NO_IMPLEMENT ();

#endif  /* linux || sun */

}

TAO_END_VERSIONED_NAMESPACE_DECL
