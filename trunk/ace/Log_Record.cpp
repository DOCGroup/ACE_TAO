// Log_Record.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Log_Record.h"
#include "ace/streams.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Log_Record.i"
#endif

ACE_RCSID(ace, Log_Record, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_Log_Record)

const ASYS_TCHAR *ACE_Log_Record::priority_names_[] =
{
  ASYS_TEXT ("<none>"),
  ASYS_TEXT ("LM_SHUTDOWN"),
  ASYS_TEXT ("LM_TRACE"),
  ASYS_TEXT ("LM_DEBUG"),
  ASYS_TEXT ("LM_INFO"),
  ASYS_TEXT ("LM_NOTICE"),
  ASYS_TEXT ("LM_WARNING"),
  ASYS_TEXT ("LM_STARTUP"),
  ASYS_TEXT ("LM_ERROR"),
  ASYS_TEXT ("LM_CRITICAL"),
  ASYS_TEXT ("LM_ALERT"),
  ASYS_TEXT ("LM_EMERGENCY")
};

const ASYS_TCHAR *
ACE_Log_Record::priority_name (ACE_Log_Priority p)
{
  return ACE_Log_Record::priority_names_[ACE::log2 (p)];
}

u_long
ACE_Log_Record::priority (void) const
{
  ACE_TRACE ("ACE_Log_Record::priority");
  return ACE::log2 (this->type_);
}

void
ACE_Log_Record::priority (u_long p)
{
  ACE_TRACE ("ACE_Log_Record::priority");
  this->type_ = p;
}

void
ACE_Log_Record::dump (void) const
{
  // ACE_TRACE ("ACE_Log_Record::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("length_ = %d\n"), this->length_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ntype_ = %d\n"), this->type_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ntime_stamp_ = (%d, %d)\n"), this->time_stamp_.sec (), this->time_stamp_.usec ()));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\npid_ = %d\n"), this->pid_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\nmsg_data_ = %s\n"), this->msg_data_));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

void
ACE_Log_Record::msg_data (const ASYS_TCHAR *data)
{
  // ACE_TRACE ("ACE_Log_Record::msg_data");
  ACE_OS::strncpy (this->msg_data_, data, ACE_Log_Record::MAXLOGMSGLEN);
  this->round_up ();
}

ACE_Log_Record::ACE_Log_Record (ACE_Log_Priority lp,
                                long ts_sec,
                                long p)
  : length_ (0),
    type_ (long (lp)),
    time_stamp_ (ts_sec),
    pid_ (p)
{
  // ACE_TRACE ("ACE_Log_Record::ACE_Log_Record");
}

ACE_Log_Record::ACE_Log_Record (ACE_Log_Priority lp,
                                const ACE_Time_Value &ts,
                                long p)
  : length_ (0),
    type_ (long (lp)),
    time_stamp_ (ts),
    pid_ (p)
{
  // ACE_TRACE ("ACE_Log_Record::ACE_Log_Record");
}

void
ACE_Log_Record::round_up (void)
{
  // ACE_TRACE ("ACE_Log_Record::round_up");
  // Determine the length of the payload.
  int len = (sizeof *this - MAXLOGMSGLEN)
    + (ACE_OS::strlen (this->msg_data_) + 1);

  // Round up to the alignment.
  this->length_ = 1 + ((len + ACE_Log_Record::ALIGN_WORDB - 1)
                       & ~(ACE_Log_Record::ALIGN_WORDB - 1));
}

ACE_Log_Record::ACE_Log_Record (void)
  : length_ (0),
    type_ (0),
    time_stamp_ (0L),
    pid_ (0)
{
  // ACE_TRACE ("ACE_Log_Record::ACE_Log_Record");
}

int
ACE_Log_Record::format_msg (const ASYS_TCHAR *host_name,
                            u_long verbose_flag,
                            ASYS_TCHAR *verbose_msg)
{
  if (ACE_BIT_ENABLED (verbose_flag, ACE_Log_Msg::VERBOSE))
    {
      time_t now = this->time_stamp_.sec ();
      ASYS_TCHAR ctp[26]; // 26 is a magic number...

      if (ACE_OS::ctime_r (&now, ctp, sizeof ctp) == 0)
        return -1;

      /* 01234567890123456789012345 */
      /* Wed Oct 18 14:25:36 1989n0 */

      ctp[19] = '\0'; // NUL-terminate after the time.
      ctp[24] = '\0'; // NUL-terminate after the date.

# if defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS)
      const ASYS_TCHAR *lhost_name =  (const ASYS_TCHAR *) ((host_name == 0)
        ? ((char *) ASYS_TEXT ("<local_host>")) : ((char *) host_name));
# else /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */
      const ASYS_TCHAR *lhost_name = host_name ==
        0 ? ASYS_TEXT ("<local_host>") : host_name;
# endif /* ! defined (ACE_HAS_BROKEN_CONDITIONAL_STRING_CASTS) */

      ACE_OS::sprintf (verbose_msg,
                       ASYS_TEXT ("%s.%03d %s@%s@%d@%s@%s"),
                       ctp + 4,
                       this->time_stamp_.usec () / 1000,
                       ctp + 20,
                       lhost_name,
                       this->pid_,
                       ACE_Log_Record::priority_name (ACE_Log_Priority (this->type_)),
                       this->msg_data_);
    }
  else if (ACE_BIT_ENABLED (verbose_flag, ACE_Log_Msg::VERBOSE_LITE))
    {
      ACE_OS::sprintf (verbose_msg,
                       ASYS_TEXT ("%s@%s"),
                       ACE_Log_Record::priority_name (ACE_Log_Priority (this->type_)),
                       this->msg_data_);
    }
  else
    {
      ACE_OS::sprintf (verbose_msg,
                       ASYS_TEXT ("%s"),
                       this->msg_data_);
    }

  return 0;
}

#if defined (ACE_HAS_WINCE)

int
ACE_Log_Record::print (const ASYS_TCHAR *host_name,
                       u_long verbose_flag,
                       ACE_CE_Bridge *log_window)
{
  ASYS_TCHAR verbose_msg [MAXVERBOSELOGMSGLEN];
  int result = this->format_msg (host_name, verbose_flag, verbose_msg);

  if (result == 0)
    {
      if (log_window == 0)
        log_window = ACE_CE_Bridge::get_default_winbridge ();

      // <verbose_cstring> will be deleted by <write_msg> function
      log_window->write_msg (verbose_msg);
    }

  return result;
}

#endif /* defined (ACE_HAS_WINCE) */

int
ACE_Log_Record::print (const ASYS_TCHAR *host_name,
                       u_long verbose_flag,
                       FILE *fp)
{
  ASYS_TCHAR verbose_msg [MAXVERBOSELOGMSGLEN];
  int result = this->format_msg (host_name, verbose_flag, verbose_msg);

  if (result == 0)
    {
      if (fp != NULL)
        {
          size_t verbose_msg_len = ACE_OS::strlen (verbose_msg);
          size_t fwrite_result = ACE_OS::fwrite (verbose_msg,
                                                 1,
                                                 verbose_msg_len,
                                                 fp);

          // We should have written everything
          if (fwrite_result != verbose_msg_len)
            {
              result = -1;
            }
          else
            {
              ACE_OS::fflush (fp);
            }
        }
    }

  return result;
}

#if ! defined (ACE_LACKS_IOSTREAM_TOTALLY)

int
ACE_Log_Record::print (const ASYS_TCHAR *host_name,
                       u_long verbose_flag,
                       ostream &s)
{
  ASYS_TCHAR verbose_msg [MAXVERBOSELOGMSGLEN];
  int result = this->format_msg (host_name, verbose_flag, verbose_msg);

  if (result == 0)
    {
      s << verbose_msg;
      s.flush ();
    }

  return result;
}

#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */
