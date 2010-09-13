/* -*- c++ -*- */
// $Id$

#ifndef JAWS_REACTIVE_SOCKET_H
#define JAWS_REACTIVE_SOCKET_H

#include "ace/Addr.h"
#include "ace/Synch.h"
#include "ace/Acceptor.h"
#include "ace/Singleton.h"
#include "ace/SOCK_Stream.h"
#include "ace/SOCK_Acceptor.h"

#include "jaws3/Export.h"
#include "jaws3/Socket.h"

class JAWS_Reactive_Socket;

class JAWS_Export JAWS_Reactive_Socket : public JAWS_Socket_Impl
{
public:

  static JAWS_Reactive_Socket * instance (void)
  {
    return ACE_Singleton<JAWS_Reactive_Socket, ACE_SYNCH_MUTEX>::instance ();
  }

  void accept ( const ACE_Addr &local_sap
              , ACE_SOCK_Stream &new_stream
              , JAWS_Event_Completer *completer
              , void *act = 0
              );
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , JAWS_Event_Completer *completer
               , void *act = 0
               );
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // ADDR_ANY is assumed for the local access point.

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , const ACE_Addr &local_sap
               , JAWS_Event_Completer *completer
               , void *act = 0
               );
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // Use the specified local access point.

  void accept ( const ACE_Addr &local_sap
              , ACE_SOCK_Stream &new_stream
              , JAWS_Event_Completer *completer
              , const ACE_Time_Value &timeout
              , void *act = 0
              );
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , JAWS_Event_Completer *completer
               , const ACE_Time_Value &timeout
               , void *act = 0
               );
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // ADDR_ANY is assumed for the local access point.

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , const ACE_Addr &local_sap
               , JAWS_Event_Completer *completer
               , const ACE_Time_Value &timeout
               , void *act = 0
               );
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // Use the specified local access point.

};


#endif /* JAWS_REACTIVE_SOCKET_H */
