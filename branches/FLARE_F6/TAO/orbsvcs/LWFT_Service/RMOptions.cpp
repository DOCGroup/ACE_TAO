// -*- C++ -*-
// $Id$

#include <sstream>

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Arg_Shifter.h"

#include "RMOptions.h"

/// Initialize the static data member.
RMOptions * volatile RMOptions::instance_ = 0;
ACE_Auto_Ptr<RMOptions> RMOptions::deleter_;
ACE_Thread_Mutex RMOptions::lock_;

RMOptions::RMOptions (void)
  : hertz_ (0.2),
    proactive_ (true),
    static_mode_ (false),
    use_naming_service_ (false)
{
}

RMOptions *
RMOptions::instance (void)
{
  if (instance_ == 0)
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
      
      if (instance_ == 0)
        {
          instance_ = new RMOptions;
          deleter_.reset (instance_);
        }
    }
  
  return instance_;
}

bool
RMOptions::parse_args (int &argc, char **argv)
{
  ACE_Arg_Shifter as (argc, argv);

  while (as.is_anything_left ())
    {
      const ACE_TCHAR *arg = 0;

      if (0 != (arg = as.get_the_parameter (ACE_TEXT ("-hertz"))))
        {
          std::istringstream istr (arg);
          
          if (!(istr >> hertz_))
            {
              return false;
            }
            
          as.consume_arg ();
        }
      else if (as.cur_arg_strncasecmp (ACE_TEXT ("-proactive")) == 0)
        {
          proactive_ = true;
          as.consume_arg ();
        }
      else if (as.cur_arg_strncasecmp (ACE_TEXT ("-static")) == 0)
        {
          static_mode_ = true;
          as.consume_arg ();
        }
      else if (as.cur_arg_strncasecmp (ACE_TEXT ("-use_ns")) == 0)
        {
          use_naming_service_ = true;
          as.consume_arg ();
        }
      else
        {
          as.ignore_arg ();
        }
    }

  return true;
};


bool
RMOptions::proactive (void) const
{
  return proactive_;
}

double
RMOptions::hertz (void) const
{
  return hertz_;
}

bool
RMOptions::static_mode (void) const
{
  return static_mode_;
}

bool
RMOptions::use_naming_service (void) const
{
  return use_naming_service_;
}
