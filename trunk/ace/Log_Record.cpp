// Log_Record.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Log_Record.h"
#include "ace/streams.h"

ACE_ALLOC_HOOK_DEFINE(ACE_Log_Record)

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

// Print out the record on the stderr stream with the appropriate
// format.

int
ACE_Log_Record::print (const ASYS_TCHAR *host_name,
		       int verbose, 
		       FILE *fp)
{
  // ACE_TRACE ("ACE_Log_Record::print");

  int ret;

  if (verbose)
    {
      time_t now = this->time_stamp_.sec ();
      ASYS_TCHAR ctp[26]; // 26 is a magic number...

      if (ACE_OS::ctime_r (&now, ctp, sizeof ctp) == 0)
	return -1;

      /* 01234567890123456789012345 */
      /* Wed Oct 18 14:25:36 1989n0 */

      ctp[19] = '\0'; // NUL-terminate after the time.
      ctp[24] = '\0'; // NUL-terminate after the date.

      const ASYS_TCHAR *lhost_name = host_name ==
        0 ? ASYS_TEXT ("<local_host>") : host_name;

      ret =  ACE_OS::fprintf (fp, ASYS_TEXT ("%s.%d %s@%s@%d@%d@%s"),
			      ctp + 4, 
			      this->time_stamp_.usec () / 1000,
			      ctp + 20, 
			      lhost_name, 
			      this->pid_,
			      this->type_, 
			      this->msg_data_);
    }
  else
    ret =  ACE_OS::fprintf (fp, ASYS_TEXT ("%s"), this->msg_data_);

  if (ret > 0)
    ACE_OS::fflush (fp);
  return ret;
}

#if !defined (ACE_HAS_WINCE)
int
ACE_Log_Record::print (const ASYS_TCHAR host_name[],
		       int verbose, 
		       ostream &s)
{
  // ACE_TRACE ("ACE_Log_Record::print");

  if (verbose)
    {
      time_t now = this->time_stamp_.sec ();
      ASYS_TCHAR ctp[26]; // 26 is a magic number...

      if (ACE_OS::ctime_r (&now, ctp, sizeof ctp) == 0)
	return -1;

      /* 01234567890123456789012345 */
      /* Wed Oct 18 14:25:36 1989n0 */

      ctp[19] = '\0'; // NUL-terminate after the time.
      ctp[24] = '\0'; // NUL-terminate after the date.

      const ASYS_TCHAR *lhost_name = host_name ==
        0 ? ASYS_TEXT ("<local_host>") : host_name;

      s << (ctp + 4) << '.'
	// The following line isn't portable, so I've commented it out...
	// << setw (3) << setfill ('0') << this->time_stamp_.usec () / 1000 << ' '
	<< this->time_stamp_.usec () / 1000 << ' '
	<< (ctp + 20)
	<< '@'
	<< lhost_name
	<< '@'
	<< this->pid_
	<< '@'
	<< this->type_
	<< '@';
    }

  s << this->msg_data_;
  s.flush ();
  return 0;
}
#endif /* ! ACE_HAS_WINCE */
