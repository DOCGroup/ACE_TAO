// $Id$

#define JAWS_BUILD_DLL
#include "jaws3/Socket.h"


JAWS_Socket::JAWS_Socket (JAWS_Socket_Impl *impl = 0)
  : impl_ (impl)
{
}

void
JAWS_Socket::accept ( const ACE_Addr &local_sap
                    , ACE_SOCK_Stream &new_stream
                    , JAWS_Event_Completer *completer
                    , void *act
                    )
{
  this->impl_->accept (local_sap, new_stream, completer, act);
}

void
JAWS_Socket::connect ( const ACE_Addr &remote_sap
                     , ACE_SOCK_Stream &new_stream
                     , JAWS_Event_Completer *completer
                     , void *act = 0
                     )
{
  this->impl_->connect (remote_sap, new_stream, completer, act);
}

void
JAWS_Socket::connect ( const ACE_Addr &remote_sap
                     , ACE_SOCK_Stream &new_stream
                     , const ACE_Addr &local_sap
                     , JAWS_Event_Completer *completer
                     , void *act = 0
                     )
{
  this->impl_->connect (remote_sap, new_stream, local_sap, completer, act);
}

void
JAWS_Socket::accept ( const ACE_Addr &local_sap
                    , ACE_SOCK_Stream &new_stream
                    , JAWS_Event_Completer *completer
                    , const ACE_Time_Value &timeout
                    , void *act = 0
                    )
{
  this->impl_->accept (local_sap, new_stream, completer, timeout, act);
}

void
JAWS_Socket::connect ( const ACE_Addr &remote_sap
                     , ACE_SOCK_Stream &new_stream
                     , JAWS_Event_Completer *completer
                     , const ACE_Time_Value &timeout
                     , void *act = 0
                     )
{
  this->impl_->connect (remote_sap, new_stream, completer, timeout, act);
}

void
JAWS_Socket::connect ( const ACE_Addr &remote_sap
                     , ACE_SOCK_Stream &new_stream
                     , const ACE_Addr &local_sap
                     , JAWS_Event_Completer *completer
                     , const ACE_Time_Value &timeout
                     , void *act = 0
                     )
{
  this->impl_->connect ( remote_sap
                       , new_stream
                       , local_sap
                       , completer
                       , timeout
                       , act
                       );
}

