/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

#if !defined (HTTP_HANDLER_H)
#define HTTP_HANDLER_H

// Forward declarations
class Message_Block;
class HTTP_Handler_Factory;

#include "ace/Asynch_IO.h"
#include "apps/JAWS/server/HTTP_Request.h"
#include "apps/JAWS/server/HTTP_Response.h"
#include "apps/JAWS/server/IO.h"

class HTTP_Handler : protected JAWS_IO_Handler
  //
  // = TITLE
  //     
  //     This class is used to implement the HTTP protocol
  //
  // = DESCRIPTION
  //     
  //	 The HTTP_Handler class is a state based implementation of the
  //	 HTTP protocol. Therefore, it can be used synchronously and
  //	 asynchronously. It uses an abstract IO class to move between
  //	 different HTTP protocol states. It is up to the IO class to
  //	 decide on synchronous or asynchronous I/O.
{
  // Friend I/O classes. Can call protected methods.
  friend class JAWS_Synch_IO;
  friend class JAWS_Asynch_IO;

  // Factories
  friend class Asynch_HTTP_Handler_Factory;
  friend class Synch_HTTP_Handler_Factory;

public:
  virtual void open (ACE_HANDLE handle, 
		     ACE_Message_Block &initial_data);
  // The handler is initialized with a connection <handle> of a new
  // client and any <initial_data> that came across. The
  // <initial_data> block will be of MAX_REQUEST_SIZE and the number
  // of bytes in <initial_data> can be found from
  // <initial_data>.length ()

protected:
  HTTP_Handler (JAWS_IO &io,
		HTTP_Handler_Factory &factory);
  // The constructor is passed the factory that created <this> and the
  // IO mechanism that the handler should use.

  virtual ~HTTP_Handler (void);
  // Destructor

  virtual void timeout (void);
  // This method is called by the framework when there is a timeout.

  virtual void done (void);
  // This is the termination state of the handler. After successful or
  // unsuccessful completions, the handler will end up in this state
  // (method).
  
  virtual void request_too_long (void);
  // Request too long.

  HTTP_Handler_Factory &factory_;
  // Reference to the creating factory.

protected:
  // Methods inherited from JAWS_IO_Handler

  void read_complete (ACE_Message_Block &data);
  void read_error (void);
  void transmit_file_complete (void);
  void transmit_file_error (int result);
  void receive_file_complete (void);
  void receive_file_error (int result);
  void write_error (void);
  void confirmation_message_complete (void);
  void error_message_complete (void);

public:
  enum 
  {
    MAX_REQUEST_SIZE = 8192,
    METHODSIZ = 10, 
    VERSIONSIZ = 10
  };

private:
  ACE_Message_Block *request_data_;
  // This points to the request sent by the client

  ACE_HANDLE handle_;
  // I/O handle to the client

  HTTP_Request request_;
  HTTP_Response response_;

  JAWS_IO &io_;
  // IO class used by the handler

};

class HTTP_Handler_Factory
  //
  // = TITLE
  //     
  //     This class is used to create new HTTP handlers
  //
  // = DESCRIPTION
  //     
  //     This is an abstract factory for creating new HTTP handlers
{
public:
  virtual ~HTTP_Handler_Factory (void);
  // Destructor

  virtual HTTP_Handler *create_http_handler (void) = 0;
  // This creates a new HTTP_Handler

  virtual void destroy_http_handler (HTTP_Handler &handler,
				     JAWS_IO &io) = 0;
  // The HTTP handler will call this method from HTTP_Handler::done to
  // tell the factory to reap up the handler as it is now done with
  // the protocol
};

class Synch_HTTP_Handler_Factory : public HTTP_Handler_Factory
  //
  // = TITLE
  //     
  //     This class is used to create new HTTP handlers that will use
  //     Synch IO
  //
  // = DESCRIPTION
  //     
{
public:
  HTTP_Handler *create_http_handler (void);
  // This creates a new HTTP_Handler

  void destroy_http_handler (HTTP_Handler &handler,
			     JAWS_IO &io);
  // The HTTP handler will call this method from HTTP_Handler::done to
  // tell the factory to reap up the handler as it is now done with
  // the protocol
};

// This only works on Win32
#if defined (ACE_WIN32)
class Asynch_HTTP_Handler_Factory : public HTTP_Handler_Factory, public ACE_Service_Handler
{
public:
  void destroy_http_handler (HTTP_Handler &handler,
			     JAWS_IO &io);
  // The HTTP handler will call this method from HTTP_Handler::done to
  // tell the factory to reap up the handler as it is now done with
  // the protocol

  void open (ACE_HANDLE handle,
	     ACE_Message_Block &message_block);
  // <open> is called by ACE_Asynch_Acceptor to initialize a new
  // instance of ACE_Service_Handler that has been created after the a
  // new connection is accepted. 
  //
  // This will act as a creation point for new handlers

private:
  HTTP_Handler *create_http_handler (void);
  // This method is private as users are not allowed to create new
  // handlers. New handlers can only be created by the framework when
  // new client connections arrive.
};
#endif /* ACE_WIN32 */

#endif /* HTTP_HANDLER_H */
