/* -*- C++ -*- */
// $Id$

// Signal router.

#ifndef _EVENT_ANALYZER_H
#define _EVENT_ANALYZER_H

#include "ace/Stream.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/Synch.h"

#if defined (ACE_HAS_THREADS)

class Event_Analyzer : public ACE_Task<ACE_MT_SYNCH>
{
public:
  virtual int open (void *a = 0);
  virtual int close (u_long flags = 0);
  virtual int put (ACE_Message_Block *msg, ACE_Time_Value * = 0);

  // Dynamic linking hooks.
  virtual int init (int argc, char *argv[]);
  virtual int fini (void);
  virtual int info (char **info_string, size_t length) const;

private:
  virtual int control (ACE_Message_Block *);
};

#endif /* ACE_HAS_THREADS */
#endif /* _EVENT_ANALYZER_H */
