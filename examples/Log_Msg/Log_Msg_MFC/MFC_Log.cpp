// $Id$
// MFC_Log.cpp: implementation of the MFC_Log class.
//
//////////////////////////////////////////////////////////////////////

#include "MFC_Log.h"

#if defined (ACE_WIN32)
#  include "stdafx.h"
#endif /* ACE_WIN32 */

#include "ace/Log_Record.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MFC_Log::MFC_Log()
{

}

void
MFC_Log::log(ACE_Log_Record& log_record)
{
  unsigned long priority = log_record.type();
  char Message[512];

  ACE_OS::sprintf(Message,"%d.%03ld %s", log_record.time_stamp().sec(),
  log_record.time_stamp().usec()/ 1000, log_record.msg_data());

#if defined (ACE_WIN32)
  ::OutputDebugString(Message);
#endif /* ACE_WIN32 */
}

MFC_Log::~MFC_Log()
{

}
