/* -*- C++ -*- */
// $Id$

#ifndef END_TASK_H
#define END_TASK_H

// Listing 1 code/ch18
class EndTask : public BasicTask {
protected:
  virtual int process (Message *)
  {
    ACE_TRACE (ACE_TEXT ("EndTask::process()"));
    return 0;
  }

  virtual int next_step (ACE_Message_Block *mb)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("EndTask::next_step() - ")
                ACE_TEXT ("end of the line.\n")));
    mb->release ();
    return 0;
  }
};
// Listing 1

#endif /* END_TASK_H */
