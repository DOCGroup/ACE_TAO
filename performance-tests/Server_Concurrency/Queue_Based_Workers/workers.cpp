// $Id$

#include "ace/Task.h"
#include "ace/Get_Opt.h"

static size_t number_of_messages = 100;
static size_t message_size = 100;
static size_t number_of_workers = 10;
static size_t burst_size = 10;
static size_t timeout_between_bursts = 1;
static size_t debug = 0;

typedef ACE_Task<ACE_MT_SYNCH> TASK;

class Worker_Task : public TASK
{
public:
  Worker_Task (ACE_Message_Queue<ACE_MT_SYNCH> *mq);
  int svc (void);

  size_t messages_dequeued_;

};

class IO_Task : public TASK
{
public:
  IO_Task (ACE_Message_Queue<ACE_MT_SYNCH> *mq);
  int svc (void);
};

Worker_Task::Worker_Task (ACE_Message_Queue<ACE_MT_SYNCH> *mq)
  : TASK (0, mq),
    messages_dequeued_ (0)
{
}

int
Worker_Task::svc (void)
{
  for (;;)
    {
      ACE_Message_Block *message_block = 0;
      int result = this->getq (message_block);
      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Worker_Task::svc (%t) -> %p\n",
                             "getq error"),
                            -1);
        }

      ACE_Message_Block::ACE_Message_Type message_type =
        message_block->msg_type ();

      message_block->release ();

      // If STOP message, break loop and end the task.
      if (message_type == ACE_Message_Block::MB_STOP)
        {
          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) stop message dequeued after %d data messages\n",
                          this->messages_dequeued_));
            }

          break;
        }
      else
        {
          // Counter.
          ++this->messages_dequeued_;

          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) dequeued its %d message\n",
                          this->messages_dequeued_));
            }
        }
    }

  return 0;
}

IO_Task::IO_Task (ACE_Message_Queue<ACE_MT_SYNCH> *mq)
  : TASK (0, mq)
{
}

int
IO_Task::svc (void)
{
  size_t i = 0;
  size_t messages_queued = 1;
  size_t burst = 1;

  // Data messages.
  while (number_of_messages > 0)
    {
      for (i = 1;
           i <= burst_size && number_of_messages > 0;
           ++i, --number_of_messages, ++messages_queued)
        {
          ACE_Message_Block *message_block = 0;
          ACE_NEW_RETURN (message_block,
                          ACE_Message_Block (message_size),
                          -1);

          int result = this->putq (message_block);
          if (result == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "IO::svc (%t) -> %p\n",
                                 "putq error"),
                                -1);
            }

          if (debug)
            {
              ACE_DEBUG ((LM_DEBUG,
                          "(%t) IO thread -> burst %d: message %d; overall message %d\n",
                          burst,
                          i,
                          messages_queued));
            }
        }

      ++burst;
      ACE_OS::sleep (timeout_between_bursts);
    }

  // Terminate messages.
  for (i = 0; i < number_of_workers; ++i)
    {
      ACE_Message_Block *message_block = 0;
      ACE_NEW_RETURN (message_block,
                      ACE_Message_Block (0,
                                         ACE_Message_Block::MB_STOP),
                      -1);

      int result = this->putq (message_block);
      if (result == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "IO::svc (%t) -> %p\n",
                             "putq error"),
                            -1);
        }
    }

  return 0;
}

static int
parse_args (int argc, ASYS_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ASYS_TEXT ("m:s:w:b:t:d"));
  int c;

  while ((c = get_opt ()) != -1)
    {
      switch (c)
        {
        case 'm':
          number_of_messages = ACE_OS::atoi (get_opt.optarg);
          break;
        case 's':
          message_size = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'w':
          number_of_workers = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'b':
          burst_size = ACE_OS::atoi (get_opt.optarg);
          break;
        case 't':
          timeout_between_bursts = ACE_OS::atoi (get_opt.optarg);
          break;
        case 'd':
          debug = 1;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "usage: %s\n"
                             "\t[-m number of messages]\n"
                             "\t[-s message size]\n"
                             "\t[-w number of workers]\n"
                             "\t[-b burst size]\n"
                             "\t[-t timeout between bursts]\n"
                             "\t[-d debug]\n",
                             argv[0]),
                            -1);
        }
    }

  return 0;
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  int result = parse_args (argc, argv);
  if (result != 0)
    {
      return result;
    }

  ACE_Message_Queue<ACE_MT_SYNCH> message_queue;

  // Workers.
  Worker_Task **workers = 0;
  ACE_NEW_RETURN (workers,
                  Worker_Task *[number_of_workers],
                  -1);

  // Create and activate them.
  for (size_t i = 0; i < number_of_workers; ++i)
    {
      ACE_NEW_RETURN (workers[i],
                      Worker_Task (&message_queue),
                      -1);

      // Activate the workers.
      result = workers[i]->activate (THR_BOUND);
      if (result != 0)
        {
          return result;
        }
    }

  // IO Task.
  IO_Task io (&message_queue);

  // Activate the workers.
  result = io.activate (THR_BOUND);
  if (result != 0)
    {
      return result;
    }

  // Wait for all threads to terminate.
  result = ACE_Thread_Manager::instance ()->wait ();

  delete[] workers;

  return result;
}
