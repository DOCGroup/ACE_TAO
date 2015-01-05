/**
 * @file Bug_2912_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2912:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2912
 *
 * This test reproduces the following interactions:
 *
 * ACE_TMAIN          Client proactor          Server proactor
 * thread             dispatcher thread        dispatcher thread
 * ----------------   -------------------      --------------------
 *
 * init_ssl
 * connect
 *                    SH::open                 SH::open
 *                    SH::read                 SH::read
 *                    SH::write
 *                                             SH::handle_read_stream
 *                                             SH::write
 *                                             SH::read
 *                    SH::handle_read_stream
 *                    SH::read
 * SH::write (causes do_SSL_read to fail)
 *
 * @author Paul Daugherty <paul@nxicom.com>
 */

#include "ace/ACE.h"
#include "../test_config.h"
#include "ace/SSL/SSL_Asynch_Stream.h"
#include "ace/Proactor.h"
#include "ace/Task.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Manual_Event.h"
#include "ace/OS_NS_unistd.h"

/* Linux kernels can't hack multiple outstanding I/O, which this
   test requires */
#if defined (ACE_HAS_THREADS) && \
    (defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)) && \
    !defined (ACE_LINUX)

/**
 * Data payload sent between client and server.  Test is not dependent
 * on payload characteristics.
 */
#define DATA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DATA_SIZE ACE_OS::strlen(DATA)

// Function to remove signals from the signal mask.
static int
disable_signal (int sigmin, int sigmax)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  sigset_t signal_set;
  if (ACE_OS::sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: (%P|%t):%p\n"),
                ACE_TEXT ("sigemptyset failed")));

  for (int i = sigmin; i <= sigmax; i++)
    ACE_OS::sigaddset (&signal_set, i);

  // Put the <signal_set>.
# if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  // In multi-threaded application this is not POSIX compliant
  // but let's leave it just in case.
  if (ACE_OS::sigprocmask (SIG_BLOCK, &signal_set, 0) != 0)
# else
  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, &signal_set, 0) != 0)
# endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: (%P|%t): %p\n"),
                       ACE_TEXT ("SIG_BLOCK failed")),
                      -1);
#else
  ACE_UNUSED_ARG (sigmin);
  ACE_UNUSED_ARG (sigmax);
#endif /* ACE_LACKS_UNIX_SIGNALS */

  return 0;
}

/**
 * Client's proactor
 *
 * The client and server must use different proactors since this bug is
 * dependent on threads.
 */
class Client_Proactor : public ACE_Proactor {};
typedef ACE_Singleton<Client_Proactor, ACE_SYNCH_RECURSIVE_MUTEX>
    Client_Proactor_Singleton;
#define CLIENT_PROACTOR Client_Proactor_Singleton::instance()

class Client_Proactor_Task : public ACE_Task_Base
{
public:
  virtual int svc (void);
};

typedef ACE_Singleton<Client_Proactor_Task, ACE_SYNCH_RECURSIVE_MUTEX>
    Client_Proactor_Task_Singleton;
#define CLIENT_PROACTOR_TASK Client_Proactor_Task_Singleton::instance()

int
Client_Proactor_Task::svc (void)
{
  // Keep RT signals on POSIX from killing us.
  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);

  CLIENT_PROACTOR->proactor_reset_event_loop ();
  CLIENT_PROACTOR->proactor_run_event_loop ();
  return 0;
}


/**
 * Server's proactor
 */
class Server_Proactor : public ACE_Proactor {};
typedef ACE_Singleton<Server_Proactor, ACE_SYNCH_RECURSIVE_MUTEX>
    Server_Proactor_Singleton;
#define SERVER_PROACTOR Server_Proactor_Singleton::instance ()

class Server_Proactor_Task : public ACE_Task_Base
{
public:
  virtual int svc (void);
};

typedef ACE_Singleton<Server_Proactor_Task, ACE_SYNCH_RECURSIVE_MUTEX>
    Server_Proactor_Task_Singleton;
#define SERVER_PROACTOR_TASK Server_Proactor_Task_Singleton::instance ()

int
Server_Proactor_Task::svc (void)
{
  // Keep RT signals on POSIX from killing us.
  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);

  SERVER_PROACTOR->proactor_reset_event_loop ();
  SERVER_PROACTOR->proactor_run_event_loop ();
  return 0;
}

/**
 * This test depends on ADH since the error is related to a missing cert.pem
 */
static DH* dh1024 = 0;

DH*
get_dh1024 ()
{
  static unsigned char dh1024_p[]={
        0xD7,0xFE,0xEC,0x06,0x28,0x03,0x34,0x96,0xB8,0x08,0x86,0x62,
            0xF1,0xA2,0xBA,0x84,0x7C,0xAF,0xA3,0x1F,0x6A,0x3D,0x03,0x20,
            0x81,0x8D,0x0E,0x43,0x3A,0x54,0x74,0x9F,0x83,0xD2,0xB7,0xE9,
            0x57,0xC1,0x67,0xE9,0x11,0x38,0x2B,0x8E,0x9B,0x1C,0x5D,0x14,
            0x18,0x7D,0x4F,0xEB,0xB1,0x4D,0xFA,0x6F,0x06,0xDD,0xDD,0x6D,
            0x9A,0xD0,0x9E,0x4F,0xE4,0x04,0x3E,0x86,0x6F,0x15,0x60,0x35,
            0x9B,0xA1,0xBA,0x53,0xBA,0x84,0xB5,0x06,0xB1,0xAD,0x94,0x25,
            0xD1,0xED,0xD2,0xF4,0xD7,0x02,0x2F,0x35,0x25,0xE7,0x2D,0x60,
            0xEE,0x7A,0x61,0xAD,0x98,0xA8,0x3D,0xAD,0xB1,0x8A,0x5E,0xCE,
            0xF0,0x09,0xD4,0x67,0x28,0x3D,0x52,0x64,0x78,0xBB,0xC3,0x9D,
            0x40,0xF4,0x72,0xDC,0xC9,0x31,0x0D,0xA3,
  };
  static unsigned char dh1024_g[]={
    0x02,
  };
  DH *dh;

  if ((dh=DH_new()) == 0) return(0);
  dh->p=BN_bin2bn(dh1024_p,sizeof(dh1024_p),0);
  dh->g=BN_bin2bn(dh1024_g,sizeof(dh1024_g),0);
  if ((dh->p == 0) || (dh->g == 0))
    { DH_free(dh); return(0); }
  return(dh);
}

extern "C" DH*
tmp_dh_callback (SSL *s, int is_export, int keylength)
{
  ACE_UNUSED_ARG(s);
  ACE_UNUSED_ARG(is_export);

  switch (keylength) {
  case 1024:
    if (dh1024 == 0)
    {
      dh1024 = get_dh1024();
    }
    return dh1024;
  default:
    /* Generating a key on the fly is very costly, so use what is there */
    ACE_ERROR ((LM_ERROR,
      ACE_TEXT ("tmp_dh_callback, unsupported key length, %d\n"),
      keylength));
  }
  return(0);
}

bool
init_ssl (void)
{
  SSL_CTX_set_quiet_shutdown (ACE_SSL_Context::instance ()->context(), 1);
  SSL_CTX_set_options (ACE_SSL_Context::instance ()->context(),
      SSL_OP_SINGLE_DH_USE);
  SSL_CTX_set_tmp_dh_callback (ACE_SSL_Context::instance ()->context (),
      tmp_dh_callback);

  if (SSL_CTX_set_cipher_list (ACE_SSL_Context::instance ()->context (), "ADH"))
  {
    return true;
  }
  else
  {
    ACE_DEBUG ((LM_ERROR,
      ACE_TEXT ("SSL_CTX_set_cipher_list failed\n")));
    return false;
  }
}


/**
 * Server's ACE_Service_Handler
 */
class Server_Service_Handler : public ACE_Service_Handler
{
public:
  Server_Service_Handler (void);

  virtual ~Server_Service_Handler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void open (ACE_HANDLE h, ACE_Message_Block&);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual void handle_read_stream (
    const ACE_Asynch_Read_Stream::Result &result);

  virtual void handle_write_stream (
    const ACE_Asynch_Write_Stream::Result &result);

  virtual void handle_wakeup (void);

  void cancel_and_close (void);

  int read_data (void);

  int write_data (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int read (ACE_Message_Block &mb, size_t bytes_to_read);

  int write (ACE_Message_Block &mb, size_t bytes_to_write);
  //FUZZ: enable check_for_lack_ACE_OS

  int safe_to_delete (void) const;

private:
  mutable ACE_SYNCH_RECURSIVE_MUTEX  mtx_;
  ACE_SSL_Asynch_Stream ssl_stream_;
  int pending_writes_;
  int pending_reads_;
  int handle_wakeup_expected_;
  int handle_wakeup_received_;
  int closing_;
};

Server_Service_Handler::Server_Service_Handler (void) :
  ssl_stream_ (ACE_SSL_Asynch_Stream::ST_SERVER),
  pending_writes_ (0),
  pending_reads_ (0),
  handle_wakeup_expected_ (0),
  handle_wakeup_received_ (0),
  closing_ (0)
{
}

Server_Service_Handler::~Server_Service_Handler (void)
{
  if (ACE_INVALID_HANDLE != this->handle ())
  {
    ACE_OS::closesocket (this->handle ());
    this->handle (ACE_INVALID_HANDLE);
  }
}

void
Server_Service_Handler::open (ACE_HANDLE h, ACE_Message_Block&)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  if (this->ssl_stream_.open (*this, h, 0, this->proactor ()) != 0)
  {
    //FUZZ: disable check_for_lack_ACE_OS
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT("Server_Service_Handler::open: ")
                ACE_TEXT("ACE_SSL_Asynch_Stream::open failed, %d\n"),
                (int)errno));
    //FUZZ: enable check_for_lack_ACE_OS

    this->cancel_and_close ();
  }
  else
  {
    if (this->read_data () < 0)
    {
      this->cancel_and_close ();
    }
  }
}

void
Server_Service_Handler::handle_read_stream(
  const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->pending_reads_--;

  if (!result.success () || 0 == result.bytes_transferred ())
  {
    // Error or remote disconnect

    result.message_block ().release ();

    if (!this->closing_)
    {
      // No error message when shutting down

      if (!result.success ())
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("Server_Service_Handler::handle_read_stream: error: %d\n"),
          result.error ()));
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG,
          ACE_TEXT ("Server_Service_Handler::handle_read_stream: remote disconnect\n")));
      }
    }

    this->cancel_and_close ();
  }
  else if (result.bytes_transferred () < result.bytes_to_read ())
  {
    // More to read...

    if (this->read (result.message_block (),
      result.bytes_to_read () - result.bytes_transferred ()) < 0)
    {
      result.message_block ().release ();

      this->cancel_and_close ();
    }
  }
  else
  {
    // Read it all

    result.message_block ().release ();

    // Send data back

    if (this->write_data () < 0)
    {
      this->cancel_and_close ();
    }

    // Next read

    else if (this->read_data () < 0)
    {
      this->cancel_and_close ();
    }
  }
}

void
Server_Service_Handler::handle_write_stream (
  const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->pending_writes_--;

  if (!result.success () || 0 == result.bytes_transferred ())
  {
    // Error

    result.message_block ().release ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
      "Server_Service_Handler::handle_write_stream: error: %d\n"),
      result.error ()));

    this->cancel_and_close ();
  }
  else if (result.bytes_transferred () < result.bytes_to_write ())
  {
    // More to write...

    if (this->write(result.message_block (),
      result.bytes_to_write () - result.bytes_transferred ()) < 0)
    {
      result.message_block ().release ();

      this->cancel_and_close ();
    }
  }
  else
  {
    // Wrote it all

    result.message_block().release ();
  }
}

void
Server_Service_Handler::handle_wakeup (void)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->handle_wakeup_received_ = 1;
}

void
Server_Service_Handler::cancel_and_close (void)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->closing_ = 1;
  this->ssl_stream_.cancel ();
  this->handle_wakeup_expected_ = -1 == this->ssl_stream_.close ();
}

int
Server_Service_Handler::read_data (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_NORETURN(mb, ACE_Message_Block (DATA_SIZE));

  int ret = this->read (*mb, DATA_SIZE);
  if (ret < 0)
  {
    mb->release ();
    return -1;
  }
  else
  {
    return 0;
  }
}

int
Server_Service_Handler::write_data (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_NORETURN(mb, ACE_Message_Block (DATA_SIZE));
  ACE_OS::memcpy (mb->wr_ptr (), DATA, DATA_SIZE);
  mb->wr_ptr (DATA_SIZE);

  int ret = this->write (*mb, DATA_SIZE);
  if (ret < 0)
  {
    mb->release ();
    return -1;
  }
  else
  {
    return 0;
  }
}

int
Server_Service_Handler::read (ACE_Message_Block &mb, size_t bytes_to_read)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  int ret;
  if ((ret = this->ssl_stream_.read (mb, bytes_to_read)) < 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
      "Server_Service_Handler::read: read failed: %d\n"), (int)errno));
  }
  else
  {
    this->pending_reads_++;
  }
  return ret;
}

int
Server_Service_Handler::write (ACE_Message_Block &mb, size_t bytes_to_write)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  int ret;
  if ((ret = this->ssl_stream_.write (mb, bytes_to_write)) < 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
      "Server_Service_Handler::write: write failed: %d\n"), (int)errno));
  }
  else
  {
    this->pending_writes_++;
  }
  return ret;
}

int
Server_Service_Handler::safe_to_delete (void) const
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  return 0 == this->pending_writes_ &&
    0 == this->pending_reads_ &&
    (!this->handle_wakeup_expected_ || this->handle_wakeup_received_);
}

/**
 * Server's acceptor
 */
class Acceptor : public ACE_Asynch_Acceptor<Server_Service_Handler>
{
public:
  Acceptor (void);

  virtual ~Acceptor (void);

  virtual int cancel (void);

  virtual int validate_connection (const ACE_Asynch_Accept::Result& result,
    const ACE_INET_Addr &remote, const ACE_INET_Addr& local);

  virtual Server_Service_Handler *make_handler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int accept (size_t bytes_to_read = 0, const void *act = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual void handle_accept (const ACE_Asynch_Accept::Result &result);

  int safe_to_delete (void) const;

  void prepare_for_connection (Server_Service_Handler *service_handler);

  mutable ACE_SYNCH_RECURSIVE_MUTEX  mtx_;
  int accept_cnt_;
  int cancel_flag_;
  Server_Service_Handler *service_handler_;
};

typedef ACE_Singleton<Acceptor, ACE_SYNCH_RECURSIVE_MUTEX> Acceptor_Singleton;
#define ACCEPTOR Acceptor_Singleton::instance ()

Acceptor::Acceptor (void) :
  accept_cnt_ (0),
  cancel_flag_ (0),
  service_handler_ (0)
{
}

Acceptor::~Acceptor (void)
{
}

int
Acceptor::cancel (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  this->cancel_flag_ = 1;
  this->reissue_accept (0);

  ACE_HANDLE h = this->get_handle ();
  if (h != ACE_INVALID_HANDLE)
  {
    this->ACE_Asynch_Acceptor<Server_Service_Handler>::cancel ();

    ACE_OS::closesocket (h);
    this->handle (ACE_INVALID_HANDLE);
  }

  return 0;
}

int
Acceptor::safe_to_delete (void) const
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);
  return (this->cancel_flag_ != 0 && this->accept_cnt_ == 0) ? 1 : 0;
}

void
Acceptor::prepare_for_connection (Server_Service_Handler *service_handler)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);
  this->service_handler_ = service_handler;
}

int
Acceptor::validate_connection (const ACE_Asynch_Accept::Result& result,
  const ACE_INET_Addr & /*remote*/, const ACE_INET_Addr& /*local*/)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  if (0 != this->service_handler_ && result.success ())
  {
    return 0;
  }
  else
  {
    return -1;
  }
}

Server_Service_Handler*
Acceptor::make_handler (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, 0);

  ACE_ASSERT (0 != this->service_handler_);
  Server_Service_Handler *service_handler = this->service_handler_;
  this->service_handler_ = 0;
  return service_handler;
}

int
Acceptor::accept (size_t bytes_to_read, const void *act)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  if  (this->cancel_flag_!= 0)
    return -1;

  this->accept_cnt_++;
  int rc = this->ACE_Asynch_Acceptor<Server_Service_Handler>::accept (
    bytes_to_read, act);
  if (rc != 0)
    this->accept_cnt_--;

  return rc;
}

void
Acceptor::handle_accept (const ACE_Asynch_Accept::Result &result)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->ACE_Asynch_Acceptor<Server_Service_Handler>::handle_accept (result);

  --this->accept_cnt_;
}


/**
 * Client's ACE_Service_Handler
 */
class Client_Service_Handler : public ACE_Service_Handler
{
public:
  Client_Service_Handler (void);

  virtual ~Client_Service_Handler (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual void open (ACE_HANDLE h, ACE_Message_Block&);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual void handle_read_stream (
    const ACE_Asynch_Read_Stream::Result &result);

  virtual void handle_write_stream (
    const ACE_Asynch_Write_Stream::Result &result);

  virtual void handle_wakeup (void);

  void cancel_and_close (void);

  int read_data (void);

  int write_data (void);

  //FUZZ: disable check_for_lack_ACE_OS
  int read (ACE_Message_Block &mb, size_t bytes_to_read);

  int write (ACE_Message_Block &mb, size_t bytes_to_write);
  //FUZZ: enable check_for_lack_ACE_OS

  int safe_to_delete (void) const;

  int wait_for_external_write_queue (ACE_Time_Value *wait_time);

  int wait_for_read_completed (ACE_Time_Value *wait_time);

  int read_successful (void) const;

private:
  mutable ACE_SYNCH_RECURSIVE_MUTEX  mtx_;
  ACE_SSL_Asynch_Stream ssl_stream_;
  ACE_Manual_Event ready_for_external_write_;
  ACE_Manual_Event read_completed_;
  int read_successful_;
  int pending_writes_;
  int pending_reads_;
  int handle_wakeup_expected_;
  int handle_wakeup_received_;
  int completed_reads_;
  int closing_;
};

Client_Service_Handler::Client_Service_Handler (void) :
  ssl_stream_ (ACE_SSL_Asynch_Stream::ST_CLIENT),
  read_successful_ (0),
  pending_writes_ (0),
  pending_reads_ (0),
  handle_wakeup_expected_ (0),
  handle_wakeup_received_ (0),
  completed_reads_ (0),
  closing_ (0)
{
}

Client_Service_Handler::~Client_Service_Handler (void)
{
  if (ACE_INVALID_HANDLE != this->handle ())
  {
    ACE_OS::closesocket (this->handle ());
    this->handle (ACE_INVALID_HANDLE);
  }
}

void
Client_Service_Handler::open (ACE_HANDLE h, ACE_Message_Block&)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  if (this->ssl_stream_.open (*this, h, 0, this->proactor ()) != 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
      "Client_Service_Handler::open: ACE_SSL_Asynch_Stream::open failed, %d\n"),
      (int)errno));
    this->cancel_and_close ();
  }
  else
  {
    ACE_Message_Block *mb = 0;
    ACE_NEW_NORETURN(mb, ACE_Message_Block (DATA_SIZE));

    if (this->read_data () < 0 || this->write_data () < 0)
    {
      this->cancel_and_close ();
    }
  }
}

void
Client_Service_Handler::handle_read_stream (
  const ACE_Asynch_Read_Stream::Result &result)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->pending_reads_--;

  if (!result.success () || 0 == result.bytes_transferred ())
  {
    // Error or remote disconnect

    result.message_block ().release ();

    if (!this->closing_)
    {
      // No error message when shutting down

      if (!result.success ())
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
          "Client_Service_Handler::handle_read_stream: error: %d\n"),
          result.error ()));
      }
      else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
          "Client_Service_Handler::handle_read_stream: remote disconnect\n")));
      }
    }

    this->read_completed_.signal ();

    this->cancel_and_close ();
  }
  else if (result.bytes_transferred () < result.bytes_to_read ())
  {
    // More to read...

    if (this->read (result.message_block(),
      result.bytes_to_read () - result.bytes_transferred ()) < 0)
    {
      result.message_block ().release ();

      this->cancel_and_close ();
    }
  }
  else
  {
    // Read it all

    this->completed_reads_++;

    result.message_block ().release ();

    // We now have sent and received data in the proactor thread.  Signal the
    // main thread to try sending data in the main thread.
    if (this->completed_reads_ == 1)
    {
      this->ready_for_external_write_.signal ();
    }
    else
    {
      // The main thread wrote data that was echoed back to us on our
      // second read.  If we get here, the test was successful in that
      // the main thread successfully sent data to the server.
      this->read_successful_ = 1;
      this->read_completed_.signal ();
    }

    // Next read
    if (this->read_data () < 0)
    {
      this->cancel_and_close ();
    }
  }
}

void
Client_Service_Handler::handle_write_stream (
  const ACE_Asynch_Write_Stream::Result &result)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->pending_writes_--;

  if (!result.success () || 0 == result.bytes_transferred ())
  {
    // Error

    result.message_block ().release ();

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
      "Client_Service_Handler::handle_write_stream: error: %d\n"),
      result.error ()));

    this->cancel_and_close ();
  }
  else if (result.bytes_transferred () < result.bytes_to_write ())
  {
    // More to write...

    if (this->write (result.message_block(),
      result.bytes_to_write () - result.bytes_transferred ()) < 0)
    {
      result.message_block ().release ();

      this->cancel_and_close ();
    }
  }
  else
  {
    // Wrote it all

    result.message_block ().release ();
  }
}

void
Client_Service_Handler::handle_wakeup (void)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->handle_wakeup_received_ = 1;
}

void
Client_Service_Handler::cancel_and_close (void)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->closing_ = 1;
  this->ssl_stream_.cancel ();
  this->handle_wakeup_expected_ = -1 == this->ssl_stream_.close ();
}

int
Client_Service_Handler::read_data (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_NORETURN(mb, ACE_Message_Block (DATA_SIZE));

  int ret = this->read (*mb, DATA_SIZE);
  if (ret < 0)
  {
    mb->release ();
    return -1;
  }
  else
  {
    return 0;
  }
}

int
Client_Service_Handler::write_data (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  ACE_Message_Block *mb = 0;
  ACE_NEW_NORETURN (mb, ACE_Message_Block (DATA_SIZE));
  ACE_OS::memcpy (mb->wr_ptr (), DATA, DATA_SIZE);
  mb->wr_ptr (DATA_SIZE);

  int ret = this->write (*mb, DATA_SIZE);
  if (ret < 0)
  {
    mb->release ();
    return -1;
  }
  else
  {
    return 0;
  }
}

int
Client_Service_Handler::read (ACE_Message_Block &mb, size_t bytes_to_read)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  int ret;
  if ((ret = this->ssl_stream_.read (mb, bytes_to_read)) < 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
        "Client_Service_Handler::read: read failed: %d\n"), (int)errno));
  }
  else
  {
    this->pending_reads_++;
  }
  return ret;
}

int
Client_Service_Handler::write (ACE_Message_Block &mb, size_t bytes_to_write)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  int ret;
  if ((ret = this->ssl_stream_.write (mb, bytes_to_write)) < 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
        "Client_Service_Handler::write: write failed: %d\n"), (int)errno));
  }
  else
  {
    this->pending_writes_++;
  }
  return ret;
}

int
Client_Service_Handler::safe_to_delete (void) const
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  return 0 == this->pending_writes_ &&
    0 == this->pending_reads_ &&
    (!this->handle_wakeup_expected_ || this->handle_wakeup_received_);
}

int
Client_Service_Handler::wait_for_external_write_queue (ACE_Time_Value *wait_time)
{
  return this->ready_for_external_write_.wait (wait_time, 0);
}

int
Client_Service_Handler::wait_for_read_completed (ACE_Time_Value *wait_time)
{
  return this->read_completed_.wait (wait_time, 0);
}

int
Client_Service_Handler::read_successful (void) const
{
  return this->read_successful_;
}

/**
 * Client's connector
 */
class Connector : public ACE_Asynch_Connector<Client_Service_Handler>
{
public:
  Connector (void);

  virtual ~Connector (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int connect (
    const ACE_INET_Addr &remote_sap,
    const ACE_INET_Addr &local_sap =
      (const ACE_INET_Addr &)ACE_Addr::sap_any,
    int reuse_addr = 1,
    const void *act = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int validate_connection (
    const ACE_Asynch_Connect::Result& result,
    const ACE_INET_Addr &remote,
    const ACE_INET_Addr& local);

  int safe_to_delete (void) const;

  void prepare_for_connection (Client_Service_Handler *service_handler);

protected:
  virtual void handle_connect (const ACE_Asynch_Connect::Result &result);

  virtual Client_Service_Handler* make_handler (void);

  mutable ACE_SYNCH_RECURSIVE_MUTEX  mtx_;
  Client_Service_Handler *service_handler_;
  int connecting_;
};

typedef ACE_Singleton<Connector, ACE_SYNCH_RECURSIVE_MUTEX> Connector_Singleton;
#define CONNECTOR Connector_Singleton::instance ()

Connector::Connector (void) :
  service_handler_ (0)
{
}

Connector::~Connector (void)
{
}

int
Connector::connect (const ACE_INET_Addr &remote_sap,
  const ACE_INET_Addr &local_sap, int reuse_addr, const void *act)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  this->connecting_ = 1;

  return this->ACE_Asynch_Connector<Client_Service_Handler>::connect (
    remote_sap, local_sap, reuse_addr, act);
}

int
Connector::validate_connection (const ACE_Asynch_Connect::Result& result,
  const ACE_INET_Addr &remote, const ACE_INET_Addr& local)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  ACE_UNUSED_ARG (result);
  ACE_UNUSED_ARG (remote);
  ACE_UNUSED_ARG (local);

  if (!result.success ())
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
      "Connector::validate_connection failed: %d\n"), result.error ()));
    return -1;
  }
  else
  {
    return 0;
  }
}

void
Connector::handle_connect (const ACE_Asynch_Connect::Result &result)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);

  this->ACE_Asynch_Connector<Client_Service_Handler>::handle_connect (result);

  this->connecting_ = 0;
}

Client_Service_Handler*
Connector::make_handler (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, 0);

  ACE_ASSERT(0 != this->service_handler_);
  Client_Service_Handler *service_handler = this->service_handler_;
  this->service_handler_ = 0;
  return service_handler;
}

int
Connector::safe_to_delete (void) const
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_, -1);

  return 0 == this->connecting_;
}

void
Connector::prepare_for_connection (Client_Service_Handler *service_handler)
{
  ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard, this->mtx_);
  this->service_handler_ = service_handler;
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2912_Regression_Test"));

  // SSL_CTX_set_cipher_list, etc.
  init_ssl ();

  // Keep RT signals on POSIX from killing us.
  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);

  int ret = 0;
  Client_Service_Handler client_handler;
  Server_Service_Handler server_handler;
  ACE_Time_Value wait_time (10, 0);

  // Client and Server will utilize different proactors since this test
  // depends on SSL thread error state behavior.

  CLIENT_PROACTOR_TASK->activate ();
  SERVER_PROACTOR_TASK->activate ();

  // Open server acceptor and client connector

  if (0 == ret)
  {
    ret = ACCEPTOR->open (
      ACE_INET_Addr (ACE_DEFAULT_SERVER_PORT),
      0,
      0,
      ACE_DEFAULT_ASYNCH_BACKLOG,
      1,
      SERVER_PROACTOR,
      1);

      if (-1 == ret)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
          "ACE_Asynch_Acceptor::open failed, %d\n"), (int)errno));
      }
  }

  if (0 == ret)
  {
    ret = CONNECTOR->open (0, CLIENT_PROACTOR, 1);

    if (-1 == ret)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
        "ACE_Asynch_Connector::open failed, %d\n"), (int)errno));
    }
  }

  // Supply server_handler and client_handler to acceptor and connector and
  // connect client to the server.

  if (0 == ret)
  {
    ACCEPTOR->prepare_for_connection (&server_handler);
    CONNECTOR->prepare_for_connection (&client_handler);

    ret = CONNECTOR->connect (
      ACE_INET_Addr (ACE_DEFAULT_SERVER_PORT, ACE_LOCALHOST));

    if (-1 == ret)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
        "ACE_Asynch_Connector::connect failed, %d\n"), (int)errno));
    }
  }

  if (0 == ret)
  {
    ret = client_handler.wait_for_external_write_queue (&wait_time);
    if (-1 == ret)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
        "Timed out waiting for client's write readiness\n")));
    }
  }

  // Client sends data to server

  if (0 == ret)
  {
    ret = client_handler.write_data ();
  }

  // Client waits for echo reply from server

  if (0 == ret)
  {
    ret = client_handler.wait_for_read_completed (&wait_time);

    if (-1 == ret)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT (
        "Timed out waiting for client's read to complete\n")));
    }
  }

  if (0 == ret)
  {
    if (client_handler.read_successful () == 1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Success\n")));
      ret = 0;
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Client's read failed\n")));
      ret = -1;
    }
  }

  // Cleanup and shutdown

  ACCEPTOR->cancel ();
  while (!ACCEPTOR->safe_to_delete ())
    ACE_OS::sleep (ACE_Time_Value (0, 500000));

  CONNECTOR->cancel ();
  while (!CONNECTOR->safe_to_delete ())
    ACE_OS::sleep (ACE_Time_Value (0, 500000));

  client_handler.cancel_and_close ();
  while (!client_handler.safe_to_delete ())
    ACE_OS::sleep (ACE_Time_Value (0, 500000));

  server_handler.cancel_and_close ();
  while (!server_handler.safe_to_delete ())
    ACE_OS::sleep (ACE_Time_Value (0, 500000));

  CLIENT_PROACTOR->proactor_end_event_loop ();
  CLIENT_PROACTOR_TASK->wait ();

  SERVER_PROACTOR->proactor_end_event_loop ();
  SERVER_PROACTOR_TASK->wait ();

  ACE_END_TEST;

  return 0;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2912_Regression_Test"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("threads or proactor not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS && (ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS) && !linux */
