// Trace.cpp
// $Id$

// We need this to get the status of ACE_NTRACE...

#define ACE_BUILD_DLL
#include "ace/config.h"

// Turn off tracing for the duration of this file.
#if defined (ACE_NTRACE)
#undef ACE_NTRACE
#endif /* ACE_NTRACE */
#define ACE_NTRACE 1

// This must come first to avoid "order of include" problems...

#if !defined (ACE_HAS_INLINED_OSCALLS) && !defined (ACE_WIN32)
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

ACE_Trace::ACE_Trace (const char *n, 
		      int line, 
		      const char *file)
{
  this->name_ = n;

  if (ACE_Trace::enable_tracing_)
    {
      ACE_Log_Msg *lm = ACE_LOG_MSG;
      if (lm->tracing_enabled ()
	  && lm->trace_active () == 0)
	{
	  lm->trace_active (1);
	  ACE_DEBUG ((LM_TRACE, "%*s(%t) calling %s in file `%s' on line %d\n",
		      ACE_Trace::nesting_indent_ * lm->inc (),
		      "", this->name_, file, line));
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
	  ACE_DEBUG ((LM_TRACE, "%*s(%t) leaving %s\n", 
		      ACE_Trace::nesting_indent_ * lm->dec (),
		      "", this->name_));
	  lm->trace_active (0);
	}
    }
}

