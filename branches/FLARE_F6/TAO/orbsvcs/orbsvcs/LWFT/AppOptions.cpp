// -*- C++ -*-
// $Id$

#include "AppOptions.h"

#include <sstream>

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Arg_Shifter.h"

/// Initialize the static data member.
AppOptions * volatile AppOptions::instance_ = 0;
ACE_Auto_Ptr<AppOptions> AppOptions::deleter_;
ACE_Thread_Mutex AppOptions::lock_;

AppOptions::AppOptions (void)
  : host_monitor_ior_ ("file://hm.ior"),
    port_ (5000),
    role_ (2),
    load_ (40.0),
    use_dds_ (false)
{
  char hostname [100];
  gethostname (hostname, sizeof (hostname));
  host_id_ = hostname;
//  ACE_DEBUG ((LM_DEBUG, "Hostname is %s.\n", hostname));
}

AppOptions *
AppOptions::instance (void)
{
  if (instance_ == 0)
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
      
      if (instance_ == 0)
        {
          instance_ = new AppOptions ();
          deleter_.reset (instance_);
        }
    }
  
  return instance_;
}

bool
AppOptions::parse_args (int &argc, char **argv)
{
  ACE_Arg_Shifter as (argc, argv);

  while (as.is_anything_left ())
    {
      const ACE_TCHAR *arg = 0;

      if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-hm_port"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> port_))
            {
              return false;
            }
            
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-hm_ior"))))
        {
          host_monitor_ior_ = arg;        
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-proc_id"))))
        {
          process_id_ = arg;        
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-info_file"))))
        {
          object_info_file_ = arg;        
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-app_id"))))
        {
          app_id_ = arg;        
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-role"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> role_))
            {
              return false;
            }
            
          as.consume_arg ();
        }
      else if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-load"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> load_))
            {
              return false;
            }
            
          as.consume_arg ();
        }
      else if (as.cur_arg_strncasecmp (ACE_TEXT ("-use_dds")) == 0)
        {
          use_dds_ = true;
          as.consume_arg ();
        }
      else
        {
          as.ignore_arg ();
        }
    }

  return true;
}

std::string
AppOptions::host_id (void) const
{
  return host_id_;
}

std::string
AppOptions::host_monitor_ior (void) const
{
  return host_monitor_ior_;
}

u_short
AppOptions::port (void) const
{
  return port_;
}

std::string
AppOptions::object_info_file (void) const
{
  return object_info_file_;
}

std::string
AppOptions::process_id (void) const
{
  return process_id_;
}

std::string
AppOptions::app_id (void) const
{
  return app_id_;
}

size_t
AppOptions::role (void) const
{
  return role_;
}

double
AppOptions::load (void) const
{
  return load_;
}

bool
AppOptions::use_dds (void) const
{
  return use_dds_;
}




