// -*- C++ -*-

//=============================================================================
/**
 *  @file    HTBP_Session.h
 *
 *  @author Phil Mesnier
 */
//=============================================================================

#ifndef ACE_HTBP_SESSION_H
#define ACE_HTBP_SESSION_H
#include /**/ "ace/pre.h"

#include "ace/SOCK_IO.h"
#include "ace/Hash_Map_Manager.h"
#include "ace/Synch.h"
#include "ace/Message_Queue.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "HTBP_Addr.h"
#include "HTBP_Export.h"
#include "HTBP_Channel.h"

#include "HTBP_Stream.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward declarations.
class ACE_HTBP_Filter;
class ACE_Event_Handler;

namespace ACE
{
  namespace HTBP
  {
    class Session_Id_t
    {
    public:
      ACE_UINT32 id_;
      Addr    local_;
      Addr    peer_;

      u_long hash () const;
      bool operator == (Session_Id_t const& other) const;
    };

    /**
     * @class Session
     *
     * @brief Defines the methods in the <Session> abstraction.
     *
     * A session is an entity that combines two Ht_Channels that connect directly
     * to a proxy to manage communication with a remote peer. The Session may
     * persist longer than either stream, assuming that the proxy is libel to
     * close a connection at any time.
     *
     * This means that the session needs to be able to reconnect to the remote
     * peer. This also means that the session needs to be aware of its location
     * If it is outside the proxy and looses a stream, oh well. If it is inside,
     * then the next time a stream is required, then it must reconnect before
     * returning the stream.
     *
     * The session does not queue outbound messages. That is going to be the
     * responsibility of the application, or a higher level protocol wrapper.
     */
    class HTBP_Export Session
    {
    public:
      // Initialization and termination methods.
      /// Constructor.
      Session ();

      /// Constructor (sets the underlying session id with <sid>).
      Session (Addr const& peer,
               Addr const& local,
               ACE_UINT32 sid = 0,
               ACE_INET_Addr *proxy = nullptr,
               bool take_proxy = false);
      explicit Session (Session_Id_t const& id,
               ACE_INET_Addr *proxy = nullptr,
               bool take_proxy = false);

      Session (Session const& other) = delete;
      Session& operator= (Session const& other) = delete;
      Session (Session&& other) = delete;
      Session& operator= (Session&& other) = delete;

      /// Destructor.
      ~Session ();

      /// The following methods are specific to the Session
      static ACE_UINT32 next_session_id ();

      static int add_session (Session *);
      static int remove_session (Session *);
      static int find_session (Session_Id_t const&,
                               Session *&out);

      Stream *stream () const;
      void stream (Stream *);

      int enqueue (ACE_Message_Block *msg);
      int flush_outbound_queue ();

      int close_inbound () const;
      int close_outbound () const;

      /// get references to the actual streams based on the direction
      /// of data flow if this session is on the inside of the proxy
      /// ie, has a non-null proxy addr, then the inbound stream is
      /// the out_to_in stream, otherwise it is the in_to_out
      /// stream. The outbound is the opposite of the inbound.
      /// Whenever an application wishes to send data, whether that is
      /// request or reply data, it uses the outbound stream, and it
      /// should associate an event handler with the inbound stream
      /// for receiving data.
      Channel *inbound () const;
      Channel *outbound () const;
      void inbound (Channel *);
      void outbound (Channel *);

      int enable (int value);
      int disable (int value);

      Session_Id_t const& session_id() const;
      void session_id (ACE_UINT32 );

      ACE_INET_Addr const* proxy_addr () const;
      void proxy_addr (ACE_INET_Addr *, bool destroy = false);

      Addr const& peer_addr () const;
      Addr const& local_addr () const;

      void peer_addr (Addr const&);
      void local_addr (Addr const&);

      /// invoke close on both streams, then remove self from session map
      int close ();

      ACE_Event_Handler *handler ();
      void handler (ACE_Event_Handler *);
      void reactor (ACE_Reactor *);
      void detach (Channel *);

      int sock_flags() const;

    private:
      /// Connected Stream ensures that the particular stream is
      /// connected to the proxy, if possible. The result is same as
      /// the reference passed in, so that it may be used inline for
      /// the inboundor outbound methods

      void reconnect () const;
      void reconnect_i (Channel *) const;

      using Session_Map = ACE_Hash_Map_Manager<Session_Id_t, Session*, ACE_SYNCH_MUTEX>;
      using Map_Entry = ACE_Hash_Map_Entry<Session_Id_t, Session*>;
      static Session_Map session_map_;
      static ACE_UINT32 last_session_id_;
      static ACE_SYNCH_MUTEX session_id_lock_;

      ACE_INET_Addr *proxy_addr_;
      bool destroy_proxy_addr_;

      Session_Id_t session_id_;

      Channel *inbound_;
      Channel *outbound_;

      bool closed_;

      ACE_Event_Handler *handler_;
      ACE_Reactor *reactor_;

      ACE_Message_Queue<ACE_SYNCH> outbound_queue_;
      Stream * stream_;
      int sock_flags_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "HTBP_Session.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTBP_SESSION_H */
