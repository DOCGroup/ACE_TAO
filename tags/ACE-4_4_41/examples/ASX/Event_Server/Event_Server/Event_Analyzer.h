/* -*- C++ -*- */
// $Id$

#if !defined (_EVENT_ANALYZER_H)
#define _EVENT_ANALYZER_H     

#include "ace/Stream.h"
#include "ace/Module.h"
#include "ace/Task.h"
#include "ace/Synch.h"

class Event_Analyzer : public ACE_Task<ACE_SYNCH>
  // = TITLE
  //     This is a "no-op" class that just forwards all its message
  //     blocks onto its neighboring Module in the Stream.  In a real
  //     application these tasks would be where the Stream processing
  //     would go.
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

#endif /* _EVENT_ANALYZER_H */
