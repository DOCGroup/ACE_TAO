// $Id$

#include "ace/Addr.h"
#include "ace/Acceptor.h"
#include "ace/Singleton.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#define JAWS_BUILD_DLL

#include "jaws3/Socket.h"
#include "jaws3/Reactive_Socket.h"


class JAWS_Socket_EC_SH_Adapter;

typedef ACE_Oneshot_Acceptor<JAWS_Socket_EC_SH_Adapter, ACE_SOCK_ACCEPTOR>
        JAWS_Socket_Oneshot_Acceptor;


void
JAWS_Reactive_Socket::accept ( const ACE_Addr &local_sap
                             , ACE_SOCK_Stream &new_stream
                             , JAWS_Event_Completer *completer
                             , void *act
                             )
{
}


void
JAWS_Reactive_Socket::connect ( const ACE_Addr &remote_sap
                              , ACE_SOCK_Stream &new_stream
                              , JAWS_Event_Completer *completer
                              , void *act
                              )
{
}


void
JAWS_Reactive_Socket::connect ( const ACE_Addr &remote_sap
                              , ACE_SOCK_Stream &new_stream
                              , const ACE_Addr &local_sap
                              , JAWS_Event_Completer *completer
                              , void *act
                              )
{
}


void
JAWS_Reactive_Socket::accept ( const ACE_Addr &local_sap
                             , ACE_SOCK_Stream &new_stream
                             , JAWS_Event_Completer *completer
                             , const ACE_Time_Value &timeout
                             , void *act
                             )
{
}


void
JAWS_Reactive_Socket::connect ( const ACE_Addr &remote_sap
                              , ACE_SOCK_Stream &new_stream
                              , JAWS_Event_Completer *completer
                              , const ACE_Time_Value &timeout
                              , void *act
                              )
{
}


void
JAWS_Reactive_Socket::connect ( const ACE_Addr &remote_sap
                              , ACE_SOCK_Stream &new_stream
                              , const ACE_Addr &local_sap
                              , JAWS_Event_Completer *completer
                              , const ACE_Time_Value &timeout
                              , void *act
                              )
{
}
