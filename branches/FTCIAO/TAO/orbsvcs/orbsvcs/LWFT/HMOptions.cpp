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
    port_range_begin_ (7000),
    RM_update_freq_ (1),
    load_monitor_freq_(2),
    debug_level_ (0)    
{
  char hostname [100];
  gethostname (hostname, sizeof (hostname));
  host_id_ = hostname;
//  ACE_DEBUG ((LM_DEBUG, "Hostname is %s.\n", hostname));
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
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-port_range_begin"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> port_range_begin_))
            {
              return false;
            }
                
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
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-debug"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> debug_level_))
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
HMOptions::port_range_begin (void) const
{
  return port_range_begin_;
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

void 
HMOptions::set_debug_level (void)
{
  u_long mask = LM_EMERGENCY | LM_ALERT | LM_CRITICAL | LM_ERROR;

  switch (debug_level_)
    {
    case 0: break;
    case 1:
      {
        mask |= LM_WARNING;
        break;
      }
    case 2:
      {
        mask |= LM_WARNING | LM_DEBUG;
        break;
      }
    case 3:
      {
        mask |= LM_WARNING | LM_DEBUG | LM_TRACE;
        break;
      }
    case 4:
      {
        mask |= LM_WARNING | LM_DEBUG | LM_TRACE | LM_NOTICE;
        break;
      }
    case 5:
      {
        mask |= LM_WARNING | LM_DEBUG | LM_TRACE | LM_NOTICE | LM_INFO;
        break;
      }
    default: break;
    }

  ACE_LOG_MSG->priority_mask (mask,
                              ACE_Log_Msg::PROCESS);
}
