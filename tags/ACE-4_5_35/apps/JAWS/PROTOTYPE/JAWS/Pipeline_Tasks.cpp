// $Id$

#include "JAWS/JAWS.h"

#include "JAWS/IO.h"
#include "JAWS/Pipeline_Tasks.h"
#include "JAWS/Data_Block.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Policy.h"

ACE_RCSID(JAWS, Pipeline_Tasks, "$Id$")

JAWS_Pipeline_Handler::JAWS_Pipeline_Handler (void)
  : policy_ (0)
{
}

JAWS_Pipeline_Handler::~JAWS_Pipeline_Handler (void)
{
}

int
JAWS_Pipeline_Handler::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  JAWS_Data_Block *db = ACE_dynamic_cast (JAWS_Data_Block *, mb);

  int status = this->handle_put (db, tv);

  if (status != -1 && status != 2)
    {
      JAWS_Pipeline_Handler *task = db->io_handler ()->task ();
      JAWS_Pipeline_Handler *next
        = ACE_dynamic_cast (JAWS_Pipeline_Handler *, task->next ());

      db->io_handler ()->task (next);
    }

  return status;
}

JAWS_Dispatch_Policy *
JAWS_Pipeline_Handler::policy (void)
{
  return this->policy_;
}

void
JAWS_Pipeline_Handler::policy (JAWS_Dispatch_Policy *policy)
{
  this->policy_ = policy;
}

int
JAWS_Pipeline_Accept_Task::put (ACE_Message_Block *mb, ACE_Time_Value *tv)
{
  JAWS_Data_Block *db = ACE_dynamic_cast (JAWS_Data_Block *, mb);

  JAWS_Pipeline_Handler *task = db->task ();
  JAWS_Pipeline_Handler *next
    = ACE_dynamic_cast (JAWS_Pipeline_Handler *, task->next ());

  JAWS_IO_Handler *ioh = this->new_handler (db);
  if (ioh == 0)
    {
      ACE_ERROR ((LM_ERROR, "%p\n", "JAWS_Pipeline_Accept_Task::put"));
      return -1;
    }

  ioh->task (next);
  db->io_handler (ioh);

  return this->handle_put (ioh->message_block (), tv);
}

int
JAWS_Pipeline_Accept_Task::handle_put (JAWS_Data_Block *data,
                                       ACE_Time_Value *)
{
  int result = -1;

  // JAWS_Data_Block should contain an INET_Addr and an IO
  JAWS_IO_Handler *handler = data->io_handler ();
  JAWS_Dispatch_Policy *policy = this->policy ();

  if (policy == 0) policy = data->policy ();

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
        // Move on to next stage in pipeline
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

#if defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS)
        if (policy->ratio () > 1)
          {
            for (int i = 1; i < policy->ratio (); i++)
              {
                JAWS_IO_Handler *ioh = this->new_handler (data);
                if (handler == 0)
                  break;
                ioh->task (handler->task ());
                io->accept (ioh);
                if (ioh->status () == JAWS_IO_Handler::ACCEPT_ERROR)
                  {
                    ioh->message_block ()->release ();
                    ioh->factory ()->destroy_io_handler (ioh);
                    break;
                  }
              }
            policy->ratio (1);
          }
#endif /* defined (ACE_WIN32) || defined (ACE_HAS_AIO_CALLS) */

        break;
      }
    }

  // In asynchronous and synchronous models, we can --
  //   have the io_handler set the new task in the data_block

  // In asynchronous model, we can --
  //   insert a wait task into the task queue

  return result;
}

JAWS_IO_Handler *
JAWS_Pipeline_Accept_Task::new_handler (JAWS_Data_Block *data)
{
  // Create a new handler and message block
  JAWS_Data_Block *ndb = new JAWS_Data_Block (*data);
  if (ndb == 0)
    {
      JAWS_TRACE ("JAWS_Pipeline_Accept_Task::new_handler, failed DB");
      return 0;
    }

  JAWS_Dispatch_Policy *policy =
    (this->policy () == 0) ? data->policy () : this->policy ();
  JAWS_IO_Handler_Factory *ioh_factory = policy->ioh_factory ();

  JAWS_IO_Handler *nioh = ioh_factory->create_io_handler ();
  if (nioh == 0)
    {
      ndb->release ();
      return 0;
    }

  ndb->io_handler (nioh);
  nioh->task (data->task ());
  nioh->message_block (ndb);

  return nioh;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>;
template class ACE_Singleton<JAWS_Pipeline_Accept_Task, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate JAWS_Pipeline_Abstract_Handler<JAWS_Data_Block>
#pragma instantiate ACE_Singleton<JAWS_Pipeline_Accept_Task, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
