// $Id$
// MFC_Log.cpp: implementation of the MFC_Log class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MFC_Log.h"

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
  ::OutputDebugString(Message);
}

MFC_Log::~MFC_Log()
{

}
