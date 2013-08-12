// $Id$

/**
 * @file StreamHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_STREAM_HANDLER_H
#define ACE_IOS_STREAM_HANDLER_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"
#include "ace/Reactor_Notification_Strategy.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
  {
    namespace IOS
      {
        /**
        * @class ACE_IOS_StreamHandler
        *
        * @brief Encapsulates streamed connection.
        *
        * This class provides the connection point for the
        * ACE Acceptor and Connector based patterns and the
        * ACE Reactor framework to C++ standard streams
        * based classes.
        */
        template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
        class StreamHandler
          : public ACE_Svc_Handler<ACE_PEER_STREAM, ACE_SYNCH_USE>
          {
            public:
              // useful traits
              typedef StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE> this_type;
              typedef ACE_Svc_Handler<ACE_PEER_STREAM, ACE_SYNCH_USE> base_type;
              typedef ACE_Message_Queue<ACE_SYNCH_USE> mq_type;

              /// Constructor
              StreamHandler (const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults,
                             ACE_Thread_Manager *thr_mgr = 0,
                             mq_type *mq = 0,
                             ACE_Reactor *reactor = ACE_Reactor::instance ());

              /// Destructor
              virtual ~StreamHandler ();

              /// Activate the connection
              virtual int open (void * = 0);

              /// Close the connection
              virtual int close (u_long flags = 0);

              /// Called to handle incoming data when using StreamHandler in
              /// reactive mode
              virtual int handle_input (ACE_HANDLE);

              /// Called to handle outgoing data when using StreamHandler in
              /// reactive mode
              virtual int handle_output (ACE_HANDLE);

              /// Called by streambuffer to read/receive new data from peer
              int read_from_stream (void * buf, size_t length, size_t char_size);

              /// Called by streambuffer to send new data to peer
              int write_to_stream (const void * buf, size_t length, size_t char_size);

              /// Returns true as long as the connection to peer is active
              bool is_connected () const;

              /// Returns true if StreamHandler has been configured for reactive mode
              bool using_reactor () const;

            private:
              enum
                {
                  MAX_INPUT_SIZE = 4096
                };

              /// Attempts to receive data from peer and queue it.
              /// Called either from handle_input in reactive mode or
              /// directly from read_from_stream when non-reactive.
              int handle_output_i (ACE_Time_Value* timeout = 0);

              /// Attempts to send queued data to peer.
              /// Called either from handle_output in reactive mode
              /// or directly from write_to_stream when non-reactive.
              int handle_input_i (size_t rdlen, ACE_Time_Value* timeout = 0);

              /// processes queued input
              int process_input (char* buf,
                                 size_t& char_length,
                                 size_t char_size,
                                 ACE_Time_Value* timeout);

              /// Returns true if a timeout is to be used on IO operations.
              bool use_timeout () const;

              /// Returns true is the queued data contains at least char_size bytes.
              bool char_in_queue (size_t char_size);

              bool connected_;
              ACE_Synch_Options sync_opt_;
              bool send_timeout_;
              bool receive_timeout_;
              ACE_Reactor_Notification_Strategy notification_strategy_;

              class NotificationStrategyGuard
                {
                  public:
                    NotificationStrategyGuard (this_type& queue_owner,
                                               ACE_Reactor_Notification_Strategy* ns)
                      : queue_owner_ (queue_owner)
                      {
                        this->queue_owner_.msg_queue ()->notification_strategy (ns);
                      }
                    ~NotificationStrategyGuard ()
                      {
                        this->queue_owner_.msg_queue ()->notification_strategy (0);
                      }
                  private:
                    this_type& queue_owner_;
                };
          };

        typedef StreamHandler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>  SockStreamHandler;
      }
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/INet/StreamHandler.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("StreamHandler.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_STREAM_HANDLER_H */
