// $Id$

#include "JAWS/JAWS.h"

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Data_Block.h"
#include "JAWS/Policy.h"

JAWS_Abstract_IO_Handler::~JAWS_Abstract_IO_Handler (void)
{
}

JAWS_IO_Handler_Factory::~JAWS_IO_Handler_Factory (void)
{
}

JAWS_IO_Handler::JAWS_IO_Handler (JAWS_IO_Handler_Factory *factory)
  : status_ (0),
    mb_ (0),
    handle_ (ACE_INVALID_HANDLE),
    task_ (0),
    factory_ (factory)
{
  // this->io_->handler (this);
}

JAWS_IO_Handler::~JAWS_IO_Handler (void)
{
  if (this->mb_)
    this->mb_->release ();
  this->mb_ = 0;
  ACE_OS::close (this->handle_);
  this->handle_ = ACE_INVALID_HANDLE;
  this->status_ = 0;
}

void
JAWS_IO_Handler::accept_complete (ACE_HANDLE handle)
{
  // callback into pipeline task, notify that the accept has completed
  this->handle_ = handle;
  this->status_ = ACCEPT_OK;

  JAWS_Dispatch_Policy *policy = this->mb_->policy ();

  // Do this so that Thread Per Request can spawn a new thread
  policy->concurrency ()->activate_hook ();
}

void
JAWS_IO_Handler::accept_error (void)
{
  // callback into pipeline task, notify that the accept has failed
  this->status_ = ACCEPT_ERROR;
}

void
JAWS_IO_Handler::read_complete (ACE_Message_Block *data)
{
  ACE_UNUSED_ARG (data);
  // We can call back into the pipeline task at this point
  // this->pipeline_->read_complete (data);
  this->status_ = READ_OK;
}

void
JAWS_IO_Handler::read_error (void)
{
  // this->pipeline_->read_error ();
  this->status_ = READ_ERROR;
}

void
JAWS_IO_Handler::transmit_file_complete (void)
{
  // this->pipeline_->transmit_file_complete ();
  this->status_ = TRANSMIT_OK;
}

void
JAWS_IO_Handler::transmit_file_error (int result)
{
  ACE_UNUSED_ARG (result);
  // this->pipeline_->transmit_file_complete (result);
  this->status_ = TRANSMIT_ERROR;
}

void
JAWS_IO_Handler::receive_file_complete (void)
{
  this->status_ = RECEIVE_OK;
}

void
JAWS_IO_Handler::receive_file_error (int result)
{
  ACE_UNUSED_ARG(result);
  this->status_ = RECEIVE_ERROR;
}

void
JAWS_IO_Handler::write_error (void)
{
  ACE_DEBUG ((LM_DEBUG, " (%t) error in writing response\n"));

  this->status_ = WRITE_ERROR;
  this->done ();
}

void
JAWS_IO_Handler::confirmation_message_complete (void)
{
  this->status_ = WRITE_OK;
}

void
JAWS_IO_Handler::error_message_complete (void)
{
  this->status_ = WRITE_OK;
}

JAWS_IO_Handler_Factory *
JAWS_IO_Handler::factory (void)
{
  return this->factory_;
}

ACE_HANDLE
JAWS_IO_Handler::handle (void)
{
  return this->handle_;
}

void
JAWS_IO_Handler::task (JAWS_Pipeline_Handler *ph)
{
  this->task_ = ph;
}

JAWS_Pipeline_Handler *
JAWS_IO_Handler::task (void)
{
  return this->task_;
}

void
JAWS_IO_Handler::message_block (JAWS_Data_Block *mb)
{
  this->mb_ = mb;
}

JAWS_Data_Block *
JAWS_IO_Handler::message_block (void)
{
  return this->mb_;
}

void
JAWS_IO_Handler::done (void)
{
  this->factory ()->destroy_io_handler (this);
}

int
JAWS_IO_Handler::status (void)
{
  return this->status_;
}

JAWS_Synch_IO_Handler::JAWS_Synch_IO_Handler (JAWS_IO_Handler_Factory *factory)
  : JAWS_IO_Handler (factory)
{
  // this->io_->handler (this);
}

JAWS_Synch_IO_Handler::~JAWS_Synch_IO_Handler (void)
{
}

JAWS_IO_Handler *
JAWS_Synch_IO_Handler_Factory::create_io_handler (void)
{
  JAWS_TRACE ("JAWS_Synch_IO_Handler_Factory::create");

  JAWS_Synch_IO *io;
  JAWS_Synch_IO_Handler *handler;

  io = new JAWS_Synch_IO;
  if (io == 0) return 0;

  handler = new JAWS_Synch_IO_Handler (this);
  if (handler == 0) delete io;

  return handler;
}

void
JAWS_Synch_IO_Handler_Factory::destroy_io_handler (JAWS_IO_Handler *handler)
{
  JAWS_TRACE ("JAWS_Synch_IO_Handler_Factory::destroy");
  delete handler;
}

#if defined (ACE_WIN32)

JAWS_Asynch_Handler::JAWS_Asynch_Handler (JAWS_IO_Handler *ioh)
  : ioh_ (ioh)
{
}

JAWS_Asynch_Handler::~JAWS_Asynch_Handler (void)
{
}

void
JAWS_Asynch_Handler::dispatch_handler (void)
{
#if 0
  // A future version of ACE will support this.
  ACE_Thread_ID tid = ACE_OS::thr_self ();
#else
  // Do it this way for now
  ACE_hthread_t thr_handle;
  ACE_OS::thr_self (thr_handle);

  ACE_thread_t thr_name;
  thr_name = ACE_OS::thr_self ();

  ACE_Thread_ID tid (thr_handle, thr_name);
#endif /* 0 */

  JAWS_IO_Handler **iohref = this->find (tid);

  *iohref = this->handler ();
}

void
JAWS_Asynch_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result
                                         &result)
{
  // This callback is for this->receive_file()

  this->dispatch_handler ();

  if (result.act () != 0)
    {
      int code = 0;
      if (result.success () && result.bytes_transferred () != 0)
        {
          if (result.message_block ().length ()
              == result.message_block ().size ())
            code = ACE_Filecache_Handle::SUCCESS;
          else
            {
              ACE_Asynch_Read_Stream ar;
              if (ar.open (*this, this->handler ()->handle ()) == -1
                  || ar.read (result.message_block (),
                              result.message_block ().size ()
                              - result.message_block ().length (),
                              result.act ()) == -1)
                code = -1;
              else
                return;
            }
        }
      else
        code = -1;

      if (code == ACE_Filecache_Handle::SUCCESS)
        this->handler ()->receive_file_complete ();
      else
        this->handler ()->receive_file_error (code);

      delete &result.message_block ();
      delete (ACE_Filecache_Handle *) result.act ();
    }
  else
    {
      // This callback is for this->read()
      if (result.success ()
          && result.bytes_transferred () != 0)
        this->handler ()->read_complete (result.message_block ());
      else
        this->handler ()->read_error ();
    }
}

void
JAWS_Asynch_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result
                                          &result)
{
  result.message_block ().release ();

  if (result.act () == (void *) CONFORMATION)
    this->handler ()->confirmation_message_complete ();
  else
    this->handler ()->error_message_complete ();
}

void
JAWS_Asynch_Handler::handle_transmit_file (const
                                           ACE_Asynch_Transmit_File::Result
                                           &result)
{
  if (result.success ())
    this->handler ()->transmit_file_complete ();
  else
    this->handler ()->transmit_file_error (-1);

  delete result.header_and_trailer ();
  delete (ACE_Filecache_Handle *) result.act ();
}

void
JAWS_Asynch_Handler::handle_accept (const ACE_Asynch_Accept::Result &result)
{
}

JAWS_Asynch_IO_Handler::JAWS_Asynch_IO_Handler (JAWS_IO_Handler_Factory
                                                *factory)
  : JAWS_IO_Handler (factory),
    handler_ (this)
{
  // this->io_->handler (this);
}

JAWS_Asynch_IO_Handler::~JAWS_Asynch_IO_Handler (void)
{
}

ACE_Handler *
JAWS_Asynch_IO_Handler::handler (void)
{
  return &this->handler_;
}

JAWS_IO_Handler *
JAWS_Asynch_IO_Handler_Factory::create_io_handler (void)
{
  JAWS_TRACE ("JAWS_Synch_IO_Handler_Factory::create");

  JAWS_Asynch_IO *io;
  JAWS_Asynch_IO_Handler *handler;

  io = new JAWS_Asynch_IO;
  if (io == 0) return 0;

  handler = new JAWS_Asynch_IO_Handler (this);
  if (handler == 0) delete io;

  return handler;
}

void
JAWS_Asynch_IO_Handler_Factory::destroy_io_handler (JAWS_IO_Handler *handler)
{
  JAWS_TRACE ("JAWS_Asynch_IO_Handler_Factory::destroy");
  delete handler;
}

#endif /* ACE_WIN32 */


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<JAWS_Synch_IO_Handler_Factory, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Singleton<JAWS_Synch_IO_Handler_Factory, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#if defined (ACE_WIN32)
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<JAWS_Asynch_IO_Handler_Factory, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Singleton<JAWS_Asynch_IO_Handler_Factory, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_WIN32 */
