// $Id$

#include "JAWS/JAWS.h"

#include "JAWS/Pipeline_Tasks.h"
#include "JAWS/Data_Block.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

JAWS_Pipeline_Handler::~JAWS_Pipeline_Handler (void)
{
}

int
JAWS_Pipeline_Handler::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  JAWS_Data_Block *db = ACE_dynamic_cast (JAWS_Data_Block *, mb->data_block ());

  int status = this->handle_put (db, tv);

  if (status != -1)
    {
      JAWS_Pipeline_Handler *task = db->io_handler ()->task ();
      JAWS_Pipeline_Handler *next
        = ACE_dynamic_cast (JAWS_Pipeline_Handler *, task->next ());

      db->io_handler ()->task (next);
    }

  return status;
}

int
JAWS_Pipeline_Accept_Task::handle_put (JAWS_Data_Block *data,
                                       ACE_Time_Value *)
{
  int result = -1;

  // JAWS_Data_Block should contain an INET_Addr and an IO
  JAWS_IO_Handler *handler = data->io_handler ();
  JAWS_Dispatch_Policy *policy = data->policy ();
  JAWS_Pipeline_Handler *task = handler->task ();

  // data->policy ()->update (handler);

  JAWS_IO *io = policy->io ();
  io->accept (handler);

  // When accept returns, the resulting handle should be stored into
  // the JAWS_DATA_BLOCK somewhere.

  // Check the handler for status of the io call
  switch (handler->status ())
    {
    case JAWS_IO_Handler::ACCEPT_OK:
      {
      result = 0;
      JAWS_TRACE ("JAWS_Pipeline_Accept_Task::handle_put ACCEPT_OK");
      // At this point need to move to the next task in the pipeline!
      // The framework should automatically call the next stage.
      data->task (task);
      break;
      }
    case JAWS_IO_Handler::ACCEPT_ERROR:
      {
      result = -1;
      JAWS_TRACE ("JAWS_Pipeline_Accept_Task::handle_put ACCEPT_ERROR");
      // Should recycle the thread
      break;
      }
    default:
      {
      result = 1;
      JAWS_TRACE ("JAWS_Pipeline_Accept_Task::handle_put ACCEPT_IDLE");
      // Should mean that the IO is asynchronous, and the word isn't out
      // yet.
      break;
      }
    }

  // In asynchronous and synchronous models, we can --
  //   have the io_handler set the new task in the data_block

  // In asynchronous model, we can --
  //   insert a wait task into the task queue

  return result;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>;
template class ACE_Singleton<JAWS_Pipeline_Accept_Task, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>
#pragma instantiate ACE_Singleton<JAWS_Pipeline_Accept_Task, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
