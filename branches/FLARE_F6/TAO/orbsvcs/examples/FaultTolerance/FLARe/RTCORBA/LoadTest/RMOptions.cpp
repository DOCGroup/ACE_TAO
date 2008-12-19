#include "RMOptions.h"

#include <sstream>

#include "ArgPair.h"

#include "ace/Global_Macros.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"

/// Initialize the static data member.
RMOptions * volatile RMOptions::instance_ = 0;
ACE_Auto_Ptr<RMOptions> RMOptions::deleter_;
ACE_Thread_Mutex RMOptions::lock_;

RMOptions::RMOptions (void)
  : hertz_ (0.2),
    proactive_ (true),
    arg_pair_ (0,0)
{
}

RMOptions *RMOptions::instance (void)
{
  if (! instance_)
  {
    ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, lock_, 0);
    
    if (! instance_)
    {
      instance_ = new RMOptions ();
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
    
  ACE_Get_Opt get_opts (argc, argv, "h:p:");
  int c;

  while ((c = get_opts ()) != -1)
      switch (c)
        {
          case 'h':
            {
              std::istringstream istr (get_opts.opt_arg ());
              if (!(istr >> hertz_))
                  return false;
              break;
            }
          case 'p':
            {
              std::istringstream istr (get_opts.opt_arg ());
              int val;
              if (!(istr >> val))
                  return false;
              proactive_ = val? true : false;
              break;
            }
        }
  return retval;
};


bool RMOptions::proactive () const
{
  return proactive_;
}

double RMOptions::hertz () const
{
  return hertz_;
}
