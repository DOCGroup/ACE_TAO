// $Id$

#include "JAWS/IO.h"
#include "JAWS/IO_Handler.h"

JAWS_IO_Handler_Factory::~JAWS_IO_Handler_Factory (void)
{
}

JAWS_Synch_IO_Handler::JAWS_Synch_IO_Handler (JAWS_IO *io,
                                              JAWS_IO_Handler_Factory *factory)
  : state_ (0),
    io_ (io),
    pipeline_ (0),
    factory_ (factory)
{
  this->io_.handler (this);
}

JAWS_Synch_IO_Handler::~JAWS_Synch_IO_Handler (void)
{
  if (this->state_)
    this->state_->release ();
  this->state_ = 0;
}

void
JAWS_Synch_IO_Handler::accept_complete (void)
{
  // callback into pipeline task, notify that the accept has completed
}

void
JAWS_Synch_IO_Handler::accept_error (void)
{
  // callback into pipeline task, notify that the accept has failed
}

void
JAWS_Synch_IO_Handler::read_complete (ACE_Message_Block &data)
{
  // We can call back into the pipeline task at this point
  this->pipeline_->read_complete (data);
}

void
JAWS_Synch_IO_Handler::read_error (void)
{
  this->pipeline_->read_error ();
}

void
JAWS_Synch_IO_Handler::transmit_file_complete (void)
{
  this->pipeline_->transmit_file_complete ();
}

void
JAWS_Synch_IO_Handler::transmit_file_error (int result)
{
  this->pipeline_->transmit_file_complete (result);
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
  ACE_DEBUG ((LM_DEBUG, " (%t) %s error in writing response\n",
              request_.uri ()));

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

void
JAWS_Synch_IO_Handler::done (void)
{
  this->factory()->destroy_http_handler (this, this->io_);
}

JAWS_IO_Handler *
JAWS_Synch_IO_Handler_Factory::create_io_handler (void)
{
  JAWS_Synch_IO *io;
  JAWS_Synch_IO_Handler *handler;

  io = new JAWS_Synch_IO;
  if (io == 0) return 0;

  handler = new JAWS_Synch_IO_Handler (io, this);
  if (handler == 0) destroy_io_handler (0, io);

  return handler;
}

void
JAWS_Synch_IO_Handler_Factory::destroy_io_handler (JAWS_IO_Handler *handler,
                                                   JAWS_IO *io)
{
  delete handler;
  delete io;
}
