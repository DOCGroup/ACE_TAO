// $Id$

#define ACE_BUILD_DLL
#include "ace/Asynch_IO.h"

ACE_RCSID(ace, Asynch_IO, "$Id$")

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) || (defined (ACE_HAS_AIO_CALLS))
// This only works on platforms with Asynchronous IO

#include "ace/Proactor.h"
#include "ace/Message_Block.h"
#include "ace/INET_Addr.h"
#include "ace/Asynch_IO_Impl.h"

#if !defined (__ACE_INLINE__)
#include "ace/Asynch_IO.i"
#endif /* __ACE_INLINE__ */

// *********************************************************************

u_long
ACE_Asynch_Result::bytes_transferred (void) const
{
  return this->implementation ()->bytes_transferred ();
}

const void *
ACE_Asynch_Result::act (void) const
{
  return this->implementation ()->act ();
}

int
ACE_Asynch_Result::success (void) const
{
  return this->implementation ()->success ();
}

const void *
ACE_Asynch_Result::completion_key (void) const
{
  return this->implementation ()->completion_key ();
}

u_long
ACE_Asynch_Result::error (void) const
{
  return this->implementation ()->error ();
}

ACE_HANDLE
ACE_Asynch_Result::event (void) const
{
  return this->implementation ()->event ();
}

u_long
ACE_Asynch_Result::offset (void) const
{
  return this->implementation ()->offset ();
}

u_long
ACE_Asynch_Result::offset_high (void) const
{
  return this->implementation ()->offset_high ();
}

int
ACE_Asynch_Result::priority (void) const
{
  return this->implementation ()->priority ();
}

ACE_Asynch_Result::ACE_Asynch_Result (ACE_Asynch_Result_Impl *implementation)
  : implementation_ (implementation)
{
}

ACE_Asynch_Result::~ACE_Asynch_Result (void)
{
  // Proactor deletes the implementation when the <complete> finishes. 
}

ACE_Asynch_Result_Impl *
ACE_Asynch_Result::implementation (void) const
{
  return this->implementation_;
}

// *********************************************************************

int
ACE_Asynch_Operation::open (ACE_Handler &handler,
                            ACE_HANDLE handle,
                            const void *completion_key,
                            ACE_Proactor *proactor)
{
  return this->implementation ()->open (handler,
                                        handle,
                                        completion_key,
                                        proactor);
}

int
ACE_Asynch_Operation::cancel (void)
{
  return this->implementation ()->cancel ();
}

ACE_Proactor *
ACE_Asynch_Operation::proactor (void) const
{
  return this->implementation ()->proactor ();
}

ACE_Asynch_Operation_Impl *
ACE_Asynch_Operation::implementation (void) const
{
  return this->implementation_;
}

ACE_Asynch_Operation::ACE_Asynch_Operation (void)
  : implementation_ (0)
{
}

ACE_Asynch_Operation::~ACE_Asynch_Operation (void)
{
  delete this->implementation_;
  this->implementation_ = 0;
}

void
ACE_Asynch_Operation::implementation (ACE_Asynch_Operation_Impl *implementation)
{
  this->implementation_ = implementation;
}

ACE_Proactor *
ACE_Asynch_Operation::get_proactor (ACE_Proactor *user_proactor,
                                    ACE_Handler &handler) const
{
  if (user_proactor == 0)
    {
      // Grab the singleton proactor if <handler->proactor> is zero
      user_proactor = handler.proactor ();
      if (user_proactor == 0)
        user_proactor = ACE_Proactor::instance ();
    }

  return user_proactor;
}

// ************************************************************

ACE_Asynch_Read_Stream::ACE_Asynch_Read_Stream (void)
  : implementation_ (0)
{
}

ACE_Asynch_Read_Stream::~ACE_Asynch_Read_Stream (void)
{
}

int
ACE_Asynch_Read_Stream::open (ACE_Handler &handler,
                              ACE_HANDLE handle,
                              const void *completion_key,
                              ACE_Proactor *proactor)
{
  // Get a proactor for/from the user.
  proactor = this->get_proactor (proactor, handler);

  // Delete the old implementation.
  delete this->implementation_;
  this->implementation_ = 0;

  // Now let us get the implementation initialized.
  ACE_Asynch_Read_Stream_Impl *implementation = proactor->create_asynch_read_stream ();
  if (implementation == 0)
    return -1;
  
  // Set the implementation class
  this->implementation (implementation);
  
  // Call the <open> method of the base class. 
  return ACE_Asynch_Operation::open (handler,
                                     handle,
                                     completion_key,
                                     proactor);
}
  
int
ACE_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
                              u_long bytes_to_read,
                              const void *act,
                              int priority)
{
  return this->implementation ()->read (message_block,
                                        bytes_to_read,
                                        act,
                                        priority);
}

ACE_Asynch_Read_Stream_Impl *
ACE_Asynch_Read_Stream::implementation (void) const
{
  return implementation_;
}

void
ACE_Asynch_Read_Stream::implementation (ACE_Asynch_Read_Stream_Impl *implementation)
{
  this->implementation_ = implementation;

  // Init the base class' implementation also.
  ACE_Asynch_Operation::implementation (this->implementation_);
}

// ************************************************************

u_long
ACE_Asynch_Read_Stream::Result::bytes_to_read (void) const
{
  return this->implementation ()->bytes_to_read ();
}

ACE_Message_Block &
ACE_Asynch_Read_Stream::Result::message_block (void) const
{
  return this->implementation ()->message_block ();
}

ACE_HANDLE
ACE_Asynch_Read_Stream::Result::handle (void) const
{
  return this->implementation ()->handle ();
}

ACE_Asynch_Read_Stream::Result::Result (ACE_Asynch_Read_Stream_Result_Impl *implementation)
  : ACE_Asynch_Result (implementation), 
    implementation_ (implementation)
{
}

ACE_Asynch_Read_Stream::Result::~Result (void)
{
  // Proactor will delete the implementation after <complete> is
  // finished.
}

ACE_Asynch_Read_Stream_Result_Impl *
ACE_Asynch_Read_Stream::Result::implementation (void) const
{
  return this->implementation_;
}

// ***************************************************

ACE_Asynch_Write_Stream::ACE_Asynch_Write_Stream (void)
  : implementation_ (0)
{
}

ACE_Asynch_Write_Stream::~ACE_Asynch_Write_Stream (void)
{
}

int
ACE_Asynch_Write_Stream::open (ACE_Handler &handler,
                               ACE_HANDLE handle,
                               const void *completion_key,
                               ACE_Proactor *proactor)
{
  // Get a proactor for/from the user.
  proactor = this->get_proactor (proactor, handler);

  // Delete the old implementation.
  delete this->implementation_;
  this->implementation_ = 0;

  // Now let us get the implementation initialized.
  ACE_Asynch_Write_Stream_Impl *implementation = proactor->create_asynch_write_stream ();
  if (implementation == 0)
    return -1;
  
  // Set the implementation class
  this->implementation (implementation);
  
  // Call the <open> method of the base class.
  return ACE_Asynch_Operation::open (handler,
                                     handle,
                                     completion_key,
                                     proactor);
}

int
ACE_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
                                u_long bytes_to_write,
                                const void *act,
                                int priority)
{
  return this->implementation ()->write (message_block,
                                         bytes_to_write,
                                         act,
                                         priority);
}

ACE_Asynch_Write_Stream_Impl *
ACE_Asynch_Write_Stream::implementation (void) const
{
  return this->implementation_;
}

void
ACE_Asynch_Write_Stream::implementation (ACE_Asynch_Write_Stream_Impl *implementation)
{
  this->implementation_ = implementation;

  // Init the base class' implementation also.
  ACE_Asynch_Operation::implementation (this->implementation_);
}

// ************************************************************

u_long
ACE_Asynch_Write_Stream::Result::bytes_to_write (void) const
{
  return this->implementation ()->bytes_to_write ();
}

ACE_Message_Block &
ACE_Asynch_Write_Stream::Result::message_block (void) const
{
  return this->implementation ()->message_block ();
}

ACE_HANDLE
ACE_Asynch_Write_Stream::Result::handle (void) const
{
  return this->implementation ()->handle ();
}

ACE_Asynch_Write_Stream::Result::Result (ACE_Asynch_Write_Stream_Result_Impl *implementation)
  : ACE_Asynch_Result (implementation),
    implementation_ (implementation)
{
}

ACE_Asynch_Write_Stream::Result::~Result (void)
{
  // Proactor will delte the implementation when the <complete> call
  // finishes.
}

ACE_Asynch_Write_Stream_Result_Impl *
ACE_Asynch_Write_Stream::Result::implementation (void) const
{
  return this->implementation_;
}

// ************************************************************

ACE_Asynch_Read_File::ACE_Asynch_Read_File (void)
  : implementation_ (0)
{
} 

ACE_Asynch_Read_File::~ACE_Asynch_Read_File (void)
{
} 

int
ACE_Asynch_Read_File::open (ACE_Handler &handler,
                            ACE_HANDLE handle,
                            const void *completion_key,
                            ACE_Proactor *proactor)
{
  // Get a proactor for/from the user.
  proactor = this->get_proactor (proactor, handler);

  // Delete the old implementation.
  delete this->implementation_;
  this->implementation_ = 0;

  // Now let us get the implementation initialized.
  ACE_Asynch_Read_File_Impl *implementation = proactor->create_asynch_read_file ();
  if (implementation == 0)
    return -1;
  
  // Set the implementation class
  this->implementation (implementation);
  
  // Call the <open> method of the base class.
  return ACE_Asynch_Operation::open (handler,
                                     handle,
                                     completion_key,
                                     proactor);
}

int
ACE_Asynch_Read_File::read (ACE_Message_Block &message_block,
                            u_long bytes_to_read,
                            u_long offset,
                            u_long offset_high,
                            const void *act,
                            int priority)
{
  return this->implementation ()->read (message_block,
                                        bytes_to_read,
                                        offset,
                                        offset_high,
                                        act,
                                        priority);
}

ACE_Asynch_Read_File_Impl *
ACE_Asynch_Read_File::implementation (void) const
{
  return this->implementation_;
}

void
ACE_Asynch_Read_File::implementation (ACE_Asynch_Read_File_Impl *implementation)
{
  this->implementation_ = implementation;

  // Set also the base class' implementation.
  ACE_Asynch_Read_Stream::implementation (implementation);
}

// ************************************************************

ACE_Asynch_Read_File::Result::Result (ACE_Asynch_Read_File_Result_Impl *implementation)
  : ACE_Asynch_Read_Stream::Result (implementation),
    implementation_ (implementation)
{
}
  
ACE_Asynch_Read_File::Result::~Result (void)
{
  // Proactor will delete the implementation when <complete> call
  // completes.
}

ACE_Asynch_Read_File_Result_Impl *
ACE_Asynch_Read_File::Result::implementation (void) const
{
  return this->implementation_;
}

// ************************************************************

ACE_Asynch_Write_File::ACE_Asynch_Write_File (void)
  : implementation_ (0)
{
}

ACE_Asynch_Write_File::~ACE_Asynch_Write_File (void)
{
}

int
ACE_Asynch_Write_File::open (ACE_Handler &handler,
                             ACE_HANDLE handle,
                             const void *completion_key,
                             ACE_Proactor *proactor)
{
  // Get a proactor for/from the user.
  proactor = this->get_proactor (proactor, handler);

  // Delete the old implementation.
  delete this->implementation_;
  this->implementation_ = 0;

  // Now let us get the implementation initialized.
  ACE_Asynch_Write_File_Impl *implementation = proactor->create_asynch_write_file ();
  if (implementation == 0)
    return -1;
  
  // Set the implementation class
  this->implementation (implementation);
  
  // Call the <open> method of the base class.
  return ACE_Asynch_Operation::open (handler,
                                     handle,
                                     completion_key,
                                     proactor);
}

int
ACE_Asynch_Write_File::write (ACE_Message_Block &message_block,
                              u_long bytes_to_write,
                              u_long offset,
                              u_long offset_high,
                              const void *act,
                              int priority)
{
  return this->implementation ()->write (message_block,
                                         bytes_to_write,
                                         offset,
                                         offset_high,
                                         act,
                                         priority);
}

ACE_Asynch_Write_File_Impl *
ACE_Asynch_Write_File::implementation (void) const
{
  return this->implementation_;
}

void
ACE_Asynch_Write_File::implementation (ACE_Asynch_Write_File_Impl *implementation)
{
  this->implementation_ = implementation;
  
  // Init the base class' implementation also.
  ACE_Asynch_Write_Stream::implementation (implementation);
}

// ************************************************************

ACE_Asynch_Write_File::Result::Result (ACE_Asynch_Write_File_Result_Impl *implementation)
  : ACE_Asynch_Write_Stream::Result (implementation),
    implementation_ (implementation)
{
}

ACE_Asynch_Write_File::Result::~Result (void)
{
  // Proactor will delete the implementation when the <complete> call
  // completes. 
}

ACE_Asynch_Write_File_Result_Impl *
ACE_Asynch_Write_File::Result::implementation (void) const
{
  return this->implementation_;
}

// *********************************************************************

ACE_Asynch_Accept::ACE_Asynch_Accept (void)
  : implementation_ (0)
{
}

ACE_Asynch_Accept::~ACE_Asynch_Accept (void)
{
}

int
ACE_Asynch_Accept::open (ACE_Handler &handler,
                         ACE_HANDLE handle,
                         const void *completion_key,
                         ACE_Proactor *proactor)
{
  // Get a proactor for/from the user.
  proactor = this->get_proactor (proactor, handler);

  // Delete the old implementation.
  delete this->implementation_;
  this->implementation_ = 0;

  // Now let us get the implementation initialized.
  ACE_Asynch_Accept_Impl *implementation = proactor->create_asynch_accept ();
  if (implementation == 0)
    return -1;
  
  // Set the implementation class
  this->implementation (implementation);
  
  // Call the <open> method of the base class.
  return ACE_Asynch_Operation::open (handler,
                                     handle,
                                     completion_key,
                                     proactor);
}

int
ACE_Asynch_Accept::accept (ACE_Message_Block &message_block,
                           u_long bytes_to_read,
                           ACE_HANDLE accept_handle,
                           const void *act,
                           int priority)
{
  return this->implementation ()->accept (message_block,
                                          bytes_to_read,
                                          accept_handle,
                                          act,
                                          priority);
}

ACE_Asynch_Accept_Impl *
ACE_Asynch_Accept::implementation (void)  const
{
  return this->implementation_;
}

void
ACE_Asynch_Accept::implementation (ACE_Asynch_Accept_Impl *implementation)
{
  this->implementation_ = implementation;
  // Set the implementation in the base class.
  ACE_Asynch_Operation::implementation (implementation);
}

// ************************************************************

u_long
ACE_Asynch_Accept::Result::bytes_to_read (void) const
{
  return this->implementation ()->bytes_to_read ();
}

ACE_Message_Block &
ACE_Asynch_Accept::Result::message_block (void) const
{
  return this->implementation ()->message_block ();
}

ACE_HANDLE
ACE_Asynch_Accept::Result::listen_handle (void) const
{
  return this->implementation ()->listen_handle ();
}

ACE_HANDLE
ACE_Asynch_Accept::Result::accept_handle (void) const
{
  return this->implementation ()->accept_handle ();
}

ACE_Asynch_Accept::Result::Result (ACE_Asynch_Accept_Result_Impl *implementation)
  : ACE_Asynch_Result (implementation),
    implementation_ (implementation)
{
}

ACE_Asynch_Accept::Result::~Result (void)
{
  // Proactor will delete the implementation when the <complete> call
  // completes.
}

ACE_Asynch_Accept_Result_Impl *
ACE_Asynch_Accept::Result::implementation (void) const
{
  return this->implementation_;
}

// ************************************************************

ACE_Asynch_Transmit_File::ACE_Asynch_Transmit_File (void)
  : implementation_ (0)
{
}

ACE_Asynch_Transmit_File::~ACE_Asynch_Transmit_File (void)
{
}

int
ACE_Asynch_Transmit_File::open (ACE_Handler &handler,
                                ACE_HANDLE handle,
                                const void *completion_key,
                                ACE_Proactor *proactor)
{
  // Get a proactor for/from the user.
  proactor = this->get_proactor (proactor, handler);

  // Delete the old implementation.
  delete this->implementation_;
  this->implementation_ = 0;

  // Now let us get the implementation initialized.
  ACE_Asynch_Transmit_File_Impl *implementation = proactor->create_asynch_transmit_file ();
  if (implementation == 0)
    return -1;
  
  // Set the implementation class
  this->implementation (implementation);
  
  // Call the <open> method of the base class.
  return ACE_Asynch_Operation::open (handler,
                                     handle,
                                     completion_key,
                                     proactor);
}

int
ACE_Asynch_Transmit_File::transmit_file (ACE_HANDLE file,
                                         Header_And_Trailer *header_and_trailer,
                                         u_long bytes_to_write,
                                         u_long offset,
                                         u_long offset_high,
                                         u_long bytes_per_send,
                                         u_long flags,
                                         const void *act,
                                         int priority)
{
  return this->implementation ()->transmit_file (file,
                                                 header_and_trailer,
                                                 bytes_to_write,
                                                 offset,
                                                 offset_high,
                                                 bytes_per_send,
                                                 flags,
                                                 act,
                                                 priority);
}

ACE_Asynch_Transmit_File_Impl *
ACE_Asynch_Transmit_File::implementation (void) const
{
  return this->implementation_;
}

void
ACE_Asynch_Transmit_File::implementation (ACE_Asynch_Transmit_File_Impl *implementation)
{
  this->implementation_ = implementation;
  // Set the base class' implementation also.
  ACE_Asynch_Operation::implementation (this->implementation_);
}

// ***********************************************************************************

ACE_HANDLE
ACE_Asynch_Transmit_File::Result::socket (void) const
{
  return this->implementation ()->socket ();
}

ACE_HANDLE
ACE_Asynch_Transmit_File::Result::file (void) const
{
  return this->implementation ()->file ();
}

ACE_Asynch_Transmit_File::Header_And_Trailer *
ACE_Asynch_Transmit_File::Result::header_and_trailer (void) const
{
  return this->implementation ()->header_and_trailer ();
}

u_long
ACE_Asynch_Transmit_File::Result::bytes_to_write (void) const
{
  return this->implementation ()->bytes_to_write ();
}

u_long
ACE_Asynch_Transmit_File::Result::bytes_per_send (void) const
{
  return this->implementation ()->bytes_per_send ();
}

u_long
ACE_Asynch_Transmit_File::Result::flags (void) const
{
  return this->implementation ()->flags ();
}

ACE_Asynch_Transmit_File::Result::Result (ACE_Asynch_Transmit_File_Result_Impl *implementation)
  : ACE_Asynch_Result (implementation),
    implementation_ (implementation)
{
}

ACE_Asynch_Transmit_File::Result::~Result (void)
{
}

ACE_Asynch_Transmit_File_Result_Impl *
ACE_Asynch_Transmit_File::Result::implementation (void) const
{
  return this->implementation_;
}

// ************************************************************

ACE_Asynch_Transmit_File::Header_And_Trailer::Header_And_Trailer (ACE_Message_Block *header,
                                                                  u_long header_bytes,
                                                                  ACE_Message_Block *trailer,
                                                                  u_long trailer_bytes)
  : header_ (header),
    header_bytes_ (header_bytes),
    trailer_ (trailer),
    trailer_bytes_ (trailer_bytes)
{
}

ACE_Asynch_Transmit_File::Header_And_Trailer::~Header_And_Trailer (void)
{
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::header_and_trailer (ACE_Message_Block *header,
                                                                  u_long header_bytes,
                                                                  ACE_Message_Block *trailer,
                                                                  u_long trailer_bytes)
{
  this->header (header);
  this->header_bytes (header_bytes);
  this->trailer (trailer);
  this->trailer_bytes (trailer_bytes);
}

ACE_Message_Block *
ACE_Asynch_Transmit_File::Header_And_Trailer::header (void) const
{
  return this->header_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::header (ACE_Message_Block *message_block)
{
  this->header_ = message_block;
}

u_long
ACE_Asynch_Transmit_File::Header_And_Trailer::header_bytes (void) const
{
  return this->header_bytes_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::header_bytes (u_long bytes)
{
  this->header_bytes_ = bytes;
}

ACE_Message_Block *
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer (void) const
{
  return this->trailer_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer (ACE_Message_Block *message_block)
{
  this->trailer_ = message_block;
}

u_long
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer_bytes (void) const
{
  return this->trailer_bytes_;
}

void
ACE_Asynch_Transmit_File::Header_And_Trailer::trailer_bytes (u_long bytes)
{
  this->trailer_bytes_ = bytes;
}

ACE_LPTRANSMIT_FILE_BUFFERS
ACE_Asynch_Transmit_File::Header_And_Trailer::transmit_buffers (void)
{
  // If both are zero, return zero
  if (this->header_ == 0 && this->trailer_ == 0)
    return 0;
  else
    {
      // Something is valid

      // If header is valid, set the fields
      if (this->header_ != 0)
        {
          this->transmit_buffers_.Head = this->header_->rd_ptr ();
          this->transmit_buffers_.HeadLength = this->header_bytes_;
        }
      else
        {
          this->transmit_buffers_.Head = 0;
          this->transmit_buffers_.HeadLength = 0;
        }

      // If trailer is valid, set the fields
      if (this->trailer_ != 0)
        {
          this->transmit_buffers_.Tail = this->trailer_->rd_ptr ();
          this->transmit_buffers_.TailLength = this->trailer_bytes_;
        }
      else
        {
          this->transmit_buffers_.Tail = 0;
          this->transmit_buffers_.TailLength = 0;
        }

      // Return the transmit buffers
      return &this->transmit_buffers_;
    }
}

// *********************************************************************

ACE_Handler::ACE_Handler (void)
  : proactor_ (0)
{
}

ACE_Handler::ACE_Handler (ACE_Proactor *d)
  : proactor_ (d)
{
}

ACE_Handler::~ACE_Handler (void)
{
}

void
ACE_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_accept (const ACE_Asynch_Accept::Result &result)
{
 
	ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_write_file (const ACE_Asynch_Write_File::Result &result)
{
  ACE_UNUSED_ARG (result);
}

void
ACE_Handler::handle_time_out (const ACE_Time_Value &tv,
                              const void *act)
{
  ACE_UNUSED_ARG (tv);
  ACE_UNUSED_ARG (act);
}

ACE_Proactor *
ACE_Handler::proactor (void)
{
  return this->proactor_;
}

void
ACE_Handler::proactor (ACE_Proactor *p)
{
  this->proactor_ = p;
}

ACE_HANDLE
ACE_Handler::handle (void) const
{
  return ACE_INVALID_HANDLE;
}

// ************************************************************

ACE_Service_Handler::ACE_Service_Handler (void)
{
}

ACE_Service_Handler::~ACE_Service_Handler (void)
{
}

void
ACE_Service_Handler::addresses (const ACE_INET_Addr &remote_address,
                                const ACE_INET_Addr &local_address)
{
  // Default behavior is to print out the addresses.
  ASYS_TCHAR local_address_buf[BUFSIZ], remote_address_buf[BUFSIZ];
  if (local_address.addr_to_string (local_address_buf, sizeof local_address_buf) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("can't obtain local_address's address string")));

  if (remote_address.addr_to_string (remote_address_buf, sizeof remote_address_buf) == -1)
    ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("can't obtain remote_address's address string")));

  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("On fd %d\n"), this->handle ()));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("local address %s\n"), local_address_buf));
  ACE_DEBUG ((LM_DEBUG,  ASYS_TEXT ("remote address %s\n"), remote_address_buf));
}

void
ACE_Service_Handler::act (const void *)
{
}

void
ACE_Service_Handler::open (ACE_HANDLE,
                           ACE_Message_Block &)
{
}

#endif /* ACE_WIN32 || ACE_HAS_AIO_CALLS */
