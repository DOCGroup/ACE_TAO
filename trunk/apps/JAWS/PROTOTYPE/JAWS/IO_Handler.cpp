// $Id$

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"
#include "JAWS/Data_Block.h"

JAWS_IO_Handler_Factory::~JAWS_IO_Handler_Factory (void)
{
}

JAWS_Synch_IO_Handler::JAWS_Synch_IO_Handler (JAWS_IO_Handler_Factory *factory)
  : status_ (0),
    mb_ (0),
    handle_ (ACE_INVALID_HANDLE),
    task_ (0),
    factory_ (factory)
{
  // this->io_->handler (this);
}

JAWS_Synch_IO_Handler::~JAWS_Synch_IO_Handler (void)
{
  if (this->mb_)
    this->mb_->release ();
  this->mb_ = 0;
}

void
JAWS_Synch_IO_Handler::accept_complete (ACE_HANDLE handle)
{
  // callback into pipeline task, notify that the accept has completed
  this->handle_ = handle;
}

void
JAWS_Synch_IO_Handler::accept_error (void)
{
  // callback into pipeline task, notify that the accept has failed
}

void
JAWS_Synch_IO_Handler::read_complete (ACE_Message_Block &data)
{
  ACE_UNUSED_ARG (data);
  // We can call back into the pipeline task at this point
  // this->pipeline_->read_complete (data);
}

void
JAWS_Synch_IO_Handler::read_error (void)
{
  // this->pipeline_->read_error ();
}

void
JAWS_Synch_IO_Handler::transmit_file_complete (void)
{
  // this->pipeline_->transmit_file_complete ();
}

void
JAWS_Synch_IO_Handler::transmit_file_error (int result)
{
  ACE_UNUSED_ARG (result);
  // this->pipeline_->transmit_file_complete (result);
}

void
JAWS_Synch_IO_Handler::receive_file_complete (void)
{
}

void
JAWS_Synch_IO_Handler::receive_file_error (int result)
{
  ACE_UNUSED_ARG(result);
}

void
JAWS_Synch_IO_Handler::write_error (void)
{
  ACE_DEBUG ((LM_DEBUG, " (%t) error in writing response\n"));

  this->done ();
}

void
JAWS_Synch_IO_Handler::confirmation_message_complete (void)
{
}

void
JAWS_Synch_IO_Handler::error_message_complete (void)
{
}

JAWS_IO_Handler_Factory *
JAWS_Synch_IO_Handler::factory (void)
{
  return this->factory_;
}

ACE_HANDLE
JAWS_Synch_IO_Handler::handle (void)
{
  return this->handle_;
}

void
JAWS_Synch_IO_Handler::task (JAWS_Pipeline_Handler *ph)
{
  this->task_ = ph;
}

JAWS_Pipeline_Handler *
JAWS_Synch_IO_Handler::task (void)
{
  return this->task_;
}

void
JAWS_Synch_IO_Handler::done (void)
{
  this->factory ()->destroy_io_handler (this);
}

int
JAWS_Synch_IO_Handler::status (void)
{
  return this->status_;
}

JAWS_IO_Handler *
JAWS_Synch_IO_Handler_Factory::create_io_handler (void)
{
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
  delete handler;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<JAWS_Synch_IO_Handler_Factory, ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Singleton<JAWS_Synch_IO_Handler_Factory, ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
