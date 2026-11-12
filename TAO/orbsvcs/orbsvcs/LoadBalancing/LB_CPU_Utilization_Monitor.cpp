#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/LoadBalancing/LB_CPU_Utilization_Monitor.h"
#include "tao/ORB_Constants.h"
#include "ace/OS_NS_time.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/os_include/os_netdb.h"

#include <cstring>
#include <cstdio>
#include <memory>
#include <string>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

static double calc_cpu_loading ()
{
  char buf[1024] {};
  static unsigned long prev_idle {};
  static double prev_total {};

  std::unique_ptr<FILE, int (*) (FILE *)> file_ptr (
    ACE_OS::fopen ("/proc/stat", "r"), ACE_OS::fclose);
  if (!file_ptr)
    return 0.0;

  unsigned long user {};
  unsigned long nice {};
  unsigned long idle {};
  unsigned long sys {};

  while (ACE_OS::fgets (buf, sizeof (buf), file_ptr.get ()) != nullptr)
    {
      char * const item = ACE_OS::strtok (buf, " \t\n");
      char * const arg = ACE_OS::strtok (nullptr, "\n");

      if (item == nullptr || arg == nullptr)
        continue;
      if (item[0] == 'c' && std::strlen (item) == 3)
        {
          std::sscanf (arg, "%lu %lu %lu %lu", &user, &nice, &sys, &idle);
          break;
        }
    }

  double const delta_idle_value = static_cast<double> (idle - prev_idle);
  double const total = static_cast<double> (user + nice + sys + idle);
  double const time_passed = total - prev_total;
  double const percent_cpu_load = time_passed > 0.0
    ? 100.0 - (delta_idle_value / time_passed * 100.0)
    : 0.0;

  prev_idle = idle;
  prev_total = total;

  return percent_cpu_load;
}

TAO_LB_CPU_Utilization_Monitor::TAO_LB_CPU_Utilization_Monitor (char const * location_id,
                                                                char const * location_kind)
  : location_ (1)
{
  this->location_.length (1);

  if (location_id == nullptr)
    {
      char host[MAXHOSTNAMELEN + 1] {};
      if (ACE_OS::hostname (host, sizeof (host)) != 0)
        {
          // Couldn't determine hostname.  Use the current time
          // instead.
          CORBA::ULong const t = static_cast<CORBA::ULong> (ACE_OS::time ());
          std::string const time_string = std::to_string (t);

          this->location_[0].id = CORBA::string_dup (time_string.c_str ());
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

      if (location_kind != nullptr)
        this->location_[0].kind = CORBA::string_dup (location_kind);
    }
}

TAO_LB_CPU_Utilization_Monitor::~TAO_LB_CPU_Utilization_Monitor () = default;

CosLoadBalancing::Location *
TAO_LB_CPU_Utilization_Monitor::the_location ()
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
TAO_LB_CPU_Utilization_Monitor::loads ()
{
#if defined (ACE_LINUX)
  CORBA::Float const load = static_cast<CORBA::Float> (calc_cpu_loading ());

  CosLoadBalancing::LoadList * tmp {};
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
  throw CORBA::NO_IMPLEMENT ();

#endif  /* linux */

}

TAO_END_VERSIONED_NAMESPACE_DECL
