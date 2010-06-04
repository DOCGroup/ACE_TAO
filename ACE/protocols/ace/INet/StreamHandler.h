// $Id$

/**
 * @file StreamHandler.h
 *
 * @author Martin Corino <mcorino@remedy.nl>
 */

#ifndef ACE_IOS_STREAM_HANDLER_H
#define ACE_IOS_STREAM_HANDLER_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Svc_Handler.h"

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
        */
        template <ACE_PEER_STREAM_1, ACE_SYNCH_DECL>
        class StreamHandler
          : public ACE_Svc_Handler<ACE_PEER_STREAM, ACE_SYNCH_USE>
          {
            public:
              typedef StreamHandler<ACE_PEER_STREAM, ACE_SYNCH_USE> this_type;
              typedef ACE_Svc_Handler<ACE_PEER_STREAM, ACE_SYNCH_USE> base_type;
              typedef ACE_Message_Queue<ACE_SYNCH_USE> mq_type;

              /// Constructor
              StreamHandler (const ACE_Synch_Options &synch_options = ACE_Synch_Options::defaults,
                             ACE_Thread_Manager *thr_mgr = 0,
                             mq_type *mq = 0,
                             ACE_Reactor *reactor = ACE_Reactor::instance ());

              virtual ~StreamHandler ();

              /// Activate the connection
              virtual int open (void * = 0);

              /// Close the connection
              virtual int close (u_long flags = 0);

              virtual int handle_input (ACE_HANDLE);

              virtual int handle_output (ACE_HANDLE);

              int read_from_stream (void * buf, size_t length, u_short char_size);

              int write_to_stream (const void * buf, size_t length, u_short char_size);

              bool is_connected ();

              bool using_reactor ();

            private:
              enum
                {
                  MAX_INPUT_SIZE = 4096
                };

              int handle_output_i (ACE_Time_Value* timeout = 0);

              int handle_input_i (ACE_Time_Value* timeout = 0);

              int process_input (char* buf,
                                 size_t& char_length,
                                 u_short char_size,
                                 ACE_Time_Value* timeout);

              bool reactive () const;

              bool use_timeout () const;

              bool char_in_queue (u_short char_size);

              bool connected_;
              ACE_Synch_Options sync_opt_;
              bool send_timeout_;
              bool receive_timeout_;
          };

        typedef StreamHandler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>  SockStreamHandler;
      };
  }

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "StreamHandler.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("StreamHandler.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_IOS_STREAM_HANDLER_H */
