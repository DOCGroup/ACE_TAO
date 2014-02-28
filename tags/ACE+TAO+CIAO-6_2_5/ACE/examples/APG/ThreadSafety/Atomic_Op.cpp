// $Id$

#include "ace/Synch.h"
#include "ace/Task.h"
#include "ace/Log_Msg.h"
#include "ace/Atomic_Op.h"

#if defined(RUNNING_ON_UNSAFE_MULTIPROCESSOR)
// Listing 1 code/ch14
typedef ACE_Atomic_Op<ACE_Thread_Mutex, unsigned int> SafeUInt;
// Listing 1
// Listing 2 code/ch14
typedef ACE_Atomic_Op<ACE_Thread_Mutex, int> SafeInt;
// Listing 2
#else
typedef ACE_Atomic_Op<ACE_Null_Mutex, unsigned int> SafeUInt;
typedef ACE_Atomic_Op<ACE_Null_Mutex, int> SafeInt;
#endif /* RUNNING_ON_UNSAFE_MULTIPROCESSOR) */

static const unsigned int Q_SIZE = 2;
static const int MAX_PROD = 10;

// Listing 3 code/ch14
class Producer : public ACE_Task_Base
{
public:
  Producer (int *buf, SafeUInt &in, SafeUInt &out)
    : buf_(buf), in_(in), out_(out)
  { }

  int svc (void)
  {
    SafeInt itemNo = 0;
    while (1)
      {
        // Busy wait.
        do
          { }
        while (in_.value () - out_.value () == Q_SIZE);

        itemNo++;
        buf_[in_.value () % Q_SIZE] = itemNo.value ();
        in_++;

        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Produced %d\n"),
                    itemNo.value ()));

        if (check_termination (itemNo.value ()))
          break;
      }

    return 0;
  }

  int check_termination (int item)
  {
    return (item == MAX_PROD);
  }

private:
  int * buf_;
  SafeUInt& in_;
  SafeUInt& out_;
};

class Consumer : public ACE_Task_Base
{
public:
  Consumer (int *buf, SafeUInt &in, SafeUInt& out)
    : buf_(buf), in_(in), out_(out)
  { }

  int svc (void)
  {
    while (1)
      {
        int item;

        // Busy wait.
        do
          { }
        while (in_.value () - out_.value () == 0);

        item = buf_[out_.value () % Q_SIZE];
        out_++;

        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Consumed %d\n"),
                    item));

        if (check_termination (item))
          break;
      }

    return 0;
  }

  int check_termination (int item)
  {
    return (item == MAX_PROD);
  }

private:
  int * buf_;
  SafeUInt& in_;
  SafeUInt& out_;
};
// Listing 3

// Listing 4 code/ch14
int ACE_TMAIN (int, ACE_TCHAR *[])
{
  int shared_buf[Q_SIZE];
  SafeUInt in = 0;
  SafeUInt out = 0;

  Producer producer (shared_buf, in, out);
  Consumer consumer (shared_buf, in, out);

  producer.activate();
  consumer.activate();
  producer.wait();
  consumer.wait();

  return 0;
}
// Listing 4

