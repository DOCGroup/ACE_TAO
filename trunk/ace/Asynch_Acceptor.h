/* -*- C++ -*- */
// $Id: Asynch_Acceptor.h,v

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Asynch_Acceptor.h
//
// = AUTHOR
//    Irfan Pyarali (irfan@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_ASYNCH_ACCEPTOR_H)
#define ACE_ASYNCH_ACCEPTOR_H

#include "ace/OS.h"

#if defined (ACE_WIN32)
// This only works on Win32 platforms

#include "ace/Asynch_IO.h"

// Forward declarations
class ACE_Message_Block;
class ACE_INET_Addr;

template <class HANDLER>
class ACE_Asynch_Acceptor : public ACE_Handler 
  //     
  // = TITLE
  //
  //     This class is an example of the Acceptor Pattern.  This class
  //     will accept new connections and create new HANDLER to handle
  //     the new connections.
  // 
  // = DESCRIPTION
  //     
{
public:
  ACE_Asynch_Acceptor (void);
  // A do nothing constructor.

  ~ACE_Asynch_Acceptor (void);
  // Virtual destruction

  int open (const ACE_INET_Addr &address,
	    size_t bytes_to_read = 0,
	    int pass_addresses = 0,
	    int backlog = 5,
	    int reuse_addr = 1,
	    ACE_Proactor *proactor = 0);
  // This starts the listening process at the port specified by
  // <address>.  ACE_Asynch_Acceptor initiates the AcceptEx calls with
  // <bytes_to_read>.  The buffer for the initial data will be created
  // by ACE_Asynch_Acceptor.  This buffer will be passed to the
  // handler in the <ACE_Service_Handler::open> callback.  If this
  // buffer is required past the <open> callback, the
  // ACE_Service_Handler must copy the data.  If the <pass_addresses>
  // flag is set, ACE_Asynch_Acceptor will call
  // <ACE_Service_Handler::addresses> before calling
  // <ACE_Service_Handler::open>.  The <backlog> parameter specifies
  // the listen backlog and the outstanding AcceptEx calls.

  int accept (size_t bytes_to_read = 0);
  // This initiates a new asynchronous accept through the AcceptEx call.

  static size_t address_size (void);
  // This is required by the AcceptEx call.

  int cancel (void);
  // This cancels all pending accepts operations that were issued by
  // the calling thread.  The function does not cancel accept
  // operations issued by other threads.

protected:
  virtual void handle_accept (const ACE_Asynch_Accept::Result &result);
  // This is called when an outstanding accept completes. 

  ACE_HANDLE handle (void) const;
  // Return the listen handle.

  void parse_address (ACE_Message_Block &message_block,
		      ACE_INET_Addr &remote_address,
		      ACE_INET_Addr &local_address);
  // This parses the address from read buffer.

  virtual HANDLER *make_handler (void);
  // This is the template method used to create new handler.
  // Subclasses must overwrite this method if a new handler creation
  // strategy is required.
  
private:
  ACE_HANDLE listen_handle_;
  // Handle used to listen for new connections.

  ACE_Asynch_Accept asynch_accept_;
  // Asynch_Accept used to make life easier :-)

  int pass_addresses_;
  // Flag that indicates if parsing of addresses is necessary.

  int bytes_to_read_;
  // Bytes to be read with the accept call.
};

#if defined (__ACE_INLINE__)
#include "ace/Asynch_Acceptor.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Asynch_Acceptor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Asynch_Acceptor.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_WIN32 */
#endif /* ACE_ASYNCH_ACCEPTOR_H */

