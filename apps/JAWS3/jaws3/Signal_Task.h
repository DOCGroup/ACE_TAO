/* -*- c++ -*- */
// $Id$

#ifndef JAWS_SIGNAL_TASK_H
#define JAWS_SIGNAL_TASK_H

#include "ace/Synch.h"
#include "ace/Signal.h"
#include "ace/Singleton.h"

#include "jaws3/Export.h"

class JAWS_Export JAWS_Signal_Task
{
public:

  JAWS_Signal_Task (void);

  static JAWS_Signal_Task * instance (void)
  {
    return ACE_Singleton<JAWS_Signal_Task, ACE_SYNCH_MUTEX>::instance ();
  }

  ACE_Sig_Set & sigset (void) { return this->sigset_; }

private:

  ACE_Sig_Set sigset_;

};

#endif /* JAWS_SIGNAL_TASK_H */
