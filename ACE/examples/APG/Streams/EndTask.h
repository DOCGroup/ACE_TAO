/* -*- C++ -*- */
#ifndef END_TASK_H
#define END_TASK_H

// Listing 1 code/ch18
class TheEndTask : public BasicTask {
protected:
  int process (Message *) override
  {
    ACE_TRACE ("EndTask::process()");
    return 0;
  }

  int next_step (ACE_Message_Block *mb) override
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TheEndTask::next_step() - ")
                ACE_TEXT ("end of the line.\n")));
    mb->release ();
    return 0;
  }
};
// Listing 1

#endif /* END_TASK_H */
