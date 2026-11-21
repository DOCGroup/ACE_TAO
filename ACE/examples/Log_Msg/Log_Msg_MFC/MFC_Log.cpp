// MFC_Log.cpp: implementation of the MFC_Log class.
//
//////////////////////////////////////////////////////////////////////

#include "ace/OS_NS_stdio.h"
#include "MFC_Log.h"

#if defined (ACE_WIN32)
#  include "stdafx.h"
#endif /* ACE_WIN32 */

#include "ace/Log_Record.h"
#include "ace/Time_Value.h"

void
MFC_Log::log (ACE_Log_Record& log_record)
{
  ACE_TCHAR Message[512] {};

  ACE_OS::snprintf (Message,
                    sizeof Message / sizeof *Message,
                    ACE_INT64_FORMAT_SPECIFIER ACE_TEXT (".%03ld %s"),
                    static_cast<ACE_INT64> (log_record.time_stamp ().sec ()),
                    static_cast<long> (log_record.time_stamp ().usec () / 1000),
                    log_record.msg_data ());

#if defined (ACE_WIN32)
  ACE_TEXT_OutputDebugString (Message);
#endif /* ACE_WIN32 */
}
