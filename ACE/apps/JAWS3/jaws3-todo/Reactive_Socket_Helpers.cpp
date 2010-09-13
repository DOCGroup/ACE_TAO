/* -*- c++ -*- */
// $Id$

#include "ace/Addr.h"
#include "ace/Synch.h"
#include "ace/Acceptor.h"
#include "ace/Singleton.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "jaws3/Reactive_Socket_Helpers.h"

JAWS_Socket_EC_SH_Adapter
::JAWS_Socket_EC_SH_Adapter ( ACE_SOCK_Stream *stream
                            , JAWS_Event_Completer *completer
                            , void *act
                            , JAWS_EC_CALLBACK callback
                            , JAWS_Event_Result::JE_REASON ok_reason
                            , JAWS_Event_Result::JE_REASON timeout_reason
                            , JAWS_Event_Result::JE_REASON error_reason
                            )
  : stream_ (stream)
  , completer_ (completer)
  , act_ (act)
  , callback_ (callback)
  , ok_reason_ (ok_reason)
  , timeout_reason_ (timeout_reason)
  , error_reason_ (error_reason)
{
}


int
JAWS_Socket_EC_SH_Adapter::open (void *)
{
  JAWS_Event_Result result ( 0
                           , JAWS_Event_Result::JE_OK
                           , this->ok_reason_
                           , 0
                           , this->stream_);

  *this->stream_ = this->peer ();
  this->set_handle (ACE_INVALID_HANDLE);
  (this->completer_->*callback_) (result, this->act_);

  return -1;
}


JAWS_Socket_Accept_EC_SH_Adapter
::JAWS_Socket_Accept_EC_SH_Adapter ( ACE_SOCK_Stream *stream
                                   , JAWS_Event_Completer *completer
                                   , void *act
                                   )
  : JAWS_Socket_EC_SH_Adapter ( stream
                              , completer
                              , act
                              , & JAWS_Event_Completer::accept_complete
                              , JAWS_Event_Result::JE_ACCEPT_OK
                              , JAWS_Event_Result::JE_ACCEPT_TIMEOUT
                              , JAWS_Event_Result::JE_ACCEPT_FAIL
                              )
{
}


JAWS_Socket_Connect_EC_SH_Adapter::
JAWS_Socket_Connect_EC_SH_Adapter ( ACE_SOCK_Stream *stream
                                  , JAWS_Event_Completer *completer
                                  , void *act
                                  )
  : JAWS_Socket_EC_SH_Adapter ( stream
                              , completer
                              , act
                              , & JAWS_Event_Completer::connect_complete
                              , JAWS_Event_Result::JE_CONNECT_OK
                              , JAWS_Event_Result::JE_CONNECT_TIMEOUT
                              , JAWS_Event_Result::JE_CONNECT_FAIL
                              )
{
}
