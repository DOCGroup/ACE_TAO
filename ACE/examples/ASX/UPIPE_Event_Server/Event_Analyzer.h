/* -*- C++ -*- */
// Signal router.

#ifndef _EVENT_ANALYZER_H
#define _EVENT_ANALYZER_H

#include "ace/Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Module.h"
#include "ace/Task.h"

#if defined (ACE_HAS_THREADS)

class Event_Analyzer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Event_Analyzer () = default;
  Event_Analyzer (Event_Analyzer const &) = delete;
  Event_Analyzer (Event_Analyzer &&) = delete;
  Event_Analyzer &operator= (Event_Analyzer const &) = delete;
  Event_Analyzer &operator= (Event_Analyzer &&) = delete;

  int open (void *a = nullptr) override;
  int close (u_long flags = 0) override;
  int put (ACE_Message_Block *msg, ACE_Time_Value * = nullptr) override;

  // Dynamic linking hooks.
  int init (int argc, ACE_TCHAR *argv[]) override;
  int fini () override;
  int info (ACE_TCHAR **info_string, size_t length) const override;

private:
  virtual int control (ACE_Message_Block *);
};

#endif /* ACE_HAS_THREADS */
#endif /* _EVENT_ANALYZER_H */
