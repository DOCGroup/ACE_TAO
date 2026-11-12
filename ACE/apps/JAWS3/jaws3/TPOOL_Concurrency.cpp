#include "ace/Message_Block.h"
#include <cstdlib>

#ifndef JAWS_BUILD_DLL
#define JAWS_BUILD_DLL
#endif

#include "jaws3/TPOOL_Concurrency.h"
#include "jaws3/Protocol_Handler.h"
#include "jaws3/Options.h"

JAWS_TPOOL_Concurrency::JAWS_TPOOL_Concurrency ()
  : number_of_threads_ (5)
  , shutdown_task_ (false)
  , error_ (false)
{
  char const *value = JAWS_Options::instance ()->getenv ("JAWS_TPOOL_THREADS");
  if (value != nullptr)
    this->number_of_threads_ = std::atoi (value);
  else
    this->number_of_threads_ = std::atoi (JAWS_DEFAULT_TPOOL_THREADS);

  if (this->number_of_threads_ <= 0)
    this->number_of_threads_ = 5;

  int const r = this->activate ( THR_BOUND | THR_JOINABLE,
                                 this->number_of_threads_);

  if (r < 0)
    {
      // ACE_ERROR
      this->error_ = true;
      this->shutdown_task_ = true;
    }
}

int
JAWS_TPOOL_Concurrency::putq (JAWS_Protocol_Handler *ph)
{
  if (this->error_)
    return -1;

  JAWS_CONCURRENCY_TASK *task = this;
  return task->putq (& ph->mb_);
}

int
JAWS_TPOOL_Concurrency::getq (JAWS_Protocol_Handler *&ph)
{
  ph = nullptr;

  JAWS_CONCURRENCY_TASK *task = this;

  if (this->shutdown_task_ && task->msg_queue ()->message_count () == 0)
    return -1;

  ACE_Message_Block *mb {};

  int result = task->getq (mb);

  if (result != -1)
    {
      ph = reinterpret_cast<JAWS_Protocol_Handler *> (mb->base ());

      if (ph == nullptr)
        {
          // Shutdown this task;
          this->shutdown_task_ = true;
          if (this->number_of_threads_ && this->number_of_threads_-- > 1)
            {
              task->putq (mb);
              result = -1;
            }
        }
    }

  return result;
}
