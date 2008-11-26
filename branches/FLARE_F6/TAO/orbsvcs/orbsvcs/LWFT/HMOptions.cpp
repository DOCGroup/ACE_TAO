// -*- C++ -*-
// $Id$

#include <sstream>

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Arg_Shifter.h"

#include "HMOptions.h"

/// Initialize the static data member.
HMOptions * volatile HMOptions::instance_ = 0;
ACE_Auto_Ptr<HMOptions> HMOptions::deleter_;
ACE_Thread_Mutex HMOptions::lock_;

HMOptions::HMOptions (void)
  : RM_ior_ ("file://rm.ior"),
    HM_ior_file_ ("hm.ior"),
    RM_update_freq_ (1),
    load_monitor_freq_(2)
{
  char hostname [100];
  gethostname (hostname, sizeof (hostname));
  host_id_ = hostname;
  ACE_DEBUG ((LM_DEBUG, "Hostname is %s.\n", hostname));
}

HMOptions *
HMOptions::instance (void)
{
  if (instance_ == 0)
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
      
      if (instance_ == 0)
        {
          instance_ = new HMOptions ();
          deleter_.reset (instance_);
        }
    }
    
  return instance_;
}

bool
HMOptions::parse_args (int &argc, char **argv)
{
  ACE_Arg_Shifter as (argc, argv);

  while (as.is_anything_left ())
    {
      const ACE_TCHAR *arg = 0;

      if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-util_file"))))
        {
          util_file_ = arg;
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-ior_file"))))
        {
          HM_ior_file_ = arg;
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-rm_ior"))))
        {
          RM_ior_ = arg;
          ior_access_ = std::make_pair ('i', RM_ior_);
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-rm_name"))))
        {
          RM_ior_ = arg;
          ior_access_ = std::make_pair ('n', RM_ior_);
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-host_id"))))
        {
          host_id_ = arg;
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-rm_update_freq"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> RM_update_freq_))
            {
              return false;
            }
                
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-load_mon_freq"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> load_monitor_freq_))
            {
              return false;
            }
                
          as.consume_arg ();
        }
      else
        {
          as.ignore_arg ();
        }
    }
/*
  ACE_Get_Opt get_opts (argc, argv, "-i:h:r:l:n:o:u:");
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'u':
            {
              util_file_ = std::string (get_opts.opt_arg ());
              break;
            }
          case 'o':
            {
              HM_ior_file_ = std::string (get_opts.opt_arg ());
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
                {
                  return false;
                }
                
              break;
            }
          case 'l':
            {
              std::istringstream istr (get_opts.opt_arg ());
              
              if (!(istr >> load_monitor_freq_))
                {
                  return false;
                }
                
              break;
            }
          case 'n':
            {
              RM_ior_ = std::string (get_opts.opt_arg ());
              ior_access_ = std::make_pair ('n', RM_ior_);
              break;
            }
        }
    }
*/        
  return true;
};

std::string
HMOptions::HM_ior_file (void) const
{
  return HM_ior_file_;
}

std::string
HMOptions::RM_ior (void) const
{
  return RM_ior_;
}

std::string
HMOptions::host_id (void) const
{
  return host_id_;
}

int
HMOptions::RM_update_freq (void) const
{
  return RM_update_freq_;
}

std::string
HMOptions::util_file (void) const
{
  return util_file_;
}

int
HMOptions::load_monitor_freq (void) const
{
  return load_monitor_freq_;
}

std::pair<char, std::string>
HMOptions::ior_access (void) const
{
  return ior_access_;
}

