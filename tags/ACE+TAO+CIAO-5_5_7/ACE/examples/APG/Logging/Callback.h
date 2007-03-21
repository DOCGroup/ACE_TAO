// $Id$

#ifndef APG_CALLBACK_H
#define APG_CALLBACK_H

#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"

class Callback : public ACE_Log_Msg_Callback
{
public:
  void log (ACE_Log_Record &log_record) {
#  if defined (ACE_LACKS_IOSTREAM_TOTALLY)
    log_record.print (ACE_TEXT (""), 0, stderr);
    log_record.print (ACE_TEXT (""), ACE_Log_Msg::VERBOSE, stderr);
#  else
    log_record.print (ACE_TEXT (""), 0, cerr);
    log_record.print (ACE_TEXT (""), ACE_Log_Msg::VERBOSE, cerr);
#  endif /* ACE_LACKS_IOSTREAM_TOTALLY */
  }
};

#endif /* APG_CALLBACK_H */
