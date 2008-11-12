// -*- C++ -*-
// $Id$

#include <sstream>

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

#include "RMOptions.h"

/// Initialize the static data member.
RMOptions * volatile RMOptions::instance_ = 0;
ACE_Auto_Ptr<RMOptions> RMOptions::deleter_;
ACE_Thread_Mutex RMOptions::lock_;

RMOptions::RMOptions (void)
  : hertz_ (0.2),
    proactive_ (true),
    static_mode_ (false),
    use_naming_service_ (false),
    arg_pair_ (0, 0)
{
}

RMOptions *
RMOptions::instance (void)
{
  if (! instance_)
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
    
    if (! instance_)
    {
      instance_ = new RMOptions;
      deleter_.reset (instance_);
    }
  }
  
  return instance_;
}

bool
RMOptions::parse_args (int argc, char **argv)
{
  bool retval = true;
  this->arg_pair_ = ArgPair (argc, argv);
    
  ACE_Get_Opt get_opts (argc, argv, "h:p:s:n");
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'h':
            {
              std::istringstream istr (get_opts.opt_arg ());
              
              if (!(istr >> hertz_))
                {
                  return false;
                }
                  
              break;
            }
          case 'p':
            {
              std::istringstream istr (get_opts.opt_arg ());
              int val;
              
              if (!(istr >> val))
                {
                  return false;
                }
                  
              proactive_ = val ? true : false;
              break;
            }
          case 's' :
	          {
              std::istringstream istr (get_opts.opt_arg ());
              int val;
              
              if (!(istr >> val))
                {
                  return false;
                }
                  
	            static_mode_ = val ? true : false;
	            break;
	          }
	        case 'n':
	          use_naming_service_ = true;
	          break;
        }
    }
    
  return retval;
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
