// $Id$

#include "ace/config-all.h"

#if !defined (ACE_WIN32) && !defined (ACE_LACKS_UNIX_SYSLOG)

#include "ace/ACE.h"
#include "ace/Logging/Log_Msg.h"
#include "ace/Logging/Log_Msg_UNIX_Syslog.h"
#include "ace/Logging/Log_Record.h"

// NOTE:
//   The ACE_Log_Msg_UNIX_Syslog class can use the openlog(),
//   setlogmask(), syslog() and closelog() routines in a thread safe
//   manner (versus using openlog_r(), et. al.), as the ACE_Log_Msg
//   class uses the lock provided by its local ACE_Log_Msg_Manager
//   class when calling the methods of the backend classes.  As a
//   result, logging semantics with respect to the UNIX syslog
//   facility, will be the same for all threads in a process.  This
//   should not be too limiting, as the ACE_Log_Msg class itself can
//   be used to control thread specific logging behavior.

ACE_Log_Msg_UNIX_Syslog::ACE_Log_Msg_UNIX_Syslog (void)
{
}

ACE_Log_Msg_UNIX_Syslog::~ACE_Log_Msg_UNIX_Syslog (void)
{
  (void) this->close ();
}

int
ACE_Log_Msg_UNIX_Syslog::open (const ACE_TCHAR * logger_key)
{
  if (logger_key == 0)
    logger_key = ACE_Log_Msg::program_name ();

  // Initialize the UNIX syslog facility.  Default the syslog log
  // options LOG_CONS and LOG_PID to be set.  There really should be a
  // logging strategy option to control the syslog log options,
  // however, we'll take the easy way out for now.
  openlog (ACE_const_cast (char *, logger_key),
           LOG_CONS|LOG_PID,
           ACE_DEFAULT_SYSLOG_FACILITY);

  // Enable logging of all syslog priorities.  If logging of all
  // priorities is not desired, use the ACE_Log_Msg::priority_mask()
  // method to control the log output sent to the syslog daemon via
  // the log() method, or use the system's syslog.conf file to select
  // desired level of information.

#if !defined (ACE_LACKS_SETLOGMASK)
  (void) setlogmask (LOG_UPTO (LOG_DEBUG));
#endif /* ACE_LACKS_SETLOGMASK */

  return 0;
}

int
ACE_Log_Msg_UNIX_Syslog::reset (void)
{
  return this->close ();
}

int
ACE_Log_Msg_UNIX_Syslog::close (void)
{
  closelog();
  return 0;
}

int
ACE_Log_Msg_UNIX_Syslog::log (ACE_Log_Record &log_record)
{
  int syslog_priority = this->convert_log_priority (log_record.type ());
  int flags = ACE_LOG_MSG->flags ();

  // The UNIX syslog() facility does not support multi-line messages.
  // Break up the message data into separate lines and send each line
  // to the syslog daemon.

  ACE_TCHAR message[ACE_Log_Record::MAXVERBOSELOGMSGLEN];
  ACE_OS::strcpy (message, log_record.msg_data ());

  for (ACE_TCHAR *line = ACE_OS::strtok (message, ACE_LIB_TEXT ("\n"));
       line != 0;
       line = ACE_OS::strtok (0, ACE_LIB_TEXT ("\n")))
    {
      // Format the message line.  Note that the processing for
      // VERBOSE is the same as for VERBOSE_LITE, since syslog()
      // already provides us with the hostname and PID.  However, the
      // timestamp is duplicated (albeit a shortened version) to
      // provide a timestamp with greater precision than that provided
      // by syslog().
      if (ACE_BIT_ENABLED (flags, ACE_Log_Msg::VERBOSE)
          || ACE_BIT_ENABLED (flags, ACE_Log_Msg::VERBOSE_LITE))
        {
          ACE_TCHAR date_and_time[35];
          syslog (syslog_priority,
                  ACE_LIB_TEXT ("%s: %s: %s"),
                  ACE::timestamp (date_and_time, sizeof (date_and_time), 1),
                  ACE_Log_Record::priority_name (ACE_Log_Priority(log_record.type ())),
                  line);
        }
      else // No formatting required.
        syslog (syslog_priority, ACE_LIB_TEXT ("%s"), line);
    }

  return 0;
}

int
ACE_Log_Msg_UNIX_Syslog::convert_log_priority (int lm_priority)
{
  int syslog_priority;
  switch (lm_priority)
  {
  case LM_TRACE:
  case LM_DEBUG:
    syslog_priority = LOG_DEBUG;
    break;
  case LM_STARTUP:
  case LM_SHUTDOWN:
  case LM_INFO:
    syslog_priority = LOG_INFO;
    break;
  case LM_NOTICE:
    syslog_priority = LOG_NOTICE;
    break;
  case LM_WARNING:
    syslog_priority = LOG_WARNING;
    break;
  case LM_CRITICAL:
    syslog_priority = LOG_CRIT;
    break;
  case LM_ALERT:
    syslog_priority = LOG_ALERT;
    break;
  case LM_EMERGENCY:
    syslog_priority = LOG_EMERG;
    break;
  case LM_ERROR:
  default:
    syslog_priority = LOG_ERR;
    break;
  }

  return syslog_priority;
}

int
ACE_Log_Msg_UNIX_Syslog::convert_log_mask (int lm_mask)
{
  int syslog_mask = 0;

  if (ACE_BIT_ENABLED (lm_mask, LM_TRACE)
      || ACE_BIT_ENABLED (lm_mask, LM_DEBUG))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_DEBUG));

  if (ACE_BIT_ENABLED (lm_mask, LM_STARTUP)
      || ACE_BIT_ENABLED (lm_mask, LM_SHUTDOWN)
      || ACE_BIT_ENABLED (lm_mask, LM_INFO))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_INFO));

  if (ACE_BIT_ENABLED (lm_mask, LM_NOTICE))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_NOTICE));

  if (ACE_BIT_ENABLED (lm_mask, LM_ERROR))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_ERR));

  if (ACE_BIT_ENABLED (lm_mask, LM_WARNING))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_WARNING));

  if (ACE_BIT_ENABLED (lm_mask, LM_CRITICAL))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_CRIT));

  if (ACE_BIT_ENABLED (lm_mask, LM_ALERT))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_ALERT));

  if (ACE_BIT_ENABLED (lm_mask, LM_EMERGENCY))
    ACE_SET_BITS (syslog_mask, LOG_MASK(LOG_EMERG));

  return syslog_mask;
}

#endif /* !defined (ACE_WIN32) && !defined (ACE_LACKS_UNIX_SYSLOG) */
