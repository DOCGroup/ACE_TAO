// $Id$

// We need this to get the status of ACE_NTRACE...

#define ACE_BUILD_DLL
#include "ace/inc_user_config.h"

ACE_RCSID(ace, Trace, "$Id$")

// Turn off tracing for the duration of this file.
#if defined (ACE_NTRACE)
#undef ACE_NTRACE
#endif /* ACE_NTRACE */
#define ACE_NTRACE 1

// This must come first to avoid "order of include" problems...

#if !defined (ACE_HAS_INLINED_OSCALLS) && !defined(ACE_HAS_ONE_DEFINITION_RULE)
#define ACE_HAS_INLINED_OSCALLS
#include "ace/ACE.h"
#undef ACE_HAS_INLINED_OSCALLS
#else
#include "ace/ACE.h"
#endif /* !ACE_HAS_INLINED_OSCALLS */

#include "ace/Log_Msg.h"
#include "ace/Trace.h"

#if !defined (__ACE_INLINE__)
#include "ace/Trace.i"
#endif /* __ACE_INLINE__ */

// = Static initialization.

// Keeps track of how far to indent per trace call.
int ACE_Trace::nesting_indent_ = ACE_Trace::DEFAULT_INDENT;

// Is tracing enabled?
int ACE_Trace::enable_tracing_ = ACE_Trace::DEFAULT_TRACING;

ACE_ALLOC_HOOK_DEFINE(ACE_Trace)

void
ACE_Trace::dump (void) const
{
}

// Determine whether or not tracing is enabled

int
ACE_Trace::is_tracing(void)
{
  return ACE_Trace::enable_tracing_;
}

// Enable the tracing facility.

void
ACE_Trace::start_tracing (void)
{
  ACE_Trace::enable_tracing_ = 1;
}

// Disable the tracing facility.

void
ACE_Trace::stop_tracing (void)
{
  ACE_Trace::enable_tracing_ = 0;
}

// Change the nesting indentation level.

void
ACE_Trace::set_nesting_indent (int indent)
{
  ACE_Trace::nesting_indent_ = indent;
}

// Perform the first part of the trace, which prints out the string N,
// the LINE, and the ACE_FILE as the function is entered.

ACE_Trace::ACE_Trace (const ASYS_TCHAR *n,
                      int line,
                      const ASYS_TCHAR *file)
{
#if defined (ACE_NLOGGING)
  ACE_UNUSED_ARG (line);
  ACE_UNUSED_ARG (file);
#endif /* ACE_NLOGGING */

  this->name_ = n;

  if (ACE_Trace::enable_tracing_)
    {
      ACE_Log_Msg *lm = ACE_LOG_MSG;
      if (lm->tracing_enabled ()
          && lm->trace_active () == 0)
        {
          lm->trace_active (1);
          ACE_DEBUG ((LM_TRACE, ASYS_TEXT ("%*s(%t) calling %s in file `%s' on line %d\n"),
                      ACE_Trace::nesting_indent_ * lm->inc (),
                      ASYS_TEXT (""), this->name_, file, line));
          lm->trace_active (0);
        }
    }
}

// Perform the second part of the trace, which prints out the NAME as
// the function is exited.

ACE_Trace::~ACE_Trace (void)
{
  if (ACE_Trace::enable_tracing_)
    {
      ACE_Log_Msg *lm = ACE_LOG_MSG;
      if (lm->tracing_enabled ()
          && lm->trace_active () == 0)
        {
          lm->trace_active (1);
          ACE_DEBUG ((LM_TRACE, ASYS_TEXT ("%*s(%t) leaving %s\n"),
                      ACE_Trace::nesting_indent_ * lm->dec (),
                      ASYS_TEXT (""), this->name_));
          lm->trace_active (0);
        }
    }
}
