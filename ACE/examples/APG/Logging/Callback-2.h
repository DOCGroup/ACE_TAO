#ifndef APG_CALLBACK2_H
#define APG_CALLBACK2_H

#include "ace/OS_NS_time.h"
#include "ace/streams.h"
#include "ace/Log_Msg_Callback.h"
#include "ace/Log_Record.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"

class Callback : public ACE_Log_Msg_Callback
{
public:
  Callback () = default;
  Callback (Callback const &) = delete;
  Callback (Callback &&) = delete;
  Callback &operator= (Callback const &) = delete;
  Callback &operator= (Callback &&) = delete;

  void log (ACE_Log_Record &log_record) override
    {
      cerr << "Log Message Received:" << endl;
      unsigned long const msg_severity = log_record.type ();
      ACE_Log_Priority const prio =
        static_cast<ACE_Log_Priority> (msg_severity);
      ACE_TCHAR const *prio_name =
        ACE_Log_Record::priority_name (prio);
      cerr << "\tType:        "
           << ACE_TEXT_ALWAYS_CHAR (prio_name)
           << endl;

      cerr << "\tLength:      " << log_record.length () << endl;

      time_t const epoch = log_record.time_stamp ().sec ();
      cerr << "\tTime_Stamp:  "
           << ACE_TEXT_ALWAYS_CHAR (ACE_OS::ctime (&epoch))
           << flush;

      cerr << "\tPid:         " << log_record.pid () << endl;

      ACE_CString data (">> ");
      data += ACE_TEXT_ALWAYS_CHAR (log_record.msg_data ());

      cerr << "\tMsgData:     " << data.c_str () << endl;
    }
};

#endif /* APG_CALLBACK2_H */
