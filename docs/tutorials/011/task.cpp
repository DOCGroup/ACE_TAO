
// $Id$

#include "task.h"
#include "block.h"
#include "data.h"

Task::Task (void)
: barrier_ (0)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task ctor 0x%x\n", (void *) this));
}

Task::~Task (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task dtor 0x%x\n", (void *) this));

  ACE_Message_Block *message;
  this->getq (message);
  message->release ();

  delete barrier_;
}

int Task::start (int threads)
{
  barrier_ = new ACE_Barrier (threads);
  return this->activate (THR_NEW_LWP, threads);
}
int Task::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task close 0x%x\n", (void *) this));
  return inherited::close (flags);
}

int Task::svc (void)
{
  this->barrier_->wait ();

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Task 0x%x starts in thread %d\n", (void *) this, ACE_Thread::self ()));

  ACE_Message_Block *message;
  while (1)
  {
    if (this->getq (message) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getq"), -1);
    }

    if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
    {
      this->putq (message);

      break;
    }

    const char *cp = message->rd_ptr ();
    message->rd_ptr (strlen (cp) + 1);  // Don't forget to skip the NULL we
                                        // inserted

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Block 0x%x contains (%s)\n", (void *) message, cp));

    /*
       Create a Data object into which we can extract the message block
       contents. 
     */
    Data data;
    /*
       Use the rd_ptr() to access the message block data.  Note that we've
       already moved it past the text string in the block. 
     */
    ACE_OS::memmove ((char *) &data, message->rd_ptr (), sizeof (data));
    message->rd_ptr (sizeof (data));  // Move the rd_ptr() beyond the data.

    /*
       Invoke a couple of method calls on the object we constructed. 
     */
    data.who_am_i ();
    data.what_am_i ();

    /*
       An alternate approach:

       Data * data;
       data = (Data *)message->rd_ptr();
       data->who_am_i();
       data->what_am_i();
       message->rd_ptr(sizeof(Data));

       Even though this cuts down on the number of copies & constructions, I'm
       not real fond of it.  You can get into trouble in a hurry by treating
       memory blocks as multiple data types... 
     */


    ACE_OS::sleep (ACE_Time_Value (0, 5000));

    message->release ();
  }

  return (0);
}
