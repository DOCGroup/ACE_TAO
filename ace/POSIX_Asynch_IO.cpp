// $Id$

#include "ace/POSIX_Asynch_IO.h"

#if defined (ACE_HAS_AIO_CALLS)


#include "ace/Proactor.h"
#include "ace/Message_Block.h"
#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/Task_T.h"
#include "ace/POSIX_Proactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/POSIX_Asynch_IO.i"
#endif /* __ACE_INLINE__ */

u_long
ACE_POSIX_Asynch_Result::bytes_transferred (void) const
{
  return this->bytes_transferred_;
}

const void *
ACE_POSIX_Asynch_Result::act (void) const
{
  return this->act_;
}

int
ACE_POSIX_Asynch_Result::success (void) const
{
  return this->success_;
}

const void *
ACE_POSIX_Asynch_Result::completion_key (void) const
{
  return this->completion_key_;
}

u_long
ACE_POSIX_Asynch_Result::error (void) const
{
  return this->error_;
}

ACE_HANDLE
ACE_POSIX_Asynch_Result::event (void) const
{
  return ACE_INVALID_HANDLE;
}

u_long
ACE_POSIX_Asynch_Result::offset (void) const
{
  return this->aio_offset;
}

u_long
ACE_POSIX_Asynch_Result::offset_high (void) const
{
  //
  // @@ Support aiocb64??
  //
  ACE_NOTSUP_RETURN (0);
}

int
ACE_POSIX_Asynch_Result::priority (void) const
{
  return this->aio_reqprio;
}

int
ACE_POSIX_Asynch_Result::signal_number (void) const
{
  return this->aio_sigevent.sigev_signo;
}
int
ACE_POSIX_Asynch_Result::post_completion (ACE_Proactor_Impl *proactor_impl)
{
  // Get to the platform specific implementation.
  ACE_POSIX_Proactor *posix_proactor = ACE_dynamic_cast (ACE_POSIX_Proactor *,
                                                         proactor_impl);

  if (posix_proactor == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Dynamic cast to POSIX Proactor failed\n"), -1);

  // Post myself.
  return posix_proactor->post_completion (this);
}

ACE_POSIX_Asynch_Result::~ACE_POSIX_Asynch_Result (void)
{
}

ACE_POSIX_Asynch_Result::ACE_POSIX_Asynch_Result (ACE_Handler &handler,
                                                  const void* act,
                                                  ACE_HANDLE event,
                                                  u_long offset,
                                                  u_long offset_high,
                                                  int priority,
                                                  int signal_number)
  : ACE_Asynch_Result_Impl (),
    aiocb (),
    handler_ (handler),
    act_ (act),
    success_ (0),
    completion_key_ (0)
{
  aio_offset = offset;
  aio_reqprio = priority;
  aio_sigevent.sigev_signo = signal_number;

  // Event is not used on POSIX.
ACE_UNUSED_ARG (event);

  //
  // @@ Support offset_high with aiocb64.
  //
  ACE_UNUSED_ARG (offset_high);

  // Other fields in the <aiocb> will be initialized by the
  // subclasses.
}

// ****************************************************************

int
ACE_POSIX_Asynch_Operation::open (ACE_Handler &handler,
                                  ACE_HANDLE handle,
                                  const void *completion_key,
                                  ACE_Proactor *proactor)
{
  this->proactor_ = proactor;
  this->handler_ = &handler;
  this->handle_ = handle;

  // Grab the handle from the <handler> if <handle> is invalid
  if (this->handle_ == ACE_INVALID_HANDLE)
    this->handle_ = this->handler_->handle ();
  if (this->handle_ == ACE_INVALID_HANDLE)
    return -1;

#if 0
  // @@ If <proactor> is 0, let us not bother about getting this
  // Proactor, we have already got the specific implementation
  // Proactor.

  // If no proactor was passed
  if (this->proactor_ == 0)
    {
      // Grab the proactor from the <Service_Config> if
      // <handler->proactor> is zero
      this->proactor_ = this->handler_->proactor ();
      if (this->proactor_ == 0)
        this->proactor_ = ACE_Proactor::instance();
    }
#endif /* 0 */

  // AIO stuff is present. So no registering.
  ACE_UNUSED_ARG (completion_key);
  return 0;
}

int
ACE_POSIX_Asynch_Operation::cancel (void)
{
  // @@ Not implemented.
  ACE_NOTSUP_RETURN (0);
}

ACE_Proactor *
ACE_POSIX_Asynch_Operation::proactor (void) const
{
  return this->proactor_;
}

ACE_POSIX_Asynch_Operation::~ACE_POSIX_Asynch_Operation (void)
{
}

ACE_POSIX_Asynch_Operation::ACE_POSIX_Asynch_Operation (void)
  : ACE_Asynch_Operation_Impl (),
    handler_ (0),
    handle_ (ACE_INVALID_HANDLE)
{
}

// *********************************************************************

ACE_POSIX_AIOCB_Proactor *
ACE_POSIX_AIOCB_Asynch_Operation::posix_proactor (void) const
{
  return this->posix_aiocb_proactor_;
}

ACE_POSIX_AIOCB_Asynch_Operation::ACE_POSIX_AIOCB_Asynch_Operation (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_POSIX_Asynch_Operation (),
    posix_aiocb_proactor_ (posix_aiocb_proactor)
{
}

ACE_POSIX_AIOCB_Asynch_Operation::~ACE_POSIX_AIOCB_Asynch_Operation (void)
{
}

// If the ptr is o, just check whether there is any slot free and
// return 0 if yes, else return -1. If a valid ptr is passed, keep it
// in a free slot.
int
ACE_POSIX_AIOCB_Asynch_Operation::register_aio_with_proactor (ACE_POSIX_Asynch_Result *result)
{
  return this->posix_proactor ()->register_aio_with_proactor (result);
}

// *********************************************************************

ACE_POSIX_SIG_Proactor *
ACE_POSIX_SIG_Asynch_Operation::posix_proactor (void) const
{
  return this->posix_sig_proactor_;
}

ACE_POSIX_SIG_Asynch_Operation::ACE_POSIX_SIG_Asynch_Operation (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_POSIX_Asynch_Operation (),
    posix_sig_proactor_ (posix_sig_proactor)
{
}

ACE_POSIX_SIG_Asynch_Operation::~ACE_POSIX_SIG_Asynch_Operation (void)
{
}

// *********************************************************************

u_long
ACE_POSIX_Asynch_Read_Stream_Result::bytes_to_read (void) const
{
  return this->aio_nbytes;
}

ACE_Message_Block &
ACE_POSIX_Asynch_Read_Stream_Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_POSIX_Asynch_Read_Stream_Result::handle (void) const
{
  return this->aio_fildes;
}

ACE_POSIX_Asynch_Read_Stream_Result::ACE_POSIX_Asynch_Read_Stream_Result (ACE_Handler &handler,
                                                                          ACE_HANDLE handle,
                                                                          ACE_Message_Block &message_block,
                                                                          u_long bytes_to_read,
                                                                          const void* act,
                                                                          ACE_HANDLE event,
                                                                          int priority,
                                                                          int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Read_Stream_Result_Impl (),
    ACE_POSIX_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    message_block_ (message_block)
{
  this->aio_fildes = handle;
  this->aio_buf = message_block.wr_ptr ();
  this->aio_nbytes = bytes_to_read;
  ACE_UNUSED_ARG (event);
}

void
ACE_POSIX_Asynch_Read_Stream_Result::complete (u_long bytes_transferred,
                                               int success,
                                               const void *completion_key,
                                               u_long error)
{
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // <errno> is available in the aiocb.
  ACE_UNUSED_ARG (error);

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Create the interface result class.
  ACE_Asynch_Read_Stream::Result result (this);

  // Call the application handler.
  this->handler_.handle_read_stream (result);
}

ACE_POSIX_Asynch_Read_Stream_Result::~ACE_POSIX_Asynch_Read_Stream_Result (void)
{
}

// = Base class operations. These operations are here to kill
//   dominance warnings. These methods call the base class methods. 

u_long
ACE_POSIX_Asynch_Read_Stream_Result::bytes_transferred (void) const
{
  return ACE_POSIX_Asynch_Result::bytes_transferred ();
}

const void *
ACE_POSIX_Asynch_Read_Stream_Result::act (void) const
{
  return ACE_POSIX_Asynch_Result::act ();
}

int
ACE_POSIX_Asynch_Read_Stream_Result::success (void) const
{
  return ACE_POSIX_Asynch_Result::success ();
}

const void *
ACE_POSIX_Asynch_Read_Stream_Result::completion_key (void) const
{
  return ACE_POSIX_Asynch_Result::completion_key ();
}

u_long
ACE_POSIX_Asynch_Read_Stream_Result::error (void) const
{
  return ACE_POSIX_Asynch_Result::error ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Read_Stream_Result::event (void) const
{
  return ACE_POSIX_Asynch_Result::event ();
}

u_long
ACE_POSIX_Asynch_Read_Stream_Result::offset (void) const
{
  return ACE_POSIX_Asynch_Result::offset ();
}

u_long
ACE_POSIX_Asynch_Read_Stream_Result::offset_high (void) const
{
  return ACE_POSIX_Asynch_Result::offset_high ();
}

int
ACE_POSIX_Asynch_Read_Stream_Result::priority (void) const
{
  return ACE_POSIX_Asynch_Result::priority ();
}

int
ACE_POSIX_Asynch_Read_Stream_Result::signal_number (void) const
{
  return ACE_POSIX_Asynch_Result::signal_number ();
}

int
ACE_POSIX_Asynch_Read_Stream_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_POSIX_Asynch_Result::post_completion (proactor);
}

// ************************************************************

ACE_POSIX_AIOCB_Asynch_Read_Stream::ACE_POSIX_AIOCB_Asynch_Read_Stream (ACE_POSIX_AIOCB_Proactor  *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_POSIX_AIOCB_Asynch_Operation (posix_aiocb_proactor)
{
}

int
ACE_POSIX_AIOCB_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                                          u_long bytes_to_read,
                                          const void *act,
                                          int priority,
                                          int signal_number)
{
  // Create the Asynch_Result.
  ACE_POSIX_Asynch_Read_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Read_Stream_Result (*this->handler_,
                                                       this->handle_,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       this->posix_proactor ()->get_handle (),
                                                       priority,
                                                       signal_number),
                  -1);

  ssize_t return_val = this->shared_read (result);
  if (return_val == -1)
    delete result;
  return return_val;
}

ACE_POSIX_AIOCB_Asynch_Read_Stream::~ACE_POSIX_AIOCB_Asynch_Read_Stream (void)
{
}

int
ACE_POSIX_AIOCB_Asynch_Read_Stream::shared_read (ACE_POSIX_Asynch_Read_Stream_Result *result)
{
  // AIO_CONTROL_BLOCKS strategy.

  // Store this <result> with the proactor.

  // Make sure there is space in the aiocb list.
  if (this->register_aio_with_proactor (0) == -1)
    {
      // @@ Set errno to EAGAIN so that applications will know this as
      // a queueing failure and try again this aio_read it they want.
      errno = EAGAIN;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Fatal error:%N:%l:%p\n",
                         "AIOContol Block Array is full!!!. Didnt issue the aio call"),
                        -1);
    }

  // Setup AIOCB.

  // We are not making use of the RT signal queueing in this
  // strategy.
  result->aio_sigevent.sigev_notify = SIGEV_NONE;

  // Fire off the aio read.
  if (aio_read (result) == -1)
    // Queueing failed.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Asynch_Read_Stream: aio_read queueing failed"),
                      -1);

  // <aio_read> successfully issued. Store the <result> with
  // proactor.
  if (this->register_aio_with_proactor (result) == -1)
    // This shouldn't happen anyway, since we have already checked for
    // availability of free slots.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Fatal error:%N:%l:%p\n",
                       "AIOContol Block Array is full!!!. Didnt issue the aio call"),
                      -1);

  // <aio_read> successfully issued and ptr stored.
  return 0;
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route the
// call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_AIOCB_Asynch_Read_Stream::open (ACE_Handler &handler,
                                          ACE_HANDLE handle,
                                          const void *completion_key,
                                          ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_AIOCB_Asynch_Read_Stream::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_AIOCB_Asynch_Read_Stream::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *********************************************************************

ACE_POSIX_SIG_Asynch_Read_Stream::ACE_POSIX_SIG_Asynch_Read_Stream (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_POSIX_SIG_Asynch_Operation (posix_sig_proactor)
{
}

int
ACE_POSIX_SIG_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                                        u_long bytes_to_read,
                                        const void *act,
                                        int priority,
                                        int signal_number)
{
  // Create the Asynch_Result.
  ACE_POSIX_Asynch_Read_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Read_Stream_Result (*this->handler_,
                                                       this->handle_,
                                                       message_block,
                                                       bytes_to_read,
                                                       act,
                                                       this->posix_sig_proactor_->get_handle (),
                                                       priority,
                                                       signal_number),
                  -1);

  ssize_t return_val = this->shared_read (result);
  if (return_val == -1)
    delete result;
  return return_val;
}

ACE_POSIX_SIG_Asynch_Read_Stream::~ACE_POSIX_SIG_Asynch_Read_Stream (void)
{
}

int
ACE_POSIX_SIG_Asynch_Read_Stream::shared_read (ACE_POSIX_Asynch_Read_Stream_Result *result)
{
  // Setup AIOCB.

  // We want queuing of RT signal to notify completion.
  result->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  result->aio_sigevent.sigev_signo = result->signal_number ();
  result->aio_sigevent.sigev_value.sival_ptr = (void *) result;
  
  // Fire off the aio read.
  if (aio_read (result) == -1)
    // Queueing failed.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%N%l:%p\n",
                       "Asynch_Read_Stream: aio_read queueing failed"),
                      -1);
  return 0;
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route the
// call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_SIG_Asynch_Read_Stream::open (ACE_Handler &handler,
                                        ACE_HANDLE handle,
                                        const void *completion_key,
                                        ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_SIG_Asynch_Read_Stream::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_SIG_Asynch_Read_Stream::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// ****************************************************************

u_long
ACE_POSIX_Asynch_Write_Stream_Result::bytes_to_write (void) const
{
  return this->aio_nbytes;
}

ACE_Message_Block &
ACE_POSIX_Asynch_Write_Stream_Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_POSIX_Asynch_Write_Stream_Result::handle (void) const
{
  return this->aio_fildes;
}

ACE_POSIX_Asynch_Write_Stream_Result::ACE_POSIX_Asynch_Write_Stream_Result (ACE_Handler &handler,
                                                                            ACE_HANDLE handle,
                                                                            ACE_Message_Block &message_block,
                                                                            u_long bytes_to_write,
                                                                            const void* act,
                                                                            ACE_HANDLE event,
                                                                            int priority,
                                                                            int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Write_Stream_Result_Impl (),
    ACE_POSIX_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    message_block_ (message_block)
{
  this->aio_fildes = handle;
  this->aio_buf = message_block.rd_ptr ();
  this->aio_nbytes = bytes_to_write;
  ACE_UNUSED_ARG (event);
}

void
ACE_POSIX_Asynch_Write_Stream_Result::complete (u_long bytes_transferred,
                                                int success,
                                                const void *completion_key,
                                                u_long error)
{
  // Get all the data copied.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // <errno> is available in the aiocb.
  ACE_UNUSED_ARG (error);

  // Appropriately move the pointers in the message block.
  this->message_block_.rd_ptr (bytes_transferred);

  // Create the interface result class.
  ACE_Asynch_Write_Stream::Result result (this);

  // Call the application handler.
  this->handler_.handle_write_stream (result);
}

ACE_POSIX_Asynch_Write_Stream_Result::~ACE_POSIX_Asynch_Write_Stream_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

u_long
ACE_POSIX_Asynch_Write_Stream_Result::bytes_transferred (void) const
{
  return ACE_POSIX_Asynch_Result::bytes_transferred ();
}

const void *
ACE_POSIX_Asynch_Write_Stream_Result::act (void) const
{
  return ACE_POSIX_Asynch_Result::act ();
}

int
ACE_POSIX_Asynch_Write_Stream_Result::success (void) const
{
  return ACE_POSIX_Asynch_Result::success ();
}

const void *
ACE_POSIX_Asynch_Write_Stream_Result::completion_key (void) const
{
  return ACE_POSIX_Asynch_Result::completion_key ();
}

u_long
ACE_POSIX_Asynch_Write_Stream_Result::error (void) const
{
  return ACE_POSIX_Asynch_Result::error ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Write_Stream_Result::event (void) const
{
  return ACE_POSIX_Asynch_Result::event ();
}

u_long
ACE_POSIX_Asynch_Write_Stream_Result::offset (void) const
{
  return ACE_POSIX_Asynch_Result::offset ();
}

u_long
ACE_POSIX_Asynch_Write_Stream_Result::offset_high (void) const
{
  return ACE_POSIX_Asynch_Result::offset_high ();
}

int
ACE_POSIX_Asynch_Write_Stream_Result::priority (void) const
{
  return ACE_POSIX_Asynch_Result::priority ();
}

int
ACE_POSIX_Asynch_Write_Stream_Result::signal_number (void) const
{
  return ACE_POSIX_Asynch_Result::signal_number ();
}

int
ACE_POSIX_Asynch_Write_Stream_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_POSIX_Asynch_Result::post_completion (proactor);
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Write_Stream::ACE_POSIX_AIOCB_Asynch_Write_Stream (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_POSIX_AIOCB_Asynch_Operation (posix_aiocb_proactor)
{
}

int
ACE_POSIX_AIOCB_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                            u_long bytes_to_write,
                                            const void *act,
                                            int priority,
                                            int signal_number)
{
  ACE_POSIX_Asynch_Write_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Write_Stream_Result (*this->handler_,
                                                        this->handle_,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        this->posix_proactor ()->get_handle (),
                                                        priority,
                                                        signal_number),
                  -1);

  ssize_t return_val = this->shared_write (result);
  if (return_val == -1)
    delete result;
  return return_val;
}

ACE_POSIX_AIOCB_Asynch_Write_Stream::~ACE_POSIX_AIOCB_Asynch_Write_Stream (void)
{
}

int
ACE_POSIX_AIOCB_Asynch_Write_Stream::shared_write (ACE_POSIX_Asynch_Write_Stream_Result *result)
{
  // AIO_CONTROL_BLOCKS strategy.

  // Make sure there is space in the aiocb list.
  if (this->register_aio_with_proactor (0) == -1)
    {
      // @@ Set errno to EAGAIN so that applications will know this as
      // a queueing failure and try again this aio_read it they want.
      errno = EAGAIN;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Fatal error:%N:%l:%p\n",
                         "AIOContol Block Array is full!!!. Didnt issue the aio call"),
                        -1);
    }

  // Setup AIOCB.
  result->aio_sigevent.sigev_notify = SIGEV_NONE;

  // Fire off the aio write.
  if (aio_write (result) == -1)
    // Queueing failed.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Asynch_Write_Stream: aio_write queueing failed"),
                      -1);

  // Success. Store the <result> with Proactor.
  if (this->register_aio_with_proactor (result) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Fatal error:%N:%l:%p\n",
                       "AIOContol Block Array is full!!!"),
                      -1);

  // Aio successfully issued.
  return 0;
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_AIOCB_Asynch_Write_Stream::open (ACE_Handler &handler,
                                     ACE_HANDLE handle,
                                     const void *completion_key,
                                     ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_AIOCB_Asynch_Write_Stream::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_AIOCB_Asynch_Write_Stream::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *********************************************************************

ACE_POSIX_SIG_Asynch_Write_Stream::ACE_POSIX_SIG_Asynch_Write_Stream (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_POSIX_SIG_Asynch_Operation (posix_sig_proactor)
{
}

int
ACE_POSIX_SIG_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                          u_long bytes_to_write,
                                          const void *act,
                                          int priority,
                                          int signal_number)
{
  ACE_POSIX_Asynch_Write_Stream_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Write_Stream_Result (*this->handler_,
                                                        this->handle_,
                                                        message_block,
                                                        bytes_to_write,
                                                        act,
                                                        this->posix_sig_proactor_->get_handle (),
                                                        priority,
                                                        signal_number),
                  -1);

  ssize_t return_val = this->shared_write (result);
  if (return_val == -1)
    delete result;
  return return_val;
}

ACE_POSIX_SIG_Asynch_Write_Stream::~ACE_POSIX_SIG_Asynch_Write_Stream (void)
{
}

int
ACE_POSIX_SIG_Asynch_Write_Stream::shared_write (ACE_POSIX_Asynch_Write_Stream_Result *result)
{
  // Setup AIOCB.

  // We want queuing of RT signal to notify completion.
  result->aio_sigevent.sigev_notify = SIGEV_SIGNAL;
  result->aio_sigevent.sigev_signo = result->signal_number ();
  result->aio_sigevent.sigev_value.sival_ptr = (void *) result;

  // Fire off the aio write.
  if (aio_write (result) == -1)
    // Queueing failed.
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Asynch_Write_Stream: aio_write queueing failed"),
                      -1);
  return 0;
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_SIG_Asynch_Write_Stream::open (ACE_Handler &handler,
                                     ACE_HANDLE handle,
                                     const void *completion_key,
                                     ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_SIG_Asynch_Write_Stream::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_SIG_Asynch_Write_Stream::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *************************************************************

ACE_POSIX_Asynch_Read_File_Result::ACE_POSIX_Asynch_Read_File_Result (ACE_Handler &handler,
                                                                      ACE_HANDLE handle,
                                                                      ACE_Message_Block &message_block,
                                                                      u_long bytes_to_read,
                                                                      const void* act,
                                                                      u_long offset,
                                                                      u_long offset_high,
                                                                      ACE_HANDLE event,
                                                                      int priority,
                                                                      int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Read_Stream_Result_Impl (),
    ACE_Asynch_Read_File_Result_Impl (),
    ACE_POSIX_Asynch_Read_Stream_Result (handler,
                                         handle,
                                         message_block,
                                         bytes_to_read,
                                         act,
                                         event,
                                         priority,
                                         signal_number)
{
  this->aio_offset = offset;
  //
  // @@ Use aiocb64??
  //
  ACE_UNUSED_ARG (offset_high);
}

void
ACE_POSIX_Asynch_Read_File_Result::complete (u_long bytes_transferred,
                                             int success,
                                             const void *completion_key,
                                             u_long error)
{
  // Copy all the data.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // <errno> is available in the aiocb.
  ACE_UNUSED_ARG (error);

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Create the interface result class.
  ACE_Asynch_Read_File::Result result (this);

  // Call the application handler.
  this->handler_.handle_read_file (result);
}

ACE_POSIX_Asynch_Read_File_Result::~ACE_POSIX_Asynch_Read_File_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

u_long
ACE_POSIX_Asynch_Read_File_Result::bytes_transferred (void) const
{
  return ACE_POSIX_Asynch_Result::bytes_transferred ();
}

const void *
ACE_POSIX_Asynch_Read_File_Result::act (void) const
{
  return ACE_POSIX_Asynch_Result::act ();
}

int
ACE_POSIX_Asynch_Read_File_Result::success (void) const
{
  return ACE_POSIX_Asynch_Result::success ();
}

const void *
ACE_POSIX_Asynch_Read_File_Result::completion_key (void) const
{
  return ACE_POSIX_Asynch_Result::completion_key ();
}

u_long
ACE_POSIX_Asynch_Read_File_Result::error (void) const
{
  return ACE_POSIX_Asynch_Result::error ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Read_File_Result::event (void) const
{
  return ACE_POSIX_Asynch_Result::event ();
}

u_long
ACE_POSIX_Asynch_Read_File_Result::offset (void) const
{
  return ACE_POSIX_Asynch_Result::offset ();
}

u_long
ACE_POSIX_Asynch_Read_File_Result::offset_high (void) const
{
  return ACE_POSIX_Asynch_Result::offset_high ();
}

int
ACE_POSIX_Asynch_Read_File_Result::priority (void) const
{
  return ACE_POSIX_Asynch_Result::priority ();
}

int
ACE_POSIX_Asynch_Read_File_Result::signal_number (void) const
{
  return ACE_POSIX_Asynch_Result::signal_number ();
}

// The following methods belong to
// ACE_POSIX_Asynch_Read_Stream_Result. They are here to avoid
// dominace warnings. These methods route their call to the 
// ACE_POSIX_Asynch_Read_Stream_Result base class.

u_long
ACE_POSIX_Asynch_Read_File_Result::bytes_to_read (void) const
{
  return ACE_POSIX_Asynch_Read_Stream_Result::bytes_to_read ();
}

ACE_Message_Block &
ACE_POSIX_Asynch_Read_File_Result::message_block (void) const
{
  return ACE_POSIX_Asynch_Read_Stream_Result::message_block ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Read_File_Result::handle (void) const
{
  return ACE_POSIX_Asynch_Read_Stream_Result::handle ();
}

int
ACE_POSIX_Asynch_Read_File_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_POSIX_Asynch_Result::post_completion (proactor);
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Read_File::ACE_POSIX_AIOCB_Asynch_Read_File (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_Asynch_Read_File_Impl (),
    ACE_POSIX_AIOCB_Asynch_Read_Stream (posix_aiocb_proactor)
{
}

int
ACE_POSIX_AIOCB_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                        u_long bytes_to_read,
                                        u_long offset,
                                        u_long offset_high,
                                        const void *act,
                                        int priority,
                                        int signal_number)
{
  ACE_POSIX_Asynch_Read_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Read_File_Result (*this->handler_,
                                                     this->handle_,
                                                     message_block,
                                                     bytes_to_read,
                                                     act,
                                                     offset,
                                                     offset_high,
                                                     this->posix_proactor ()->get_handle (),
                                                     priority,
                                                     signal_number),
                  -1);

  return this->shared_read (result);
}

ACE_POSIX_AIOCB_Asynch_Read_File::~ACE_POSIX_AIOCB_Asynch_Read_File (void)
{
}

int
ACE_POSIX_AIOCB_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                        u_long bytes_to_read,
                                        const void *act,
                                        int priority,
                                        int signal_number)
{
  return ACE_POSIX_AIOCB_Asynch_Read_Stream::read (message_block,
                                                   bytes_to_read,
                                                   act,
                                                   priority,
                                                   signal_number);
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_AIOCB_Asynch_Read_File::open (ACE_Handler &handler,
                                        ACE_HANDLE handle,
                                        const void *completion_key,
                                        ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_AIOCB_Asynch_Read_File::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_AIOCB_Asynch_Read_File::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// ************************************************************

ACE_POSIX_SIG_Asynch_Read_File::ACE_POSIX_SIG_Asynch_Read_File (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Read_Stream_Impl (),
    ACE_Asynch_Read_File_Impl (),
    ACE_POSIX_SIG_Asynch_Read_Stream (posix_sig_proactor)
{
}

int
ACE_POSIX_SIG_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                      u_long bytes_to_read,
                                      u_long offset,
                                      u_long offset_high,
                                      const void *act,
                                      int priority,
                                      int signal_number)
{
  ACE_POSIX_Asynch_Read_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Read_File_Result (*this->handler_,
                                                     this->handle_,
                                                     message_block,
                                                     bytes_to_read,
                                                     act,
                                                     offset,
                                                     offset_high,
                                                     this->posix_sig_proactor_->get_handle (),
                                                     priority,
                                                     signal_number),
                  -1);

  return this->shared_read (result);
}

int
ACE_POSIX_SIG_Asynch_Read_File::read (ACE_Message_Block &message_block,
                                      u_long bytes_to_read,
                                      const void *act,
                                      int priority,
                                      int signal_number)
{
  return ACE_POSIX_SIG_Asynch_Read_Stream::read (message_block,
                                                 bytes_to_read,
                                                 act,
                                                 priority,
                                                 signal_number);
}

ACE_POSIX_SIG_Asynch_Read_File::~ACE_POSIX_SIG_Asynch_Read_File (void)
{
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_SIG_Asynch_Read_File::open (ACE_Handler &handler,
                                      ACE_HANDLE handle,
                                      const void *completion_key,
                                      ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_SIG_Asynch_Read_File::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_SIG_Asynch_Read_File::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// ************************************************************

ACE_POSIX_Asynch_Write_File_Result::ACE_POSIX_Asynch_Write_File_Result (ACE_Handler &handler,
                                                                        ACE_HANDLE handle,
                                                                        ACE_Message_Block &message_block,
                                                                        u_long bytes_to_write,
                                                                        const void* act,
                                                                        u_long offset,
                                                                        u_long offset_high,
                                                                        ACE_HANDLE event,
                                                                        int priority,
                                                                        int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Write_Stream_Result_Impl (),
    ACE_Asynch_Write_File_Result_Impl (),
    ACE_POSIX_Asynch_Write_Stream_Result (handler,
                                          handle,
                                          message_block,
                                          bytes_to_write,
                                          act,
                                          event,
                                          priority,
                                          signal_number)
{
  this->aio_offset = offset;
  //
  // @@ Support offset_high with aiocb64.
  //
  ACE_UNUSED_ARG (offset_high);
}

void
ACE_POSIX_Asynch_Write_File_Result::complete (u_long bytes_transferred,
                                              int success,
                                              const void *completion_key,
                                              u_long error)
{
  // Copy the data.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // <error> is available in <aio_resultp.aio_error>
  ACE_UNUSED_ARG (error);

  // Appropriately move the pointers in the message block.
  this->message_block_.rd_ptr (bytes_transferred);

  // Create the interface result class.
  ACE_Asynch_Write_File::Result result (this);

  // Call the application handler.
  this->handler_.handle_write_file (result);
}

ACE_POSIX_Asynch_Write_File_Result::~ACE_POSIX_Asynch_Write_File_Result  (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

u_long
ACE_POSIX_Asynch_Write_File_Result::bytes_transferred (void) const
{
  return ACE_POSIX_Asynch_Result::bytes_transferred ();
}

const void *
ACE_POSIX_Asynch_Write_File_Result::act (void) const
{
  return ACE_POSIX_Asynch_Result::act ();
}

int
ACE_POSIX_Asynch_Write_File_Result::success (void) const
{
  return ACE_POSIX_Asynch_Result::success ();
}

const void *
ACE_POSIX_Asynch_Write_File_Result::completion_key (void) const
{
  return ACE_POSIX_Asynch_Result::completion_key ();
}

u_long
ACE_POSIX_Asynch_Write_File_Result::error (void) const
{
  return ACE_POSIX_Asynch_Result::error ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Write_File_Result::event (void) const
{
  return ACE_POSIX_Asynch_Result::event ();
}

u_long
ACE_POSIX_Asynch_Write_File_Result::offset (void) const
{
  return ACE_POSIX_Asynch_Result::offset ();
}

u_long
ACE_POSIX_Asynch_Write_File_Result::offset_high (void) const
{
  return ACE_POSIX_Asynch_Result::offset_high ();
}

int
ACE_POSIX_Asynch_Write_File_Result::priority (void) const
{
  return ACE_POSIX_Asynch_Result::priority ();
} 

int
ACE_POSIX_Asynch_Write_File_Result::signal_number (void) const
{
  return ACE_POSIX_Asynch_Result::signal_number ();
} 

// The following methods belong to
// ACE_POSIX_Asynch_Write_Stream_Result. They are here to avoid
// dominance warnings. These methods route their call to the 
// ACE_POSIX_Asynch_Write_Stream_Result base class.

u_long
ACE_POSIX_Asynch_Write_File_Result::bytes_to_write (void) const
{
  return ACE_POSIX_Asynch_Write_Stream_Result::bytes_to_write ();
}

ACE_Message_Block &
ACE_POSIX_Asynch_Write_File_Result::message_block (void) const
{
  return ACE_POSIX_Asynch_Write_Stream_Result::message_block ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Write_File_Result::handle (void) const
{
  return ACE_POSIX_Asynch_Write_Stream_Result::handle ();
}

int
ACE_POSIX_Asynch_Write_File_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_POSIX_Asynch_Result::post_completion (proactor);
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Write_File::ACE_POSIX_AIOCB_Asynch_Write_File (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_Asynch_Write_File_Impl (),
    ACE_POSIX_AIOCB_Asynch_Write_Stream (posix_aiocb_proactor)
{
}

int
ACE_POSIX_AIOCB_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                          u_long bytes_to_write,
                                          u_long offset,
                                          u_long offset_high,
                                          const void *act,
                                          int priority,
                                          int signal_number)
{
  ACE_POSIX_Asynch_Write_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Write_File_Result (*this->handler_,
                                                      this->handle_,
                                                      message_block,
                                                      bytes_to_write,
                                                      act,
                                                      offset,
                                                      offset_high,
                                                      this->posix_proactor ()->get_handle (),
                                                      priority,
                                                      signal_number),
                  -1);

  return this->shared_write (result);
}

ACE_POSIX_AIOCB_Asynch_Write_File::~ACE_POSIX_AIOCB_Asynch_Write_File (void)
{
}

int
ACE_POSIX_AIOCB_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                          u_long bytes_to_write,
                                          const void *act,
                                          int priority,
                                          int signal_number)
{
  return ACE_POSIX_AIOCB_Asynch_Write_Stream::write (message_block,
                                                     bytes_to_write,
                                                     act,
                                                     priority,
                                                     signal_number);
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_AIOCB_Asynch_Write_File::open (ACE_Handler &handler,
                                   ACE_HANDLE handle,
                                   const void *completion_key,
                                   ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_AIOCB_Asynch_Write_File::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_AIOCB_Asynch_Write_File::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *********************************************************************

ACE_POSIX_SIG_Asynch_Write_File::ACE_POSIX_SIG_Asynch_Write_File (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Write_Stream_Impl (),
    ACE_Asynch_Write_File_Impl (),
    ACE_POSIX_SIG_Asynch_Write_Stream (posix_sig_proactor)
{
}

int
ACE_POSIX_SIG_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                        u_long bytes_to_write,
                                        u_long offset,
                                        u_long offset_high,
                                        const void *act,
                                        int priority,
                                        int signal_number)
{
  ACE_POSIX_Asynch_Write_File_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Write_File_Result (*this->handler_,
                                                      this->handle_,
                                                      message_block,
                                                      bytes_to_write,
                                                      act,
                                                      offset,
                                                      offset_high,
                                                      this->posix_sig_proactor_->get_handle (),
                                                      priority,
                                                      signal_number),
                  -1);

  return this->shared_write (result);
}

ACE_POSIX_SIG_Asynch_Write_File::~ACE_POSIX_SIG_Asynch_Write_File (void)
{
}

int
ACE_POSIX_SIG_Asynch_Write_File::write (ACE_Message_Block &message_block,
                                        u_long bytes_to_write,
                                        const void *act,
                                        int priority,
                                        int signal_number)
{
  return ACE_POSIX_SIG_Asynch_Write_Stream::write (message_block,
                                                   bytes_to_write,
                                                   act,
                                                   priority,
                                                   signal_number);
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_SIG_Asynch_Write_File::open (ACE_Handler &handler,
                                   ACE_HANDLE handle,
                                   const void *completion_key,
                                   ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_SIG_Asynch_Write_File::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_SIG_Asynch_Write_File::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *********************************************************************

u_long
ACE_POSIX_Asynch_Accept_Result::bytes_to_read (void) const
{
  return this->aio_nbytes;
}

ACE_Message_Block &
ACE_POSIX_Asynch_Accept_Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE
ACE_POSIX_Asynch_Accept_Result::listen_handle (void) const
{
  return this->listen_handle_;
}

ACE_HANDLE
ACE_POSIX_Asynch_Accept_Result::accept_handle (void) const
{
  return this->aio_fildes;
}

ACE_POSIX_Asynch_Accept_Result::ACE_POSIX_Asynch_Accept_Result (ACE_Handler &handler,
                                                                ACE_HANDLE listen_handle,
                                                                ACE_HANDLE accept_handle,
                                                                ACE_Message_Block &message_block,
                                                                u_long bytes_to_read,
                                                                const void* act,
                                                                ACE_HANDLE event,
                                                                int priority,
                                                                int signal_number)

  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Accept_Result_Impl (),
    ACE_POSIX_Asynch_Result (handler, act, event, 0, 0, priority, signal_number),
    message_block_ (message_block),
    listen_handle_ (listen_handle)
{
  this->aio_fildes = accept_handle;
  this->aio_nbytes = bytes_to_read;
}

void
ACE_POSIX_Asynch_Accept_Result::complete (u_long bytes_transferred,
                                          int success,
                                          const void *completion_key,
                                          u_long error)
{
  // Copy the data.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Create the interface result class.
  ACE_Asynch_Accept::Result result (this);

  // Call the application handler.
  this->handler_.handle_accept (result);
}

ACE_POSIX_Asynch_Accept_Result::~ACE_POSIX_Asynch_Accept_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

u_long
ACE_POSIX_Asynch_Accept_Result::bytes_transferred (void) const
{
  return ACE_POSIX_Asynch_Result::bytes_transferred ();
}

const void *
ACE_POSIX_Asynch_Accept_Result::act (void) const
{
  return ACE_POSIX_Asynch_Result::act ();
}

int
ACE_POSIX_Asynch_Accept_Result::success (void) const
{
  return ACE_POSIX_Asynch_Result::success ();
}

const void *
ACE_POSIX_Asynch_Accept_Result::completion_key (void) const
{
  return ACE_POSIX_Asynch_Result::completion_key ();
}

u_long
ACE_POSIX_Asynch_Accept_Result::error (void) const
{
  return ACE_POSIX_Asynch_Result::error ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Accept_Result::event (void) const
{
  return ACE_POSIX_Asynch_Result::event ();
}

u_long
ACE_POSIX_Asynch_Accept_Result::offset (void) const
{
  return ACE_POSIX_Asynch_Result::offset ();
}

u_long
ACE_POSIX_Asynch_Accept_Result::offset_high (void) const
{
  return ACE_POSIX_Asynch_Result::offset_high ();
}

int
ACE_POSIX_Asynch_Accept_Result::priority (void) const
{
  return ACE_POSIX_Asynch_Result::priority ();
}

int
ACE_POSIX_Asynch_Accept_Result::signal_number (void) const
{
  return ACE_POSIX_Asynch_Result::signal_number ();
}

int
ACE_POSIX_Asynch_Accept_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_POSIX_Asynch_Result::post_completion (proactor);
}

// *********************************************************************

class ACE_Export ACE_POSIX_Asynch_Accept_Handler : public ACE_Event_Handler
{
  // = TITLE
  //     For the POSIX implementation, we have two helper classes
  //     (ACE_POSIX_AIOCB_Asynch_Accept_Hander and
  //     ACE_POSIX_SIG_Asynch_Accept_Handler) to do <Asynch_Accept>. This
  //     class abstracts out the commonalities on these two helper classes.
  //
  // = DESCRIPTION
  //
public:
  ~ACE_POSIX_Asynch_Accept_Handler (void);
  // Destructor.

protected:
  ACE_POSIX_Asynch_Accept_Handler (ACE_Reactor* reactor,
                                   ACE_POSIX_Proactor *posix_proactor);
  // Constructor. Give the reactor so that it can activate/deactivate
  // the handlers. Give also the proactor used here, so that the
  // handler can send the <POSIX_Asynch_Accept> result block through
  // <post_completion>. 

  int register_accept_call_i (ACE_POSIX_Asynch_Accept_Result* result);
  // Worker method for registering this <accept> call with the local
  // handler. This method has the common code found between the two
  // differnt implementation subclasses. This method assumes that
  // locks are already obtained to access the shared the queues.

  ACE_POSIX_Asynch_Accept_Result* deregister_accept_call (void);
  // Method for deregistering.

  ACE_Reactor* reactor_;
  // Reactor used by the Asynch_Accept. We need this here to enable
  // and disable the <handle> now and then, depending on whether any
  // <accept> is pending or no.

  ACE_POSIX_Proactor *posix_proactor_;
  // POSIX_Proactor.

  ACE_Unbounded_Queue<ACE_POSIX_Asynch_Accept_Result*> result_queue_;
  // Queue of Result pointers that correspond to all the <accept>'s
  // pending.

  ACE_Thread_Mutex lock_;
  // The lock to protect the  result queue which is shared. The queue
  // is updated by main thread in the register function call and
  // through the auxillary thread  in the deregister fun. So let us
  // mutex it.
};

// *********************************************************************

class ACE_Export ACE_POSIX_AIOCB_Asynch_Accept_Handler : public ACE_POSIX_Asynch_Accept_Handler
{
  // = TITLE
  //     For the POSIX implementation, this class takes care of doing
  //     <Asynch_Accept> for AIOCB strategy.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_AIOCB_Asynch_Accept_Handler (ACE_Reactor *reactor,
                                         ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor);
  // Constructor. Give the reactor so that it can activate/deactivate
  // the handlers. Give also the proactor used here, so that the
  // handler can send information through the notification pipe
  // (<post_completion>). 

  ~ACE_POSIX_AIOCB_Asynch_Accept_Handler (void);
  // Destructor.
  
  int register_accept_call (ACE_POSIX_Asynch_Accept_Result* result);
  // Register this <accept> call with the local handler.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when accept event comes up on the <listen_handle>.
};

// *********************************************************************

class ACE_Export ACE_POSIX_SIG_Asynch_Accept_Handler : public ACE_POSIX_Asynch_Accept_Handler
{
  // = TITLE
  //     For the POSIX implementation, this class takes care of doing
  //     Asynch_Accept.
  //
  // = DESCRIPTION
  //
public:
  ACE_POSIX_SIG_Asynch_Accept_Handler (ACE_Reactor* reactor,
                                       ACE_POSIX_SIG_Proactor *posix_sig_proactor);
  // Constructor. Give the reactor so that it can activate/deactivate
  // the handlers. Give also the proactor used here, so that the
  // handler can send information through <post_completion>.

  ~ACE_POSIX_SIG_Asynch_Accept_Handler (void);
  // Destructor.

  int register_accept_call (ACE_POSIX_Asynch_Accept_Result* result);
  // Register this <accept> call with the local handler.

  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  // Called when accept event comes up on the <listen_handle>.
};

// *********************************************************************

ACE_POSIX_Asynch_Accept_Handler::ACE_POSIX_Asynch_Accept_Handler (ACE_Reactor* reactor,
                                                                  ACE_POSIX_Proactor *posix_proactor)
  : reactor_ (reactor),
    posix_proactor_ (posix_proactor)
{
}

ACE_POSIX_Asynch_Accept_Handler::~ACE_POSIX_Asynch_Accept_Handler (void)
{
}

int
ACE_POSIX_Asynch_Accept_Handler::register_accept_call_i (ACE_POSIX_Asynch_Accept_Result* result)
{
  // Insert this result to the queue.
  int insert_result = this->result_queue_.enqueue_tail (result);
  if (insert_result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:ACE_POSIX_Asynch_Accept_Handler::register_accept_call failed\n"),
                      -1);

  // If this is the only item, then it means there the set was empty
  // before. So enable the <handle> in the reactor.
  if (this->result_queue_.size () == 1)
    {
      int return_val = this->reactor_->resume_handler (result->listen_handle ());
      if (return_val == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:ACE_POSIX_Asynch_Accept_Handler::register_accept_call: "
                           "Reactor::resume_handler failed\n"),
                          -1);
    }
  
  return 0;
}

ACE_POSIX_Asynch_Accept_Result *
ACE_POSIX_Asynch_Accept_Handler::deregister_accept_call (void)
{
  // The queue is updated by main thread in the register function call and
  // thru the auxillary thread  in the deregister fun. So let us mutex
  // it.
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, 0);

  // Get the first item (result ptr) from the Queue.
  ACE_POSIX_Asynch_Accept_Result* result = 0;
  if (this->result_queue_.dequeue_head (result) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):%p\n",
                       "ACE_POSIX_Asynch_Accept_Handler::"
                       "deregister_accept_call:dequeueing failed"),
                      0);

  ACE_ASSERT (result != 0);

  // Disable the <handle> in the reactor if no <accept>'s are pending.
  if (this->result_queue_.size () == 0)
    {
      if (this->reactor_->suspend_handler (result->listen_handle ()) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%N:%l:(%P | %t):%p\n",
                           "ACE_POSIX_Asynch_Accept_Handler::"
                           "deregister_accept_call:suspend handler failed"),
                          0);  
    }

  // Return the result pointer.
  return result;
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Accept_Handler::ACE_POSIX_AIOCB_Asynch_Accept_Handler (ACE_Reactor* reactor,
                                                                              ACE_POSIX_AIOCB_Proactor* posix_aiocb_proactor)
  : ACE_POSIX_Asynch_Accept_Handler (reactor, posix_aiocb_proactor)
{
}

ACE_POSIX_AIOCB_Asynch_Accept_Handler::~ACE_POSIX_AIOCB_Asynch_Accept_Handler (void)
{
}

int
ACE_POSIX_AIOCB_Asynch_Accept_Handler::register_accept_call (ACE_POSIX_Asynch_Accept_Result* result)
{
  // The queue is updated by main thread in the register function call
  // and thru the auxillary thread in the deregister fun. So let us
  // mutex it.
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);

  return register_accept_call_i (result);
}

int
ACE_POSIX_AIOCB_Asynch_Accept_Handler::handle_input (ACE_HANDLE /* fd */)
{
  // An <accept> has been sensed on the <listen_handle>. We should be
  // able to just go ahead and do the <accept> now on this <fd>. This
  // should be the same as the <listen_handle>.

  // Deregister this info pertaining to this <accept> call.
  ACE_POSIX_Asynch_Accept_Result* result = this->deregister_accept_call ();
  if (result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):%p\n",
                       "ACE_POSIX_AIOCB_Asynch_Accept_Handler::"
                       "handle_input:deregister_accept_call failed"),
                      -1);

  // Issue <accept> now.
  // @@ We shouldnt block here since we have already done poll/select
  // thru reactor. But are we sure?
  ACE_HANDLE new_handle = ACE_OS::accept (result->listen_handle (), 0, 0);
  if (new_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):%p\n",
                       "ACE_POSIX_AIOCB_Asynch_Accept_Handler::"
                       "handle_input:<accept> system call failed"),
                      -1);

  // Accept has completed.

  // Store the new handle.
  result->aio_fildes = new_handle;

  // Notify the main process about this completion
  // Send the Result through the notification pipe.
  if (this->posix_proactor_->post_completion (result) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "ACE_POSIX_AIOCB_Asynch_Accept_Handler::"
                       "handle_input:<post_completion> failed"),
                      -1);
  
  return 0;
}

// *********************************************************************

ACE_POSIX_SIG_Asynch_Accept_Handler::ACE_POSIX_SIG_Asynch_Accept_Handler (ACE_Reactor* reactor,
                                                                          ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_POSIX_Asynch_Accept_Handler (reactor, posix_sig_proactor)
{
}

ACE_POSIX_SIG_Asynch_Accept_Handler::~ACE_POSIX_SIG_Asynch_Accept_Handler (void)
{
}

int
ACE_POSIX_SIG_Asynch_Accept_Handler::register_accept_call (ACE_POSIX_Asynch_Accept_Result* result)
{
  // The queue is updated by main thread in the register function call
  // and thru the auxillary thread in the deregister fun. So let us
  // mutex it.
  ACE_GUARD_RETURN (ACE_Thread_Mutex, ace_mon, this->lock_, -1);
  
  // Do the work.
  if (this->register_accept_call_i (result) == -1)
    return -1;
  
  return 0;
}

int
ACE_POSIX_SIG_Asynch_Accept_Handler::handle_input (ACE_HANDLE /* fd */)
{
  // An <accept> has been sensed on the <listen_handle>. We should be
  // able to just go ahead and do the <accept> now on this <fd>. This
  // should be the same as the <listen_handle>.
  
  // Deregister this info pertaining to this <accept> call.
  ACE_POSIX_Asynch_Accept_Result* result = this->deregister_accept_call ();
  if (result == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:(%P | %t):%p\n",
                       "ACE_POSIX_SIG_Asynch_Accept_Handler::"
                       "handle_input:deregister_accept_call failed"),
                      -1);
  
  // Issue <accept> now.
  // @@ We shouldnt block here since we have already done poll/select
  // thru reactor. But are we sure?
  ACE_HANDLE new_handle = ACE_OS::accept (result->listen_handle (), 0, 0);
  if (new_handle == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "ACE_POSIX_SIG_Asynch_Accept_Handler::"
                       "handle_input:<accept> system call failed"),
                      -1);

  // Accept has completed.
  
  // Store the new handle.
  result->aio_fildes = new_handle;

  // Notify the main process about this completion.
  if (this->posix_proactor_->post_completion (result) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:(%P | %t):%p\n",
                       "ACE_POSIX_SIG_Asynch_Accept_Handler::"
                       "handle_input:<post_completion> failed"),
                      -1);
  
  return 0;
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Accept::ACE_POSIX_AIOCB_Asynch_Accept (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Accept_Impl (),
    ACE_POSIX_AIOCB_Asynch_Operation (posix_aiocb_proactor),
    accept_handler_ (0)
{
}

int
ACE_POSIX_AIOCB_Asynch_Accept::accept (ACE_Message_Block &message_block,
                                       u_long bytes_to_read,
                                       ACE_HANDLE accept_handle,
                                       const void *act,
                                       int priority,
                                       int signal_number)
{
  // Sanity check: make sure that enough space has been allocated by
  // the caller.
  size_t address_size = sizeof (sockaddr_in) + sizeof (sockaddr);
  size_t space_in_use = message_block.wr_ptr () - message_block.base ();
  size_t total_size = message_block.size ();
  size_t available_space = total_size - space_in_use;
  size_t space_needed = bytes_to_read + 2 * address_size;
  if (available_space < space_needed)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("Buffer too small\n")), -1);

  // Common code for both WIN and POSIX.
  ACE_POSIX_Asynch_Accept_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Accept_Result (*this->handler_,
                                                  this->handle_,
                                                  accept_handle,
                                                  message_block,
                                                  bytes_to_read,
                                                  act,
                                                  this->posix_proactor ()->get_handle (),
                                                  priority,
                                                  signal_number),
                  -1);

  // Register this <accept> call with the local handler.
  if (this->accept_handler_->register_accept_call (result) == -1)
    return -1;
  
  return 0;
}

int
ACE_POSIX_AIOCB_Asynch_Accept::open (ACE_Handler &handler,
                                     ACE_HANDLE handle,
                                     const void *completion_key,
                                     ACE_Proactor *proactor)
{
  int result = ACE_POSIX_Asynch_Operation::open (handler,
                                                 handle,
                                                 completion_key,
                                                 proactor);
  if (result == -1)
    return result;

  // Init the Asynch_Accept_Handler now. It needs to keep Proactor
  // also with it.
  ACE_NEW_RETURN (this->accept_handler_,
                  ACE_POSIX_AIOCB_Asynch_Accept_Handler (&this->reactor_,
                                                         this->posix_proactor ()),
                  -1);

  // Register the handle with the reactor.
  int return_val = this->reactor_.register_handler (this->handle_,
                                                    this->accept_handler_,
                                                    ACE_Event_Handler::ACCEPT_MASK);
  if (return_val == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:Reactor::register_handler failed\n"),
                      -1);

  // Suspend the <handle> now. Enable only when the <accept> is issued
  // by the application.
  return_val = this->reactor_.suspend_handler (this->handle_);
  if (return_val == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:Reactor::suspend_handler failed\n"),
                      -1);

  // Spawn the thread. It is the only thread we are going to have. It
  // will do the <handle_events> on the reactor.
  return_val = ACE_Thread_Manager::instance ()->spawn (ACE_POSIX_AIOCB_Asynch_Accept::thread_function,
                                                       (void *) &this->reactor_);
  if (return_val  == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:Thread_Manager::spawn failed\n"),
                      -1);

  return 0;
}

ACE_POSIX_AIOCB_Asynch_Accept::~ACE_POSIX_AIOCB_Asynch_Accept (void)
{
}

void*
ACE_POSIX_AIOCB_Asynch_Accept::thread_function (void* arg_reactor)
{
  ACE_DEBUG ((LM_DEBUG, "ACE_Asynch_Accept::thread_function called\n"));

  // Retrieve the reactor pointer from the argument.
  ACE_Reactor* reactor = (ACE_Reactor *) arg_reactor;

  // It should be valid Reactor, since we have a reactor_ ,e,ner we
  // are passing only that one here.
  if (reactor == 0)
    ACE_ERROR ((LM_ERROR,
                "%n:%l:Invalid Reactor pointer passed to the thread_function\n",
                0));

  // For this reactor, this thread is the owner.
  reactor->owner (ACE_OS::thr_self ());

  // Handle events.
  int result = 0;
  while (result != -1)
    {
      result = reactor->handle_events ();
      ACE_DEBUG ((LM_DEBUG,
                  "ACE_Asynch_Accept::Thread_Function : handle_events : result = [%d]\n",
                  result));
    }

  ACE_DEBUG ((LM_DEBUG, "Exiting ACE_Asynch_Accept::thread_function \n"));

  return 0;
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_AIOCB_Asynch_Accept::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_AIOCB_Asynch_Accept::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *********************************************************************

ACE_POSIX_SIG_Asynch_Accept::ACE_POSIX_SIG_Asynch_Accept (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Accept_Impl (),
    ACE_POSIX_SIG_Asynch_Operation (posix_sig_proactor),
    accept_handler_ (0)
{
}

int
ACE_POSIX_SIG_Asynch_Accept::accept (ACE_Message_Block &message_block,
                                     u_long bytes_to_read,
                                     ACE_HANDLE accept_handle,
                                     const void *act,
                                     int priority,
                                     int signal_number)
{
  // Sanity check: make sure that enough space has been allocated by
  // the caller.
  size_t address_size = sizeof (sockaddr_in) + sizeof (sockaddr);
  size_t space_in_use = message_block.wr_ptr () - message_block.base ();
  size_t total_size = message_block.size ();
  size_t available_space = total_size - space_in_use;
  size_t space_needed = bytes_to_read + 2 * address_size;
  if (available_space < space_needed)
    ACE_ERROR_RETURN ((LM_ERROR, ASYS_TEXT ("Buffer too small\n")), -1);

  // Common code for both WIN and POSIX.
  ACE_POSIX_Asynch_Accept_Result *result = 0;
  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Accept_Result (*this->handler_,
                                                  this->handle_,
                                                  accept_handle,
                                                  message_block,
                                                  bytes_to_read,
                                                  act,
                                                  this->posix_sig_proactor_->get_handle (),
                                                  priority,
                                                  signal_number),
                  -1);

  // Register this <accept> call with the local handler.
  this->accept_handler_->register_accept_call (result);

  return 0;
}


int
ACE_POSIX_SIG_Asynch_Accept::open (ACE_Handler &handler,
                                     ACE_HANDLE handle,
                                     const void *completion_key,
                                     ACE_Proactor *proactor)
{
  int result = ACE_POSIX_Asynch_Operation::open (handler,
                                                 handle,
                                                 completion_key,
                                                 proactor);
  if (result == -1)
    return result;

  // Init the Asynch_Accept_Handler now. It needs to keep Proactor
  // also with it.
  ACE_NEW_RETURN (this->accept_handler_,
                  ACE_POSIX_SIG_Asynch_Accept_Handler (&this->reactor_,
                                                       this->posix_proactor ()),
                  -1);

  // Register the handle with the reactor.
  int return_val = this->reactor_.register_handler (this->handle_,
                                                    this->accept_handler_,
                                                    ACE_Event_Handler::ACCEPT_MASK);
  if (return_val == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:Reactor::register_handler failed\n"),
                      -1);

  // Suspend the <handle> now. Enable only when the <accept> is issued
  // by the application.
  return_val = this->reactor_.suspend_handler (this->handle_);
  if (return_val == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:Reactor::suspend_handler failed\n"),
                      -1);

  // Spawn the thread. It is the only thread we are going to have. It
  // will do the <handle_events> on the reactor.
  return_val = ACE_Thread_Manager::instance ()->spawn (ACE_POSIX_SIG_Asynch_Accept::thread_function,
                                                       (void *)&this->reactor_);
  if (return_val  == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%N:%l:Thread_Manager::spawn failed\n"),
                      -1);

  return 0;
}

ACE_POSIX_SIG_Asynch_Accept::~ACE_POSIX_SIG_Asynch_Accept (void)
{
}

void*
ACE_POSIX_SIG_Asynch_Accept::thread_function (void* arg_reactor)
{
  // Retrieve the reactor pointer from the argument.
  ACE_Reactor* reactor = (ACE_Reactor *) arg_reactor;
  if (reactor == 0)
    reactor = ACE_Reactor::instance ();

  // For this reactor, this thread is the owner.
  reactor->owner (ACE_OS::thr_self ());

  // Handle events. Wait for any connection events. 
  int result = 0;
  while (result != -1)
    result = reactor->handle_events ();
  
  return 0;
}

// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route
// the call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_SIG_Asynch_Accept::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_SIG_Asynch_Accept::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
}

// *********************************************************************

ACE_HANDLE
ACE_POSIX_Asynch_Transmit_File_Result::socket (void) const
{
  return this->socket_;
}

ACE_HANDLE
ACE_POSIX_Asynch_Transmit_File_Result::file (void) const
{
  return this->aio_fildes;
}

ACE_Asynch_Transmit_File::Header_And_Trailer *
ACE_POSIX_Asynch_Transmit_File_Result::header_and_trailer (void) const
{
  return this->header_and_trailer_;
}

u_long
ACE_POSIX_Asynch_Transmit_File_Result::bytes_to_write (void) const
{
  return this->aio_nbytes;
}

u_long
ACE_POSIX_Asynch_Transmit_File_Result::bytes_per_send (void) const
{
  return this->bytes_per_send_;
}

u_long
ACE_POSIX_Asynch_Transmit_File_Result::flags (void) const
{
  return this->flags_;
}

ACE_POSIX_Asynch_Transmit_File_Result::ACE_POSIX_Asynch_Transmit_File_Result (ACE_Handler &handler,
                                                                              ACE_HANDLE socket,
                                                                              ACE_HANDLE file,
                                                                              ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                                                              u_long bytes_to_write,
                                                                              u_long offset,
                                                                              u_long offset_high,
                                                                              u_long bytes_per_send,
                                                                              u_long flags,
                                                                              const void *act,
                                                                              ACE_HANDLE event,
                                                                              int priority,
                                                                              int signal_number)
  : ACE_Asynch_Result_Impl (),
    ACE_Asynch_Transmit_File_Result_Impl (),
    ACE_POSIX_Asynch_Result (handler, act, event, offset, offset_high, priority, signal_number),
    socket_ (socket),
    header_and_trailer_ (header_and_trailer),
    bytes_per_send_ (bytes_per_send),
    flags_ (flags)
{
  this->aio_fildes = file;
  this->aio_nbytes = bytes_to_write;
}

void
ACE_POSIX_Asynch_Transmit_File_Result::complete (u_long bytes_transferred,
                                                 int success,
                                                 const void *completion_key,
                                                 u_long error)
{
  // Copy the data.
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // We will not do this because (a) the header and trailer blocks may
  // be the same message_blocks and (b) in cases of failures we have
  // no idea how much of what (header, data, trailer) was sent.
  /*
    if (this->success_ && this->header_and_trailer_ != 0)
    {
    ACE_Message_Block *header = this->header_and_trailer_->header ();
    if (header != 0)
    header->rd_ptr (this->header_and_trailer_->header_bytes ());

    ACE_Message_Block *trailer = this->header_and_trailer_->trailer ();
    if (trailer != 0)
    trailer->rd_ptr (this->header_and_trailer_->trailer_bytes ());
    }
  */

  // Create the interface result class.
  ACE_Asynch_Transmit_File::Result result (this);

  // Call the application handler.
  this->handler_.handle_transmit_file (result);
}

ACE_POSIX_Asynch_Transmit_File_Result::~ACE_POSIX_Asynch_Transmit_File_Result (void)
{
}

// Base class operations. These operations are here to kill dominance
// warnings. These methods call the base class methods.

u_long
ACE_POSIX_Asynch_Transmit_File_Result::bytes_transferred (void) const
{
  return ACE_POSIX_Asynch_Result::bytes_transferred ();
}

const void *
ACE_POSIX_Asynch_Transmit_File_Result::act (void) const
{
  return ACE_POSIX_Asynch_Result::act ();
}

int
ACE_POSIX_Asynch_Transmit_File_Result::success (void) const
{
  return ACE_POSIX_Asynch_Result::success ();
}

const void *
ACE_POSIX_Asynch_Transmit_File_Result::completion_key (void) const
{
  return ACE_POSIX_Asynch_Result::completion_key ();
}

u_long
ACE_POSIX_Asynch_Transmit_File_Result::error (void) const
{
  return ACE_POSIX_Asynch_Result::error ();
}

ACE_HANDLE
ACE_POSIX_Asynch_Transmit_File_Result::event (void) const
{
  return ACE_POSIX_Asynch_Result::event ();
}

u_long
ACE_POSIX_Asynch_Transmit_File_Result::offset (void) const
{
  return ACE_POSIX_Asynch_Result::offset ();
}

u_long
ACE_POSIX_Asynch_Transmit_File_Result::offset_high (void) const
{
  return ACE_POSIX_Asynch_Result::offset_high ();
}

int
ACE_POSIX_Asynch_Transmit_File_Result::priority (void) const
{
  return ACE_POSIX_Asynch_Result::priority ();
}

int
ACE_POSIX_Asynch_Transmit_File_Result::signal_number (void) const
{
  return ACE_POSIX_Asynch_Result::signal_number ();
}

int
ACE_POSIX_Asynch_Transmit_File_Result::post_completion (ACE_Proactor_Impl *proactor)
{
  return ACE_POSIX_Asynch_Result::post_completion (proactor);
}

// *********************************************************************

class ACE_Export ACE_POSIX_Asynch_Transmit_Handler : public ACE_Handler
{
  // = TITLE
  //
  //     Auxillary handler for doing <Asynch_Transmit_File> in
  //     Unix. <ACE_POSIX_Asynch_Transmit_File> internally uses this.
  //
  // = DESCRIPTION
  //
  //     This is a helper class for implementing
  //     <ACE_POSIX_Asynch_Transmit_File> in Unix systems. This class
  //     abstracts out all the commonalities in the two different
  //     POSIX Transmit Handler implementations.

public:
  virtual ~ACE_POSIX_Asynch_Transmit_Handler (void);
  // Destructor.

protected:
  ACE_POSIX_Asynch_Transmit_Handler (ACE_POSIX_Asynch_Transmit_File_Result *result);
  // Constructor. Result pointer will have all the information to do
  // the file transmission (socket, file, application handler, bytes
  // to write).

  ACE_POSIX_Asynch_Transmit_File_Result *result_;
  // The asynch result pointer made from the initial transmit file
  // request.

  ACE_Message_Block *mb_;
  // Message bloack used to do the transmission.

  enum ACT
  {
    HEADER_ACT  = 1,
    DATA_ACT    = 2,
    TRAILER_ACT = 3
  };

  ACT header_act_;
  ACT data_act_;
  ACT trailer_act_;
  // ACT to transmit header, data and trailer.

  size_t file_offset_;
  // Current offset of the file being transmitted.

  size_t file_size_;
  // Total size of the file.

  size_t bytes_transferred_;
  // Number of bytes transferred on the stream.
};

// ************************************************************

class ACE_Export ACE_POSIX_AIOCB_Asynch_Transmit_Handler : public ACE_POSIX_Asynch_Transmit_Handler
{
  // = TITLE
  //
  //     Auxillary handler for doing <Asynch_Transmit_File> in
  //     Unix. <ACE_POSIX_Asynch_Transmit_File> internally uses this.
  //
  // = DESCRIPTION
  //
  //     This is a helper class for implementing
  //     <ACE_POSIX_Asynch_Transmit_File> in Unix systems.

public:
  ACE_POSIX_AIOCB_Asynch_Transmit_Handler (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor,
                                           ACE_POSIX_Asynch_Transmit_File_Result *result);
  // Constructor. Result pointer will have all the information to do
  // the file transmission (socket, file, application handler, bytes
  // to write).

  virtual ~ACE_POSIX_AIOCB_Asynch_Transmit_Handler (void);
  // Destructor.

  int transmit (void);
  // Do the transmission. All the info to do the transmission is in
  // the <result> member.

protected:
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete.

  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This is called when asynchronous reads from the file complete.

  int initiate_read_file (void);
  // Issue asynch read from  the file.

  ACE_POSIX_AIOCB_Asynch_Read_File rf_;
  // To read from the file to be transmitted.

  ACE_POSIX_AIOCB_Asynch_Write_Stream ws_;
  // Write stream to write the header, trailer and the data.
};

// *********************************************************************

class ACE_Export ACE_POSIX_SIG_Asynch_Transmit_Handler : public ACE_POSIX_Asynch_Transmit_Handler
{
  // = TITLE
  //     Auxillary handler for doing <Asynch_Transmit_File> in
  //     Unix. <ACE_POSIX_Asynch_Transmit_File> internally uses this.
  //
  // = DESCRIPTION
  //     This is a helper class for implementing
  //     <ACE_POSIX_Asynch_Transmit_File> in Unix systems.
public:
  ACE_POSIX_SIG_Asynch_Transmit_Handler (ACE_POSIX_SIG_Proactor *proactor,
                                         ACE_POSIX_Asynch_Transmit_File_Result *result);
  // Constructor. Result pointer will have all the information to do
  // the file transmission (socket, file, application handler, bytes
  // to write).

  virtual ~ACE_POSIX_SIG_Asynch_Transmit_Handler (void);
  // Destructor.

  int transmit (void);
  // Do the transmission. All the info to do the transmission is in
  // the <result> member.

protected:
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This is called when asynchronous writes from the socket complete.

  virtual void handle_read_file (const ACE_Asynch_Read_File::Result &result);
  // This is called when asynchronous reads from the file complete.

  int initiate_read_file (void);
  // Issue asynch read from  the file.

  ACE_POSIX_SIG_Asynch_Read_File rf_;
  // To read from the file to be transmitted.

  ACE_POSIX_SIG_Asynch_Write_Stream ws_;
  // Write stream to write the header, trailer and the data.
};

// *********************************************************************

// Constructor.
ACE_POSIX_Asynch_Transmit_Handler::ACE_POSIX_Asynch_Transmit_Handler (ACE_POSIX_Asynch_Transmit_File_Result *result)
  : result_ (result),
    mb_ (0),
    header_act_ (this->HEADER_ACT),
    data_act_ (this->DATA_ACT),
    trailer_act_ (this->TRAILER_ACT),
    file_offset_ (result->offset ()),
    file_size_ (0),
    bytes_transferred_ (0)
{
  // Allocate memory for the message block.
  ACE_NEW (this->mb_,
           ACE_Message_Block (this->result_->bytes_per_send ()
                              + 1));
  // Init the file size.
  file_size_ = ACE_OS::filesize (this->result_->file ());
}

// Destructor.
ACE_POSIX_Asynch_Transmit_Handler::~ACE_POSIX_Asynch_Transmit_Handler (void)
{
  delete result_;
  mb_->release ();
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Transmit_Handler::ACE_POSIX_AIOCB_Asynch_Transmit_Handler (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor,
                                                                                  ACE_POSIX_Asynch_Transmit_File_Result *result)
  : ACE_POSIX_Asynch_Transmit_Handler (result),
    rf_ (posix_aiocb_proactor),
    ws_ (posix_aiocb_proactor)
{
}

ACE_POSIX_AIOCB_Asynch_Transmit_Handler::~ACE_POSIX_AIOCB_Asynch_Transmit_Handler (void)
{
}

// Do the transmission.
// Initiate transmitting the header. When that completes
// handle_write_stream will be called, there start transmitting the file.
int
ACE_POSIX_AIOCB_Asynch_Transmit_Handler::transmit (void)
{
  // No proactor is given for the <open>'s. Because we are using the
  // concrete implementations of the  Asynch_Operations, and we have
  // already given them the specific proactor, so they wont need the
  // general <proactor> interface pointer.

  // Open Asynch_Read_File.
  if (this->rf_.open (*this,
                      this->result_->file (),
                      0,
                      0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Asynch_Transmit_Handler:read_file open failed\n"),
                      -1);

  // Open Asynch_Write_Stream.
  if (this->ws_.open (*this,
                      this->result_->socket (),
                      0,
                      0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Asynch_Transmit_Handler:write_stream open failed\n"),
                      -1);

  // Transmit the header.
  if (this->ws_.write (*this->result_->header_and_trailer ()->header (),
                       this->result_->header_and_trailer ()->header_bytes (),
                       (void *) &this->header_act_,
                       0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Asynch_Transmit_Handler:transmitting header:write_stream failed\n"),
                      -1);
  return 0;
}

void
ACE_POSIX_AIOCB_Asynch_Transmit_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  // Update bytes transferred so far.
  this->bytes_transferred_ += result.bytes_transferred ();

  // Check the success parameter.
  if (result.success () == 0)
    {
      // Failure.
      ACE_ERROR ((LM_ERROR,
                  "Asynch_Transmit_File failed.\n"));
      
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   0,      // Failure.
                                   0,      // @@ Completion key.
                                   0);     // @@ Error no.
        }
      ACE_SEH_FINALLY
        {
          // This is crucial to prevent memory leaks. This deletes
          // the result pointer also.
          delete this;
        }
    }
  
  // Write stream successful.

  // Partial write to socket.
  int unsent_data = result.bytes_to_write () - result.bytes_transferred ();
  if (unsent_data != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "%N:%l:Partial write to socket: Asynch_write called again\n"));

      // Duplicate the message block and retry remaining data
      if (this->ws_.write (*result.message_block ().duplicate (),
                           unsent_data,
                           result.act (),
                           this->result_->priority (),
                           this->result_->signal_number ()) == -1)
        {
          // @@ Handle this error.
          ACE_ERROR ((LM_ERROR,
                      "Asynch_Transmit_Handler:write_stream failed\n"));
          return;
        }

      // @@ Handling *partial write* to a socket.  Let us not continue
      // further before this write finishes. Because proceeding with
      // another read and then write might change the order of the
      // file transmission, because partial write to the stream is
      // always possible.
      return;
    }

  // Not a partial write. A full write.

  // Check ACT to see what was sent.
  ACT act = *(ACT *) result.act ();

  switch (act)
    {
    case TRAILER_ACT:
      // If it is the "trailer" that is just sent, then transmit file
      // is complete.
      // Call the application handler.
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   1,      // @@ Success.
                                   0,      // @@ Completion key.
                                   0);     // @@ Errno.
        }
      ACE_SEH_FINALLY
        {
          delete this;
        }
      break;

    case HEADER_ACT:
    case DATA_ACT:
      // If header/data was sent, initiate the file data transmission.
      if (this->initiate_read_file () == -1)
        // @@ Handle this error.
        ACE_ERROR ((LM_ERROR,
                    "Error:Asynch_Transmit_Handler:read_file couldnt be initiated\n"));
      break;

    default:
      // @@ Handle this error.
      ACE_ERROR ((LM_ERROR,
                  "Error:ACE_Asynch_Transmit_Handler::handle_write_stream::Unexpected act\n"));
    }
}

void
ACE_POSIX_AIOCB_Asynch_Transmit_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  // Failure.
  if (result.success () == 0)
    {
      //
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   0,      // Failure.
                                   0,      // @@ Completion key.
                                   errno); // Error no.
        }
      ACE_SEH_FINALLY
        {
          delete this;
        }
      return;
    }

  // Read successful.
  if (result.bytes_transferred () == 0)
    return;

  // Increment offset.
  this->file_offset_ += result.bytes_transferred ();

  // Write data to network.
  if (this->ws_.write (result.message_block (),
                       result.bytes_transferred (),
                       (void *)&this->data_act_,
                       this->result_->priority (),
                       this->result_->signal_number ()) == -1)
    {
      // @@ Handle this error.
      ACE_ERROR ((LM_ERROR,
                  "Error:ACE_Asynch_Transmit_File : write to the stream failed\n"));
      return;
    }
}

int
ACE_POSIX_AIOCB_Asynch_Transmit_Handler::initiate_read_file (void)
{
  // Is there something to read.
  if (this->file_offset_ >= this->file_size_)
    {
      // File is sent. Send the trailer.
      if (this->ws_.write (*this->result_->header_and_trailer ()->trailer (),
                           this->result_->header_and_trailer ()->trailer_bytes (),
                           (void *)&this->trailer_act_,
                           this->result_->priority (),
                           this->result_->signal_number ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:Asynch_Transmit_Handler:write_stream writing trailer failed\n"),
                          -1);
      return 0;
    }
  else
    {
      // @@ Is this right??
      // Previous reads and writes are over. For the new read, adjust
      // the wr_ptr and the rd_ptr to the beginning.
      this->mb_->rd_ptr (this->mb_->base ());
      this->mb_->wr_ptr (this->mb_->base ());

      // Inititiate an asynchronous read from the file.
      if (this->rf_.read (*this->mb_,
                          this->mb_->size () - 1,
                          this->file_offset_,
                          0, // @@ offset_high !!! if aiocb64 is used.
                          0, // Act
                          this->result_->priority (),
                          this->result_->signal_number ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:Asynch_Transmit_Handler::read from file failed\n"),
                          -1);
      return 0;
    }
}

// *********************************************************************

ACE_POSIX_SIG_Asynch_Transmit_Handler::ACE_POSIX_SIG_Asynch_Transmit_Handler (ACE_POSIX_SIG_Proactor *posix_sig_proactor,
                                                                              ACE_POSIX_Asynch_Transmit_File_Result *result)
  : ACE_POSIX_Asynch_Transmit_Handler (result),
    rf_ (posix_sig_proactor),
    ws_ (posix_sig_proactor)
{
}

ACE_POSIX_SIG_Asynch_Transmit_Handler::~ACE_POSIX_SIG_Asynch_Transmit_Handler (void)
{
}

// Do the transmission.
// Initiate transmitting the header. When that completes
// handle_write_stream will be called, there start transmitting the file.
int
ACE_POSIX_SIG_Asynch_Transmit_Handler::transmit (void)
{
  // The Proactor given for the <open>'s is not going to be
  // used. Because we are using the
  // concrete implementations of the  Asynch_Operations, and we have
  // already given them the specific proactor, so they wont need the
  // general <proactor> interface pointer.

  // Open Asynch_Read_File.
  if (this->rf_.open (*this,
                      this->result_->file (),
                      this->result_->completion_key ()) // Completion key
      == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Asynch_Transmit_Handler:read_file open failed\n"),
                      -1);

  // Open Asynch_Write_Stream.
  if (this->ws_.open (*this,
                      this->result_->socket (),
                      this->result_->completion_key ())  // Completion key
      == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "ACE_Asynch_Transmit_Handler:write_stream open failed\n"),
                      -1);

  // Transmit the header.
  if (this->ws_.write (*this->result_->header_and_trailer ()->header (),
                       this->result_->header_and_trailer ()->header_bytes (),
                       (void *) &this->header_act_,
                       this->result_->priority (),
                       this->result_->signal_number ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Asynch_Transmit_Handler:transmitting header:write_stream failed\n"),
                      -1);
  return 0;
}

void
ACE_POSIX_SIG_Asynch_Transmit_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  // Update bytes transferred so far.
  this->bytes_transferred_ += result.bytes_transferred ();

  // Check the success parameter.
  if (result.success () == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Asynch_Transmit_File failed.\n"));

      // Check the success parameter.
      if (result.success () == 0)
        {
          // Failure.
          ACE_ERROR ((LM_ERROR,
                      "Asynch_Transmit_File failed.\n"));

          ACE_SEH_TRY
            {
              this->result_->complete (this->bytes_transferred_,
                                       0,      // Failure.
                                       0,      // @@ Completion key.
                                       0);     // @@ Error no.
            }
          ACE_SEH_FINALLY
            {
              // This is crucial to prevent memory leaks. This deletes
              // the result pointer also.
              delete this;
            }
        }
    }

  // Write stream successful.

  // Partial write to socket.
  int unsent_data = result.bytes_to_write () - result.bytes_transferred ();
  if (unsent_data != 0)
    {
      // Reset pointers.
      result.message_block ().rd_ptr (result.bytes_transferred ());

      // Duplicate the message block and retry remaining data
      if (this->ws_.write (*result.message_block ().duplicate (),
                           unsent_data,
                           result.act (),
                           result.priority (),
                           this->result_->signal_number ()) == -1)
        {
          // @@ Handle this error.
          ACE_ERROR ((LM_ERROR,
                      "Asynch_Transmit_Handler:write_stream failed\n"));
          return;
        }

      // @@ Handling *partial write* to a socket.  Let us not continue
      // further before this write finishes. Because proceeding with
      // another read and then write might change the order of the
      // file transmission, because partial write to the stream is
      // always possible.
      return;
    }

  // Not a partial write. A full write.

  // Check ACT to see what was sent.
  ACT act = *(ACT *) result.act ();

  switch (act)
    {
    case TRAILER_ACT:
      // If it is the "trailer" that is just sent, then transmit file
      // is complete.
      // Call the application handler.
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   1,      // @@ Success.
                                   0,      // @@ Completion key.
                                   0);     // @@ Errno.
        }
      ACE_SEH_FINALLY
        {
          delete this;
        }
      break;

    case HEADER_ACT:
    case DATA_ACT:
      // If header/data was sent, initiate the file data transmission.
      if (this->initiate_read_file () == -1)
        // @@ Handle this error.
        ACE_ERROR ((LM_ERROR,
                    "Error:Asynch_Transmit_Handler:read_file couldnt be initiated\n"));
      break;

    default:
      // @@ Handle this error.
      ACE_ERROR ((LM_ERROR,
                  "Error:ACE_Asynch_Transmit_Handler::handle_write_stream::Unexpected act\n"));
    }
}

void
ACE_POSIX_SIG_Asynch_Transmit_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  // Failure.
  if (result.success () == 0)
    {
      //
      ACE_SEH_TRY
        {
          this->result_->complete (this->bytes_transferred_,
                                   0,      // Failure.
                                   0,      // @@ Completion key.
                                   errno); // Error no.
        }
      ACE_SEH_FINALLY
        {
          delete this;
        }
      return;
    }

  // Read successful.
  if (result.bytes_transferred () == 0)
    return;

  // Increment offset and write data to network.
  this->file_offset_ += result.bytes_transferred ();
  if (this->ws_.write (result.message_block (),
                       result.bytes_transferred (),
                       (void *)&this->data_act_,
                       result.priority (),
                       result.signal_number ()) == -1)
    {
      // @@ Handle this error.
      ACE_ERROR ((LM_ERROR,
                  "Error:ACE_Asynch_Transmit_File : write to the stream failed\n"));
      return;
    }
}

int
ACE_POSIX_SIG_Asynch_Transmit_Handler::initiate_read_file (void)
{
  // Is there something to read.
  if (this->file_offset_ >= this->file_size_)
    {
      // File is sent. Send the trailer.
      if (this->ws_.write (*this->result_->header_and_trailer ()->trailer (),
                           this->result_->header_and_trailer ()->trailer_bytes (),
                           (void *)&this->trailer_act_,
                           this->result_->priority (),
                           this->result_->signal_number ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:Asynch_Transmit_Handler:write_stream writing trailer failed\n"),
                          -1);
      return 0;
    }
  else
    {
      // Inititiate an asynchronous read from the file.
      if (this->rf_.read (*this->mb_,
                          this->mb_->size () - 1,
                          this->file_offset_,
                          0,    // @@, offset_high, not implemented.
                          0,    // ACT
                          this->result_->priority (),
                          this->result_->signal_number ()) == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Error:Asynch_Transmit_Handler::read from file failed\n"),
                          -1);
      return 0;
    }
}

// *********************************************************************

ACE_POSIX_AIOCB_Asynch_Transmit_File::ACE_POSIX_AIOCB_Asynch_Transmit_File (ACE_POSIX_AIOCB_Proactor *posix_aiocb_proactor)
  : ACE_Asynch_Operation_Impl (),
    ACE_Asynch_Transmit_File_Impl (),
    ACE_POSIX_AIOCB_Asynch_Operation (posix_aiocb_proactor)
{
}

int
ACE_POSIX_AIOCB_Asynch_Transmit_File::transmit_file (ACE_HANDLE file,
                                                     ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                                     u_long bytes_to_write,
                                                     u_long offset,
                                                     u_long offset_high,
                                                     u_long bytes_per_send,
                                                     u_long flags,
                                                     const void *act,
                                                     int priority,
                                                     int signal_number)
{
  // Adjust these parameters if there are default values specified.
  ssize_t file_size = ACE_OS::filesize (file);

  if (file_size == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%N:%l:%p\n",
                       "POSIX_AIOCB_Asynch_Transmit_File:filesize failed"),
                      -1);

  if (bytes_to_write == 0)
    bytes_to_write = file_size;

  if (offset > (size_t) file_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Asynch_Transmit_File:File size is less than offset"),
                      -1);

  if (offset != 0)
    bytes_to_write = file_size - offset + 1;

  if (bytes_per_send == 0)
    bytes_per_send = bytes_to_write;

  // Configure the result parameter.
  ACE_POSIX_Asynch_Transmit_File_Result *result = 0;

  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Transmit_File_Result (*this->handler_,
                                                         this->handle_,
                                                         file,
                                                         header_and_trailer,
                                                         bytes_to_write,
                                                         offset,
                                                         offset_high,
                                                         bytes_per_send,
                                                         flags,
                                                         act,
                                                         this->posix_proactor ()->get_handle (),
                                                         priority,
                                                         signal_number),
                  -1);

  // Make the auxillary handler and initiate transmit.
  ACE_POSIX_AIOCB_Asynch_Transmit_Handler *transmit_handler = 0;

  ACE_NEW_RETURN (transmit_handler,
                  ::ACE_POSIX_AIOCB_Asynch_Transmit_Handler (this->posix_proactor (),
                                                             result),
                  -1);

  ssize_t return_val = transmit_handler->transmit ();

  if (return_val == -1)
    // This deletes the <result> in it too.
    delete transmit_handler;

  return 0;
}

ACE_POSIX_AIOCB_Asynch_Transmit_File::~ACE_POSIX_AIOCB_Asynch_Transmit_File (void)
{
}


// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route the
// call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_AIOCB_Asynch_Transmit_File::open (ACE_Handler &handler,
                                      ACE_HANDLE handle,
                                      const void *completion_key,
                                      ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_AIOCB_Asynch_Transmit_File::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_AIOCB_Asynch_Transmit_File::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
} 

// *********************************************************************

ACE_POSIX_SIG_Asynch_Transmit_File::ACE_POSIX_SIG_Asynch_Transmit_File (ACE_POSIX_SIG_Proactor *posix_sig_proactor)
    : ACE_Asynch_Operation_Impl (),
      ACE_Asynch_Transmit_File_Impl (),
      ACE_POSIX_SIG_Asynch_Operation (posix_sig_proactor)
{
}

int
ACE_POSIX_SIG_Asynch_Transmit_File::transmit_file (ACE_HANDLE file,
                                                   ACE_Asynch_Transmit_File::Header_And_Trailer *header_and_trailer,
                                                   u_long bytes_to_write,
                                                   u_long offset,
                                                   u_long offset_high,
                                                   u_long bytes_per_send,
                                                   u_long flags,
                                                   const void *act,
                                                   int priority,
                                                   int signal_number)
{
  // Adjust these parameters if there are default values specified.
  ssize_t file_size = ACE_OS::filesize (file);

  if (file_size == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       ":Asynch_Transmit_File:Couldnt know the file size"),
                      -1);

  if (bytes_to_write == 0)
    bytes_to_write = file_size;

  if (offset > (size_t) file_size)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error:%p\n",
                       "Asynch_Transmit_File:File size is less than offset"),
                      -1);

  if (offset != 0)
    bytes_to_write = file_size - offset + 1;

  if (bytes_per_send == 0)
    bytes_per_send = bytes_to_write;

  // Configure the result parameter.
  ACE_POSIX_Asynch_Transmit_File_Result *result = 0;

  ACE_NEW_RETURN (result,
                  ACE_POSIX_Asynch_Transmit_File_Result (*this->handler_,
                                                         this->handle_,
                                                         file,
                                                         header_and_trailer,
                                                         bytes_to_write,
                                                         offset,
                                                         offset_high,
                                                         bytes_per_send,
                                                         flags,
                                                         act,
                                                         this->posix_sig_proactor_->get_handle (),
                                                         priority,
                                                         signal_number),
                  -1);

  // Make the auxillary handler and initiate transmit.
  ACE_POSIX_SIG_Asynch_Transmit_Handler *transmit_handler = 0;

  ACE_NEW_RETURN (transmit_handler,
                  ::ACE_POSIX_SIG_Asynch_Transmit_Handler (this->posix_sig_proactor_, result),
                  -1);

  ssize_t return_val = transmit_handler->transmit ();

  if (return_val == -1)
    // This deletes the <result> in it too.
    delete transmit_handler;

  return 0;
}

ACE_POSIX_SIG_Asynch_Transmit_File::~ACE_POSIX_SIG_Asynch_Transmit_File  (void)
{
}


// Methods belong to ACE_POSIX_Asynch_Operation base class. These
// methods are defined here to avoid dominance warnings. They route the
// call to the ACE_POSIX_Asynch_Operation base class.

int
ACE_POSIX_SIG_Asynch_Transmit_File::open (ACE_Handler &handler,
                                      ACE_HANDLE handle,
                                      const void *completion_key,
                                      ACE_Proactor *proactor)
{
  return ACE_POSIX_Asynch_Operation::open (handler,
					   handle,
					   completion_key,
					   proactor);
}

int
ACE_POSIX_SIG_Asynch_Transmit_File::cancel (void)
{
  return ACE_POSIX_Asynch_Operation::cancel ();
}

ACE_Proactor *
ACE_POSIX_SIG_Asynch_Transmit_File::proactor (void) const
{
  return ACE_POSIX_Asynch_Operation::proactor ();
} 

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_Queue<ACE_POSIX_Asynch_Accept_Result *>;
template class ACE_Node<ACE_POSIX_Asynch_Accept_Result *>;
template class ACE_Unbounded_Queue_Iterator<ACE_POSIX_Asynch_Accept_Result *>;
#elif  defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_Queue<ACE_POSIX_Asynch_Accept_Result *>
#pragma instantiate ACE_Node<ACE_POSIX_Asynch_Accept_Result *>
#pragma instantiate ACE_Unbounded_Queue_Iterator<ACE_POSIX_Asynch_Accept_Result *>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_AIO_CALLS */
