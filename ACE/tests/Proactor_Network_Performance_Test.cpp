// ============================================================================
/**
 *  @file Proactor_Network_Performance_Test.cpp
 *
 *  Benchmark-oriented network coverage for selectable ACE Proactor backends.
 *  The control plane uses synchronous socket setup while the data plane uses
 *  asynchronous I/O so throughput and progress behavior can be compared across
 *  backends and platforms.
 */
// ============================================================================

#include "test_config.h"

#if defined (ACE_HAS_THREADS) && (defined (ACE_HAS_WIN32_OVERLAPPED_IO) || defined (ACE_HAS_AIO_CALLS))

#include "ace/ACE.h"
#include "ace/Asynch_IO.h"
#include "ace/Condition_Thread_Mutex.h"
#include "ace/Get_Opt.h"
#include "ace/INET_Addr.h"
#include "ace/Message_Block.h"
#include "ace/OS_NS_errno.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_sys_socket.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Proactor.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_CODgram.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Dgram.h"
#include "ace/SOCK_Stream.h"
#include "ace/Sock_Connect.h"
#include "ace/Task.h"
#include "ace/Thread_Mutex.h"
#include "ace/Time_Value.h"
#include "ace/os_include/os_limits.h"
#include "ace/os_include/netinet/os_tcp.h"

#include "Proactor_Test_Backend.h"

namespace
{
  enum Transport
  {
    TRANSPORT_TCP,
    TRANSPORT_UDP
  };

  struct Config
  {
    Config ()
      : backend (Proactor_Test_Backend::BACKEND_DEFAULT)
      , transport (TRANSPORT_TCP)
      , family (AF_INET)
      , sessions (64)
      , messages_per_endpoint (1024)
      , payload_size (1024)
      , write_depth (8)
      , thread_count (4)
      , max_aio_operations (0)
      , listen_port (0)
      , progress_timeout (10)
      , overall_timeout (120)
      , udp_end_markers (16)
      , udp_receive_buffer (0)
      , udp_send_buffer (0)
    {
    }

    Proactor_Test_Backend::Type backend;
    Transport transport;
    int family;
    size_t sessions;
    size_t messages_per_endpoint;
    size_t payload_size;
    size_t write_depth;
    int thread_count;
    size_t max_aio_operations;
    u_short listen_port;
    long progress_timeout;
    long overall_timeout;
    size_t udp_end_markers;
    size_t udp_receive_buffer;
    size_t udp_send_buffer;
  };

  const ACE_UINT32 UDP_MAGIC = 0x504E5054U; // PNPT
  const ACE_UINT32 UDP_DATA = 1U;
  const ACE_UINT32 UDP_END = 2U;

  struct Udp_Header
  {
    ACE_UINT32 magic_;
    ACE_UINT32 type_;
    ACE_UINT32 session_;
    ACE_UINT32 reserved_;
  };

  bool
  is_cancel_error (u_long error)
  {
#if defined (ACE_WIN32)
    return error == ERROR_OPERATION_ABORTED;
#else
    return error == ECANCELED;
#endif /* ACE_WIN32 */
  }

  bool
  is_retryable_socket_error (u_long error)
  {
#if defined (ACE_WIN32)
    return error == WSAEWOULDBLOCK || error == ERROR_IO_PENDING;
#else
    return error == EAGAIN || error == EWOULDBLOCK;
#endif /* ACE_WIN32 */
  }

  bool
  is_udp_peer_closed_error (u_long error)
  {
#if defined (ACE_WIN32)
    return error == WSAECONNRESET;
#else
    return error == ECONNREFUSED;
#endif /* ACE_WIN32 */
  }

  const ACE_TCHAR *
  transport_name (Transport transport)
  {
    return transport == TRANSPORT_TCP ? ACE_TEXT ("tcp") : ACE_TEXT ("udp");
  }

  const ACE_TCHAR *
  family_name (int family)
  {
    return family == AF_INET6 ? ACE_TEXT ("ipv6") : ACE_TEXT ("ipv4");
  }

  ACE_Time_Value
  time_now ()
  {
    return ACE_OS::gettimeofday ();
  }

  double
  seconds_between (const ACE_Time_Value &start,
                   const ACE_Time_Value &end)
  {
    const ACE_Time_Value delta = end - start;
    return static_cast<double> (delta.sec ()) +
      (static_cast<double> (delta.usec ()) / 1000000.0);
  }

  int parse_unsigned_long_arg (const ACE_TCHAR *text,
                               unsigned long &value);
  int parse_size_arg (const ACE_TCHAR *text,
                      size_t &value);
  int parse_u_short_arg (const ACE_TCHAR *text,
                         u_short &value);
  int parse_int_arg (const ACE_TCHAR *text,
                     int &value);
  int prepare_socket_handle (ACE_HANDLE handle);
  int configure_udp_socket_buffer (ACE_HANDLE handle,
                                   int option_name,
                                   size_t requested_size,
                                   size_t &actual_size);
  int configure_udp_socket_buffers (ACE_HANDLE handle,
                                    const Config &config,
                                    size_t &actual_receive_size,
                                    size_t &actual_send_size);

  class Benchmark_State
  {
  public:
    Benchmark_State (const Config &config,
                     size_t expected_endpoints)
      : config_ (config)
      , lock_ ()
      , done_ (lock_)
      , expected_endpoints_ (expected_endpoints)
      , completed_endpoints_ (0)
      , failed_ (false)
      , stalled_ (false)
      , timed_out_ (false)
      , first_error_code_ (0)
      , total_write_ops_ (0)
      , total_read_ops_ (0)
      , total_sent_bytes_ (0)
      , total_received_bytes_ (0)
      , udp_data_sent_messages_ (0)
      , udp_data_received_messages_ (0)
      , udp_socket_buffers_recorded_ (false)
      , udp_receive_buffer_actual_ (0)
      , udp_send_buffer_actual_ (0)
    {
      this->first_error_[0] = 0;
      this->start_time_ = time_now ();
      this->last_progress_time_ = this->start_time_;
      this->end_time_ = this->start_time_;
    }

    void note_write (size_t bytes,
                     bool count_as_data_message)
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      ++this->total_write_ops_;
      this->total_sent_bytes_ += bytes;
      if (count_as_data_message)
        ++this->udp_data_sent_messages_;
      this->last_progress_time_ = time_now ();
      this->done_.broadcast ();
    }

    void note_read (size_t bytes,
                    bool count_as_data_message)
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      ++this->total_read_ops_;
      this->total_received_bytes_ += bytes;
      if (count_as_data_message)
        ++this->udp_data_received_messages_;
      this->last_progress_time_ = time_now ();
      this->done_.broadcast ();
    }

    void note_error (const ACE_TCHAR *where,
                     size_t session_id,
                     const ACE_TCHAR *role,
                     u_long error = 0)
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      if (!this->failed_)
        {
          this->failed_ = true;
          this->first_error_code_ = error;
          ACE_OS::snprintf (this->first_error_,
                            sizeof (this->first_error_) / sizeof (this->first_error_[0]),
                            ACE_TEXT ("%s session=%lu side=%s"),
                            where,
                            static_cast<unsigned long> (session_id),
                            role);
        }
      this->end_time_ = time_now ();
      this->done_.broadcast ();
    }

    void endpoint_done ()
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      ++this->completed_endpoints_;
      this->last_progress_time_ = time_now ();
      if (this->completed_endpoints_ >= this->expected_endpoints_)
        this->end_time_ = this->last_progress_time_;
      this->done_.broadcast ();
    }

    void note_udp_socket_buffers (size_t actual_receive_size,
                                  size_t actual_send_size)
    {
      ACE_GUARD (ACE_Thread_Mutex, guard, this->lock_);
      if (this->udp_socket_buffers_recorded_)
        return;
      this->udp_socket_buffers_recorded_ = true;
      this->udp_receive_buffer_actual_ = actual_receive_size;
      this->udp_send_buffer_actual_ = actual_send_size;
    }

    bool wait_for_completion (void)
    {
      const ACE_Time_Value absolute_deadline =
        time_now () + ACE_Time_Value (this->config_.overall_timeout);

      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, this->lock_, false);

      while (!this->failed_ && this->completed_endpoints_ < this->expected_endpoints_)
        {
          const ACE_Time_Value now = time_now ();
          if (now >= absolute_deadline)
            {
              this->failed_ = true;
              this->timed_out_ = true;
              this->end_time_ = now;
              break;
            }

          const ACE_Time_Value progress_deadline =
            this->last_progress_time_ + ACE_Time_Value (this->config_.progress_timeout);
          if (now >= progress_deadline)
            {
              this->end_time_ = now;
              this->failed_ = true;
              this->stalled_ = true;
              break;
            }

          ACE_Time_Value wake = absolute_deadline < progress_deadline
            ? absolute_deadline
            : progress_deadline;
          if (this->done_.wait (&wake) == -1 && errno != ETIME)
            {
              this->failed_ = true;
              this->end_time_ = time_now ();
              break;
            }
        }

      if (!this->failed_ && this->completed_endpoints_ >= this->expected_endpoints_)
        this->end_time_ = time_now ();

      return !this->failed_;
    }

    bool validate (void) const
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, const_cast<ACE_Thread_Mutex &> (this->lock_), false);
      return this->validate_i ();
    }

    int report (void) const
    {
      ACE_GUARD_RETURN (ACE_Thread_Mutex, guard, const_cast<ACE_Thread_Mutex &> (this->lock_), -1);

      const double elapsed = seconds_between (this->start_time_, this->end_time_);
      const double safe_elapsed = elapsed > 0.0 ? elapsed : 0.000001;
      const double send_mib = static_cast<double> (this->total_sent_bytes_) / (1024.0 * 1024.0);
      const double recv_mib = static_cast<double> (this->total_received_bytes_) / (1024.0 * 1024.0);
      const double send_mib_per_sec = send_mib / safe_elapsed;
      const double recv_mib_per_sec = recv_mib / safe_elapsed;
      const double completions_per_sec =
        static_cast<double> (this->total_write_ops_ + this->total_read_ops_) / safe_elapsed;
      const double avg_write_us =
        this->total_write_ops_ == 0
        ? 0.0
        : (safe_elapsed * 1000000.0) / static_cast<double> (this->total_write_ops_);
      const double avg_read_us =
        this->total_read_ops_ == 0
        ? 0.0
        : (safe_elapsed * 1000000.0) / static_cast<double> (this->total_read_ops_);
      const double avg_completion_us =
        (this->total_write_ops_ + this->total_read_ops_) == 0
        ? 0.0
        : (safe_elapsed * 1000000.0)
          / static_cast<double> (this->total_write_ops_ + this->total_read_ops_);
      size_t lost_messages = 0;
      double loss_pct = 0.0;

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Benchmark config: backend=%s transport=%s family=%s sessions=%B ")
                  ACE_TEXT ("messages=%B payload=%B depth=%B threads=%d elapsed=%.3f sec\n"),
                  Proactor_Test_Backend::name (this->config_.backend),
                  transport_name (this->config_.transport),
                  family_name (this->config_.family),
                  this->config_.sessions,
                  this->config_.messages_per_endpoint,
                  this->config_.payload_size,
                  this->config_.write_depth,
                  this->config_.thread_count,
                  elapsed));

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Totals: write_ops=%B read_ops=%B sent=%B bytes recv=%B bytes ")
                  ACE_TEXT ("send=%.2f MiB/s recv=%.2f MiB/s completions=%.2f/s\n"),
                  this->total_write_ops_,
                  this->total_read_ops_,
                  this->total_sent_bytes_,
                  this->total_received_bytes_,
                  send_mib_per_sec,
                  recv_mib_per_sec,
                  completions_per_sec));

      if (this->config_.transport == TRANSPORT_UDP)
        {
          const size_t expected_data_messages =
            this->config_.sessions * this->config_.messages_per_endpoint;
          lost_messages =
            expected_data_messages > this->udp_data_received_messages_
            ? expected_data_messages - this->udp_data_received_messages_
            : 0;
          loss_pct = expected_data_messages == 0
            ? 0.0
            : (100.0 * static_cast<double> (lost_messages)
               / static_cast<double> (expected_data_messages));

          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("UDP delivery: data_sent=%B data_recv=%B lost=%B (%.2f%%)\n"),
                      this->udp_data_sent_messages_,
                      this->udp_data_received_messages_,
                      lost_messages,
                      loss_pct));
          ACE_DEBUG ((LM_INFO,
                      ACE_TEXT ("UDP socket buffers: req_rcv=%B req_snd=%B actual_rcv=%B actual_snd=%B\n"),
                      this->config_.udp_receive_buffer,
                      this->config_.udp_send_buffer,
                      this->udp_receive_buffer_actual_,
                      this->udp_send_buffer_actual_));
        }

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("PERF_RESULT benchmark=network backend=%s transport=%s family=%s ")
                  ACE_TEXT ("sessions=%B messages=%B payload=%B depth=%B threads=%d ")
                  ACE_TEXT ("elapsed_sec=%.6f send_mib_per_sec=%.6f recv_mib_per_sec=%.6f ")
                  ACE_TEXT ("completions_per_sec=%.6f avg_write_us=%.3f avg_read_us=%.3f ")
                  ACE_TEXT ("avg_completion_us=%.3f write_ops=%B read_ops=%B sent_bytes=%B ")
                  ACE_TEXT ("recv_bytes=%B udp_loss_pct=%.6f udp_lost=%B ")
                  ACE_TEXT ("udp_rcvbuf_req=%B udp_sndbuf_req=%B ")
                  ACE_TEXT ("udp_rcvbuf_actual=%B udp_sndbuf_actual=%B\n"),
                  Proactor_Test_Backend::name (this->config_.backend),
                  transport_name (this->config_.transport),
                  family_name (this->config_.family),
                  this->config_.sessions,
                  this->config_.messages_per_endpoint,
                  this->config_.payload_size,
                  this->config_.write_depth,
                  this->config_.thread_count,
                  elapsed,
                  send_mib_per_sec,
                  recv_mib_per_sec,
                  completions_per_sec,
                  avg_write_us,
                  avg_read_us,
                  avg_completion_us,
                  this->total_write_ops_,
                  this->total_read_ops_,
                  this->total_sent_bytes_,
                  this->total_received_bytes_,
                  loss_pct,
                  lost_messages,
                  this->config_.udp_receive_buffer,
                  this->config_.udp_send_buffer,
                  this->udp_receive_buffer_actual_,
                  this->udp_send_buffer_actual_));

      if (this->failed_)
        {
          if (this->first_error_[0] != 0)
            {
              if (this->first_error_code_ != 0)
                {
                  ACE_LOG_MSG->errnum (static_cast<int> (this->first_error_code_));
                  ACE_LOG_MSG->log (LM_ERROR,
                                    ACE_TEXT ("Benchmark failed: %s: %p\n"),
                                    this->first_error_,
                                    ACE_TEXT ("error"));
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_TEXT ("Benchmark failed: %s\n"),
                              this->first_error_));
                }
            }
          else if (this->stalled_)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Benchmark failed: no progress for %d seconds\n"),
                          this->config_.progress_timeout));
            }
          else if (this->timed_out_)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Benchmark failed: timed out after %d seconds\n"),
                          this->config_.overall_timeout));
            }
          else
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Benchmark failed\n")));
            }
          return -1;
        }

      if (!this->validate_i ())
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Benchmark completed but validation failed\n")));
          return -1;
        }

      return 0;
    }

  private:
    bool validate_i (void) const
    {
      if (this->failed_)
        return false;

      const size_t expected_data_messages =
        this->config_.transport == TRANSPORT_UDP
        ? this->config_.sessions * this->config_.messages_per_endpoint
        : this->config_.sessions * this->config_.messages_per_endpoint * 2;
      const size_t expected_data_bytes = expected_data_messages * this->config_.payload_size;

      if (this->config_.transport == TRANSPORT_TCP)
        return this->total_sent_bytes_ == expected_data_bytes
          && this->total_received_bytes_ == expected_data_bytes;

      return this->total_sent_bytes_ >= expected_data_bytes
        && this->udp_data_sent_messages_ >= expected_data_messages
        && this->udp_data_received_messages_ > 0
        && this->total_received_bytes_ > 0;
    }

    Config config_;
    mutable ACE_Thread_Mutex lock_;
    ACE_Condition_Thread_Mutex done_;
    ACE_Time_Value start_time_;
    ACE_Time_Value last_progress_time_;
    ACE_Time_Value end_time_;
    size_t expected_endpoints_;
    size_t completed_endpoints_;
    bool failed_;
    bool stalled_;
    bool timed_out_;
    u_long first_error_code_;
    ACE_TCHAR first_error_[256];
    size_t total_write_ops_;
    size_t total_read_ops_;
    size_t total_sent_bytes_;
    size_t total_received_bytes_;
    size_t udp_data_sent_messages_;
    size_t udp_data_received_messages_;
    bool udp_socket_buffers_recorded_;
    size_t udp_receive_buffer_actual_;
    size_t udp_send_buffer_actual_;
  };

  class Proactor_Task : public ACE_Task<ACE_MT_SYNCH>
  {
  public:
    Proactor_Task ()
      : proactor_ (0)
    {
    }

    ~Proactor_Task ()
    {
      this->stop ();
    }

    int init_backend (const Config &config)
    {
      if (this->proactor_ != 0)
        return -1;

      size_t max_ops = config.max_aio_operations;
      if (max_ops == 0)
        {
          const size_t read_ops_per_session =
            config.transport == TRANSPORT_TCP ? 2 : 1;
          const size_t write_ops_per_session =
            config.transport == TRANSPORT_TCP ? config.write_depth * 2
                                              : config.write_depth;

          // The benchmark seeds all initial endpoints before the proactor
          // threads start, so the implicit slot budget must cover that
          // startup burst rather than only the steady-state write depth.
          max_ops =
            config.sessions * (read_ops_per_session + write_ops_per_session) + 32;
          if (max_ops < 256)
            max_ops = 256;
        }

      if (Proactor_Test_Backend::create_proactor (config.backend,
                                                  max_ops,
                                                  this->proactor_,
                                                  true) != 0)
        return -1;

      return 0;
    }

    int start_threads (int thread_count)
    {
      if (this->proactor_ == 0)
        return -1;
      return this->activate (THR_NEW_LWP | THR_JOINABLE, thread_count);
    }

    int stop (void)
    {
      ACE_Proactor *proactor = this->proactor_;
      if (proactor == 0)
        return 0;

      proactor->proactor_end_event_loop ();
      this->wait ();
#if defined (ACE_WIN32)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Skipping ACE_Proactor::close_singleton() on Windows test shutdown\n")));
#else
      ACE_Proactor::close_singleton ();
#endif
      this->proactor_ = 0;
      return 0;
    }

    virtual int svc (void)
    {
      return ACE_Proactor::instance ()->proactor_run_event_loop ();
    }

  private:
    ACE_Proactor *proactor_;
  };

  class Endpoint_Base : public ACE_Handler
  {
  public:
    Endpoint_Base (Benchmark_State &state,
                   const Config &config,
                   size_t session_id,
                   const ACE_TCHAR *role)
      : state_ (state)
      , config_ (config)
      , session_id_ (session_id)
      , role_ (role)
      , lock_ ()
      , handle_ (ACE_INVALID_HANDLE)
      , failed_ (false)
      , completed_ (false)
    {
    }

    virtual ~Endpoint_Base (void)
    {
      this->close_handle ();
    }

  protected:
    void close_handle (void)
    {
      if (this->handle_ != ACE_INVALID_HANDLE)
        {
          ACE_OS::closesocket (this->handle_);
          this->handle_ = ACE_INVALID_HANDLE;
        }
    }

    void report_error (const ACE_TCHAR *where,
                       u_long error = 0)
    {
      if (!this->failed_)
        {
          this->failed_ = true;
          this->state_.note_error (where, this->session_id_, this->role_, error);
        }
    }

    Benchmark_State &state_;
    Config config_;
    size_t session_id_;
    const ACE_TCHAR *role_;
    mutable ACE_Recursive_Thread_Mutex lock_;
    ACE_HANDLE handle_;
    bool failed_;
    bool completed_;
  };

  class Stream_Endpoint : public Endpoint_Base
  {
  public:
    Stream_Endpoint (Benchmark_State &state,
                     const Config &config,
                     size_t session_id,
                     const ACE_TCHAR *role)
      : Endpoint_Base (state, config, session_id, role)
      , rs_ ()
      , ws_ ()
      , sends_remaining_ (config.messages_per_endpoint)
      , expected_receive_bytes_ (config.messages_per_endpoint * config.payload_size)
      , received_bytes_ (0)
      , writes_inflight_ (0)
      , read_inflight_ (false)
      , write_shutdown_ (false)
      , read_complete_ (false)
    {
    }

    void start (ACE_HANDLE handle)
    {
      ACE_GUARD (ACE_Recursive_Thread_Mutex, guard, this->lock_);
      this->handle_ = handle;
      this->set_nodelay ();

      if (this->ws_.open (*this, this->handle_) != 0)
        {
          this->report_error (ACE_TEXT ("stream write open"), ACE_OS::last_error ());
          delete this;
          return;
        }

      if (this->rs_.open (*this, this->handle_) != 0)
        {
          this->report_error (ACE_TEXT ("stream read open"), ACE_OS::last_error ());
          delete this;
          return;
        }

      if (this->issue_read () == -1)
        {
          delete this;
          return;
        }

      this->issue_writes ();
      if (this->maybe_finish ())
        delete this;
    }

    virtual void handle_read_stream (const ACE_Asynch_Read_Stream::Result &result)
    {
      bool delete_self = false;
      ACE_GUARD (ACE_Recursive_Thread_Mutex, guard, this->lock_);
      ACE_Message_Block &mb = result.message_block ();
      this->read_inflight_ = false;

      if (result.error () != 0)
        {
          mb.release ();
          if (is_retryable_socket_error (result.error ()))
            {
              (void) this->issue_read ();
            }
          else if (!is_cancel_error (result.error ()))
            this->report_error (ACE_TEXT ("stream read"), result.error ());
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      if (result.bytes_transferred () == 0)
        {
          mb.release ();
          if (this->received_bytes_ < this->expected_receive_bytes_)
            this->report_error (ACE_TEXT ("stream unexpected eof"));
          else
            this->read_complete_ = true;
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      this->received_bytes_ += result.bytes_transferred ();
      this->state_.note_read (result.bytes_transferred (), false);
      mb.release ();

      if (this->received_bytes_ >= this->expected_receive_bytes_)
        this->read_complete_ = true;
      else
        (void) this->issue_read ();

      delete_self = this->maybe_finish ();
      if (delete_self)
        {
          guard.release ();
          delete this;
        }
    }

    virtual void handle_write_stream (const ACE_Asynch_Write_Stream::Result &result)
    {
      bool delete_self = false;
      ACE_GUARD (ACE_Recursive_Thread_Mutex, guard, this->lock_);
      ACE_Message_Block &mb = result.message_block ();

      if (result.error () != 0)
        {
          if (is_retryable_socket_error (result.error ()))
            {
              if (this->ws_.write (mb, result.bytes_to_write ()) == 0)
                return;
            }
          mb.release ();
          if (!is_cancel_error (result.error ()))
            this->report_error (ACE_TEXT ("stream write"), result.error ());
          if (this->writes_inflight_ > 0)
            --this->writes_inflight_;
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      if (result.bytes_transferred () < result.bytes_to_write ())
        {
          this->state_.note_write (result.bytes_transferred (), false);
          if (this->ws_.write (mb,
                               result.bytes_to_write () - result.bytes_transferred ()) != 0)
            {
              mb.release ();
              this->report_error (ACE_TEXT ("stream partial rewrite"), ACE_OS::last_error ());
              if (this->writes_inflight_ > 0)
                --this->writes_inflight_;
            }
          return;
        }

      this->state_.note_write (result.bytes_transferred (), false);
      mb.release ();
      if (this->writes_inflight_ > 0)
        --this->writes_inflight_;

      this->issue_writes ();
      delete_self = this->maybe_finish ();
      if (delete_self)
        {
          guard.release ();
          delete this;
        }
    }

  private:
    void set_nodelay (void)
    {
      ACE_SOCK_Stream stream;
      stream.set_handle (this->handle_);
      int nodelay = 1;
      (void) stream.set_option (ACE_IPPROTO_TCP,
                                TCP_NODELAY,
                                &nodelay,
                                sizeof (nodelay));
      stream.set_handle (ACE_INVALID_HANDLE);
    }

    int issue_read (void)
    {
      if (this->failed_ || this->completed_ || this->read_inflight_)
        return -1;

      const size_t read_size =
        this->config_.payload_size * (this->config_.write_depth > 0 ? this->config_.write_depth : 1);
      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb, ACE_Message_Block (read_size), -1);

      if (this->rs_.read (*mb, mb->space ()) == -1)
        {
          mb->release ();
          this->report_error (ACE_TEXT ("stream read issue"), ACE_OS::last_error ());
          return -1;
        }

      this->read_inflight_ = true;
      return 0;
    }

    void issue_writes (void)
    {
      while (!this->failed_
             && !this->completed_
             && this->sends_remaining_ > 0
             && this->writes_inflight_ < this->config_.write_depth)
        {
          ACE_Message_Block *mb = 0;
          ACE_NEW (mb, ACE_Message_Block (this->config_.payload_size));
          if (mb == 0)
            {
              this->report_error (ACE_TEXT ("stream message_block alloc"));
              return;
            }

          ACE_OS::memset (mb->wr_ptr (),
                          static_cast<int> ('A' + (this->session_id_ % 26)),
                          this->config_.payload_size);
          mb->wr_ptr (this->config_.payload_size);

          if (this->ws_.write (*mb, this->config_.payload_size) == -1)
            {
              mb->release ();
              this->report_error (ACE_TEXT ("stream write issue"), ACE_OS::last_error ());
              return;
            }

          --this->sends_remaining_;
          ++this->writes_inflight_;
        }
    }

    bool maybe_finish (void)
    {
      if (this->completed_)
        return false;

      if (!this->write_shutdown_
          && this->sends_remaining_ == 0
          && this->writes_inflight_ == 0
          && this->handle_ != ACE_INVALID_HANDLE)
        {
          ACE_OS::shutdown (this->handle_, ACE_SHUTDOWN_WRITE);
          this->write_shutdown_ = true;
        }

      if (this->failed_)
        {
          if (this->writes_inflight_ == 0 && !this->read_inflight_)
            {
              this->completed_ = true;
              this->state_.endpoint_done ();
              return true;
            }
          return false;
        }

      if (this->write_shutdown_
          && this->read_complete_
          && this->writes_inflight_ == 0
          && !this->read_inflight_)
        {
          this->completed_ = true;
          this->state_.endpoint_done ();
          return true;
        }
      return false;
    }

    ACE_Asynch_Read_Stream rs_;
    ACE_Asynch_Write_Stream ws_;
    size_t sends_remaining_;
    size_t expected_receive_bytes_;
    size_t received_bytes_;
    size_t writes_inflight_;
    bool read_inflight_;
    bool write_shutdown_;
    bool read_complete_;
  };

  class Datagram_Endpoint : public Endpoint_Base
  {
  public:
    Datagram_Endpoint (Benchmark_State &state,
                       const Config &config,
                       size_t session_id,
                       const ACE_TCHAR *role,
                       const ACE_INET_Addr &peer_addr,
                       bool send_enabled,
                       bool receive_enabled)
      : Endpoint_Base (state, config, session_id, role)
      , rs_ ()
      , ws_ ()
      , peer_addr_ (peer_addr)
      , send_enabled_ (send_enabled)
      , receive_enabled_ (receive_enabled)
      , sends_remaining_ (config.messages_per_endpoint)
      , end_markers_remaining_ (config.udp_end_markers)
      , writes_inflight_ (0)
      , read_inflight_ (false)
      , peer_end_seen_ (false)
      , sends_complete_ (false)
    {
    }

    void start (ACE_HANDLE handle)
    {
      ACE_GUARD (ACE_Recursive_Thread_Mutex, guard, this->lock_);
      this->handle_ = handle;

      if (this->send_enabled_ && this->ws_.open (*this, this->handle_) != 0)
        {
          this->report_error (ACE_TEXT ("dgram write open"), ACE_OS::last_error ());
          delete this;
          return;
        }

      if (this->receive_enabled_ && this->rs_.open (*this, this->handle_) != 0)
        {
          this->report_error (ACE_TEXT ("dgram read open"), ACE_OS::last_error ());
          delete this;
          return;
        }

      if (this->receive_enabled_ && this->issue_read () == -1)
        {
          delete this;
          return;
        }

      if (this->send_enabled_)
        this->issue_writes ();
      else
        {
          this->sends_remaining_ = 0;
          this->end_markers_remaining_ = 0;
          this->sends_complete_ = true;
        }

      if (this->maybe_finish ())
        delete this;
    }

    virtual void handle_read_dgram (const ACE_Asynch_Read_Dgram::Result &result)
    {
      bool delete_self = false;
      ACE_GUARD (ACE_Recursive_Thread_Mutex, guard, this->lock_);
      ACE_Message_Block *mb = result.message_block ();
      this->read_inflight_ = false;

      if (result.error () != 0)
        {
          mb->release ();
          if (is_retryable_socket_error (result.error ()))
            {
              (void) this->issue_read ();
            }
          else if (!is_cancel_error (result.error ()))
            this->report_error (ACE_TEXT ("dgram read"), result.error ());
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      if (result.bytes_transferred () < sizeof (Udp_Header))
        {
          mb->release ();
          this->report_error (ACE_TEXT ("dgram short header"));
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      const Udp_Header *header =
        reinterpret_cast<const Udp_Header *> (mb->rd_ptr ());

      if (header->magic_ != UDP_MAGIC || header->session_ != this->session_id_)
        {
          mb->release ();
          this->report_error (ACE_TEXT ("dgram invalid header"));
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      if (header->type_ == UDP_DATA)
        this->state_.note_read (result.bytes_transferred (), true);
      else if (header->type_ == UDP_END)
        {
          this->peer_end_seen_ = true;
          this->state_.note_read (result.bytes_transferred (), false);
        }
      else
        {
          mb->release ();
          this->report_error (ACE_TEXT ("dgram invalid type"));
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      mb->release ();

      if (this->receive_enabled_ && !this->peer_end_seen_)
        (void) this->issue_read ();

      delete_self = this->maybe_finish ();
      if (delete_self)
        {
          guard.release ();
          delete this;
        }
    }

    virtual void handle_write_dgram (const ACE_Asynch_Write_Dgram::Result &result)
    {
      bool delete_self = false;
      ACE_GUARD (ACE_Recursive_Thread_Mutex, guard, this->lock_);
      ACE_Message_Block *mb = result.message_block ();
      const bool data_message = mb->msg_type () == ACE_Message_Block::MB_DATA;

      if (result.error () != 0)
        {
          if (is_retryable_socket_error (result.error ()))
            {
              size_t bytes_to_send = result.bytes_to_write ();
              if (this->ws_.send (mb,
                                  bytes_to_send,
                                  0,
                                  this->peer_addr_) == 0)
                return;
            }
          const bool ignore_peer_closed =
            !data_message && is_udp_peer_closed_error (result.error ());
          mb->release ();
          if (ignore_peer_closed)
            {
              this->end_markers_remaining_ = 0;
              this->sends_complete_ = true;
            }
          else if (!is_cancel_error (result.error ()))
            this->report_error (ACE_TEXT ("dgram write"), result.error ());
          if (this->writes_inflight_ > 0)
            --this->writes_inflight_;
          delete_self = this->maybe_finish ();
          if (delete_self)
            {
              guard.release ();
              delete this;
            }
          return;
        }

      this->state_.note_write (result.bytes_transferred (), data_message);
      mb->release ();

      if (this->writes_inflight_ > 0)
        --this->writes_inflight_;

      this->issue_writes ();
      delete_self = this->maybe_finish ();
      if (delete_self)
        {
          guard.release ();
          delete this;
        }
    }

  private:
    int issue_read (void)
    {
      if (this->failed_ || this->completed_ || this->read_inflight_)
        return -1;

      ACE_Message_Block *mb = 0;
      ACE_NEW_RETURN (mb, ACE_Message_Block (this->config_.payload_size), -1);
      size_t bytes_received = 0;
      if (this->rs_.recv (mb,
                          bytes_received,
                          0,
                          this->config_.family) == -1)
        {
          mb->release ();
          this->report_error (ACE_TEXT ("dgram recv issue"), ACE_OS::last_error ());
          return -1;
        }

      this->read_inflight_ = true;
      return 0;
    }

    void issue_writes (void)
    {
      while (this->send_enabled_
             && !this->failed_
             && !this->completed_
             && this->writes_inflight_ < this->config_.write_depth)
        {
          ACE_UINT32 type = 0;
          if (this->sends_remaining_ > 0)
            {
              type = UDP_DATA;
              --this->sends_remaining_;
            }
          else if (this->end_markers_remaining_ > 0)
            {
              type = UDP_END;
              --this->end_markers_remaining_;
            }
          else
            {
              this->sends_complete_ = true;
              return;
            }

          ACE_Message_Block *mb = 0;
          ACE_NEW (mb, ACE_Message_Block (this->config_.payload_size));
          if (mb == 0)
            {
              this->report_error (ACE_TEXT ("dgram message_block alloc"));
              return;
            }

          ACE_OS::memset (mb->wr_ptr (), 0, this->config_.payload_size);
          Udp_Header *header = reinterpret_cast<Udp_Header *> (mb->wr_ptr ());
          header->magic_ = UDP_MAGIC;
          header->type_ = type;
          header->session_ = static_cast<ACE_UINT32> (this->session_id_);
          header->reserved_ = 0;
          mb->msg_type (type == UDP_DATA
                        ? ACE_Message_Block::MB_DATA
                        : ACE_Message_Block::MB_PROTO);

          if (type == UDP_DATA && this->config_.payload_size > sizeof (Udp_Header))
            ACE_OS::memset (mb->wr_ptr () + sizeof (Udp_Header),
                            static_cast<int> ('a' + (this->session_id_ % 26)),
                            this->config_.payload_size - sizeof (Udp_Header));

          mb->wr_ptr (this->config_.payload_size);

          if (this->ws_.send (mb, this->config_.payload_size, 0, this->peer_addr_) == -1)
            {
              mb->release ();
              this->report_error (ACE_TEXT ("dgram send issue"), ACE_OS::last_error ());
              return;
            }

          ++this->writes_inflight_;
        }
    }

    bool maybe_finish (void)
    {
      if (this->completed_)
        return false;

      if (this->failed_)
        {
          if (this->writes_inflight_ == 0 && !this->read_inflight_)
            {
              this->completed_ = true;
              this->state_.endpoint_done ();
              return true;
            }
          return false;
        }

      if (this->sends_remaining_ == 0 && this->end_markers_remaining_ == 0)
        this->sends_complete_ = true;

      const bool writes_done =
        !this->send_enabled_ || (this->sends_complete_ && this->writes_inflight_ == 0);
      const bool reads_done =
        !this->receive_enabled_ || (this->peer_end_seen_ && !this->read_inflight_);

      if (writes_done && reads_done)
        {
          this->completed_ = true;
          this->state_.endpoint_done ();
          return true;
        }
      return false;
    }

    ACE_Asynch_Read_Dgram rs_;
    ACE_Asynch_Write_Dgram ws_;
    ACE_INET_Addr peer_addr_;
    bool send_enabled_;
    bool receive_enabled_;
    size_t sends_remaining_;
    size_t end_markers_remaining_;
    size_t writes_inflight_;
    bool read_inflight_;
    bool peer_end_seen_;
    bool sends_complete_;
  };

  int
  print_usage (ACE_TCHAR *argv0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Usage: %s [-t <backend>] [-u] [-4|-6] ")
                ACE_TEXT ("[-n sessions] [-m messages] [-b payload] ")
                ACE_TEXT ("[-w write_depth] [-T threads] [-a max_aio_ops] [-p port] ")
                ACE_TEXT ("[-R udp_rcvbuf] [-S udp_sndbuf]\n"),
                argv0));
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("  default transport is tcp, default family is ipv4\n")));
    return Proactor_Test_Backend::print_type_usage (argv0);
  }

  int
  parse_args (int argc,
              ACE_TCHAR *argv[],
              Config &config)
  {
    ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("46uR:S:a:b:m:n:p:t:T:w:"));
    int c = 0;

    while ((c = get_opt ()) != EOF)
      {
        switch (c)
          {
          case '4':
            config.family = AF_INET;
            break;
          case '6':
            config.family = AF_INET6;
            break;
          case 'u':
            config.transport = TRANSPORT_UDP;
            break;
          case 'R':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.udp_receive_buffer) != 0)
              return -1;
            break;
          case 'S':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.udp_send_buffer) != 0)
              return -1;
            break;
          case 'a':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.max_aio_operations) != 0)
              return -1;
            break;
          case 'b':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.payload_size) != 0)
              return -1;
            break;
          case 'm':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.messages_per_endpoint) != 0)
              return -1;
            break;
          case 'n':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.sessions) != 0)
              return -1;
            break;
          case 'p':
            if (parse_u_short_arg (get_opt.opt_arg (),
                                   config.listen_port) != 0)
              return -1;
            break;
          case 't':
            if (Proactor_Test_Backend::parse_type (get_opt.opt_arg (), config.backend) != 0
                || !Proactor_Test_Backend::is_available (config.backend))
              return -1;
            break;
          case 'T':
            if (parse_int_arg (get_opt.opt_arg (),
                               config.thread_count) != 0)
              return -1;
            break;
          case 'w':
            if (parse_size_arg (get_opt.opt_arg (),
                                config.write_depth) != 0)
              return -1;
            break;
          default:
            return -1;
          }
      }

    if (config.sessions == 0
        || config.messages_per_endpoint == 0
        || config.payload_size == 0
        || config.write_depth == 0
        || config.thread_count <= 0)
      return -1;

    if (config.transport == TRANSPORT_UDP && config.payload_size < sizeof (Udp_Header))
      return -1;

    if (config.transport != TRANSPORT_UDP
        && (config.udp_receive_buffer != 0 || config.udp_send_buffer != 0))
      return -1;

    return 0;
  }

  int
  parse_unsigned_long_arg (const ACE_TCHAR *text,
                           unsigned long &value)
  {
    if (text == 0 || *text == 0)
      return -1;

    for (const ACE_TCHAR *pos = text; *pos != 0; ++pos)
      if (*pos < static_cast<ACE_TCHAR> ('0')
          || *pos > static_cast<ACE_TCHAR> ('9'))
        return -1;

    ACE_TCHAR *end = 0;
    errno = 0;
    const unsigned long parsed = ACE_OS::strtoul (text, &end, 10);
    if (errno != 0 || end == text || (end != 0 && *end != 0))
      return -1;

    value = parsed;
    return 0;
  }

  int
  parse_size_arg (const ACE_TCHAR *text,
                  size_t &value)
  {
    unsigned long parsed = 0;
    if (parse_unsigned_long_arg (text, parsed) != 0)
      return -1;

    size_t const converted = static_cast<size_t> (parsed);
    if (static_cast<unsigned long> (converted) != parsed)
      return -1;

    value = converted;
    return 0;
  }

  int
  parse_u_short_arg (const ACE_TCHAR *text,
                     u_short &value)
  {
    unsigned long parsed = 0;
    if (parse_unsigned_long_arg (text, parsed) != 0
        || parsed > USHRT_MAX)
      return -1;

    value = static_cast<u_short> (parsed);
    return 0;
  }

  int
  parse_int_arg (const ACE_TCHAR *text,
                 int &value)
  {
    unsigned long parsed = 0;
    if (parse_unsigned_long_arg (text, parsed) != 0
        || parsed > INT_MAX)
      return -1;

    value = static_cast<int> (parsed);
    return 0;
  }

  ACE_INET_Addr
  loopback_addr (u_short port,
                 int family)
  {
    const ACE_TCHAR *host =
      family == AF_INET6 ? ACE_TEXT ("::1") : ACE_TEXT ("127.0.0.1");
    return ACE_INET_Addr (port, host, family);
  }

  int
  run_tcp_benchmark (const Config &config)
  {
    Proactor_Task task;
    if (task.init_backend (config) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to start proactor backend %s\n"),
                         Proactor_Test_Backend::name (config.backend)),
                        -1);

    Benchmark_State state (config, config.sessions * 2);
    ACE_SOCK_Acceptor acceptor;
    ACE_INET_Addr listen_addr = loopback_addr (config.listen_port, config.family);
    if (acceptor.open (listen_addr, 1) != 0)
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("acceptor.open")),
                          -1);
      }

    if (acceptor.get_local_addr (listen_addr) != 0)
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("acceptor.get_local_addr")),
                          -1);
      }

    for (size_t index = 0; index < config.sessions; ++index)
      {
        ACE_SOCK_Connector connector;
        ACE_SOCK_Stream client_stream;
        ACE_SOCK_Stream server_stream;

        if (connector.connect (client_stream, listen_addr) != 0)
          {
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("connector.connect")),
                              -1);
          }

        if (acceptor.accept (server_stream) != 0)
          {
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("acceptor.accept")),
                              -1);
          }

        const ACE_HANDLE client_handle = client_stream.get_handle ();
        const ACE_HANDLE server_handle = server_stream.get_handle ();
        client_stream.set_handle (ACE_INVALID_HANDLE);
        server_stream.set_handle (ACE_INVALID_HANDLE);

        if (prepare_socket_handle (client_handle) != 0
            || prepare_socket_handle (server_handle) != 0)
          {
            ACE_OS::closesocket (client_handle);
            ACE_OS::closesocket (server_handle);
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("set_flags")),
                              -1);
          }

        Stream_Endpoint *client = 0;
        Stream_Endpoint *server = 0;
        ACE_NEW_RETURN (client,
                        Stream_Endpoint (state, config, index, ACE_TEXT ("client")),
                        -1);
        ACE_NEW_RETURN (server,
                        Stream_Endpoint (state, config, index, ACE_TEXT ("server")),
                        -1);
        client->start (client_handle);
        server->start (server_handle);
      }

    if (task.start_threads (config.thread_count) != 0)
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("activate")),
                          -1);
      }

    const bool completed = state.wait_for_completion ();
    task.stop ();
    return completed ? state.report () : state.report ();
  }

  int
  prepare_socket_handle (ACE_HANDLE handle)
  {
    return ACE::set_flags (handle, ACE_NONBLOCK);
  }

  int
  configure_udp_socket_buffer (ACE_HANDLE handle,
                               int option_name,
                               size_t requested_size,
                               size_t &actual_size)
  {
    actual_size = 0;

#if defined (ACE_LACKS_SO_RCVBUF)
    if (option_name == SO_RCVBUF)
      {
        errno = ENOTSUP;
        return -1;
      }
#endif /* ACE_LACKS_SO_RCVBUF */

#if defined (ACE_LACKS_SO_SNDBUF)
    if (option_name == SO_SNDBUF)
      {
        errno = ENOTSUP;
        return -1;
      }
#endif /* ACE_LACKS_SO_SNDBUF */

    if (requested_size > 0)
      {
        if (requested_size > static_cast<size_t> (INT_MAX))
          {
            errno = EINVAL;
            return -1;
          }

        const int requested = static_cast<int> (requested_size);
        if (ACE_OS::setsockopt (handle,
                                SOL_SOCKET,
                                option_name,
                                reinterpret_cast<const char *> (&requested),
                                sizeof (requested)) != 0)
          return -1;
      }

    int actual = 0;
    int actual_length = static_cast<int> (sizeof (actual));
    if (ACE_OS::getsockopt (handle,
                            SOL_SOCKET,
                            option_name,
                            reinterpret_cast<char *> (&actual),
                            &actual_length) != 0)
      return -1;

    actual_size = actual > 0 ? static_cast<size_t> (actual) : 0;
    return 0;
  }

  int
  configure_udp_socket_buffers (ACE_HANDLE handle,
                                const Config &config,
                                size_t &actual_receive_size,
                                size_t &actual_send_size)
  {
    if (configure_udp_socket_buffer (handle,
                                     SO_RCVBUF,
                                     config.udp_receive_buffer,
                                     actual_receive_size) != 0)
      return -1;

    if (configure_udp_socket_buffer (handle,
                                     SO_SNDBUF,
                                     config.udp_send_buffer,
                                     actual_send_size) != 0)
      return -1;

    return 0;
  }

  int
  run_udp_benchmark (const Config &config)
  {
    Proactor_Task task;
    if (task.init_backend (config) != 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Failed to start proactor backend %s\n"),
                         Proactor_Test_Backend::name (config.backend)),
                        -1);

    Benchmark_State state (config, config.sessions * 2);

    for (size_t index = 0; index < config.sessions; ++index)
      {
        ACE_INET_Addr server_addr = loopback_addr (0, config.family);
        ACE_INET_Addr client_addr = loopback_addr (0, config.family);
        ACE_HANDLE server_handle = ACE_INVALID_HANDLE;
        ACE_HANDLE client_handle = ACE_INVALID_HANDLE;

        ACE_SOCK_Dgram server_socket;
#if defined (ACE_WIN32)
        ACE_SOCK_Dgram client_socket;
#else
        ACE_SOCK_CODgram client_socket;
#endif /* ACE_WIN32 */

        if (server_socket.open (server_addr, config.family) != 0)
          {
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("server_socket.open")),
                              -1);
          }

        if (server_socket.get_local_addr (server_addr) != 0)
          {
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("server_socket.get_local_addr")),
                              -1);
          }

        // Win32 async datagram sends use the per-operation peer address.
        // POSIX expects the connected datagram socket behavior used
        // originally by this benchmark.
#if defined (ACE_WIN32)
        if (client_socket.open (client_addr, config.family) != 0)
#else
        if (client_socket.open (server_addr,
                                client_addr,
                                config.family) != 0)
#endif /* ACE_WIN32 */
          {
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("client_socket.open")),
                              -1);
          }

        if (client_socket.get_local_addr (client_addr) != 0)
          {
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("client_socket.get_local_addr")),
                              -1);
          }

        server_handle = server_socket.get_handle ();
        client_handle = client_socket.get_handle ();
        server_socket.set_handle (ACE_INVALID_HANDLE);
        client_socket.set_handle (ACE_INVALID_HANDLE);

        size_t server_receive_buffer = 0;
        size_t server_send_buffer = 0;
        size_t client_receive_buffer = 0;
        size_t client_send_buffer = 0;
        if (configure_udp_socket_buffers (server_handle,
                                          config,
                                          server_receive_buffer,
                                          server_send_buffer) != 0
            || configure_udp_socket_buffers (client_handle,
                                             config,
                                             client_receive_buffer,
                                             client_send_buffer) != 0
            || prepare_socket_handle (server_handle) != 0
            || prepare_socket_handle (client_handle) != 0)
          {
            ACE_OS::closesocket (server_handle);
            ACE_OS::closesocket (client_handle);
            task.stop ();
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("udp socket setup")),
                              -1);
          }

        state.note_udp_socket_buffers (server_receive_buffer,
                                       client_send_buffer);

        Datagram_Endpoint *client = 0;
        Datagram_Endpoint *server = 0;
        ACE_NEW_RETURN (client,
                        Datagram_Endpoint (state,
                                           config,
                                           index,
                                           ACE_TEXT ("client"),
                                           server_addr,
                                           true,
                                           false),
                        -1);
        ACE_NEW_RETURN (server,
                        Datagram_Endpoint (state,
                                           config,
                                           index,
                                           ACE_TEXT ("server"),
                                           client_addr,
                                           false,
                                           true),
                        -1);
        client->start (client_handle);
        server->start (server_handle);
      }

    if (task.start_threads (config.thread_count) != 0)
      {
        task.stop ();
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("activate")),
                          -1);
      }

    const bool completed = state.wait_for_completion ();
    task.stop ();
    return completed ? state.report () : state.report ();
  }
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Network_Performance_Test"));

  Config config;
  if (parse_args (argc, argv, config) != 0)
    {
      const int rc = print_usage (argv[0]);
      ACE_END_TEST;
      return rc;
    }

  if (config.family == AF_INET6 && !ACE::ipv6_enabled ())
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("IPv6 is not supported by ACE on this platform.\n")
                  ACE_TEXT ("Proactor_Network_Performance_Test (IPv6) will not be run.\n")));
      ACE_END_TEST;
      return 0;
    }

  int rc = 0;
  if (config.transport == TRANSPORT_TCP)
    rc = run_tcp_benchmark (config);
  else
    rc = run_udp_benchmark (config);

  ACE_END_TEST;
  return rc;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Proactor_Network_Performance_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Threaded Proactor networking is not supported on this platform\n")));
  ACE_END_TEST;
  return 0;
}

#endif /* ACE_HAS_THREADS && (ACE_HAS_WIN32_OVERLAPPED_IO || ACE_HAS_AIO_CALLS) */
