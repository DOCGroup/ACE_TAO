/* -*- C++ -*- */
#ifndef _EVENT_ANALYZER_H
#define _EVENT_ANALYZER_H

#include "ace/Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Module.h"
#include "ace/Task.h"

class Event_Analyzer : public ACE_Task<ACE_SYNCH>
{
  // = TITLE
  //     This class forwards all the <ACE_Message_Block>s it receives
  //     onto its neighboring Module in the Stream.
  //
  // = DESCRIPTION
  //     In a "real" event service, application-specific processing
  //     would be done in the <put> (or <svc>) method in this class.
public:
  int open (void *a = nullptr) override;
  int close (u_long flags = 0) override;

  int put (ACE_Message_Block *msg,
           ACE_Time_Value * = nullptr) override;
  // Entry point into this task.

  // Dynamic linking hooks (not used).
  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;
  int info (ACE_TCHAR **info_string,
            size_t length) const override;
private:
  virtual int control (ACE_Message_Block *);
  // Implements the watermark control processing.
};

#endif /* _EVENT_ANALYZER_H */
