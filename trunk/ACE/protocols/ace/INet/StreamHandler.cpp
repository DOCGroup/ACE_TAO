// $Id$

#ifndef ACE_IOS_STREAM_HANDLER_CPP
#define ACE_IOS_STREAM_HANDLER_CPP

#include "StreamHandler.h"
#include "ace/OS_NS_Thread.h"
#include "ace/OS_NS_errno.h"
#include "ace/Countdown_Time.h"

ACE_RCSID(NET_CLIENT,ACE_IOS_StreamHandler,"$Id$")

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {
    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::StreamHandler (
        const ACE_Synch_Options &synch_options,
        ACE_Thread_Manager *thr_mgr,
        mq_type *mq,
        ACE_Reactor *reactor)
      : ACE_Svc_Handler<ACE_PEER_STREAM, ACE_SYNCH_USE> (thr_mgr, mq, reactor),
        connected_ (false),
        send_timeout_ (false),
        receive_timeout_ (false)
      {
        ACE_TRACE ("ACE_IOS_StreamHandler - ctor");

        unsigned long opt = synch_options[ACE_Synch_Options::USE_REACTOR] ?
                              ACE_Synch_Options::USE_REACTOR : 0;
        if (synch_options[ACE_Synch_Options::USE_TIMEOUT])
          opt |= ACE_Synch_Options::USE_TIMEOUT;
        this->sync_opt_.set (opt,
                             synch_options.timeout (),
                             synch_options.arg ());
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::~StreamHandler ()
      {
        ACE_TRACE ("ACE_IOS_StreamHandler - dtor");

        this->connected_ = false;
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::open (void * /*p*/)
      {
        this->connected_ = true;
        return 0;
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::close (u_long flags)
      {
        this->connected_ = false;
        return base_type::close (flags);
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::handle_input (ACE_HANDLE)
      {
        if (this->use_timeout ())
          {
            ACE_Time_Value to = this->sync_opt_.timeout ();
            return this->handle_input_i (&to);
          }
        else
          return this->handle_input_i (0);
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::handle_input_i (ACE_Time_Value* timeout)
      {
        ACE_TRACE ("ACE_IOS_StreamHandler::handle_input_i");

        char buffer[MAX_INPUT_SIZE];
        ssize_t recv_cnt;
        size_t bytes_in = 0;

        recv_cnt = this->peer ().recv_n (buffer, sizeof(buffer), timeout, &bytes_in);

        if (bytes_in > 0)
          {
            ACE_Message_Block *mb = 0;
            ACE_NEW_RETURN (mb, ACE_Message_Block (bytes_in), -1);
            mb->copy (buffer, bytes_in);
            ACE_Time_Value nowait (ACE_OS::gettimeofday ());
            if (this->putq (mb, &nowait) == -1)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) %p; ACE_IOS_StreamHandler - discarding input data, "),
                            ACE_TEXT ("enqueue failed\n")));
                mb->release ();
                this->connected_ = false;
                return -1;
              }
          }

        if (recv_cnt <= 0)
          {
            if (recv_cnt < 0)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) %p; ACE_IOS_StreamHandler - receive failed\n")));
              }
            this->connected_ = false;
            return this->reactive() ? -1 : 0;
          }
        return 0;
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::handle_output (ACE_HANDLE)
      {
        if (this->use_timeout ())
          {
            ACE_Time_Value to = this->sync_opt_.timeout ();
            return this->handle_output_i (&to);
          }
        else
          return this->handle_output_i (0);
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::handle_output_i (ACE_Time_Value* timeout)
      {
        ACE_TRACE ("ACE_IOS_StreamHandler::handle_output_i");

        ACE_Message_Block *mb = 0;
        ACE_Time_Value nowait (ACE_OS::gettimeofday ());
        size_t bytes_out = 0;
        if (-1 != this->getq (mb, &nowait))
          {
            ssize_t send_cnt =
              this->peer ().send_n (mb->rd_ptr (), mb->length (), timeout, &bytes_out);
            if (bytes_out > 0)
              {
                mb->rd_ptr (static_cast<size_t> (bytes_out));
                if (mb->length () > 0)
                    this->ungetq (mb);
                else
                    mb->release ();
              }
            if (send_cnt <= 0)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) %p; ACE_IOS_StreamHandler - "),
                            ACE_TEXT ("send failed\n")));
                this->connected_ = false;
                return this->reactive() ? -1 : 0;
              }
          }
        return (this->msg_queue ()->is_empty ()) ? -1 : 0;
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::read_from_stream (
        void * buf,
        size_t length,
        u_short char_size)
      {
        ACE_TRACE ("ACE_IOS_StreamHandler::read_from_stream");

        size_t recv_char_count = 0;
        char* wptr = (char*)buf;
        size_t char_length = length * char_size;
        ACE_Time_Value max_wait_time = this->sync_opt_.timeout ();
        int result = 0;
        if (this->reactive ())
          {
            ACE_thread_t tid;
            this->reactor ()->owner (&tid);
            bool reactor_thread =
              ACE_OS::thr_equal (ACE_Thread::self (), tid) ? true : false;

            if (this->connected_)
              {
                if (this->reactor ()->register_handler(this,
                                                      ACE_Event_Handler::READ_MASK) != 0)
                  {
                    return -1;
                  }
              }

            // run the event loop for the maximum allowed time to get the
            // message data in
            while ((this->connected_ || this->char_in_queue (char_size)) && char_length > 0)
              {
                result = 0;
                if (reactor_thread && !this->char_in_queue (char_size))
                  {
                    // Run the event loop.
                    result = this->reactor ()->handle_events (this->use_timeout () ?
                                                                  &max_wait_time : 0);
                  }

                if (result != -1)
                  {
                    result = this->process_input (&wptr[recv_char_count],
                                                  char_length,
                                                  char_size,
                                                  this->use_timeout () ?
                                                        &max_wait_time : 0);
                  }

                if (result == -1)
                  {
                    this->reactor ()->remove_handler (this,
                                                      ACE_Event_Handler::READ_MASK);
                    return -1;
                  }

                recv_char_count += result;

                if (recv_char_count > 0)
                  {
                    break;
                  }

                if (this->use_timeout () &&
                      max_wait_time == ACE_Time_Value::zero)
                  {
                    this->reactor ()->remove_handler (this,
                                                      ACE_Event_Handler::READ_MASK);
                    this->receive_timeout_ = true;
                    return -1;
                  }
              }

            this->reactor ()->remove_handler (this,
                                              ACE_Event_Handler::READ_MASK);
          }
        else
          {
            while ((this->connected_ || this->char_in_queue (char_size)) && char_length > 0)
              {
                if (!this->char_in_queue (char_size))
                  {
                    result = this->handle_input_i (this->use_timeout () ?
                                                        &max_wait_time : 0);
                  }

                if (result == -1)
                  return result;

                result = this->process_input (&wptr[recv_char_count],
                                              char_length,
                                              char_size,
                                              this->use_timeout () ?
                                                        &max_wait_time : 0);

                if (result == -1)
                  return result;

                recv_char_count += result;

                if (recv_char_count > 0)
                  {
                    break;
                  }

                if (this->use_timeout () &&
                      max_wait_time == ACE_Time_Value::zero)
                  {
                    this->receive_timeout_ = true;
                    return -1;
                  }
              }
          }

        return static_cast<int> (recv_char_count / char_size);
      }

    // This method makes sure to only ever copy full char_size elements
    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::process_input (
        char* buf,
        size_t& char_length,
        u_short char_size,
        ACE_Time_Value* timeout)
      {
        ACE_TRACE ("ACE_IOS_StreamHandler::process_input");

        ACE_Time_Value wait (ACE_OS::gettimeofday ());
        // keep track of how much time we use here
        ACE_Countdown_Time timeout_countdown (timeout);
        // if timeout specified add it to the abs waittime
        // otherwise it's a 'nowait'
        if (timeout)
          {
            wait += *timeout;
            timeout_countdown.start ();
          }
        ACE_Message_Block *mb_remain = 0;
        size_t recv_char_count = 0;
        while (!this->msg_queue ()->is_empty () && char_length > 0)
          {
            ACE_Message_Block *mb = 0;
            if (this->getq (mb, &wait) == -1)
              {
                if (ACE_OS::last_error () == EWOULDBLOCK)
                  break; // timeout; queue still empty
                else
                  return -1; // message queue shut down
              }

            size_t copy_len = 0;

            if (mb_remain)
              {
                if ((mb_remain->length () + mb->length ()) < char_size)
                  {
                    ACE_Message_Block *mb_new = 0;
                    ACE_NEW_NORETURN (mb,
                                      ACE_Message_Block (mb_remain->length () + mb->length ()));
                    if (mb_new == 0)
                      {
                        mb->release ();
                        mb_remain->release ();
                        return -1; // out of memory error
                      }
                    mb_new->copy (mb_remain->rd_ptr (), mb_remain->length ());
                    mb_remain->release ();
                    mb_new->copy (mb->rd_ptr (), mb->length ());
                    mb->release ();
                    mb_remain = mb_new;
                    continue; // check for next msg block
                  }

                copy_len = (mb_remain->length () > char_length) ?
                                char_length :
                                mb_remain->length ();
                ACE_OS::memmove (&buf[recv_char_count],
                                  mb_remain->rd_ptr (),
                                  copy_len);
                char_length -= copy_len;
                recv_char_count += copy_len;
                mb_remain->rd_ptr (copy_len);
                if (mb_remain->length () > 0)
                  {
                    continue; // buffer is full
                  }

                // cleanup empty block
                mb_remain->release ();
                mb_remain = 0;
              }

            // normalize to total nr of char_size elements available in mb [+ mb_remain]
            size_t total_char_len = ((mb->length () + copy_len)/ char_size) * char_size;
            // what was the max we could copy?
            size_t max_copy_len = (total_char_len > char_length) ?
                                        char_length :
                                        total_char_len;
            // subtract what we possibly already copied from mb_remain
            copy_len = max_copy_len - copy_len;

            ACE_OS::memmove (&buf[recv_char_count],
                              mb->rd_ptr (),
                              copy_len);
            recv_char_count += copy_len;
            char_length -= copy_len;
            mb->rd_ptr (copy_len);
            if (mb->length () > 0)
              {
                mb_remain = mb;
              }
            else
              mb->release ();
          }

        if (mb_remain)
          {
            this->ungetq (mb_remain);
          }

        if (timeout)
          {
            // stop countdown; update timeout value
            timeout_countdown.stop ();
          }

        return static_cast<int> (recv_char_count);
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    bool StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::reactive () const
      {
        return this->sync_opt_[ACE_Synch_Options::USE_REACTOR];
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    bool StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::use_timeout () const
      {
        return this->sync_opt_[ACE_Synch_Options::USE_TIMEOUT];
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    bool StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::char_in_queue (u_short char_size)
      {
        return this->msg_queue ()->message_bytes () >= char_size;
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    int StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::write_to_stream (const void * buf, size_t length, u_short char_size)
      {
        ACE_TRACE ("ACE_IOS_StreamHandler::write_to_stream");

        size_t datasz = length * char_size;
        ACE_Message_Block *mb = 0;
        ACE_NEW_RETURN (mb, ACE_Message_Block (datasz), -1);
        mb->copy ((const char*)buf, datasz);
        ACE_Time_Value nowait (ACE_OS::gettimeofday ());
        if (this->putq (mb, &nowait) == -1)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("(%P|%t) %p; ACE_IOS_StreamHandler - discarding output data, "),
                        ACE_TEXT ("enqueue failed\n")));
            mb->release ();
            return 0;
          }

        ACE_Time_Value max_wait_time = this->sync_opt_.timeout ();
        int result = 0;

        // check if we're allowed to control the reactor if reactive
        bool reactor_thread = false;
        if (this->reactive ())
          {
            ACE_thread_t tid;
            this->reactor ()->owner (&tid);
            reactor_thread =
              ACE_OS::thr_equal (ACE_Thread::self (), tid) ? true : false;
          }

        if (this->reactive () && reactor_thread)
          {
            if (this->reactor ()->register_handler(this,
                                                   ACE_Event_Handler::WRITE_MASK) != 0)
              {
                return -1;
              }

            // run the event loop for the maximum allowed time to get the
            // message data out
            while (this->connected_)
              {
                // Run the event loop.
                result = this->reactor ()->handle_events (this->use_timeout () ?
                                                              &max_wait_time : 0);

                if (result == -1)
                  {
                    ACE_ERROR ((LM_ERROR,
                                ACE_TEXT ("(%P|%t) ACE_IOS_StreamHandler::write_to_stream - ")
                                ACE_TEXT ("handle_events failed (%d)\n"),
                                ACE_OS::last_error ()));
                  }

                // If we got our message out, no need to run the event loop any
                // further.
                if (this->msg_queue ()->is_empty ())
                  {
                    break;
                  }

                // Did we timeout? If so, stop running the loop.
                if (result == 0
                    && this->use_timeout ()
                    && max_wait_time == ACE_Time_Value::zero)
                  {
                    this->reactor ()->remove_handler (this, ACE_Event_Handler::WRITE_MASK);
                    this->send_timeout_ = true;
                    return static_cast<int> (length - (this->msg_queue ()->message_bytes () / char_size));
                  }

                // Other errors? If so, stop running the loop.
                if (result == -1)
                  {
                    this->reactor ()->remove_handler (this, ACE_Event_Handler::WRITE_MASK);
                    return -1;
                  }

                // Otherwise, keep going...
              }
          }
        else
          {
            while (this->connected_)
              {
                result = this->handle_output_i (this->use_timeout () ?
                                                    &max_wait_time : 0);

                // If we got our message out, no need to run the event loop any
                // further.
                if (this->msg_queue ()->is_empty ())
                  {
                    break;
                  }

                // Did we timeout? If so, stop running the loop.
                if (result == 0
                    && this->use_timeout ()
                    && max_wait_time == ACE_Time_Value::zero)
                  {
                    this->send_timeout_ = true;
                    return static_cast<int> (length - (this->msg_queue ()->message_bytes () / char_size));
                  }

                // Otherwise, keep going...
              }
          }

        if (this->connected_)
          return static_cast<int> (length); // all sent
        else
          return static_cast<int> (length - (this->msg_queue ()->message_bytes () / char_size));
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    bool StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::is_connected ()
      {
        return this->connected_;
      }

    template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
    bool StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE>::using_reactor ()
      {
        return this->reactive ();
      }

  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_IOS_STREAM_HANDLER_CPP */
