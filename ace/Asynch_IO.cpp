// Asynch_IO.cpp
// $Id: Asynch_IO,v

#define ACE_BUILD_DLL
#include "ace/Asynch_IO.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

#include "ace/Proactor.h"
#include "ace/Message_Block.h"
#include "ace/Service_Config.h"

#if !defined (__ACE_INLINE__)
#include "ace/Asynch_IO.i"
#endif /* __ACE_INLINE__ */

ACE_Asynch_Result::ACE_Asynch_Result (ACE_Handler &handler,
				      const void* act,
				      HANDLE event,
				      u_long offset,
				      u_long offset_high)
  : handler_ (handler),
    act_ (act),
    bytes_transferred_ (0),
    success_ (0),
    completion_key_ (0),
    error_ (0)
{
  // Set the OVERLAPPED structure
  this->Internal = 0;
  this->InternalHigh = 0;
  this->Offset = offset;
  this->OffsetHigh = offset_high;
  this->hEvent = event;
}

u_long 
ACE_Asynch_Result::bytes_transferred (void) const
{
  return this->bytes_transferred_;
}
 
const void *
ACE_Asynch_Result::act (void) const
{
  return this->act_;
}

int 
ACE_Asynch_Result::success (void) const
{
  return this->success_;
}

const void *
ACE_Asynch_Result::completion_key (void) const
{
  return this->completion_key_;
}

u_long 
ACE_Asynch_Result::error (void) const
{
  return this->error_;
}

u_long 
ACE_Asynch_Result::offset (void) const
{
  return this->Offset;
}

u_long 
ACE_Asynch_Result::offset_high (void) const
{
  return this->OffsetHigh;
}

ACE_HANDLE 
ACE_Asynch_Result::event (void) const
{
  return this->hEvent;
}

// ************************************************************

ACE_Asynch_Operation::ACE_Asynch_Operation (void)
  : handler_ (0),
    handle_ (ACE_INVALID_HANDLE)
{
}

int 
ACE_Asynch_Operation::open (ACE_Handler &handler,
			    ACE_HANDLE handle,
			    const void *completion_key)
{
  this->handler_ = &handler;
  this->handle_ = handle;

  // Grab the handle from the <handler> if <handle> is invalid
  if (this->handle_ == ACE_INVALID_HANDLE)
    this->handle_ = this->handler_->handle ();
  if (this->handle_ == ACE_INVALID_HANDLE)  
    return -1;
  
  // Grab the proactor from the <Service_Config> if
  // <handler->proactor> is zero
  ACE_Proactor *proactor = this->handler_->proactor ();
  if (proactor == 0)
    proactor = ACE_Service_Config::proactor ();

  // Register with the <proactor>
  return proactor->register_handle (this->handle_, completion_key);
}

int
ACE_Asynch_Operation::cancel (void)
{
  // All I/O operations that are canceled will complete with the error
  // ERROR_OPERATION_ABORTED. All completion notifications for the I/O
  // operations will occur normally.
  return -1; // ::CancelIO (this->handle_);
}

// ************************************************************

ACE_Asynch_Read_Stream::ACE_Asynch_Read_Stream (void)
{
}

int 
ACE_Asynch_Read_Stream::read (ACE_Message_Block &message_block,
			      u_long bytes_to_read,
			      const void *act)
{
  // Create the Asynch_Result
  Result *result = 0;
  ACE_NEW_RETURN (result, 
		  Result (*this->handler_,
			  this->handle_,
			  message_block,
			  bytes_to_read,
			  act), 
		  -1);

  return this->shared_read (result,
			    message_block,
			    bytes_to_read,
			    act);
}

int 
ACE_Asynch_Read_Stream::shared_read (ACE_Asynch_Read_Stream::Result *result,
				     ACE_Message_Block &message_block,
				     u_long bytes_to_read,
				     const void *act)
{
  u_long bytes_read;

  // Initiate the read
  int initiate_result = ::ReadFile (result->handle (),
				    result->message_block ().wr_ptr (),
				    result->bytes_to_read (),
				    &bytes_read,
				    result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ReadFile"), -1);
    }
}

// ************************************************************

ACE_Asynch_Read_Stream::Result::Result (ACE_Handler &handler,
					ACE_HANDLE handle,
					ACE_Message_Block &message_block,
					u_long bytes_to_read,
					const void* act)
  : ACE_Asynch_Result (handler, act),
    handle_ (handle),
    message_block_ (message_block),
    bytes_to_read_ (bytes_to_read)
{
}

u_long 
ACE_Asynch_Read_Stream::Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_Asynch_Read_Stream::Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE 
ACE_Asynch_Read_Stream::Result::handle (void) const
{
  return this->handle_;
}

void 
ACE_Asynch_Read_Stream::Result::complete (u_long bytes_transferred,
					  int success,
					  const void *completion_key,
					  u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_read_stream (*this);
}

// ************************************************************

ACE_Asynch_Write_Stream::ACE_Asynch_Write_Stream (void)
{
}

int 
ACE_Asynch_Write_Stream::write (ACE_Message_Block &message_block,
				u_long bytes_to_write,
				const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result, 
		  Result (*this->handler_,
			  this->handle_,
			  message_block,
			  bytes_to_write,
			  act), 
		  -1);
 
  return this->shared_write (result,
			     message_block,
			     bytes_to_write,
			     act);
}
  
int 
ACE_Asynch_Write_Stream::shared_write (ACE_Asynch_Write_Stream::Result *result,
				       ACE_Message_Block &message_block,
				       u_long bytes_to_write,
				       const void *act)
{
  u_long bytes_written;

  // Initiate the write
  int initiate_result = ::WriteFile (result->handle (),
				     result->message_block ().rd_ptr (),
				     result->bytes_to_write (),
				     &bytes_written,
				     result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "WriteFile"), -1);
    }
}

// ************************************************************

ACE_Asynch_Write_Stream::Result::Result (ACE_Handler &handler,
					 ACE_HANDLE handle,
					 ACE_Message_Block &message_block,
					 u_long bytes_to_write,
					 const void* act)
  : ACE_Asynch_Result (handler, act),
    handle_ (handle),
    message_block_ (message_block),
    bytes_to_write_ (bytes_to_write)
{
}

u_long 
ACE_Asynch_Write_Stream::Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

ACE_Message_Block &
ACE_Asynch_Write_Stream::Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE 
ACE_Asynch_Write_Stream::Result::handle (void) const
{
  return this->handle_;
}

void 
ACE_Asynch_Write_Stream::Result::complete (u_long bytes_transferred,
					   int success,
					   const void *completion_key,
					   u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.rd_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_write_stream (*this);
}

// ************************************************************

int 
ACE_Asynch_Read_File::read (ACE_Message_Block &message_block,
			    u_long bytes_to_read,
			    u_long offset,
			    u_long offset_high,
			    const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result, 
		  Result (*this->handler_,
			  this->handle_,
			  message_block,
			  bytes_to_read,
			  act,
			  offset,
			  offset_high), 
		  -1);
  
  return this->shared_read (result,
			    message_block,
			    bytes_to_read,
			    act);
}

// ************************************************************

ACE_Asynch_Read_File::Result::Result (ACE_Handler &handler,
				      ACE_HANDLE handle,
				      ACE_Message_Block &message_block,
				      u_long bytes_to_read,
				      const void* act,
				      u_long offset,
				      u_long offset_high)
  : ACE_Asynch_Read_Stream::Result (handler, handle, message_block, bytes_to_read, act)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void 
ACE_Asynch_Read_File::Result::complete (u_long bytes_transferred,
					int success,
					const void *completion_key,
					u_long error)
{  
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_read_file (*this);
}

// ************************************************************

int 
ACE_Asynch_Write_File::write (ACE_Message_Block &message_block,
			      u_long bytes_to_write,
			      u_long offset,
			      u_long offset_high,
			      const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result, 
		  Result (*this->handler_,
			  this->handle_,
			  message_block,
			  bytes_to_write,
			  act,
			  offset,
			  offset_high), 
		  -1);
  
  return this->shared_write (result,
			     message_block,
			     bytes_to_write,
			     act);
}

// ************************************************************

ACE_Asynch_Write_File::Result::Result (ACE_Handler &handler,
				       ACE_HANDLE handle,
				       ACE_Message_Block &message_block,
				       u_long bytes_to_write,
				       const void* act,
				       u_long offset,
				       u_long offset_high)
  : ACE_Asynch_Write_Stream::Result (handler, handle, message_block, bytes_to_write, act)
{
  this->Offset = offset;
  this->OffsetHigh = offset_high;
}

void 
ACE_Asynch_Write_File::Result::complete (u_long bytes_transferred,
					 int success,
					 const void *completion_key,
					 u_long error)
{  
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.rd_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_write_file (*this);
}

// ************************************************************

ACE_Asynch_Accept::ACE_Asynch_Accept (void)
{
}
  
int 
ACE_Asynch_Accept::accept (ACE_Message_Block &message_block,
			   u_long bytes_to_read,
			   ACE_HANDLE accept_handle,
			   const void *act)
{
  // Sanity check: make sure that enough space has been allocated by the caller. 
  size_t address_size = sizeof (sockaddr_in) + sizeof (sockaddr);
  size_t space_in_use = message_block.wr_ptr () - message_block.base ();
  size_t total_size = message_block.size ();
  size_t available_space = total_size - space_in_use;
  size_t space_needed = bytes_to_read + 2 * address_size;
  if (available_space < space_needed)
    ACE_ERROR_RETURN ((LM_ERROR, "Buffer too small\n"), -1);
    
  int close_accept_handle = 0;
  // If the <accept_handle> is invalid, we will create a new socket
  if (accept_handle == ACE_INVALID_HANDLE)
    {
      accept_handle = ACE_OS::socket (PF_INET,
				      SOCK_STREAM,
				      0);
      if (accept_handle == ACE_INVALID_HANDLE)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::socket"), -1);
      else
	// Remember to close the socket down if failures occur. 
	close_accept_handle = 1;
    }

  Result *result = 0;
  ACE_NEW_RETURN (result, 
		  Result (*this->handler_,
			  this->handle_,
			  accept_handle,
			  message_block,
			  bytes_to_read,
			  act), 
		  -1);
  
  u_long bytes_read;

  // Initiate the accept
  int initiate_result = ::AcceptEx ((SOCKET) result->listen_handle (),
				    (SOCKET) result->accept_handle (),
				    result->message_block ().wr_ptr (),
				    result->bytes_to_read (),
				    address_size,
				    address_size,
				    &bytes_read,
				    result);
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      if (close_accept_handle == 1)
	// Close the newly created socket
	ACE_OS::closesocket (accept_handle);

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ReadFile"), -1);
    }
}
  
// ************************************************************

u_long 
ACE_Asynch_Accept::Result::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

ACE_Message_Block &
ACE_Asynch_Accept::Result::message_block (void) const
{
  return this->message_block_;
}

ACE_HANDLE 
ACE_Asynch_Accept::Result::listen_handle (void) const
{
  return this->listen_handle_;
}
 
ACE_HANDLE 
ACE_Asynch_Accept::Result::accept_handle (void) const
{
  return this->accept_handle_;
}
    
ACE_Asynch_Accept::Result::Result (ACE_Handler &handler,
				   ACE_HANDLE listen_handle,
				   ACE_HANDLE accept_handle,
				   ACE_Message_Block &message_block,
				   u_long bytes_to_read,
				   const void* act)
  : ACE_Asynch_Result (handler, act),
    listen_handle_ (listen_handle),
    accept_handle_ (accept_handle),
    message_block_ (message_block),
    bytes_to_read_ (bytes_to_read)
{
}    
    
void 
ACE_Asynch_Accept::Result::complete (u_long bytes_transferred,
				     int success,
				     const void *completion_key,
				     u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
  this->bytes_transferred_ = bytes_transferred;
  this->success_ = success;
  this->completion_key_ = completion_key;
  this->error_ = error;

  // Appropriately move the pointers in the message block.
  this->message_block_.wr_ptr (bytes_transferred);

  // Callback: notify <handler> of completion
  this->handler_.handle_accept (*this);
}
    
// ************************************************************

ACE_Asynch_Transmit_File::ACE_Asynch_Transmit_File (void)
{
}
  
int 
ACE_Asynch_Transmit_File::transmit_file (ACE_HANDLE file,
					 Header_And_Trailer *header_and_trailer,
					 u_long bytes_to_write,					 
					 u_long offset,
					 u_long offset_high,
					 u_long bytes_per_send,
					 u_long flags,
					 const void *act)
{
  Result *result = 0;
  ACE_NEW_RETURN (result, 
		  Result (*this->handler_,
			  this->handle_,
			  file,
			  header_and_trailer,
			  bytes_to_write,
			  offset,
			  offset_high,
			  bytes_per_send,
			  flags,
			  act),
		  -1);
  
  LPTRANSMIT_FILE_BUFFERS transmit_buffers = 0;
  if (result->header_and_trailer () != 0)
    transmit_buffers = result->header_and_trailer ()->transmit_buffers ();
  
  // Initiate the transmit file
  int initiate_result = ::TransmitFile ((SOCKET) result->socket (),
					result->file (),
					result->bytes_to_write (),
					result->bytes_per_send (),
					result,
					transmit_buffers,
					result->flags ());
  if (initiate_result == 1)
    // Immediate success: the OVERLAPPED will still get queued.
    return 1;

  // If initiate failed, check for a bad error.
  errno = ::GetLastError ();
  switch (errno)
    {
    case ERROR_IO_PENDING:
      // The IO will complete proactively: the OVERLAPPED will still
      // get queued.
      return 0;

    default:
      // Something else went wrong: the OVERLAPPED will not get
      // queued.

      // Cleanup dynamically allocated Asynch_Result
      delete result;

      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "TransmitFile"), -1);
    }  
}
  
// ************************************************************

ACE_HANDLE
ACE_Asynch_Transmit_File::Result::socket (void) const
{
  return this->socket_;
}

ACE_HANDLE 
ACE_Asynch_Transmit_File::Result::file (void) const
{
  return this->file_;
}

ACE_Asynch_Transmit_File::Header_And_Trailer *
ACE_Asynch_Transmit_File::Result::header_and_trailer (void) const
{
  return this->header_and_trailer_;
}
 
u_long 
ACE_Asynch_Transmit_File::Result::bytes_to_write (void) const
{
  return this->bytes_to_write_;
}

u_long 
ACE_Asynch_Transmit_File::Result::bytes_per_send (void) const
{
  return this->bytes_per_send_;
}

u_long 
ACE_Asynch_Transmit_File::Result::flags (void) const
{
  return this->flags_;
}
    
ACE_Asynch_Transmit_File::Result::Result (ACE_Handler &handler,
					  ACE_HANDLE socket,
					  ACE_HANDLE file,
					  Header_And_Trailer *header_and_trailer,
					  u_long bytes_to_write,
					  u_long offset,
					  u_long offset_high,
					  u_long bytes_per_send,
					  u_long flags,
					  const void *act)
  : ACE_Asynch_Result (handler, act, 0, offset, offset_high),
    socket_ (socket),
    file_ (file),
    header_and_trailer_ (header_and_trailer),
    bytes_to_write_ (bytes_to_write),
    bytes_per_send_ (bytes_per_send),
    flags_ (flags)
{
}

void 
ACE_Asynch_Transmit_File::Result::complete (u_long bytes_transferred,
					    int success,
					    const void *completion_key,
					    u_long error)
{
  // Copy the data which was returned by GetQueuedCompletionStatus
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

  // Callback: notify <handler> of completion
  this->handler_.handle_transmit_file (*this);  
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

LPTRANSMIT_FILE_BUFFERS 
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

// ************************************************************

ACE_Handler::~ACE_Handler (void)
{
}

ACE_Handler::ACE_Handler (void)
  : proactor_ (0)
{
}

ACE_Handler::ACE_Handler (ACE_Proactor *d)
  : proactor_ (d)
{
}

void
ACE_Handler::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
}

void 
ACE_Handler::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
}

void 
ACE_Handler::handle_accept (const ACE_Asynch_Accept::Result &result)
{
}

void 
ACE_Handler::handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result)
{
}

void
ACE_Handler::handle_read_file (const ACE_Asynch_Read_File::Result &result)
{
}

void 
ACE_Handler::handle_write_file (const ACE_Asynch_Write_File::Result &result)
{
}

/*
void 
ACE_Handler::handle_notify (const ACE_Asynch_Notify::Result &result)
{
}

void 
ACE_Handler::handle_timeout (const ACE_Asynch_Timeout::Result &result)
{
}
*/

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
  char local_address_buf[BUFSIZ], remote_address_buf[BUFSIZ];
  if (local_address.addr_to_string (local_address_buf, sizeof local_address_buf) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "can't obtain local_address's address string"));
  
  if (remote_address.addr_to_string (remote_address_buf, sizeof remote_address_buf) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "can't obtain remote_address's address string"));
  
  ACE_DEBUG ((LM_DEBUG, "On fd %d\n", this->handle ()));
  ACE_DEBUG ((LM_DEBUG, "local address %s\n", local_address_buf));
  ACE_DEBUG ((LM_DEBUG, "remote address %s\n", remote_address_buf));  
}

#endif /* ACE_WIN32 */
