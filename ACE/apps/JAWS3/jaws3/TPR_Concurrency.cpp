#include "ace/Message_Block.h"

#ifndef JAWS_BUILD_DLL
#define JAWS_BUILD_DLL
#endif

#include "jaws3/Concurrency.h"
#include "jaws3/TPR_Concurrency.h"
#include "jaws3/Protocol_Handler.h"
#include "jaws3/Options.h"


JAWS_TPR_Concurrency::JAWS_TPR_Concurrency ()
  : shutdown_task_ (false)
  , error_ (false)
{
  int r;
  r = this->activate (THR_BOUND | THR_JOINABLE);
  if (r < 0)
    {
      this->error_ = true;
      this->shutdown_task_ = true;
    }
}

int
JAWS_TPR_Concurrency::putq (JAWS_Protocol_Handler *ph)
{
  if (this->error_)
    return -1;

  JAWS_CONCURRENCY_TASK *task = this;
  int result = task->putq (& ph->mb_);

  if (result != -1)
    {
      int r;
      r = this->activate ( THR_BOUND | THR_JOINABLE
                         , 1 // number of threads
                         , 1 // force active
                         );
      if (r < 0)
        {
          // ACE_ERROR
          return -1;
        }
    }

  return result;
}

int
JAWS_TPR_Concurrency::getq (JAWS_Protocol_Handler *&ph)
{
  ph = nullptr;

  JAWS_CONCURRENCY_TASK *task = this;

  if (this->shutdown_task_ && task->msg_queue ()->message_count () == 0)
    return -1;

  int const getting = ++(this->getting_);

  if (getting > 1 && task->msg_queue ()->message_count () == 0)
    {
      --(this->getting_);
      return -1;
    }

  ACE_Message_Block *mb = nullptr;
  int result = task->getq (mb);

  if (result != -1)
    {
      ph = (JAWS_Protocol_Handler *) mb->base ();

      if (ph == nullptr)
        {
          // Shutdown this task;
          this->shutdown_task_ = true;
        }
    }

  --(this->getting_);
  return result;
}
