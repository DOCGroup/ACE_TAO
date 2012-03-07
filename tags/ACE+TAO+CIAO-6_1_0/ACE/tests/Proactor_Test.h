
//=============================================================================
/**
 *  @file    Proactor_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author @author Alexander Libman <alibman@baltimore.com>
 */
//=============================================================================


#ifndef ACE_TESTS_PROACTOR_TEST_H
#define ACE_TESTS_PROACTOR_TEST_H

#include "ace/Synch_Traits.h"
#include "ace/Thread_Mutex.h"

// forward declaration
class TestData;

class Server : public ACE_Service_Handler
{
public:
  Server ();
  Server (TestData *tester, int id);
  ~Server (void);

  int id (void) { return this->id_; }
  size_t get_total_snd (void) { return this->total_snd_; }
  size_t get_total_rcv (void) { return this->total_rcv_; }
  long get_total_w   (void) { return this->total_w_; }
  long get_total_r   (void) { return this->total_r_; }

  // This is called to pass the new connection's addresses.
  virtual void addresses (const ACE_INET_Addr& peer,
                          const ACE_INET_Addr& local);

  /// This is called after the new connection has been accepted.
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);
  void cancel ();

protected:
  //// This is called when asynchronous <read> operation from the
  //// socket completes.
  /**
   * @name AIO callback handling
   *
   * These methods are called by the framework
   */
  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);

  /// This is called when an asynchronous <write> to the socket
  /// completes.
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

private:
  int initiate_read_stream (void);
  int initiate_write_stream (ACE_Message_Block &mb, size_t nbytes);

  TestData *tester_;
  int id_;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;
  ACE_SYNCH_MUTEX lock_;

  int io_count_;             // Number of currently outstanding I/O requests
  int flg_cancel_;
  size_t total_snd_;         // Number of bytes successfully sent
  size_t total_rcv_;         // Number of bytes successfully received
  int total_w_;              // Number of write operations
  int total_r_;              // Number of read operations
};

// *******************************************
//   Client
// *******************************************

class Client : public ACE_Service_Handler
{
public:

  /// This is called after the new connection has been established.
  virtual void open (ACE_HANDLE handle,
                     ACE_Message_Block &message_block);

  Client ();
  Client (TestData *tester, int id);
  ~Client (void);

  int id (void) { return this->id_; }
  size_t get_total_snd (void) { return this->total_snd_; }
  size_t get_total_rcv (void) { return this->total_rcv_; }
  int    get_total_w   (void) { return this->total_w_; }
  int    get_total_r   (void) { return this->total_r_; }

  // This is called to pass the new connection's addresses.
  virtual void addresses (const ACE_INET_Addr& peer,
                          const ACE_INET_Addr& local);

  /// This is called when asynchronous reads from the socket complete
  virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result);

  /// This is called when asynchronous writes from the socket complete
  virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result);

  void cancel (void);

private:
  int initiate_read_stream (void);
  int initiate_write_stream (void);
  void close (void);

  TestData *tester_;
  int id_;

  ACE_Asynch_Read_Stream rs_;
  ACE_Asynch_Write_Stream ws_;
  ACE_HANDLE handle_;

  ACE_SYNCH_MUTEX lock_;

  int io_count_;
  int stop_writing_;           // Writes are shut down; just read.
  int flg_cancel_;
  size_t total_snd_;
  size_t total_rcv_;
  int total_w_;
  int total_r_;
};

#endif /* ACE_TESTS_PROACTOR_TEST_H */
