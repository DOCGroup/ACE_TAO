// $Id$

#if !defined (ACE_ASYNCH_ACCEPTOR_C)
#define ACE_ASYNCH_ACCEPTOR_C

#define ACE_BUILD_DLL
#include "ace/Asynch_Acceptor.h"

ACE_RCSID(ace, Asynch_Acceptor, "$Id$")

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
    validate_new_connection_ (0),
    reissue_accept_ (1),
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
				    int reuse_addr,
				    ACE_Proactor *proactor,
                                    int validate_new_connection,
                                    int reissue_accept,
                                    int number_of_initial_accepts)
{
  this->proactor (proactor);
  this->pass_addresses_ = pass_addresses;
  this->bytes_to_read_ = bytes_to_read;
  this->validate_new_connection_ = validate_new_connection;
  this->reissue_accept_ = reissue_accept;

  // Create the listener socket
  this->listen_handle_ = ACE_OS::socket (PF_INET, SOCK_STREAM, 0);
  if (this->listen_handle_ == ACE_INVALID_HANDLE)
    ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_OS::socket")), -1);
  
  // Initialize the ACE_Asynch_Accept
  if (this->asynch_accept_.open (*this,
				 this->listen_handle_,
				 0,
				 this->proactor ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_Asynch_Accept::open")), -1);

  if (reuse_addr)
    {
      // Reuse the address
      int one = 1;
      if (ACE_OS::setsockopt (this->listen_handle_, 
			      SOL_SOCKET, 
			      SO_REUSEADDR, 
			      (const char*) &one, 
			      sizeof one) == -1)
	ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_OS::setsockopt")), -1);
    }

  // If port is not specified, bind to any port.
  static ACE_INET_Addr sa ((const ACE_INET_Addr &) ACE_Addr::sap_any);

  if (address == sa && ACE::bind_port (this->listen_handle_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE::bind_port")), -1);

  // Bind to the specified port.
  if (ACE_OS::bind (this->listen_handle_, 
		    (sockaddr *) address.get_addr (), 
		    address.get_size ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::bind"),
                      -1);
  
  // Start listening
  if (ACE_OS::listen (this->listen_handle_, backlog) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "ACE_OS::listen"), -1);

  // For the number of <intial_accepts>.
  if (number_of_initial_accepts == -1)
    number_of_initial_accepts = backlog;

  for (int i = 0; i < number_of_initial_accepts; i++)
    // Initiate accepts.
    if (this->accept (bytes_to_read) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p\n",
                         "ACE_Asynch_Acceptor::accept"), -1);	
  return 0;
}
 
template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::set_handle (ACE_HANDLE listen_socket)
{
  // Take ownership of the <listen_socket>
  this->listen_socket_ = listen_socket;

  // Initialize the ACE_Asynch_Accept
  this->asynch_accept_.set_handle (this->listen_handle_);

  return 0;
}
 
template <class HANDLER> ACE_HANDLE 
ACE_Asynch_Acceptor<HANDLER>::get_handle (void) const
{
  return this->listen_socket_;
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
      ACE_ERROR_RETURN ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("ACE_Asynch_Accept::accept")), -1);
    }
  return 0;
}

template <class HANDLER> void 
ACE_Asynch_Acceptor<HANDLER>::handle_accept (const ACE_Asynch_Accept::Result &result)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))

  // Variable for error tracking 
  int error = 0;
  
  // If the asynchronous accept fails.
  if (!error &&
      !result.success ())
    {      
      error = 1;
      ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("AcceptEx")));
    }
  
  // In order to use accept handle with other Window Sockets 1.1
  // functions, we call the setsockopt function with the
  // SO_UPDATE_ACCEPT_CONTEXT option. This option initializes the
  // socket so that other Windows Sockets routines to access the
  // socket correctly.
  if (!error &&
      ACE_OS::setsockopt (result.accept_handle (),  
                          SOL_SOCKET, 
                          SO_UPDATE_ACCEPT_CONTEXT, 
                          (char *) &this->listen_handle_, 
                          sizeof (this->listen_handle_)) == -1)
    {
      error = 1;
      ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p"),  ASYS_TEXT ("ACE_OS::setsockopt")));
    }

  // Parse address
  ACE_INET_Addr local_address, remote_address;
  if (!error &&
      this->validate_new_connection_ || this->pass_addresses_)
    {
      // Parse the addresses
      this->parse_address (result.message_block (),
                           remote_address, 
                           local_address);
    }

  // Validate remote address
  if (!error &&
      this->validate_new_connection_ &&
      this->validate_new_connection (remote_address) == -1)
    {      
      error = 1;
      ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("Address validation failed")));
    }
  
  HANDLER *new_handler = 0;
  if (!error)
    {
      // The Template method
      new_handler = this->make_handler ();
      if (new_handler == 0)
        {      
          error = 1;
          ACE_ERROR ((LM_ERROR,  ASYS_TEXT ("%p\n"),  ASYS_TEXT ("Making of new handler failed")));
        }      
    }

  // If no errors
  if (!error)
    {
      // Update the Proactor.
      new_handler->proactor (this->proactor ());

      // Pass the addresses
      if (this->pass_addresses_)
        {
          new_handler->addresses (remote_address, local_address);
        }

      // Initiate the handler
      new_handler->open (result.accept_handle (), 
                         result.message_block ());
    }

  // On failure, no choice but to close the socket 
  if (error)
    {
      ACE_OS::closesocket (result.accept_handle ());
    }
     
  // Delete the dynamically allocated message_block
  result.message_block ().release ();

  // Start off another asynchronous accept to keep the backlog going
  if (this->should_reissue_accept ())
    {
      this->accept (this->bytes_to_read_);
    }
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) */
}

template <class HANDLER> int
ACE_Asynch_Acceptor<HANDLER>::validate_new_connection (const ACE_INET_Addr &remote_address)
{
  // Default implemenation always validates the remote address.
  return 0; 
}

template <class HANDLER> int
ACE_Asynch_Acceptor<HANDLER>::cancel (void)
{
  // All I/O operations that are canceled will complete with the error
  // ERROR_OPERATION_ABORTED. All completion notifications for the I/O
  // operations will occur normally.
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && (defined (_MSC_VER) && (_MSC_VER > 1020))
  return (int) ::CancelIo (this->listen_handle_);
#else
  ACE_NOTSUP_RETURN (-1);
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) && (defined (_MSC_VER) && (_MSC_VER > 1020)) */
}

template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::parse_address (ACE_Message_Block &message_block,
					     ACE_INET_Addr &remote_address,
					     ACE_INET_Addr &local_address)
{
#if (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
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
#else
  // just in case
  errno = ENOTSUP;
#endif /* (defined (ACE_HAS_WINNT4) && (ACE_HAS_WINNT4 != 0)) || (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0)) */
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

template <class HANDLER> int 
ACE_Asynch_Acceptor<HANDLER>::pass_addresses (void) const
{
  return this->pass_addresses_;
}

template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::pass_addresses (int new_value)
{
  this->pass_addresses_ = new_value;
}

template <class HANDLER> int 
ACE_Asynch_Acceptor<HANDLER>::validate_new_connection (void) const
{
  return this->validate_new_connection_;
}

template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::validate_new_connection (int new_value)
{
  this->validate_new_connection_ = new_value;
}

template <class HANDLER> int 
ACE_Asynch_Acceptor<HANDLER>::reissue_accept (void) const
{
  return this->reissue_accept_;
}

template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::reissue_accept (int new_value)
{
  this->reissue_accept_ = new_value;
}

template <class HANDLER> int 
ACE_Asynch_Acceptor<HANDLER>::bytes_to_read (void) const
{
  return this->bytes_to_read_;
}

template <class HANDLER> void
ACE_Asynch_Acceptor<HANDLER>::bytes_to_read (int new_value)
{
  this->bytes_to_read_ = new_value;
}

template <class HANDLER> int 
ACE_Asynch_Acceptor<HANDLER>::should_reissue_accept (void) 
{
  return this->reissue_accept_;
}

#endif /* ACE_WIN32 */
#endif /* ACE_ASYNCH_ACCEPTOR_C */
