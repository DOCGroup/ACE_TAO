/* -*- c++ -*- */
// Hey, Emacs!  This is a C++ file!
// $Id$

// ============================================================================
//
// = LIBRARY
//   jaws
//
// = FILENAME
//    IO.h
//
// = AUTHOR
//    James Hu
//
// ============================================================================

#if !defined (JAWS_IO_STATE_H)
#define JAWS_IO_STATE_H

#include "ace/Asynch_IO.h"

// #include "JAWS/IO.h"
class JAWS_IO;
class JAWS_Synch_IO;
class JAWS_Asynch_IO;
class JAWS_IO_Handler;
class JAWS_Synch_IO_Handler;
class JAWS_Asynch_IO_Handler;
class JAWS_IO_Handler_Factory;
class JAWS_Synch_IO_Handler_Factory;
class JAWS_Asynch_IO_Handler_Factory;
class JAWS_Pipeline;

class JAWS_IO_Handler
  // = TITLE
  //
  //     This class defines the abstract interface for an I/O handler
  //     class in the context of Web-likes servers
  //
  // = DESCRIPTION
{
public:
  virtual void accept_complete (void) = 0;
  // This method is called by the IO class when new passive connection has
  // been established.

  virtual void accept_error (void) = 0;
  // This method is called by the IO class when new passive connection has
  // been established.

#if 0
  virtual void connect_complete (ACE_Message_Block &) = 0;
  // This method is called by the IO class when new active connection has
  // been established.

  virtual void connect_error (ACE_Message_Block &) = 0;
  // This method is called by the IO class when new active connection has
  // been established.
#endif

  virtual void read_complete (ACE_Message_Block &data) = 0;
  // This method is called by the IO class when new client data shows
  // up.

  virtual void read_error (void) = 0;
  // This method is called by the IO class when there was an error in
  // reading new data from the client.

  virtual void transmit_file_complete (void) = 0;
  // This method is called by the IO class when the requested file has
  // been successfully transmitted to the client.

  virtual void transmit_file_error (int result) = 0;
  // This method is called by the IO class when there was an error in
  // transmitting the requested file to the client.

  virtual void receive_file_complete (void) = 0;
  // This method is called by the IO class when the requested file has
  // been successfully received from the client.

  virtual void receive_file_error (int result) = 0;
  // This method is called by the IO class when there was an error in
  // receiving the requested file from the client.

  virtual void write_error (void) = 0;
  // This method is called by the IO class when there was an error in
  // writing data to the client.

  virtual void confirmation_message_complete (void) = 0;
  // This method is called by the IO class when the confirmation
  // message has been delivered to the client.

  virtual void error_message_complete (void) = 0;
  // This method is called by the IO class when the error message has
  // been delivered to the client.

  virtual JAWS_IO_Handler_Factory *factory (void) = 0;
  // Returns the factory for this IO handler
};

class JAWS_IO_Handler_Factory
#if defined (ACE_WIN32)
 : public ACE_Service_Handler
#endif
{
public:
  virtual ~JAWS_IO_Handler_Factory (void);
  // Destructor

  virtual JAWS_IO_Handler *create_io_handler (void) = 0;
  // This creates a new HTTP_Handler

  virtual void destroy_io_handler (JAWS_IO_Handler *handler, JAWS_IO *io) = 0;
  // The HTTP handler will call this method from HTTP_Handler::done to
  // tell the factory to reap up the handler as it is now done with
  // the protocol
};

class JAWS_Synch_IO_Handler : protected JAWS_IO_Handler
{
friend class JAWS_Synch_IO;
friend class JAWS_Synch_IO_Handler_Factory;

public:
  JAWS_Synch_IO_Handler (JAWS_IO *io, JAWS_IO_Handler_Factory *factory);
  virtual ~JAWS_Synch_IO_Handler (void);

protected:
  // Inherited from JAWS_IO_Handler

  virtual void accept_complete (void);
  virtual void accept_error (void);
  virtual void read_complete (ACE_Message_Block &data);
  virtual void read_error (void);
  virtual void transmit_file_complete (void);
  virtual void transmit_file_error (int result);
  virtual void receive_file_complete (void);
  virtual void receive_file_error (int result);
  virtual void write_error (void);
  virtual void confirmation_message_complete (void);
  virtual void error_message_complete (void);
  virtual JAWS_IO_Handler_Factory *factory (void);

  virtual void done (void);

private:
  ACE_Message_Block *state_;
  // This maintains the state of the request.

  JAWS_IO *io_;
  // The reference to our IO interface (synch vs. asynch)

  JAWS_Pipeline *pipeline_;
  // This is a reference to the next stage of the pipeline when the IO
  // request completes.

  JAWS_IO_Handler_Factory *factory_;
};

class JAWS_Synch_IO_Handler_Factory : public JAWS_IO_Handler_Factory
{
public:
  JAWS_IO_Handler *create_io_handler (void);
  // This creates a new HTTP_Handler

  void destroy_io_handler (JAWS_IO_Handler *handler, JAWS_IO *io);
  // The HTTP handler will call this method from HTTP_Handler::done to
  // tell the factory to reap up the handler as it is now done with
  // the protocol
};

// This only works on Win32
#if defined (ACE_WIN32)

class JAWS_Asynch_IO_Handler : protected JAWS_IO_Handler
{
public:
  JAWS_Asynch_IO_Handler (void);
  virtual ~JAWS_Asynch_IO_Handler (void);

protected:
  // Inherited from JAWS_IO_Handler

  virtual void accept_complete (void);
  virtual void accept_error (void);
  virtual void read_complete (ACE_Message_Block &data);
  virtual void read_error (void);
  virtual void transmit_file_complete (void);
  virtual void transmit_file_error (int result);
  virtual void receive_file_complete (void);
  virtual void receive_file_error (int result);
  virtual void write_error (void);
  virtual void confirmation_message_complete (void);
  virtual void error_message_complete (void);
  virtual JAWS_Handler_Factory *factory (void);

private:
  ACE_Message_Block *state_;
  // This maintains the state of the request.

  JAWS_Dispatch_Policy *policy_;
  // The reference to our IO interface (synch vs. asynch)

  JAWS_Pipeline *pipeline_;
  // This is a reference to the next stage of the pipeline when the IO
  // request completes.

  JAWS_IO_Handler_Factory *factory_;
};

class JAWS_Asynch_IO_Handler_Factory : public JAWS_IO_Handler_Factory
{
public:
  JAWS_IO_Handler *create_io_handler (void);
  // This creates a new HTTP_Handler

  void destroy_io_handler (JAWS_IO_Handler *handler, JAWS_IO *io);
  // The HTTP handler will call this method from HTTP_Handler::done to
  // tell the factory to reap up the handler as it is now done with
  // the protocol
};

#endif /* ACE_WIN32 */
#endif /* JAWS_IO_STATE_H */
