// $Id$

#include "task.h"
#include "block.h"
#include "data.h"

Task::Task (size_t n_threads)
  : barrier_ (n_threads),
    n_threads_ (n_threads)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task ctor 0x%x\n",
              (void *) this));
}

Task::~Task (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task dtor 0x%x\n",
              (void *) this));

  ACE_Message_Block *message;
  this->getq (message);
  message->release ();
}

int
Task::open (void *)
{
    return this->activate (THR_NEW_LWP,
                        this->n_threads_);

}

int
Task::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task close 0x%x\n",
              (void *) this));
  return inherited::close (flags);
}

int
Task::svc (void)
{
  this->barrier_.wait ();

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Task 0x%x starts in thread %d\n",
              (void *) this,
              ACE_Thread::self ()));

  ACE_Message_Block *message;

  for (;;)
    {
      if (this->getq (message) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "getq"),
                          -1);

      if (message->msg_type () == ACE_Message_Block::MB_HANGUP)
        {
          this->putq (message);

          break;
        }

      const char *cp = message->rd_ptr ();
      // Don't forget to skip the NULL we inserted
      message->rd_ptr (ACE_OS::strlen (cp) + 1);

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Block 0x%x contains (%s)\n",
                  (void *) message,
                  cp));

      /* Create a Data object into which we can extract the message
        block contents.  */
      Data data;

      /* Use the rd_ptr() to access the message block data.  Note that
        we've already moved it past the text string in the block.  */
      ACE_OS::memmove ((char *) &data,
                       message->rd_ptr (),
                       sizeof (data));
      message->rd_ptr (sizeof (data));  // Move the rd_ptr() beyond the data.

      /* Invoke a couple of method calls on the object we constructed.  */
      data.who_am_i ();
      data.what_am_i ();

      /* An alternate approach:

        Data * data;
        data = (Data *)message->rd_ptr();
        data->who_am_i();
        data->what_am_i();
        message->rd_ptr(sizeof(Data));

        Even though this cuts down on the number of copies &
        constructions, I'm not real fond of it.  You can get into
        trouble in a hurry by treating memory blocks as multiple data
        types...  */


      ACE_OS::sleep (ACE_Time_Value (0, 5000));

      message->release ();
    }

  return 0;
}
