#include "AppOptions.h"

#include <sstream>

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

/// Initialize the static data member.
AppOptions * volatile AppOptions::instance_ = 0;
ACE_Auto_Ptr<AppOptions> AppOptions::deleter_;
ACE_Thread_Mutex AppOptions::lock_;

AppOptions::AppOptions (void)
  : host_monitor_ior_ ("file://HostMonitor.ior"),
    port_ (5000),
    arg_pair_ (0,0)
{
  char hostname [100];
  gethostname (hostname, sizeof (hostname));
  host_id_ = hostname;
  ACE_DEBUG((LM_DEBUG,"Hostname is %s.\n",hostname));
}

AppOptions *AppOptions::instance (void)
{
  if (! instance_)
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
    
    if (! instance_)
    {
      instance_ = new AppOptions ();
      deleter_.reset (instance_);
    }
  }
  
  return instance_;
}

bool
AppOptions::parse_args (int argc, char **argv)
{
  bool retval = true;
  this->arg_pair_ = ArgPair (argc, argv);

  ACE_Get_Opt get_opts (argc, argv, "-k:z:i:p:");
  int c;

  while ((c = get_opts ()) != -1)
      switch (c)
        {
          case 'z':
            {
              std::istringstream istr (get_opts.opt_arg ());
              if (!(istr >> port_))
                return false;
              break;
            }
          case 'k':
            {
              host_monitor_ior_ = std::string (get_opts.opt_arg ());
              break;
            }
/*          case 'd':
            {
              host_id_ = std::string (get_opts.opt_arg ());
              break;
            }
*/          case 'i':
            {
              object_info_file_ = std::string (get_opts.opt_arg ());
              break;
            }
          case 'p':
            {
              process_id_ = std::string (get_opts.opt_arg ());
              break;
            }
        }
  return retval;
}

ArgPair AppOptions::arg_pair () const
{
  return this->arg_pair_;
}
std::string AppOptions::ior_output_file () const
{
  return ior_output_file_;
}

std::string AppOptions::object_info_file () const
{
  return object_info_file_;
}

std::string AppOptions::process_id () const
{
  return process_id_;
}

std::string AppOptions::host_monitor_ior () const
{
  return host_monitor_ior_;
}

u_short AppOptions::get_port () const
{
  return port_;
}

std::string AppOptions::host_id () const
{
  return host_id_;
}
