// ============================================================================
/**
 *  @file Proactor_Contract_Test.cpp
 *
 *  Focused regression coverage for common ACE Proactor contracts and
 *  backend-specific edge cases.
 */
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS)

#include "ace/Asynch_IO.h"
#include "ace/FILE_Connector.h"
#include "ace/FILE_IO.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/Log_Category.h"
#include "ace/Message_Block.h"
#include "ace/OS_NS_dirent.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Proactor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"
#include "ace/Time_Value.h"
#include "Proactor_Test_Backend.h"

namespace
{
  Proactor_Test_Backend::Type proactor_type = Proactor_Test_Backend::BACKEND_DEFAULT;

  class Library_Error_Mask_Guard
  {
  public:
    Library_Error_Mask_Guard (void)
      : category_ (ACE_Log_Category::ace_lib ())
      , thread_category_ (category_.per_thr_obj ())
      , category_mask_ (category_.priority_mask ())
      , thread_mask_ (thread_category_ != 0
                      ? thread_category_->priority_mask ()
                      : 0)
    {
      category_.priority_mask (category_mask_ & ~LM_ERROR);
      if (thread_category_ != 0)
        thread_category_->priority_mask (thread_mask_ & ~LM_ERROR);
    }

    ~Library_Error_Mask_Guard (void)
    {
      category_.priority_mask (category_mask_);
      if (thread_category_ != 0)
        thread_category_->priority_mask (thread_mask_);
    }

  private:
    Library_Error_Mask_Guard (const Library_Error_Mask_Guard &);
    Library_Error_Mask_Guard &operator= (const Library_Error_Mask_Guard &);

    ACE_Log_Category &category_;
    ACE_Log_Category_TSS *thread_category_;
    u_long const category_mask_;
    u_long const thread_mask_;
  };

  class Accept_Handler : public ACE_Handler
  {
  public:
    Accept_Handler (void)
      : done_ (false)
      , success_ (false)
      , bytes_transferred_ (static_cast<size_t> (-1))
      , accept_handle_ (ACE_INVALID_HANDLE)
      , error_ (0)
    {
    }

    virtual void handle_accept (const ACE_Asynch_Accept::Result &result)
    {
      this->done_ = true;
      this->success_ = result.success () != 0;
      this->bytes_transferred_ = result.bytes_transferred ();
      this->accept_handle_ = result.accept_handle ();
      this->error_ = result.error ();
    }

    bool done_;
    bool success_;
    size_t bytes_transferred_;
    ACE_HANDLE accept_handle_;
    u_long error_;
  };

  class Dummy_Handler : public ACE_Handler
  {
  };

  class Connect_Handler : public ACE_Handler
  {
  public:
    Connect_Handler (void)
      : done_ (false)
      , success_ (false)
      , connect_handle_ (ACE_INVALID_HANDLE)
      , error_ (0)
    {
    }

    virtual void handle_connect (const ACE_Asynch_Connect::Result &result)
    {
      this->done_ = true;
      this->success_ = result.success () != 0;
      this->connect_handle_ = result.connect_handle ();
      this->error_ = result.error ();
    }

    bool done_;
    bool success_;
    ACE_HANDLE connect_handle_;
    u_long error_;
  };

  class Transmit_Handler : public ACE_Handler
  {
  public:
    Transmit_Handler (void)
      : done_ (false)
      , success_ (false)
      , bytes_transferred_ (0)
      , error_ (0)
    {
    }

    virtual void handle_transmit_file (const ACE_Asynch_Transmit_File::Result &result)
    {
      this->done_ = true;
      this->success_ = result.success () != 0;
      this->bytes_transferred_ = result.bytes_transferred ();
      this->error_ = result.error ();
    }

    bool done_;
    bool success_;
    size_t bytes_transferred_;
    u_long error_;
  };

  class Timer_Handler : public ACE_Handler
  {
  public:
    Timer_Handler (void)
      : done_ (false)
    {
    }

    virtual void handle_time_out (const ACE_Time_Value &, const void *)
    {
      this->done_ = true;
    }

    bool done_;
  };

  size_t
  count_open_fds (void)
  {
#if defined (ACE_WIN32)
    return 0;
#else
    size_t count = 0;
    ACE_DIR *dir = ACE_OS::opendir (ACE_TEXT ("/proc/self/fd"));
    if (dir == 0)
      return 0;

    for (ACE_DIRENT *entry = ACE_OS::readdir (dir);
         entry != 0;
         entry = ACE_OS::readdir (dir))
      {
        if (ACE_OS::strcmp (entry->d_name, ".") != 0
            && ACE_OS::strcmp (entry->d_name, "..") != 0)
          ++count;
      }

    ACE_OS::closedir (dir);
    return count;
#endif /* ACE_WIN32 */
  }

  int
  parse_args (int argc, ACE_TCHAR *argv[])
  {
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("t:"));
    int c = 0;

    while ((c = get_opt ()) != EOF)
      {
        switch (c)
          {
          case 't':
            if (Proactor_Test_Backend::parse_type (get_opt.opt_arg (), proactor_type) == 0)
              {
                if (Proactor_Test_Backend::is_available (proactor_type))
                  break;

                Proactor_Test_Backend::unsupported (proactor_type);
              }
            return -1;
          default:
            return -1;
          }
      }

    return 0;
  }

  int
  run_accept_contract_test (ACE_Proactor &proactor)
  {
    ACE_SOCK_Acceptor listen_socket;
    ACE_INET_Addr listen_addr ((u_short) 0, ACE_LOCALHOST);

    if (listen_socket.open (listen_addr, 1) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.open")),
                          -1);
      }

    if (listen_socket.get_local_addr (listen_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.get_local_addr")),
                          -1);
      }

    const int addr_family = listen_addr.get_type ();

    Accept_Handler handler;
    ACE_Asynch_Accept acceptor;
    if (acceptor.open (handler, listen_socket.get_handle (), 0, &proactor) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_Asynch_Accept::open")),
                          -1);
      }

    ACE_Message_Block mb (1024);
    if (acceptor.accept (mb, 0, ACE_INVALID_HANDLE, 0, 0, ACE_SIGRTMIN, addr_family) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_Asynch_Accept::accept")),
                          -1);
      }

    ACE_Time_Value zero = ACE_Time_Value::zero;
    const int empty_poll = proactor.handle_events (zero);
    if (empty_poll != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Expected empty zero-timeout poll to return 0; got %d\n"),
                    empty_poll));
        return -1;
      }

    ACE_SOCK_Connector sock_connector;
    ACE_SOCK_Stream client_stream;
    if (sock_connector.connect (client_stream, listen_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("connector.connect")),
                          -1);
      }

    const ACE_Time_Value deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
    while (!handler.done_ && ACE_OS::gettimeofday () < deadline)
      {
        ACE_Time_Value poll_timeout = ACE_Time_Value::zero;
        const int result = proactor.handle_events (poll_timeout);
        if (result == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("ACE_Proactor::handle_events")),
                              -1);
          }

        if (!handler.done_)
          ACE_OS::sleep (ACE_Time_Value (0, 1000));
      }

    if (!handler.done_)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Timed out waiting for accept completion\n")));
        return -1;
      }

    if (!handler.success_)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Accept completion failed with error %u\n"),
                    handler.error_));
        return -1;
      }

    if (handler.accept_handle_ == ACE_INVALID_HANDLE)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Accept completion returned invalid accept handle\n")));
        return -1;
      }

    if (handler.bytes_transferred_ != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Accept completion bytes_transferred=%B; expected 0\n"),
                    handler.bytes_transferred_));
        return -1;
      }

    ACE_OS::closesocket (handler.accept_handle_);
    client_stream.close ();
    listen_socket.close ();
    return 0;
  }

  int
  run_accept_cancel_test (ACE_Proactor &proactor)
  {
    ACE_SOCK_Acceptor listen_socket;
    ACE_INET_Addr listen_addr ((u_short) 0, ACE_LOCALHOST);

    if (listen_socket.open (listen_addr, 1) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.open")),
                          -1);
      }

    if (listen_socket.get_local_addr (listen_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.get_local_addr")),
                          -1);
      }

    const int addr_family = listen_addr.get_type ();

    Accept_Handler handler;
    ACE_Asynch_Accept acceptor;
    if (acceptor.open (handler, listen_socket.get_handle (), 0, &proactor) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_Asynch_Accept::open")),
                          -1);
      }

    ACE_Message_Block mb (1024);
    if (acceptor.accept (mb, 0, ACE_INVALID_HANDLE, 0, 0, ACE_SIGRTMIN, addr_family) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_Asynch_Accept::accept")),
                          -1);
      }

    const int cancel_result = acceptor.cancel ();
    if (cancel_result != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("accept cancel returned %d; expected 0\n"),
                    cancel_result));
        return -1;
      }

    const ACE_Time_Value deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
    while (!handler.done_ && ACE_OS::gettimeofday () < deadline)
      {
        ACE_Time_Value wait_time (0, 10000);
        const int result = proactor.handle_events (wait_time);
        if (result == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("ACE_Proactor::handle_events")),
                              -1);
          }
      }

    if (!handler.done_)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Timed out waiting for canceled accept completion\n")));
        return -1;
      }

    if (handler.success_)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Canceled accept unexpectedly succeeded\n")));
        return -1;
      }

    const u_long expected_cancel_error =
#if defined (ACE_WIN32)
      ERROR_OPERATION_ABORTED
#else
      ECANCELED
#endif /* ACE_WIN32 */
      ;

    if (handler.error_ != expected_cancel_error)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Canceled accept error=%u; expected %u\n"),
                    handler.error_,
                    expected_cancel_error));
        return -1;
      }

    if (handler.bytes_transferred_ != 0)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Canceled accept bytes_transferred=%B; expected 0\n"),
                    handler.bytes_transferred_));
        return -1;
      }

    if (handler.accept_handle_ != ACE_INVALID_HANDLE)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("Canceled accept returned unexpected handle %d\n"),
                    handler.accept_handle_));
        ACE_OS::closesocket (handler.accept_handle_);
        return -1;
      }

    listen_socket.close ();
    return 0;
  }

  int
  run_connect_failure_cleanup_test (ACE_Proactor &proactor)
  {
#if defined (ACE_WIN32)
    ACE_SOCK_Acceptor remote_socket;
    ACE_INET_Addr remote_addr ((u_short) 0, ACE_LOCALHOST);
    if (remote_socket.open (remote_addr, 1) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("remote_socket.open")),
                          -1);
      }

    if (remote_socket.get_local_addr (remote_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("remote_socket.get_local_addr")),
                          -1);
      }

    // Exercise Windows connect-failure cleanup via an unused destination
    // port. Forcing a local bind collision drives the WIN32 connector down
    // an immediate-error path that corrupts teardown state.
    remote_socket.close ();
#else
    ACE_SOCK_Acceptor busy_local_socket;
    ACE_INET_Addr busy_local_addr ((u_short) 0, ACE_LOCALHOST);
    if (busy_local_socket.open (busy_local_addr, 1) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("busy_local_socket.open")),
                          -1);
      }

    if (busy_local_socket.get_local_addr (busy_local_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("busy_local_socket.get_local_addr")),
                          -1);
      }

    ACE_SOCK_Acceptor remote_socket;
    ACE_INET_Addr remote_addr ((u_short) 0, ACE_LOCALHOST);
    if (remote_socket.open (remote_addr, 1) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("remote_socket.open")),
                          -1);
      }

    if (remote_socket.get_local_addr (remote_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("remote_socket.get_local_addr")),
                          -1);
      }
#endif /* ACE_WIN32 */

    Connect_Handler handler;
    ACE_Asynch_Connect connector;
    if (connector.open (handler, ACE_INVALID_HANDLE, 0, &proactor) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_Asynch_Connect::open")),
                          -1);
      }

    const size_t fd_count_before = count_open_fds ();
    const int attempts =
#if defined (ACE_WIN32)
      4
#else
      32
#endif /* ACE_WIN32 */
      ;

    for (int i = 0; i < attempts; ++i)
      {
        handler.done_ = false;
        handler.success_ = false;
        handler.connect_handle_ = ACE_INVALID_HANDLE;
        handler.error_ = 0;
        errno = 0;
        int result = 0;
        {
          Library_Error_Mask_Guard error_mask_guard;
#if defined (ACE_WIN32)
          result = connector.connect (ACE_INVALID_HANDLE,
                                      remote_addr,
                                      ACE_Addr::sap_any,
                                      0,
                                      0);
#else
          result = connector.connect (ACE_INVALID_HANDLE,
                                      remote_addr,
                                      busy_local_addr,
                                      0,
                                      0);
#endif /* ACE_WIN32 */
        }

        if (result == 0)
          {
            const ACE_Time_Value deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
            while (!handler.done_ && ACE_OS::gettimeofday () < deadline)
              {
                ACE_Time_Value wait_time (0, 10000);
                const int handle_result = proactor.handle_events (wait_time);
                if (handle_result == -1)
                  {
                    ACE_ERROR_RETURN ((LM_ERROR,
                                       ACE_TEXT ("%p\n"),
                                       ACE_TEXT ("ACE_Proactor::handle_events")),
                                      -1);
                  }
              }

            if (!handler.done_)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("connect failure did not complete on iteration %d\n"),
                            i));
                return -1;
              }

            if (handler.success_)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("connect failure unexpectedly succeeded on iteration %d\n"),
                            i));
                return -1;
              }
          }
        else if (result != -1)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("connect failure returned unexpected rc=%d on iteration %d\n"),
                        result,
                        i));
            return -1;
          }
        else if (errno == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("connect failure cleanup test did not set errno on iteration %d\n"),
                        i));
            return -1;
          }
      }

    const size_t fd_count_after = count_open_fds ();
    if (fd_count_before != fd_count_after)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("connect failure leaked fds: before=%B after=%B\n"),
                    fd_count_before,
                    fd_count_after));
        return -1;
      }

    remote_socket.close ();
#if !defined (ACE_WIN32)
    busy_local_socket.close ();
#endif /* !ACE_WIN32 */
    return 0;
  }

  int
  run_transmit_file_contract_test (ACE_Proactor &proactor)
  {
#if defined (ACE_WIN32)
    ACE_UNUSED_ARG (proactor);
    ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("Skipping transmit_file contract on Windows; ")
                ACE_TEXT ("this test harness uses non-overlapped stream handles.\n")));
    return 0;
#endif /* ACE_WIN32 */

    ACE_SOCK_Acceptor listen_socket;
    ACE_INET_Addr listen_addr ((u_short) 0, ACE_LOCALHOST);
    if (listen_socket.open (listen_addr, 1) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.open")),
                          -1);
      }

    if (listen_socket.get_local_addr (listen_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.get_local_addr")),
                          -1);
      }

    ACE_SOCK_Connector connector;
    ACE_SOCK_Stream client_stream;
    if (connector.connect (client_stream, listen_addr) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("connector.connect")),
                          -1);
      }

    ACE_SOCK_Stream server_stream;
    if (listen_socket.accept (server_stream) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("listen_socket.accept")),
                          -1);
      }

    ACE_FILE_Connector file_connector;
    ACE_FILE_IO file;
    if (file_connector.connect (file, ACE_sap_any_cast (ACE_FILE_Addr &)) != 0)
      {
        server_stream.close ();
        client_stream.close ();
        listen_socket.close ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_FILE_Connector::connect")),
                          -1);
      }

    ACE_FILE_Addr tmp_addr;
    file.get_local_addr (tmp_addr);
    if (ACE_OS::write (file.get_handle (), "u", 1) != 1)
      {
        file.remove ();
        server_stream.close ();
        client_stream.close ();
        listen_socket.close ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("write")),
                          -1);
      }

    Transmit_Handler handler;
    ACE_Asynch_Transmit_File transmit_file;
    if (transmit_file.open (handler, server_stream.get_handle (), 0, &proactor) != 0)
      {
        file.remove ();
        server_stream.close ();
        client_stream.close ();
        listen_socket.close ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("ACE_Asynch_Transmit_File::open")),
                          -1);
      }

    errno = 0;
    const int result = transmit_file.transmit_file (file.get_handle (),
                                                    0,
                                                    1,
                                                    0,
                                                    0,
                                                    0,
                                                    0,
                                                    0);
    if (proactor_type == Proactor_Test_Backend::BACKEND_URING)
      {
        if (result != -1 || errno != ENOTSUP)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("transmit_file result=%d errno=%d; expected -1/ENOTSUP\n"),
                        result,
                        errno));
            file.remove ();
            server_stream.close ();
            client_stream.close ();
            listen_socket.close ();
            return -1;
          }
      }
    else
      {
        if (result != 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("POSIX transmit_file returned %d errno=%d; expected 0\n"),
                        result,
                        errno));
            file.remove ();
            server_stream.close ();
            client_stream.close ();
            listen_socket.close ();
            return -1;
          }

        const ACE_Time_Value deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
        while (!handler.done_ && ACE_OS::gettimeofday () < deadline)
          {
            ACE_Time_Value wait_time (0, 10000);
            const int handle_result = proactor.handle_events (wait_time);
            if (handle_result == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("%p\n"),
                                   ACE_TEXT ("ACE_Proactor::handle_events")),
                                  -1);
              }
          }

        if (!handler.done_ || !handler.success_ || handler.bytes_transferred_ == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("POSIX transmit_file completion invalid: done=%d success=%d bytes=%B error=%u\n"),
                        handler.done_,
                        handler.success_,
                        handler.bytes_transferred_,
                        handler.error_));
            file.remove ();
            server_stream.close ();
            client_stream.close ();
            listen_socket.close ();
            return -1;
          }

        char received = '\0';
        const ssize_t recv_result =
          ACE_OS::recv (client_stream.get_handle (), &received, 1, 0);
        if (recv_result != 1 || received != 'u')
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("POSIX transmit_file recv_result=%d byte=%c; expected 1/'u'\n"),
                        static_cast<int> (recv_result),
                        received));
            file.remove ();
            server_stream.close ();
            client_stream.close ();
            listen_socket.close ();
            return -1;
          }
      }

    file.remove ();
    server_stream.close ();
    client_stream.close ();
    listen_socket.close ();
    return 0;
  }

  int
  run_close_singleton_quiesced_test (void)
  {
    ACE_Proactor *proactor_ptr = 0;
    if (Proactor_Test_Backend::create_proactor (proactor_type,
                                                128,
                                                proactor_ptr,
                                                true) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Proactor_Test_Backend::create_proactor")),
                          -1);
      }

    int status = 0;
    Timer_Handler handler;
    // Drive one real async completion before tearing the singleton down.
    if (proactor_ptr->schedule_timer (handler, 0, ACE_Time_Value (0, 1000)) == -1)
      {
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("ACE_Proactor::schedule_timer")));
        status = -1;
      }
    else
      {
        const ACE_Time_Value deadline = ACE_OS::gettimeofday () + ACE_Time_Value (5);
        while (!handler.done_ && ACE_OS::gettimeofday () < deadline)
          {
            ACE_Time_Value wait_time (0, 10000);
            const int result = proactor_ptr->handle_events (wait_time);
            if (result == -1)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("%p\n"),
                            ACE_TEXT ("ACE_Proactor::handle_events")));
                status = -1;
                break;
              }
          }

        if (status == 0 && !handler.done_)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("Timed out waiting for singleton timer completion\n")));
            status = -1;
          }
      }

    if (!handler.done_)
      (void) proactor_ptr->cancel_timer (handler);

    ACE_Proactor::close_singleton ();
    return status;
  }
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Contract_Test"));

  if (parse_args (argc, argv) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Usage: %s [-t <backend>]\n"),
                  argv[0]));
      ACE_END_TEST;
      return -1;
    }

  ACE_Proactor *proactor_ptr = 0;
  if (Proactor_Test_Backend::create_proactor (proactor_type,
                                              128,
                                              proactor_ptr,
                                              false) != 0)
    {
      ACE_END_TEST;
      return -1;
    }
  ACE_Proactor &proactor = *proactor_ptr;

  int status = 0;
  if (run_accept_contract_test (proactor) != 0)
    status = -1;
  else if (run_accept_cancel_test (proactor) != 0)
    status = -1;
  else if (run_connect_failure_cleanup_test (proactor) != 0)
    status = -1;
  else if (run_transmit_file_contract_test (proactor) != 0)
    status = -1;

  delete proactor_ptr;
  if (status == 0 && run_close_singleton_quiesced_test () != 0)
    status = -1;
  ACE_END_TEST;
  return status;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Contract_Test"));
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("Asynchronous IO is unsupported.\n")
              ACE_TEXT ("Proactor_Contract_Test will not be run.\n")));
  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS */
