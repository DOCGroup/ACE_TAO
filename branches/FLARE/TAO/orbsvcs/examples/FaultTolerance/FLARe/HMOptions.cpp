#include <unistd.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "HMOptions.h"
#include "ArgPair.h"
#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

/// Initialize the static data member.
HMOptions * volatile HMOptions::instance_ = 0;
std::auto_ptr <HMOptions> HMOptions::deleter_;
ACE_Thread_Mutex HMOptions::lock_;


HMOptions::HMOptions (void)
  : RM_ior_ ("file://rm.ior"),
    arg_pair_ (0,0),
    RM_update_freq_ (1),
    load_monitor_freq_(2)
{
  char hostname [100];
  gethostname (hostname, sizeof (hostname));
  host_id_ = hostname;
  ACE_DEBUG((LM_DEBUG,"Hostname is %s.\n",hostname));
}

HMOptions *HMOptions::instance (void)
{
  if (! instance_)
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
    if (! instance_)
    {
      instance_ = new HMOptions ();
      deleter_.reset (instance_);
    }
  }
  return instance_;
}

bool
HMOptions::parse_args (int argc, char **argv)
{
  bool retval = true;
  this->arg_pair_ = ArgPair (argc, argv);
    
  ACE_Get_Opt get_opts (argc, argv, "-i:h:r:l:n:o:u:");
  int c;

  while ((c = get_opts ()) != -1)
      switch (c)
        {
          case 'u':
            {
              util_file_ = std::string (get_opts.opt_arg());
              break;
            }
          case 'o':
            {
              HM_ior_file_ = std::string (get_opts.opt_arg());
              break;
            }
          case 'i':
            {
              RM_ior_ = std::string (get_opts.opt_arg ());
              ior_access_ = std::make_pair ('i', RM_ior_);
              break;
            }
          case 'h':
            {
              host_id_ = std::string (get_opts.opt_arg ());
              break;
            }
          case 'r':
            {
              std::istringstream istr (get_opts.opt_arg ());
              if (!(istr >> RM_update_freq_))
                return false;
              break;
            }
          case 'l':
            {
              std::istringstream istr (get_opts.opt_arg ());
              if (!(istr >> load_monitor_freq_))
                return false;
              break;
            }
          case 'n':
            {
              RM_ior_ = std::string (get_opts.opt_arg ());
              ior_access_ = std::make_pair ('n', RM_ior_);
              break;
            }
        }
  return retval;
};

std::string HMOptions::HM_ior_file () const
{
  return HM_ior_file_;
}

std::string HMOptions::RM_ior () const
{
  return RM_ior_;
}

ArgPair HMOptions::arg_pair ()
{
  return arg_pair_;
}

std::string HMOptions::host_id () const
{
  return host_id_;
}

int HMOptions::RM_update_freq () const
{
  return RM_update_freq_;
}

std::string HMOptions::util_file () const
{
  return util_file_;
}

int HMOptions::load_monitor_freq () const
{
  return load_monitor_freq_;
}

std::pair <char, std::string> HMOptions::ior_access () const
{
  return ior_access_;
}
