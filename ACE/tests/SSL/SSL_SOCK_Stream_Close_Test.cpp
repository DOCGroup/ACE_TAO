/**
 * @file SSL_SOCK_Stream_Close_Test.cpp
 */

#include "../test_config.h"
#include "ace/SSL/SSL_Context.h"
#include "ace/SSL/SSL_SOCK_Acceptor.h"
#include "ace/SSL/SSL_SOCK_Connector.h"
#include "ace/Semaphore.h"
#include "ace/Signal.h"
#include "ace/Thread_Manager.h"

#include <memory>

#if defined (ACE_HAS_THREADS)
namespace
{
  long force_write_retry (BIO *bio, int operation, long result)
  {
    if (operation == BIO_CB_WRITE)
      {
        int *count = reinterpret_cast<int *> (::BIO_get_callback_arg (bio));
        ++*count;
        BIO_set_retry_write (bio);
        return -1;
      }
    return result;
  }

#if OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined (LIBRESSL_VERSION_NUMBER)
  long write_retry_callback (BIO *bio, int operation, char const *, size_t,
                             int, long, int result, size_t *)
  {
    return force_write_retry (bio, operation, result);
  }
#else
  long write_retry_callback (BIO *bio, int operation, char const *, int,
                             long, long result)
  {
    return force_write_retry (bio, operation, result);
  }
#endif

  struct Server_State
  {
    explicit Server_State (ACE_SSL_SOCK_Acceptor &listener)
      : listener_ (listener) {}

    ACE_SSL_SOCK_Acceptor &listener_;
    ACE_Semaphore ready_ {0};
    ACE_Semaphore release_ {0};
    int accept_result_ {-1};
    bool check_data_ {};
    bool data_received_ {};
  };

  ACE_THR_FUNC_RETURN server (void *arg)
  {
    Server_State &state = *static_cast<Server_State *> (arg);
    ACE_SSL_SOCK_Stream stream;
    ACE_Time_Value timeout (5);
    state.accept_result_ = state.listener_.accept (stream, nullptr, std::addressof (timeout));
    state.ready_.release ();
    if (state.accept_result_ == 0)
      {
        if (state.check_data_)
          {
            char data[32768];
            ACE_Time_Value const read_timeout (5);
            ssize_t const received = stream.recv_n (data, sizeof (data), std::addressof (read_timeout));
            state.data_received_ = received == static_cast<ssize_t> (sizeof (data));
            for (size_t i {}; state.data_received_ && i < sizeof (data); ++i)
              {
                state.data_received_ = data[i] == 'x';
              }

          }
      }
    state.release_.acquire ();
    if (state.accept_result_ == 0)
      {
        stream.close ();
      }
    return ACE_THR_FUNC_RETURN_NULL;
  }
}

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SSL_SOCK_Stream_Close_Test"));
#if defined (SIGPIPE)
  ACE_Sig_Action no_sigpipe (reinterpret_cast<ACE_SignalHandler> (SIG_IGN));
  ACE_Sig_Action original_sigpipe;
  no_sigpipe.register_action (SIGPIPE, std::addressof (original_sigpipe));
#endif

  int result {};
  ACE_SSL_Context * const context = ACE_SSL_Context::instance ();
  if (context->certificate ("dummy.pem", SSL_FILETYPE_PEM) != 0
      || context->private_key ("key.pem", SSL_FILETYPE_PEM) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot configure TLS certificate\n")), 1);
    }

  ACE_SSL_SOCK_Acceptor listener;
  ACE_INET_Addr address (static_cast<u_short> (0), ACE_TEXT ("127.0.0.1"));
  if (listener.open (address) != 0 || listener.get_local_addr (address) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot open TLS listener: %m\n")), 1);
    }

  ACE_SSL_SOCK_Connector connector;
  ACE_SSL_SOCK_Stream client;
  for (int test {}; test < 3; ++test)
    {
      Server_State state (listener);
      state.check_data_ = test == 2;
      ACE_Thread_Manager manager;
      if (manager.spawn (server, std::addressof (state)) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Cannot spawn TLS server\n")));
          result = 1;
          break;
        }

      ACE_Time_Value const timeout (5);
      int const connected = connector.connect (client, address, std::addressof (timeout));
      state.ready_.acquire ();
      if (connected != 0 || state.accept_result_ != 0)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("TLS handshake failed\n")));
          result = 1;
        }
      else
        {
          int retry_count {};
          BIO *retry_bio {};
          if (test == 1)
            {
              int send_buffer_size = 4096;
              if (client.set_option (SOL_SOCKET, SO_SNDBUF,
                                     std::addressof (send_buffer_size),
                                     sizeof (send_buffer_size)) != 0
                  || client.enable (ACE_NONBLOCK) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Cannot enable nonblocking mode\n")));
                  result = 1;
                }
              else
                {
                  // Fill the raw socket to force TLS shutdown to block; the
                  // peer does not process these bytes as TLS records.
                  char data[8192] {};
                  bool blocked {};
                  for (size_t i {}; i < 16384; ++i)
                    {
                      if (client.peer ().send (data, sizeof (data)) == -1)
                        {
                          blocked = errno == EWOULDBLOCK || errno == EAGAIN;
                          break;
                        }
                    }
                  if (!blocked)
                    {
                      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Could not fill the send buffer\n")));
                      result = 1;
                    }
                  else
                    {
                      // Force a retry in the TLS write itself, even if the
                      // kernel has freed space since the raw send blocked.
                      retry_bio = ::SSL_get_wbio (client.ssl ());
                      if (retry_bio == nullptr)
                        {
                          ACE_ERROR ((LM_ERROR, ACE_TEXT ("TLS write BIO is missing\n")));
                          result = 1;
                        }
                      else
                        {
                          ::BIO_set_callback_arg (retry_bio, reinterpret_cast<char *> (std::addressof (retry_count)));
#if OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined (LIBRESSL_VERSION_NUMBER)
                          ::BIO_set_callback_ex (retry_bio, write_retry_callback);
#else
                          ::BIO_set_callback (retry_bio, write_retry_callback);
#endif
                        }
                    }
                }
            }

          if (test == 2)
            {
              int send_buffer_size = 4096;
              if (client.set_option (SOL_SOCKET, SO_SNDBUF,
                                     std::addressof (send_buffer_size),
                                     sizeof (send_buffer_size)) != 0)
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("Cannot constrain the send buffer\n")));
                  result = 1;
                }
              char data[32768];
              for (size_t i {}; i < sizeof (data); ++i)
                {
                  data[i] = 'x';
                }
              ACE_Time_Value const send_timeout (5);
              if (client.send_n (data, sizeof (data), std::addressof (send_timeout))
                    != static_cast<ssize_t> (sizeof (data)))
                {
                  ACE_ERROR ((LM_ERROR, ACE_TEXT ("TLS send failed\n")));
                  result = 1;
                }
            }
          int const close_result = client.close ();
          if (retry_bio != nullptr && ::SSL_get_wbio (client.ssl ()) == retry_bio)
            {
#if OPENSSL_VERSION_NUMBER >= 0x10101000L && !defined (LIBRESSL_VERSION_NUMBER)
              ::BIO_set_callback_ex (retry_bio, nullptr);
#else
              ::BIO_set_callback (retry_bio, nullptr);
#endif
              ::BIO_set_callback_arg (retry_bio, nullptr);
            }
          if (test == 1 && retry_count == 0)
            {
              ACE_ERROR ((LM_ERROR, ACE_TEXT ("TLS shutdown did not encounter the write retry\n")));
              result = 1;
            }
          ACE_HANDLE const handle = client.get_handle ();
          int const shutdown_state = ::SSL_get_shutdown (client.ssl ());
          if (close_result != 0 || handle != ACE_INVALID_HANDLE || shutdown_state != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TLS close result %d, handle %H, shutdown state %d\n"),
                          close_result, handle, shutdown_state));
              result = 1;
            }
        }

      state.release_.release ();
      manager.wait ();
      if (state.check_data_ && !state.data_received_)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("TLS data lost on close\n")));
          result = 1;
        }
      if (result != 0)
        {
          break;
        }
    }
  listener.close ();
  ACE_END_TEST;
  return result;
}
#else
int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("SSL_SOCK_Stream_Close_Test"));
  ACE_END_TEST;
  return 0;
}
#endif
