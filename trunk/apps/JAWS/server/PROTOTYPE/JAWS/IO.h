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

#if !defined (JAWS_IO_H)
#define JAWS_IO_H

class ACE_Message_Block;
class JAWS_IO_Handler;
class JAWS_IO_Acceptor;

#include "ace/ACE.h"
#include "ace/Asynch_IO.h"
#include "ace/SOCK_Stream.h"

class JAWS_IO
  // = TITLE
  //
  //     This class defines the abstract interface for an I/O class in
  //     the context of Web-likes servers
  //
  // = DESCRIPTION
  //
  //     An I/O class should have the following interface. Derived
  //     classes will define the exactly how the I/O will take place
  //     (Asynchronous, Synchronous, Reactive)
{
public:
  JAWS_IO (void);
  virtual ~JAWS_IO (void);
  void handler (JAWS_IO_Handler *handler);
  // void acceptor (JAWS_IO_Acceptor *acceptor);
  void handle (ACE_HANDLE h);
  ACE_HANDLE handle (void);

  // James, please add documentation here.

  virtual void accept (ACE_INET_Addr *addr) = 0;
  // accept a passive connection

  virtual void read (ACE_Message_Block& mb, int size) = 0;
  // read from the handle size bytes into the message block.

  virtual void transmit_file (const char *filename,
                              const char *header,
                              int header_size,
                              const char *trailer,
                              int trailer_size) = 0;
  // send header, filename, trailer to the handle.

  virtual void receive_file (const char *filename,
                             void *initial_data,
                             int initial_data_length,
                             int entire_length) = 0;
  // read data from the handle and store in filename.

  virtual void send_confirmation_message (const char *buffer, int length) = 0;
  // send a confirmation message to the handle.

  virtual void send_error_message (const char *buffer, int length) = 0;
  // send an error message to the handle.

protected:
  ACE_HANDLE handle_;
  JAWS_IO_Handler *handler_;
  ACE_INET_Addr *inet_addr_;
  JAWS_IO_Acceptor *acceptor_;
};

class JAWS_Synch_IO : public JAWS_IO
  // = TITLE
  //
  //     This class defines the interface for a Synchronous I/O class.
  //
  // = DESCRIPTION
{
public:
  JAWS_Synch_IO (void);

  ~JAWS_Synch_IO (void);

  virtual void accept (ACE_INET_Addr *addr);

  void read (ACE_Message_Block& mb, int size);

  void transmit_file (const char *filename,
                      const char *header,
                      int header_size,
                      const char *trailer,
                      int trailer_size);

  void receive_file (const char *filename,
                     void *initial_data,
                     int initial_data_length,
                     int entire_length);

  void send_confirmation_message (const char *buffer,
                                  int length);

  void send_error_message (const char *buffer,
                           int length);

protected:
  virtual void send_message (const char *buffer,
                             int length);
};


// This only works on Win32
#if defined (ACE_WIN32)

class JAWS_Asynch_IO : public JAWS_IO, public ACE_Handler
  // = TITLE
  //
  //     This class defines the interface for a Asynchronous I/O class.
  //
  // = DESCRIPTION
{
public:
  JAWS_Asynch_IO (void);

  ~JAWS_Asynch_IO (void);

  void read (ACE_Message_Block& mb, int size);

  void transmit_file (const char *filename,
                      const char *header,
                      int header_size,
                      const char *trailer,
                      int trailer_size);

  void receive_file (const char *filename,
                     void *initial_data,
                     int initial_data_length,
                     int entire_length);

  void send_confirmation_message (const char *buffer,
                                  int length);

  void send_error_message (const char *buffer,
                           int length);

protected:
  enum Message_Types
  {
    CONFORMATION,
    ERROR_MESSAGE
  };

  virtual void send_message (const char *buffer,
                             int length,
                             int act);

  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);
  // This method will be called when an asynchronous read completes on
  // a stream.

  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);
  // This method will be called when an asynchronous write completes
  // on a stream.

  virtual void handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result);
  // This method will be called when an asynchronous transmit file
  // completes.
};

#endif /* ACE_WIN32 */
#endif /* JAWS_IO_H */
