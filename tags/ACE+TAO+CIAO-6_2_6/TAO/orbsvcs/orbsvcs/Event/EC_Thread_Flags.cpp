// $Id$

#include "orbsvcs/Event/EC_Thread_Flags.h"

#include "ace/OS_NS_Thread.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "orbsvcs/Log_Macros.h"
#include "ace/Sched_Params.h"



#define TETFSF(flag)  { #flag, flag }
TAO_EC_Thread_Flags::Supported_Flag TAO_EC_Thread_Flags::supported_flags_[] = {
#if defined (ACE_HAS_THREADS)
    TETFSF(THR_CANCEL_DISABLE),
    TETFSF(THR_CANCEL_ENABLE),
    TETFSF(THR_CANCEL_DEFERRED),
    TETFSF(THR_CANCEL_ASYNCHRONOUS),
    TETFSF(THR_BOUND),
    TETFSF(THR_NEW_LWP),
    TETFSF(THR_DETACHED),
    TETFSF(THR_SUSPENDED),
    TETFSF(THR_DAEMON),
    TETFSF(THR_JOINABLE),
    TETFSF(THR_SCHED_FIFO),
    TETFSF(THR_SCHED_RR),
    TETFSF(THR_SCHED_DEFAULT),
    TETFSF(THR_EXPLICIT_SCHED),
    TETFSF(THR_SCOPE_SYSTEM),
    TETFSF(THR_SCOPE_PROCESS)
#endif /* ACE_HAS_THREADS */
  };
#undef TETFSF

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Thread_Flags::~TAO_EC_Thread_Flags ()
{
}

void
TAO_EC_Thread_Flags::parse_symbols (const char* syms)
{
  // PRE: we assume nothing other than that syms is valid
  // POST:
  // 1. flags_ is bitwise-OR of all flags
  // 2. sched_ is THR_SCHED_*, or THR_SCHED_DEFAULT if not specified
  // 3. scope_ is THR_SCOPE_*, or THR_SCOPE_PROCESS if not specified

  // NOTE: I'm not sure if #2 and #3 are consistent with what happens
  // in ACE_Task::activate.  I really need to double-check that, and
  // make sure that they are consistent.

  flags_ = scope_ = sched_ = 0; // zero out everything

  // short-circuit on the trivial case
  if (syms == 0 || *syms == '\0')
    return;

  static size_t num_flags = sizeof(supported_flags_)/sizeof(Supported_Flag);
  char* s = ACE_OS_String::strdup (syms); // need a mutable string
  if (s == 0)
    return;

  const char* SEPARATORS = " |"; // this should probably be at class level
  char* ptr = 0;
  char* tok = ACE_OS_String::strtok_r (s, SEPARATORS, &ptr);
  while (tok != 0)
    {
      // This would allow for easy accomodation of flags that
      // aren't currently supported, but is it a good idea?

      if (tok[0] >= '0' && tok[0] <= '9') // Numeric, so just accept it!
        {
          // parse it as a long straight to the flags

          // If somebody specifies the scheduler this way, then they
          // lose range checking on the priority.  Bummer, but those
          // are the breaks.
          this->flags_ |= ACE_OS_String::strtol (tok, 0, 0);
        }
      else
        {
          int found = 0;
          for (size_t i = 0; !found && i < num_flags; ++i)
            {
              if (ACE_OS_String::strcasecmp (tok, supported_flags_[i].n) == 0)
                {
                  this->flags_ |= supported_flags_[i].v;

                  // Can't use a switch for this b/c for some
                  // platforms the THR_* constants end up with
                  // the same values, and compiles get upset.
                  long &sf = supported_flags_[i].v;
                  if (sf == THR_SCHED_FIFO ||
                      sf == THR_SCHED_RR ||
                      sf == THR_SCHED_DEFAULT)
                    {
                      this->sched_ = supported_flags_[i].v;
                    }
                  else if (sf == THR_SCOPE_SYSTEM ||
                           sf == THR_SCOPE_PROCESS)
                    {
                      this->scope_ = supported_flags_[i].v;
                    }
                  found = 1;
                }
            }
          if (!found)
            {
              // Ideally this would call some sort of on-error function...
              // but, it doesn't.
              ORBSVCS_ERROR ((LM_ERROR,
                          "RTEC (%P|%t) unable to parse %C as a thread flag - skipping\n",
                          tok));
            }
        }
      tok = ACE_OS_String::strtok_r (0, SEPARATORS, &ptr);
    }

  ACE_OS::free (s); // clean up after ourselves
}

long
TAO_EC_Thread_Flags::default_priority () const
{
  long priority = ACE_DEFAULT_THREAD_PRIORITY;

  // use the implementation
  if (this->sched() == 0)
    return priority;

  priority =
    ACE_Sched_Params::priority_min (this->sched()) +
    ACE_Sched_Params::priority_max (this->sched()) / 2;
  priority = ACE_Sched_Params::next_priority (this->sched(), priority);

  return priority;
}

TAO_END_VERSIONED_NAMESPACE_DECL
