#if !defined (ACE_ASYNCH_ACCEPTOR_C)
#define ACE_ASYNCH_ACCEPTOR_C

#define ACE_BUILD_DLL
#include "ace/Asynch_Acceptor.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

#include "ace/Message_Block.h"
#include "ace/INET_Addr.h"

#if !defined (__ACE_INLINE__)
#include "ace/Asynch_Acceptor.i"
#endif /* __ACE_INLINE__ */

template <class HANDLER>
ACE_Asynch_Acceptor<HANDLER>::ACE_Asynch_Acceptor (void)
  : listen_handle_ (ACE_INVALID_HANDLE),
    pass_addresses_ (0),
    bytes_to_read_ (0)
{
}
 
template <class HANDLER>
ACE_Asynch_Acceptor<HANDLER>::~ACE_Asynch_Acceptor (void)
{
  // Close down the listen socket
  if (this->listen_handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->listen_handle_);
}
 
template <class HANDLER> int 
ACE_Asynch_Acceptor<HANDLER>::open (const ACE_INET_Addr &address,
				    size_t bytes_to_read,
				    int pass_addresses,
				    int backlog,
				    int reuse_addr)
{
  this->pass_addresses_ = pass_addresses;
  this->bytes_to_read_ = bytes_to_read;

  // Create the listener socket
  this->listen_handle_ = ACE_OS::socket (PF_INET, SOCK_STREAM, 0);
  if (this->listen_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::socket"), -1);
  
  // Initialize the ACE_Asynch_Accept
  if (this->asynch_accept_.open (*this) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Accept::open"), -1);

  if (reuse_addr)
    {
      // Reuse the address
      int one = 1;
      if (ACE_OS::setsockopt (this->listen_handle_, 
			      SOL_SOCKET, 
			      SO_REUSEADDR, 
			      (const char*) &one, 
			      sizeof one) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::setsockopt"), -1);
    }

  // If port is not specified, bind to any port
  if (&address == &ACE_Addr::sap_any &&
      ACE::bind_port (this->listen_handle_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE::bind_port"), -1);

  // Bind to the specified port.
  if (ACE_OS::bind (this->listen_handle_, 
		    (sockaddr *) address.get_addr (), 
		    address.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::bind"), -1);
  
  // Start listening
  if (ACE_OS::listen (this->listen_handle_, backlog) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_OS::listen"), -1);

  // For the number of <backlog>
  for (int i = 0; i < backlog; i++)
    {
      // Initiate accepts
      if (this->accept (bytes_to_read) == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Acceptor::accept"), -1);	
    }
  
  return 0;
}
 
template <class HANDLER> int
ACE_Asynch_Acceptor<HANDLER>::accept (size_t bytes_to_read)
{
  ACE_Message_Block *message_block = 0;
  size_t space_needed = bytes_to_read + 2 * this->address_size ();

  // Create a new message block big enough for the addresses and data
  ACE_NEW_RETURN (message_block, ACE_Message_Block (space_needed), -1);

  // Initiate asynchronous accepts
  if (this->asynch_accept_.accept (*message_block,
				   bytes_to_read) == -1)
    {
      // Cleanup on error
      message_block->release ();
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Asynch_Accept::accept"), -1);
    }
  return 0;
}

template <class HANDLER> void 
ACE_Asynch_Acceptor<HANDLER>::handle_accept (const ACE_Asynch_Accept::Result &result)
{  
  // If the asynchronous accept succeeds
  if (result.success ())
    {
      // In order to use accept handle with other Window Sockets 1.1
      // functions, we call the setsockopt function with the
      // SO_UPDATE_ACCEPT_CONTEXT option. This option initializes the
      // socket so that other Windows Sockets routines to access the
      // socket correctly.

      if (ACE_OS::setsockopt (result.accept_handle (),  
			      SOL_SOCKET, 
			      SO_UPDATE_ACCEPT_CONTEXT, 
			      (char *) &this->listen_handle_, 
			      sizeof (this->listen_handle_)) == -1)
	ACE_ERROR ((LM_ERROR, "%p", "ACE_OS::setsockopt"));
      
      // The Template method
      HANDLER *new_handler = this->make_handler ();
      
      if (this->pass_addresses_)
	{
	  ACE_INET_Addr local_address, remote_address;
	  // Parse the addresses
	  this->parse_address (result.message_block (),
			       remote_address, 
			       local_address);
	  // Pass the addresses
	  new_handler->addresses (remote_address, local_address);
	}
      // Initiate the handler
      new_handler->open (result.accept_handle (),
			 result.message_block ());
    }
  else
    // The asynchronous accept fails
    {
      // Close the accept handle
      ACE_OS::closesocket (result.accept_handle ());
      ACE_ERROR ((LM_ERROR, "%p\n", "Asynchronous Accept"));
    }
  
  // Delete the dynamically allocated message_block
  result.message_block ().release ();

  // Start off another asynchronous accept to keep the backlog going
  this->accept (this->bytes_to_read_);
}

template <class HANDLER> int
ACE_Asynch_Acceptor<HANDLER>::cancel (void)
{
  // All I/O operations that are canceled will complete with the error
  // ERROR_OPERATION_ABORTED. All completion notifications for the I/O
  // operations will occur normally.
  return -1; // ::CancelIO (this->listen_handle_);
}

template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::parse_address (ACE_Message_Block &message_block,
					     ACE_INET_Addr &remote_address,
					     ACE_INET_Addr &local_address)
{
  sockaddr *local_addr = 0;
  sockaddr *remote_addr = 0;
  int local_size = 0;
  int remote_size = 0;
  
  ::GetAcceptExSockaddrs (message_block.rd_ptr (),
			  message_block.size () - 2 * this->address_size (),
			  this->address_size (),
			  this->address_size (),
			  &local_addr,
			  &local_size,
			  &remote_addr,
			  &remote_size);

  local_address.set_addr  ((sockaddr_in *)  local_addr,  local_size);
  remote_address.set_addr ((sockaddr_in *) remote_addr, remote_size);
}

template <class HANDLER> ACE_HANDLE 
ACE_Asynch_Acceptor<HANDLER>::handle (void) const
{
  return this->listen_handle_;
}

template <class HANDLER> HANDLER *
ACE_Asynch_Acceptor<HANDLER>::make_handler (void) 
{
  // Default behavior
  HANDLER *handler = 0;
  ACE_NEW_RETURN (handler, HANDLER, 0);
  return handler;
}

/* static */
template <class HANDLER> size_t
ACE_Asynch_Acceptor<HANDLER>::address_size (void)
{
  return sizeof (sockaddr) + sizeof (sockaddr_in);
}

#endif /* ACE_WIN32 */
#endif /* ACE_ASYNCH_ACCEPTOR_C */
