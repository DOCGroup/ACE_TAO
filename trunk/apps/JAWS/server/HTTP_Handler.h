#if !defined (HTTP_HANDLER_H)
#define HTTP_HANDLER_H

// Forward declarations
class JAWS_IO;
class Message_Block;
class HTTP_Handler_Factory;

#include "ace/Asynch_IO.h"
#include "JAWS/server/HTTP_Request.h"

class HTTP_Handler
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

  ~HTTP_Handler (void);
  // Destructor

  virtual void done (void);
  // This is the termination state of the handler. After successful or
  // unsuccessful completions, the handler will end up in this state
  // (method).
  
  virtual void client_data (ACE_Message_Block &data);
  // This method is called by the IO class when new client data shows
  // up.

  virtual void read_error (void);
  // This method is called by the IO class when there was an error in
  // reading new data from the client.

  virtual void transmit_file_complete (void);
  // This method is called by the IO class when the requested file has
  // been successfully transmitted to the client.

  virtual void transmit_file_error (int result);
  // This method is called by the IO class when there was an error in
  // transmitting the requested file to the client.

  virtual void receive_file_complete (void);
  // This method is called by the IO class when the requested file has
  // been successfully received from the client.

  virtual void receive_file_error (int result);
  // This method is called by the IO class when there was an error in
  // receiving the requested file from the client.

  virtual void write_error (void);
  // This method is called by the IO class when there was an error in
  // writing data to the client.

  virtual void timeout (void);
  // This method is called by the framework when there is a timeout.

  virtual int enough_data (void);
  // Has the header of the client request been read yet?
  
  virtual void confirmation_message_complete (void);
  // This method is called by the IO class when the confirmation
  // message has been delivered to the client.

  virtual void error_message_complete (void);
  // This method is called by the IO class when the error message has
  // been delivered to the client.

  virtual void parse_request (void);
  // Parse the client request

  virtual void invalid_request (int result);
  // Invalid request.

  virtual void request_too_long (void);
  // Request too long.

  virtual void serve_error (int status_code);
  // Send the <status_code> error to the client.

  virtual void serve_directory (void);
  // The client has asked for a directory listing.

  HTTP_Handler_Factory &factory_;
  // Reference to the creating factory.

public:
  enum 
  {
    MAX_REQUEST_SIZE = 1024,
    METHODSIZ = 10, 
    VERSIONSIZ = 10
  };

private:
  ACE_Message_Block *request_data_;
  // This points to the request sent by the client

  ACE_HANDLE handle_;
  // I/O handle to the client

  HTTP_Request request_;

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
