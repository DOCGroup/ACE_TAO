/* -*- c++ -*- */
#ifndef JAWS_SIGNAL_TASK_H
#define JAWS_SIGNAL_TASK_H

#include "ace/Synch.h"
#include "ace/Signal.h"
#include "ace/Singleton.h"

#include "jaws3/Export.h"

class JAWS_Export JAWS_Signal_Task
{
public:
  JAWS_Signal_Task ();

  static JAWS_Signal_Task * instance ()
  {
    return ACE_Singleton<JAWS_Signal_Task, ACE_SYNCH_MUTEX>::instance ();
  }

  ACE_Sig_Set & sigset () { return this->sigset_; }

private:
  ACE_Sig_Set sigset_;
};

#endif /* JAWS_SIGNAL_TASK_H */
