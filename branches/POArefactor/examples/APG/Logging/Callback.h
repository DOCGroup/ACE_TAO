// $Id$

#include "ace/streams.h"
#include "ace/Log_Msg.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"

class Callback : public ACE_Log_Msg_Callback
{
public:
  void log (ACE_Log_Record &log_record) {
    log_record.print (ACE_TEXT (""), 0, cerr);
    log_record.print (ACE_TEXT (""), ACE_Log_Msg::VERBOSE, cerr);
  }
};
