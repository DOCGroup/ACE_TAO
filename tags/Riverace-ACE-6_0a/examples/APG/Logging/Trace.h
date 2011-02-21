// $Id$

#ifndef TRACE_H
#define TRACE_H

#include "ace/Log_Msg.h"

// Listing 1 code/ch03
class Trace
{
public:
  Trace (const ACE_TCHAR *prefix,
   const ACE_TCHAR *name,
   int line,
   const ACE_TCHAR *file)
    {
      this->prefix_ = prefix;
      this->name_   = name;
      this->line_   = line;
      this->file_   = file;

      ACE_Log_Msg *lm = ACE_LOG_MSG;
      if (lm->tracing_enabled ()
          && lm->trace_active () == 0)
        {
          lm->trace_active (1);
          ACE_DEBUG
            ((LM_TRACE,
              ACE_TEXT ("%s%*s(%t) calling %s in file `%s'")
              ACE_TEXT (" on line %d\n"),
              this->prefix_,
              Trace::nesting_indent_ * lm->inc (),
              ACE_TEXT (""),
              this->name_,
              this->file_,
              this->line_));
          lm->trace_active (0);
        }
    }

  void setLine (int line)
    {
      this->line_ = line;
    }

  ~Trace (void)
    {
      ACE_Log_Msg *lm = ACE_LOG_MSG;
      if (lm->tracing_enabled ()
          && lm->trace_active () == 0)
        {
          lm->trace_active (1);
          ACE_DEBUG
            ((LM_TRACE,
              ACE_TEXT ("%s%*s(%t) leaving %s in file `%s'")
              ACE_TEXT (" on line %d\n"),
              this->prefix_,
              Trace::nesting_indent_ * lm->dec (),
              ACE_TEXT (""),
              this->name_,
              this->file_,
              this->line_));
          lm->trace_active (0);
        }
    }

private:
  enum { nesting_indent_ = 3 };

  const ACE_TCHAR *prefix_;
  const ACE_TCHAR *name_;
  const ACE_TCHAR *file_;
  int line_;
};
// Listing 1

// Listing 2 code/ch03
#define TRACE_PREFIX       ACE_TEXT ("TRACE ")

#if defined (TRACE)
#undef TRACE
#endif /* TRACE */

#if (ACE_NTRACE == 1)
#    define TRACE(X)
#    define TRACE_RETURN(V)     return V;
#    define TRACE_RETURN_VOID()
#else
#    define TRACE(X)                           \
            Trace ____ (TRACE_PREFIX,          \
                        ACE_TEXT (X),          \
                        __LINE__,              \
                        ACE_TEXT (__FILE__))

#    define TRACE_RETURN(V)                    \
            do { ____.setLine(__LINE__); return V; } while (0)

#    define TRACE_RETURN_VOID()                \
            do { ____.setLine(__LINE__); } while (0)
#endif
// Listing 2

//////////////////////////////////////////////////

// Listing 3 code/ch03

#define DEBUG_PREFIX       ACE_TEXT ("DEBUG%I")
#define INFO_PREFIX        ACE_TEXT ("INFO%I")
#define NOTICE_PREFIX      ACE_TEXT ("NOTICE%I")
#define WARNING_PREFIX     ACE_TEXT ("WARNING%I")
#define ERROR_PREFIX       ACE_TEXT ("ERROR%I")
#define CRITICAL_PREFIX    ACE_TEXT ("CRITICAL%I")
#define ALERT_PREFIX       ACE_TEXT ("ALERT%I")
#define EMERGENCY_PREFIX   ACE_TEXT ("EMERGENCY%I")

#define MY_DEBUG(X)     \
        ACE_DEBUG(( LM_DEBUG,  \
                    DEBUG_PREFIX \
                    X))
#define MY_INFO(X)     \
        ACE_DEBUG(( LM_INFO,  \
                    INFO_PREFIX \
                    X))
#define MY_NOTICE(X)     \
        ACE_DEBUG(( LM_NOTICE,  \
                    NOTICE_PREFIX \
                    X))
#define MY_WARNING(X)     \
        ACE_DEBUG(( LM_WARNING,  \
                    WARNING_PREFIX \
                    X))
#define MY_ERROR(X)     \
        ACE_DEBUG(( LM_ERROR,  \
                    ERROR_PREFIX \
                    X))
#define MY_CRITICAL(X)     \
        ACE_DEBUG(( LM_CRITICAL,  \
                    CRITICAL_PREFIX \
                    X))
#define MY_ALERT(X)     \
        ACE_DEBUG(( LM_ALERT,  \
                    ALERT_PREFIX \
                    X))
#define MY_EMERGENCY(X)     \
        ACE_DEBUG(( LM_EMERGENCY,  \
                    EMERGENCY_PREFIX \
                    X))
// Listing 3

#endif /* TRACE_H */
