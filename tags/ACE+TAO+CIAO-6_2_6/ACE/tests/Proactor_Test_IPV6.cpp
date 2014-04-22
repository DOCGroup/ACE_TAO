// $Id$
// ============================================================================
/**
 *  @file Proactor_Test_IPV6.cpp
 *
 *  $Id$
 *
 *  This program illustrates how the ACE_Proactor can be used to
 *  implement an application that does various asynchronous
 *  operations.
 *
 *  @author Alexander Libman <alibman@baltimore.com>
 *  @author Brian Buesker <bbuesker@qualcomm.com> - modified for IPv6 operation
 */
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS))
  // This only works on Win32 platforms and on Unix platforms
  // supporting POSIX aio calls.

#include "ace/Signal.h"

#include "ace/Service_Config.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Object_Manager.h"
#include "ace/Get_Opt.h"

#include "ace/Proactor.h"
#include "ace/Asynch_Acceptor.h"
#include "ace/Asynch_Connector.h"
#include "ace/Task.h"
#include "ace/Thread_Semaphore.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/os_include/netinet/os_tcp.h"

#include "ace/Atomic_Op.h"
#include "ace/Synch_Traits.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO)

#  include "ace/WIN32_Proactor.h"

#elif defined (ACE_HAS_AIO_CALLS)

#  include "ace/POSIX_Proactor.h"
#  include "ace/POSIX_CB_Proactor.h"
#  include "ace/SUN_Proactor.h"

#endif /* defined (ACE_HAS_WIN32_OVERLAPPED_IO) */

#include "Proactor_Test.h"


// Proactor Type (UNIX only, Win32 ignored)
typedef enum { DEFAULT = 0, AIOCB, SIG, SUN, CB } ProactorType;
static ProactorType proactor_type = DEFAULT;

// POSIX : > 0 max number aio operations  proactor,
static size_t max_aio_operations = 0;

// both: 0 run client or server / depends on host
//     != 0 run client and server
static int both = 0;

// Host that we're connecting to.
static const ACE_TCHAR *host = 0;

// number of Client instances
static int clients = 1;
const  int MAX_CLIENTS = 1000;
const  int MAX_SERVERS = 1000;

// duplex mode: == 0 half-duplex
//              != 0 full duplex
static int duplex = 0;

// number threads in the Proactor thread pool
static int threads = 1;

// Port that we're receiving connections on.
static u_short port = ACE_DEFAULT_SERVER_PORT;

// Log options
static int loglevel;       // 0 full , 1 only errors

static size_t xfer_limit;  // Number of bytes for Client to send.

static char complete_message[] =
  "GET / HTTP/1.1\r\n"
  "Accept: */*\r\n"
  "Accept-Language: C++\r\n"
  "Accept-Encoding: gzip, deflate\r\n"
  "User-Agent: Proactor_Test_IPv6/1.0 (non-compatible)\r\n"
  "Connection: Keep-Alive\r\n"
  "\r\n";

class LogLocker
{
public:

  LogLocker () { ACE_LOG_MSG->acquire (); }
  virtual ~LogLocker () { ACE_LOG_MSG->release (); }
};



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


// *************************************************************
//  MyTask is ACE_Task resposible for :
//  1. creation and deletion of
//     Proactor and Proactor thread pool
//  2. running Proactor event loop
// *************************************************************

/**
 * @class MyTask
 *
 * MyTask plays role for Proactor threads pool
 *
 * MyTask is ACE_Task resposible for:
 * 1. Creation and deletion of Proactor and Proactor thread pool
 * 2. Running Proactor event loop
 */
class MyTask : public ACE_Task<ACE_MT_SYNCH>
{
public:
  MyTask (void):
    lock_ (),
    sem_ ((unsigned int) 0),
    proactor_(0) {}

  virtual ~MyTask()
    {
      (void) this->stop ();
      (void) this->delete_proactor();
    }

  virtual int svc (void);

  int start (int num_threads,
             ProactorType type_proactor,
             size_t max_op );
  int stop  (void);

private:
  int  create_proactor (ProactorType type_proactor,
                        size_t max_op);
  int  delete_proactor (void);

  ACE_SYNCH_RECURSIVE_MUTEX lock_;
  ACE_Thread_Semaphore sem_;
  ACE_Proactor * proactor_;

};

int
MyTask::create_proactor (ProactorType type_proactor, size_t max_op)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_TEST_ASSERT (this->proactor_ == 0);

#if defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  ACE_UNUSED_ARG (type_proactor);
  ACE_UNUSED_ARG (max_op);

  ACE_WIN32_Proactor *proactor_impl = 0;

  ACE_NEW_RETURN (proactor_impl,
                  ACE_WIN32_Proactor,
                  -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("(%t) Create Proactor Type = WIN32\n")));

#elif defined (ACE_HAS_AIO_CALLS)

  ACE_POSIX_Proactor * proactor_impl = 0;

  switch (type_proactor)
    {
    case AIOCB:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_POSIX_AIOCB_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = AIOCB\n")));
      break;

#if defined(ACE_HAS_POSIX_REALTIME_SIGNALS)
    case SIG:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_POSIX_SIG_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = SIG\n")));
      break;
#endif /* ACE_HAS_POSIX_REALTIME_SIGNALS */

#  if defined (sun)
    case SUN:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_SUN_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT("(%t) Create Proactor Type = SUN\n")));
      break;
#  endif /* sun */

#  if !defined(ACE_HAS_BROKEN_SIGEVENT_STRUCT)
    case CB:
      ACE_NEW_RETURN (proactor_impl,
                      ACE_POSIX_CB_Proactor (max_op),
                      -1);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = CB\n")));
      break;
#  endif /* !ACE_HAS_BROKEN_SIGEVENT_STRUCT */

    default:
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Create Proactor Type = DEFAULT\n")));
      break;
  }

#endif // (ACE_WIN32) && !defined (ACE_HAS_WINCE)

  // always delete implementation  1 , not  !(proactor_impl == 0)
  ACE_NEW_RETURN (this->proactor_,
                  ACE_Proactor (proactor_impl, 1 ),
                  -1);
  // Set new singleton and delete it in close_singleton()
  ACE_Proactor::instance (this->proactor_, 1);
  return 0;
}

int
MyTask::delete_proactor (void)
{
  ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Delete Proactor\n")));

  ACE_Proactor::close_singleton ();
  this->proactor_ = 0;

  return 0;
}

int
MyTask::start (int num_threads,
               ProactorType type_proactor,
               size_t max_op)
{
  if (this->create_proactor (type_proactor, max_op) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to create proactor")),
                      -1);

  if (this->activate (THR_NEW_LWP, num_threads) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p.\n"),
                       ACE_TEXT ("unable to activate thread pool")),
                      -1);

  for (; num_threads > 0; num_threads--)
    {
      sem_.acquire ();
    }

  return 0;
}


int
MyTask::stop ()
{
  if (this->proactor_ != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Calling End Proactor event loop\n")));

      ACE_Proactor::end_event_loop ();
    }

  if (this->wait () == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p.\n"),
                ACE_TEXT ("unable to stop thread pool")));

  return 0;
}

int
MyTask::svc (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) MyTask started\n")));

  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);
  disable_signal (SIGPIPE, SIGPIPE);

  // signal that we are ready
  sem_.release (1);

  ACE_Proactor::run_event_loop ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) MyTask finished\n")));
  return 0;
}


// TestData collects and reports on test-related transfer and connection
// statistics.
class TestData
{
public:
  TestData ();
  bool testing_done (void);
  Server *server_up (void);
  Client *client_up (void);
  void server_done (Server *s);
  void client_done (Client *c);
  void stop_all (void);
  void report (void);

private:
  struct Local_Stats
  {
    // Track number of sessions that report start, and those that report
    // their end (and stats).
    ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> sessions_up_;
    ACE_Atomic_Op<ACE_SYNCH_MUTEX, int> sessions_down_;

    // Total read and write bytes for all sessions.
    ACE_Atomic_Op<ACE_SYNCH_MUTEX, size_t> w_cnt_;
    ACE_Atomic_Op<ACE_SYNCH_MUTEX, size_t> r_cnt_;
    // Total read and write operations issues for all sessions.
    ACE_Atomic_Op<ACE_SYNCH_MUTEX, size_t> w_ops_;
    ACE_Atomic_Op<ACE_SYNCH_MUTEX, size_t> r_ops_;
  } servers_, clients_;

  ACE_SYNCH_MUTEX list_lock_;
  Server *server_list_[MAX_SERVERS];
  Client *client_list_[MAX_CLIENTS];
};

TestData::TestData ()
{
  int i;
  for (i = 0; i < MAX_SERVERS; ++i)
    this->server_list_[i] = 0;
  for (i = 0; i < MAX_CLIENTS; ++i)
    this->client_list_[i] = 0;
}

bool
TestData::testing_done (void)
{
  int svr_up = this->servers_.sessions_up_.value ();
  int svr_dn = this->servers_.sessions_down_.value ();
  int clt_up = this->clients_.sessions_up_.value ();
  int clt_dn = this->clients_.sessions_down_.value ();

  if (svr_up == 0 && clt_up == 0)      // No connections up yet
    return false;

  return (svr_dn >= svr_up && clt_dn >= clt_up);
}

Server *
TestData::server_up (void)
{
  ++this->servers_.sessions_up_;
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, monitor, this->list_lock_, 0);

  for (int i = 0; i < MAX_SERVERS; ++i)
    {
      if (this->server_list_[i] == 0)
        {
          ACE_NEW_RETURN (this->server_list_[i], Server (this, i), 0);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Server %d up; now %d up, %d down.\n"),
                      i,
                      this->servers_.sessions_up_.value (),
                      this->servers_.sessions_down_.value ()));
          return this->server_list_[i];
        }
    }
  return 0;
}

Client *
TestData::client_up (void)
{
  ++this->clients_.sessions_up_;
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, monitor, this->list_lock_, 0);

  for (int i = 0; i < MAX_CLIENTS; ++i)
    {
      if (this->client_list_[i] == 0)
        {
          ACE_NEW_RETURN (this->client_list_[i], Client (this, i), 0);
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Client %d up; now %d up, %d down.\n"),
                      i,
                      this->clients_.sessions_up_.value (),
                      this->clients_.sessions_down_.value ()));
          return this->client_list_[i];
        }
    }
  return 0;
}

void
TestData::server_done (Server *s)
{
  this->servers_.w_cnt_ += s->get_total_snd ();
  this->servers_.r_cnt_ += s->get_total_rcv ();
  this->servers_.w_ops_ += s->get_total_w ();
  this->servers_.r_ops_ += s->get_total_r ();
  ++this->servers_.sessions_down_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Server %d gone; now %d up, %d down\n"),
              s->id (),
              this->servers_.sessions_up_.value (),
              this->servers_.sessions_down_.value ()));

  ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->list_lock_);
  int i;
  for (i = 0; i < MAX_SERVERS; ++i)
    {
      if (this->server_list_[i] == s)
        {
          if (s->id () != i)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Server %d is pos %d in list\n"),
                        s->id (),
                        i));
          this->server_list_[i] = 0;
          break;
        }
    }
  if (i >= MAX_SERVERS)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Server %@ done but not listed\n"), s));

  return;
}

void
TestData::client_done (Client *c)
{
  this->clients_.w_cnt_ += c->get_total_snd ();
  this->clients_.r_cnt_ += c->get_total_rcv ();
  this->clients_.w_ops_ += c->get_total_w ();
  this->clients_.r_ops_ += c->get_total_r ();
  ++this->clients_.sessions_down_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Client %d gone; now %d up, %d down\n"),
              c->id (),
              this->clients_.sessions_up_.value (),
              this->clients_.sessions_down_.value ()));

  ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->list_lock_);
  int i;
  for (i = 0; i < MAX_CLIENTS; ++i)
    {
      if (this->client_list_[i] == c)
        {
          if (c->id () != i)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Client %d is pos %d in list\n"),
                        c->id (),
                        i));
          this->client_list_[i] = 0;
          break;
        }
    }
  if (i >= MAX_CLIENTS)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("Client %@ done but not listed\n"), c));

  return;
}

void
TestData::stop_all (void)
{
  int i;

  // Lock and cancel everything. Then release the lock, possibly allowing
  // cleanups, then grab it again and delete all Servers and Clients.
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->list_lock_);
    for (i = 0; i < MAX_CLIENTS; ++i)
      {
        if (this->client_list_[i] != 0)
          this->client_list_[i]->cancel ();
      }

    for (i = 0; i < MAX_SERVERS; ++i)
      {
        if (this->server_list_[i] != 0)
          this->server_list_[i]->cancel ();
      }
  }
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->list_lock_);
    for (i = 0; i < MAX_CLIENTS; ++i)
      {
        if (this->client_list_[i] != 0)
          delete this->client_list_[i];
      }

    for (i = 0; i < MAX_SERVERS; ++i)
      {
        if (this->server_list_[i] != 0)
          delete this->server_list_[i];
      }
  }
}

void
TestData::report (void)
{
  // Print statistics
  ACE_TCHAR bufs [256];
  ACE_TCHAR bufr [256];

  ACE_OS::sprintf (bufs,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(") ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT (")"),
                   this->clients_.w_cnt_.value (),
                   this->clients_.w_ops_.value ());

  ACE_OS::sprintf (bufr,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(") ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT (")"),
                   this->clients_.r_cnt_.value (),
                   this->clients_.r_ops_.value ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Clients total bytes (ops): snd=%s rcv=%s\n"),
              bufs,
              bufr));

  ACE_OS::sprintf (bufs,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(") ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT (")"),
                   this->servers_.w_cnt_.value (),
                   this->servers_.w_ops_.value ());

  ACE_OS::sprintf (bufr,
                   ACE_SIZE_T_FORMAT_SPECIFIER
                   ACE_TEXT ("(") ACE_SIZE_T_FORMAT_SPECIFIER ACE_TEXT (")"),
                   this->servers_.r_cnt_.value (),
                   this->servers_.r_ops_.value ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Servers total bytes (ops): snd=%s rcv=%s\n"),
              bufs,
              bufr));

  if (this->clients_.w_cnt_.value () == 0 ||
      this->clients_.r_cnt_.value () == 0 ||
      this->servers_.w_cnt_.value () == 0 ||
      this->servers_.r_cnt_.value () == 0   )
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("It appears that this test didn't ")
                ACE_TEXT ("really do anything. Something is very wrong.\n")));
}


class Acceptor : public ACE_Asynch_Acceptor<Server>
{
public:
  Acceptor (TestData *tester);
  virtual ~Acceptor (void);

  // Virtual from ACE_Asynch_Acceptor
  Server *make_handler (void);

private:
  TestData *tester_;
};

// *************************************************************
Acceptor::Acceptor (TestData *tester)
  : tester_ (tester)
{
}

Acceptor::~Acceptor (void)
{
  this->cancel ();
}

Server *
Acceptor::make_handler (void)
{
  return this->tester_->server_up ();
}

// ***************************************************
Server::Server ()
{
  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shouldn't use this constructor!\n")));
}

Server::Server (TestData *tester, int id)
  : tester_   (tester),
    id_       (id),
    handle_   (ACE_INVALID_HANDLE),
    io_count_ (0),
    flg_cancel_(0),
    total_snd_(0),
    total_rcv_(0),
    total_w_  (0),
    total_r_  (0)
{
}

Server::~Server (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Server %d dtor; %d sends (%d bytes); ")
              ACE_TEXT ("%d recvs (%d bytes)\n"),
              this->id_,
              this->total_w_, this->total_snd_,
              this->total_r_, this->total_rcv_));
  if (this->io_count_ != 0)
    ACE_ERROR ((LM_WARNING,
                ACE_TEXT ("(%t) Server %d deleted with ")
                ACE_TEXT ("%d I/O outstanding\n"),
                this->id_,
                this->io_count_));

  // This test bounces data back and forth between Clients and Servers.
  // Therefore, if there was significantly more data in one direction, that's
  // a problem. Remember, the byte counts are unsigned values.
  int issue_data_warning = 0;
  if (this->total_snd_ > this->total_rcv_)
    {
      if (this->total_rcv_ == 0)
        issue_data_warning = 1;
      else if (this->total_snd_ / this->total_rcv_ > 2)
        issue_data_warning = 1;
    }
  else
    {
      if (this->total_snd_ == 0)
        issue_data_warning = 1;
      else if (this->total_rcv_ / this->total_snd_ > 2)
        issue_data_warning = 1;
    }
  if (issue_data_warning)
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("(%t) Above byte counts look odd; need review\n")));

  if (this->tester_ != 0)
    this->tester_->server_done (this);

  if (this->handle_ != ACE_INVALID_HANDLE)
    ACE_OS::closesocket (this->handle_);

  this->id_ = -1;
  this->handle_= ACE_INVALID_HANDLE;
}

void
Server::cancel ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

  this->flg_cancel_ = 1;
  this->ws_.cancel ();
  this->rs_.cancel ();
  return;
}


void
Server::addresses (const ACE_INET_Addr& peer, const ACE_INET_Addr&)
{
  ACE_TCHAR str[256];
  if (0 == peer.addr_to_string (str, sizeof (str) / sizeof (ACE_TCHAR)))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Server %d connection from %s\n"),
                this->id_,
                str));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) Server %d %p\n"),
                this->id_,
                ACE_TEXT ("addr_to_string")));
  return;
}


void
Server::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    // Don't buffer serial sends.
    this->handle_ = handle;
    int nodelay = 1;
    ACE_SOCK_Stream option_setter (handle);
    if (-1 == option_setter.set_option (ACE_IPPROTO_TCP,
                                        TCP_NODELAY,
                                        &nodelay,
                                        sizeof (nodelay)))
      ACE_ERROR ((LM_ERROR, "%p\n", "set_option"));

    if (this->ws_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Server::ACE_Asynch_Write_Stream::open")));
    else if (this->rs_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Server::ACE_Asynch_Read_Stream::open")));
    else
      this->initiate_read_stream ();

    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

int
Server::initiate_read_stream (void)
{
  if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  ACE_Message_Block *mb = 0;
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (1024), //BUFSIZ + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size () - 1) == -1)
    {
      mb->release ();
#if defined (ACE_WIN32)
      // On peer close, ReadFile will yield ERROR_NETNAME_DELETED; won't get
      // a 0-byte read as we would if underlying calls used WSARecv.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Server %d, peer closed\n"),
                           this->id_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) Server %d, %p\n"),
                         this->id_,
                         ACE_TEXT ("read")),
                        -1);
    }

  this->io_count_++;
  this->total_r_++;
  return 0;
}

int
Server::initiate_write_stream (ACE_Message_Block &mb, size_t nbytes)
{
  if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    {
      mb.release ();
      return -1;
    }

  if (nbytes == 0)
    {
      mb.release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) Server::ACE_Asynch_Write_Stream::write nbytes <0 ")),
                       -1);
    }

  if (this->ws_.write (mb, nbytes) == -1)
    {
      mb.release ();
#if defined (ACE_WIN32)
      // On peer close, WriteFile will yield ERROR_NETNAME_DELETED.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Server %d, peer gone\n"),
                           this->id_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) Server %d, %p\n"),
                        this->id_,
                        ACE_TEXT ("write")),
                       -1);
    }

  this->io_count_++;
  this->total_w_++;
  return 0;
}

void
Server::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_ );

    ACE_Message_Block & mb = result.message_block ();

    // Reset pointers.
    mb.rd_ptr ()[result.bytes_transferred ()] = '\0';

    if (loglevel > 1)
      {
        LogLocker log_lock;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Server %d: handle_read_stream() ****\n"),
                    this->id_));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_to_read"),
                    result.bytes_to_read ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("handle"),
                    result.handle ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_transfered"),
                    result.bytes_transferred ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("error"),
                    result.error ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),
                    mb.rd_ptr ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("**** end of message ****************\n")));
      }
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Server %d; %p\n"),
                                       this->id_,
                                       ACE_TEXT ("read"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Server %d: read %d bytes\n"),
                    this->id_,
                    result.bytes_transferred ()));
      }

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_rcv_ += result.bytes_transferred ();

        if (this->initiate_write_stream (mb,
                                         result.bytes_transferred ()) == 0)
          {
            if (duplex != 0)  // Initiate new read from the stream.
              this->initiate_read_stream ();
          }
      }
    else
      mb.release ();

    --this->io_count_;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

void
Server::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    ACE_Message_Block & mb = result.message_block ();

    if (loglevel > 1)
      {
        LogLocker log_lock;

        //mb.rd_ptr () [0] = '\0';
        mb.rd_ptr (mb.rd_ptr () - result.bytes_transferred ());

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Server %d: handle_write_stream() ****\n"),
                    this->id_));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_to_write"),
                    result.bytes_to_write ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("handle"),
                    result.handle ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_transfered"),
                    result.bytes_transferred ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("error"),
                    result.error ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),
                    mb.rd_ptr ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("**** end of message ****************\n")));
      }
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Server %d; %p\n"),
                                       this->id_,
                                       ACE_TEXT ("write"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Server %d: wrote %d bytes ok\n"),
                    this->id_,
                    result.bytes_transferred ()));
      }

    mb.release ();

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_snd_ += result.bytes_transferred ();

        if (duplex == 0)
          this->initiate_read_stream ();
      }

    --this->io_count_;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

// *******************************************
//  Connector
// *******************************************

class Connector : public ACE_Asynch_Connector<Client>
{
public:
  Connector (TestData *tester);
  virtual ~Connector (void);

  int  start (const ACE_INET_Addr &addr, int num);

  // Virtual from ACE_Asynch_Connector
  Client *make_handler (void);

private:
  TestData *tester_;
};

// *************************************************************

Connector::Connector (TestData *tester)
  : tester_ (tester)
{
}

Connector::~Connector (void)
{
  this->cancel ();
}

Client *
Connector::make_handler (void)
{
  return this->tester_->client_up ();
}


int
Connector::start (const ACE_INET_Addr& addr, int num)
{
  if (num > MAX_CLIENTS)
    num = MAX_CLIENTS;

  if (num < 0)
    num = 1;

  int rc = 0;

  // int open  ( int pass_addresses = 0,
  //             ACE_Proactor *proactor = 0,
  //             int validate_new_connection = 0 );

  if (this->open (1, 0, 1) != 0)
  {
     ACE_ERROR ((LM_ERROR,
                 ACE_TEXT ("(%t) %p\n"),
                 ACE_TEXT ("Connector::open failed")));
     return rc;
  }

  for (; rc < num;  rc++)
    {
      ACE_INET_Addr localAddr;
      if (this->connect (addr, localAddr) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p\n"),
                      ACE_TEXT ("Connector::connect failed for IPv6")));
          break;
        }
    }
  return rc;
}


Client::Client ()
{
  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Shouldn't use this constructor!\n")));
}

Client::Client (TestData *tester, int id)
  : tester_       (tester),
    id_           (id),
    handle_       (ACE_INVALID_HANDLE),
    io_count_     (0),
    stop_writing_ (0),
    flg_cancel_   (0),
    total_snd_    (0),
    total_rcv_    (0),
    total_w_      (0),
    total_r_      (0)
{
}

Client::~Client (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Client %d dtor; %d sends (%d bytes); ")
              ACE_TEXT ("%d recvs (%d bytes)\n"),
              this->id_,
              this->total_w_, this->total_snd_,
              this->total_r_, this->total_rcv_));
  if (this->io_count_ != 0)
    ACE_ERROR ((LM_WARNING,
                ACE_TEXT ("(%t) Client %d deleted with %d I/O outstanding\n"),
                this->id_,
                this->io_count_));

  // This test bounces data back and forth between Clients and Servers.
  // Therefore, if there was significantly more data in one direction, that's
  // a problem. Remember, the byte counts are unsigned values.
  int issue_data_warning = 0;
  if (this->total_snd_ > this->total_rcv_)
    {
      if (this->total_rcv_ == 0)
        issue_data_warning = 1;
      else if (this->total_snd_ / this->total_rcv_ > 2)
        issue_data_warning = 1;
    }
  else
    {
      if (this->total_snd_ == 0)
        issue_data_warning = 1;
      else if (this->total_rcv_ / this->total_snd_ > 2)
        issue_data_warning = 1;
    }
  if (issue_data_warning)
    ACE_DEBUG ((LM_WARNING,
                ACE_TEXT ("(%t) Above byte counts look odd; need review\n")));

  if (this->tester_ != 0)
    this->tester_->client_done (this);

  this->id_ = -1;
  this->handle_= ACE_INVALID_HANDLE;
  if (this->handle_ != ACE_INVALID_HANDLE)
    {
      ACE_OS::closesocket (this->handle_);
    }
  this->handle_= ACE_INVALID_HANDLE;
}

void
Client::cancel ()
{
  ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

  this->flg_cancel_ = 1;
  this->ws_.cancel ();
  this->rs_.cancel ();
  return;
}

void
Client::close ()
{
  // This must be called with the lock_ held.
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Closing Client %d writes; %d I/O outstanding\n"),
              this->id_, this->io_count_));
  ACE_OS::shutdown (this->handle_, ACE_SHUTDOWN_WRITE);
  this->stop_writing_ = 1;
  return;
}


void
Client::addresses (const ACE_INET_Addr& peer, const ACE_INET_Addr& local)
{
  char my_name[256];
  char peer_name[256];
  ACE_TCHAR local_str[256];
  ACE_INET_Addr addr ((u_short) 0, host);

  // This checks to make sure the peer address given to us matches what
  // we expect it to be.
  if (0 != peer.get_host_addr (peer_name, sizeof (peer_name)))
    {
      if (0 != addr.get_host_addr (my_name, sizeof (my_name)))
        {
          if (0 != ACE_OS::strncmp (peer_name, my_name, sizeof (my_name)))
            {
              ACE_ERROR
                ((LM_ERROR,
                  ACE_TEXT ("(%t) Sender %d peer address (%C) does not ")
                  ACE_TEXT ("match host address (%C)\n"),
                  this->id_,
                  peer_name, my_name));
              return;
            }
         }
       else
         {
           ACE_ERROR
             ((LM_ERROR,
               ACE_TEXT ("(%t) Sender %d unable to convert host addr\n"),
               this->id_));
           return;
        }
     }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) Sender %d unable to convert peer addr\n"),
                  this->id_));
      return;
    }

  if (0 == local.addr_to_string (local_str,
                                 sizeof (local_str) / sizeof (ACE_TCHAR)))
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Client %d connected on %s\n"),
                this->id_,
                local_str));
  else
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) Client %d %p\n"),
                this->id_,
                ACE_TEXT ("addr_to_string")));
  return;
}


void
Client::open (ACE_HANDLE handle, ACE_Message_Block &)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    // Don't buffer serial sends.
    this->handle_ = handle;
    int nodelay = 1;
    ACE_SOCK_Stream option_setter (handle);
    if (option_setter.set_option (ACE_IPPROTO_TCP,
                                  TCP_NODELAY,
                                  &nodelay,
                                  sizeof (nodelay)))
      ACE_ERROR ((LM_ERROR, "%p\n", "set_option"));

    // Open ACE_Asynch_Write_Stream
    if (this->ws_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Client::ACE_Asynch_Write_Stream::open")));

    // Open ACE_Asynch_Read_Stream
    else if (this->rs_.open (*this, this->handle_) == -1)
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("Client::ACE_Asynch_Read_Stream::open")));

    else if (this->initiate_write_stream () == 0)
      {
        if (duplex != 0)    // Start an asynchronous read
          this->initiate_read_stream ();
      }

    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

int
Client::initiate_write_stream (void)
{
  if (this->flg_cancel_ != 0 ||
      this->stop_writing_ ||
      this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  static const size_t complete_message_length = ACE_OS::strlen (complete_message);

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))

  ACE_Message_Block *mb1 = 0,
                    *mb2 = 0,
                    *mb3 = 0;

  // No need to allocate +1 for proper printing - the memory includes it already
  ACE_NEW_RETURN (mb1,
                  ACE_Message_Block ((char *)complete_message,
                                     complete_message_length),
                  -1);

  ACE_NEW_RETURN (mb2,
                  ACE_Message_Block ((char *)complete_message,
                                     complete_message_length),
                  -1);

  ACE_NEW_RETURN (mb3,
                  ACE_Message_Block ((char *)complete_message,
                                     complete_message_length),
                  -1);

  mb1->wr_ptr (complete_message_length);
  mb2->wr_ptr (complete_message_length);
  mb3->wr_ptr (complete_message_length);

  // chain them together
  mb1->cont (mb2);
  mb2->cont (mb3);

  if (this->ws_.writev (*mb1, mb1->total_length ()) == -1)
    {
      mb1->release ();
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) %p\n"),
                        ACE_TEXT ("Client::ACE_Asynch_Stream::writev")),
                       -1);
    }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  ACE_Message_Block *mb = 0;

  // No need to allocate +1 for proper printing - the memory includes it already
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (complete_message, complete_message_length),
                  -1);
  mb->wr_ptr (complete_message_length);

  if (this->ws_.write (*mb, mb->length ()) == -1)
    {
      mb->release ();
#if defined (ACE_WIN32)
      // On peer close, WriteFile will yield ERROR_NETNAME_DELETED.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Client %d, peer gone\n"),
                           this->id_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN((LM_ERROR,
                        ACE_TEXT ("(%t) Client %d, %p\n"),
                        this->id_,
                        ACE_TEXT ("write")),
                       -1);
    }
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  this->io_count_++;
  this->total_w_++;
  return 0;
}

int
Client::initiate_read_stream (void)
{
  if (this->flg_cancel_ != 0 || this->handle_ == ACE_INVALID_HANDLE)
    return -1;

  static const size_t complete_message_length =
    ACE_OS::strlen (complete_message);

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
  ACE_Message_Block *mb1 = 0,
                    *mb2 = 0,
                    *mb3 = 0,
                    *mb4 = 0,
                    *mb5 = 0,
                    *mb6 = 0;

  // We allocate +1 only for proper printing - we can just set the last byte
  // to '\0' before printing out
  ACE_NEW_RETURN (mb1, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb2, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb3, ACE_Message_Block (complete_message_length + 1), -1);

  // Let allocate memory for one more triplet,
  // This improves performance
  // as we can receive more the than one block at once
  // Generally, we can receive more triplets ....
  ACE_NEW_RETURN (mb4, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb5, ACE_Message_Block (complete_message_length + 1), -1);
  ACE_NEW_RETURN (mb6, ACE_Message_Block (complete_message_length + 1), -1);

  mb1->cont (mb2);
  mb2->cont (mb3);

  mb3->cont (mb4);
  mb4->cont (mb5);
  mb5->cont (mb6);


  // hide last byte in each message block, reserving it for later to set '\0'
  // for proper printouts
  mb1->size (mb1->size () - 1);
  mb2->size (mb2->size () - 1);
  mb3->size (mb3->size () - 1);

  mb4->size (mb4->size () - 1);
  mb5->size (mb5->size () - 1);
  mb6->size (mb6->size () - 1);

  // Inititiate read
  if (this->rs_.readv (*mb1, mb1->total_size () - 1) == -1)
    {
      mb1->release ();
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Client::ACE_Asynch_Read_Stream::readv")),
                        -1);
    }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  // Try to read more chunks
  size_t blksize = ( complete_message_length > BUFSIZ ) ?
                     complete_message_length : BUFSIZ;

  ACE_Message_Block *mb = 0;

  // We allocate +1 only for proper printing - we can just set the last byte
  // to '\0' before printing out
  ACE_NEW_RETURN (mb,
                  ACE_Message_Block (blksize + 1),
                  -1);

  // Inititiate read
  if (this->rs_.read (*mb, mb->size () - 1) == -1)
    {
      mb->release ();
#if defined (ACE_WIN32)
      // On peer close, ReadFile will yield ERROR_NETNAME_DELETED; won't get
      // a 0-byte read as we would if underlying calls used WSARecv.
      if (ACE_OS::last_error () == ERROR_NETNAME_DELETED)
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("(%t) Client %d, peer closed\n"),
                           this->id_),
                          -1);
#endif /* ACE_WIN32 */
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) Client %d, %p\n"),
                         this->id_,
                         ACE_TEXT ("read")),
                        -1);
    }
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

  this->io_count_++;
  this->total_r_++;
  return 0;
}

void
Client::handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    ACE_Message_Block & mb = result.message_block ();

    if (loglevel > 1)
      {
        LogLocker log_lock;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Client %d: handle_write_stream() ****\n"),
                    this->id_));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_to_write"),
                    result.bytes_to_write ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("handle"),
                    result.handle ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_transfered"),
                    result.bytes_transferred ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("error"),
                    result.error ()));

#if (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE))
        size_t bytes_transferred = result.bytes_transferred ();
        char index = 0;
        for (ACE_Message_Block* mb_i = &mb;
             (mb_i != 0) && (bytes_transferred > 0);
             mb_i = mb_i->cont ())
          {
            // write 0 at string end for proper printout (if end of mb, it's 0 already)
            mb_i->rd_ptr()[0]  = '\0';

            size_t len = mb_i->rd_ptr () - mb_i->base ();

            // move rd_ptr backwards as required for printout
            if (len >= bytes_transferred)
              {
                mb_i->rd_ptr (0 - bytes_transferred);
                bytes_transferred = 0;
              }
            else
              {
                mb_i->rd_ptr (0 - len);
                bytes_transferred -= len;
              }

            ++index;
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%s%d = %s\n"),
                        ACE_TEXT ("message_block, part "),
                        index,
                        mb_i->rd_ptr ()));
          }
#else /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */
        // write 0 at string end for proper printout (if end of mb, it's 0 already)
        mb.rd_ptr()[0]  = '\0';
        // move rd_ptr backwards as required for printout
        mb.rd_ptr (- result.bytes_transferred ());
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),
                    mb.rd_ptr ()));
#endif /* (defined (ACE_WIN32) && !defined (ACE_HAS_WINCE)) */

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("**** end of message ****************\n")));
      }
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Client %d; %p\n"),
                                       this->id_,
                                       ACE_TEXT ("write"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Client %d: wrote %d bytes ok\n"),
                    this->id_,
                    result.bytes_transferred ()));
      }

    mb.release ();

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_snd_ += result.bytes_transferred ();
        if (this->total_snd_ >= xfer_limit)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) Client %d sent %d, limit %d\n"),
                        this->id_, this->total_snd_, xfer_limit));
            this->close ();
          }
        if (duplex != 0)   // full duplex, continue write
          {
            if ((this->total_snd_- this->total_rcv_) < 1024*32 ) //flow control
              this->initiate_write_stream ();
          }
        else  // half-duplex   read reply, after read we will start write
          this->initiate_read_stream ();
      }

    --this->io_count_;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

void
Client::handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
{
  {
    ACE_GUARD (ACE_SYNCH_MUTEX, monitor, this->lock_);

    ACE_Message_Block & mb = result.message_block ();

    if (loglevel > 1)
      {
        LogLocker log_lock;

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) **** Client %d: handle_read_stream() ****\n"),
                    this->id_));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_to_read"),
                    result.bytes_to_read ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("handle"),
                    result.handle ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("bytes_transfered"),
                    result.bytes_transferred ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("act"),
                    result.act ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("success"),
                    result.success ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %@\n"),
                    ACE_TEXT ("completion_key"),
                    result.completion_key ()));
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %d\n"),
                    ACE_TEXT ("error"),
                    result.error ()));

#if defined (ACE_WIN32)
        char index = 0;
        for (ACE_Message_Block* mb_i = &mb;
             mb_i != 0;
             mb_i = mb_i->cont ())
          {
            ++index;
            // write 0 at string end for proper printout
            mb_i->wr_ptr()[0]  = '\0';

            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%s%d = %s\n"),
                        ACE_TEXT ("message_block, part "),
                        index,
                        mb_i->rd_ptr ()));
          }
#else /* ACE_WIN32 */
        // write 0 at string end for proper printout
        mb.rd_ptr()[result.bytes_transferred ()]  = '\0'; // for proper printout
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("%s = %s\n"),
                    ACE_TEXT ("message_block"),
                    mb.rd_ptr ()));
#endif /* ACE_WIN32 */

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("**** end of message ****************\n")));
      }
    else if (result.error () != 0)
      {
        ACE_Log_Priority prio;
#if defined (ACE_WIN32)
        if (result.error () == ERROR_OPERATION_ABORTED)
          prio = LM_DEBUG;
#else
        if (result.error () == ECANCELED)
          prio = LM_DEBUG;
#endif /* ACE_WIN32 */
        else
          prio = LM_ERROR;
        ACE_Log_Msg::instance ()->errnum (result.error ());
        ACE_Log_Msg::instance ()->log (prio,
                                       ACE_TEXT ("(%t) Client %d; %p\n"),
                                       this->id_,
                                       ACE_TEXT ("read"));
      }
    else if (loglevel > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%t) Client %d: read %d bytes ok\n"),
                    this->id_,
                    result.bytes_transferred ()));
      }

    mb.release ();

    if (result.error () == 0 && result.bytes_transferred () > 0)
      {
        this->total_rcv_ += result.bytes_transferred ();

        if (duplex != 0 || this->stop_writing_)  // full duplex, continue read
          this->initiate_read_stream ();
        else  // half-duplex  write, after write we will start read
          this->initiate_write_stream ();
      }

    --this->io_count_;
    if (this->io_count_ > 0)
      return;
  }
  delete this;
}

// *************************************************************
//   Configuration helpers
// *************************************************************
int
print_usage (int /* argc */, ACE_TCHAR *argv[])
{
  ACE_ERROR
    ((LM_ERROR,
      ACE_TEXT ("\nusage: %s")
      ACE_TEXT ("\n-o <max number of started aio operations for Proactor>")
      ACE_TEXT ("\n-t <Proactor type> UNIX-only, Win32-default always:")
      ACE_TEXT ("\n    a AIOCB")
      ACE_TEXT ("\n    i SIG")
      ACE_TEXT ("\n    c CB")
      ACE_TEXT ("\n    s SUN")
      ACE_TEXT ("\n    d default")
      ACE_TEXT ("\n-d <duplex mode 1-on/0-off>")
      ACE_TEXT ("\n-h <host> for Client mode")
      ACE_TEXT ("\n-n <number threads for Proactor pool>")
      ACE_TEXT ("\n-p <port to listen/connect>")
      ACE_TEXT ("\n-c <number of client instances>")
      ACE_TEXT ("\n-b run client and server at the same time")
      ACE_TEXT ("\n    f file")
      ACE_TEXT ("\n    c console")
      ACE_TEXT ("\n-v log level")
      ACE_TEXT ("\n    0 - log errors and highlights")
      ACE_TEXT ("\n    1 - log level 0 plus progress information")
      ACE_TEXT ("\n    2 - log level 1 plus operation parameters and results")
      ACE_TEXT ("\n-x max transfer byte count per Client")
      ACE_TEXT ("\n-u show this message")
      ACE_TEXT ("\n"),
      argv[0]
   ));
  return -1;
}

static int
set_proactor_type (const ACE_TCHAR *ptype)
{
  if (!ptype)
    return 0;

  switch (ACE_OS::ace_toupper (*ptype))
    {
    case 'D':
      proactor_type = DEFAULT;
      return 1;
    case 'A':
      proactor_type = AIOCB;
      return 1;
    case 'I':
      proactor_type = SIG;
      return 1;
#if defined (sun)
    case 'S':
      proactor_type = SUN;
      return 1;
#endif /* sun */
#if !defined (ACE_HAS_BROKEN_SIGEVENT_STRUCT)
     case 'C':
       proactor_type = CB;
       return 1;
#endif /* !ACE_HAS_BROKEN_SIGEVENT_STRUCT */
    default:
      break;
    }
  return 0;
}

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  // First, set up all the defaults then let any args change them.
  both = 1;                       // client and server simultaneosly
  duplex = 1;                     // full duplex is on
#if defined (ACE_HAS_IPV6)
  host = ACE_IPV6_LOCALHOST;      // server to connect (IPv6 localhost)
#else
   host = ACE_LOCALHOST;
#endif /*ACE_HAS_IPV6*/
  port = ACE_DEFAULT_SERVER_PORT; // port to connect/listen
  max_aio_operations = 512;       // POSIX Proactor params
  proactor_type = DEFAULT;        // Proactor type = default
  threads = 3;                    // size of Proactor thread pool
  clients = 10;                   // number of clients
  loglevel = 0;                   // log level : only errors and highlights
  // Default transfer limit 50 messages per Sender
  xfer_limit = 50 * ACE_OS::strlen (complete_message);

  // Linux kernels up to at least 2.6.9 (RHEL 4) can't do full duplex aio.
# if defined (ACE_LINUX)
  duplex = 0;
#endif

  if (argc == 1) // no arguments , so one button test
    return 0;

  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("x:t:o:n:p:d:h:c:v:ub"));
  int c;

  while ((c = get_opt ()) != EOF)
    {
    switch (c)
      {
      case 'x':  // xfer limit
        xfer_limit = static_cast<size_t> (ACE_OS::atoi (get_opt.opt_arg ()));
        if (xfer_limit == 0)
          xfer_limit = 1;          // Bare minimum.
        break;
      case 'b':  // both client and server
        both = 1;
        break;
      case 'v':  // log level
        loglevel = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'd':         // duplex
        duplex = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'h':         // host for sender
        host = get_opt.opt_arg ();
        break;
      case 'p':         // port number
        port = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'n':         // thread pool size
        threads = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'c':     // number of clients
        clients = ACE_OS::atoi (get_opt.opt_arg ());
        if (clients > MAX_CLIENTS)
          clients = MAX_CLIENTS;
        break;
      case 'o':     // max number of aio for proactor
        max_aio_operations = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 't':    //  Proactor Type
        if (set_proactor_type (get_opt.opt_arg ()))
          break;
        return print_usage (argc, argv);
      case 'u':
      default:
        return print_usage (argc, argv);
      } // switch
    } // while

  if (proactor_type == SUN && threads > 1)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sun aiowait is not thread-safe; ")
                  ACE_TEXT ("changing to 1 thread\n")));
      threads = 1;
    }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Test_IPV6"));

  if (::parse_args (argc, argv) == -1)
    return -1;

#if defined (ACE_HAS_IPV6)
  disable_signal (ACE_SIGRTMIN, ACE_SIGRTMAX);
  disable_signal (SIGPIPE, SIGPIPE);

  MyTask    task1;
  TestData  test;

  if (task1.start (threads, proactor_type, max_aio_operations) == 0)
    {
      Acceptor  acceptor (&test);
      Connector connector (&test);
      ACE_INET_Addr addr (port, "::");

      int rc = 0;

      if (both != 0 || host == 0) // Acceptor
        {
          // Simplify, initial read with zero size
          if (acceptor.open (addr, 0, 1) == 0)
            rc = 1;
        }

      if (both != 0 || host != 0)
        {
          if (host == 0)
            host = ACE_IPV6_LOCALHOST;

          if (addr.set (port, host, 1, addr.get_type ()) == -1)
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), host));
          else
            rc += connector.start (addr, clients);
        }

      // Wait a few seconds to let things get going, then poll til
      // all sessions are done. Note that when we exit this scope, the
      // Acceptor and Connector will be destroyed, which should prevent
      // further connections and also test how well destroyed handlers
      // are handled.
      ACE_OS::sleep (3);
    }
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Sleeping til sessions run down.\n")));
  while (!test.testing_done ())
    ACE_OS::sleep (1);

  test.stop_all ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Stop Thread Pool Task\n")));
  task1.stop ();

#endif /* ACE_HAS_IPV6 */

  ACE_END_TEST;

  return 0;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Test_IPV6"));

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Threads or Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Test_IPV6 will not be run.\n")));

  ACE_END_TEST;

  return 0;
}

#endif  /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
