/* -*- c++ -*- */
// $Id$

#ifndef JAWS_REACTIVE_SOCKET_HELPERS
#define JAWS_REACTIVE_SOCKET_HELPERS

#include "ace/Addr.h"
#include "ace/Synch.h"
#include "ace/Acceptor.h"
#include "ace/Singleton.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "jaws3/Event_Completer.h"
#include "jaws3/Event_Result.h"


typedef void (JAWS_Event_Completer:: *JAWS_EC_CALLBACK)
             (const JAWS_Event_Result &, void *act);


class JAWS_Socket_EC_SH_Adapter
  : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{

  friend class JAWS_Socket_Accept_EC_SH_Adapter;
  friend class JAWS_Socket_Connect_EC_SH_Adapter;

public:

  JAWS_Socket_EC_SH_Adapter ( ACE_SOCK_Stream *stream
                            , JAWS_Event_Completer *completer
                            , void *act
                            , JAWS_EC_CALLBACK callback
                            , JAWS_Event_Result::JE_REASON ok_reason
                            , JAWS_Event_Result::JE_REASON timeout_reason
                            , JAWS_Event_Result::JE_REASON error_reason
                            );

  int open (void *);
  // Callback from Acceptor/Connector when accept()/connect() completes.

private:

  ACE_SOCK_Stream *stream_;

  JAWS_Event_Completer *completer_;
  void *act_;

  JAWS_EC_CALLBACK callback_;
  JAWS_Event_Result::JE_REASON ok_reason_;
  JAWS_Event_Result::JE_REASON timeout_reason_;
  JAWS_Event_Result::JE_REASON error_reason_;

};


class JAWS_Socket_Accept_EC_SH_Adapter : public JAWS_Socket_EC_SH_Adapter
{
public:

  JAWS_Socket_Accept_EC_SH_Adapter ( ACE_SOCK_Stream *stream
                                   , JAWS_Event_Completer *completer
                                   , void *act
                                   );

};


class JAWS_Socket_Connect_EC_SH_Adapter : public JAWS_Socket_EC_SH_Adapter
{
public:

  JAWS_Socket_Connect_EC_SH_Adapter ( ACE_SOCK_Stream *stream
                                    , JAWS_Event_Completer *completer
                                    , void *act
                                    );

private:

  JAWS_Event_Completer *completer_;
  void *act_;

};

#endif /* JAWS_REACTIVE_SOCKET_HELPERS */
