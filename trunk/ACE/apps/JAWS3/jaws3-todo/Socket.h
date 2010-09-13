/* -*- c++ -*- */
// $Id$

#ifndef JAWS_SOCKET_H
#define JAWS_SOCKET_H

#include "ace/Addr.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"
#include "ace/SOCK_Stream.h"

#include "jaws3/Export.h"
#include "jaws3/Event_Completer.h"

class JAWS_Socket;
class JAWS_Socket_Impl;

class JAWS_Export JAWS_Socket_Impl
{
public:

  virtual ~JAWS_Socket_Impl (void) {}

  virtual void accept ( const ACE_Addr &local_sap
                      , ACE_SOCK_Stream &new_stream
                      , JAWS_Event_Completer *completer
                      , void *act = 0
                      ) = 0;
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.

  virtual void connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Stream &new_stream
                       , JAWS_Event_Completer *completer
                       , void *act = 0
                       ) = 0;
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // ADDR_ANY is assumed for the local access point.

  virtual void connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Stream &new_stream
                       , const ACE_Addr &local_sap
                       , JAWS_Event_Completer *completer
                       , void *act = 0
                       ) = 0;
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // Use the specified local access point.

  virtual void accept ( const ACE_Addr &local_sap
                      , ACE_SOCK_Stream &new_stream
                      , JAWS_Event_Completer *completer
                      , const ACE_Time_Value &timeout
                      , void *act = 0
                      ) = 0;
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.

  virtual void connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Stream &new_stream
                       , JAWS_Event_Completer *completer
                       , const ACE_Time_Value &timeout
                       , void *act = 0
                       ) = 0;
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // ADDR_ANY is assumed for the local access point.

  virtual void connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Stream &new_stream
                       , const ACE_Addr &local_sap
                       , JAWS_Event_Completer *completer
                       , const ACE_Time_Value &timeout
                       , void *act = 0
                       ) = 0;
  // The address to new_stream is passed back as the data member of
  // the JAWS_Event_Result that is returned to the completer.
  // Use the specified local access point.

};


class JAWS_Export JAWS_Socket
{
public:

  JAWS_Socket (JAWS_Socket_Impl *impl = 0);

  static JAWS_Socket * instance (void)
  {
    return ACE_Singleton<JAWS_Socket, ACE_SYNCH_MUTEX>::instance ();
  }

  void accept ( const ACE_Addr &local_sap
              , ACE_SOCK_Stream &new_stream
              , JAWS_Event_Completer *completer
              , void *act = 0
              );

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , JAWS_Event_Completer *completer
               , void *act = 0
               );

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , const ACE_Addr &local_sap
               , JAWS_Event_Completer *completer
               , void *act = 0
               );

  void accept ( const ACE_Addr &local_sap
              , ACE_SOCK_Stream &new_stream
              , JAWS_Event_Completer *completer
              , const ACE_Time_Value &timeout
              , void *act = 0
              );

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , JAWS_Event_Completer *completer
               , const ACE_Time_Value &timeout
               , void *act = 0
               );

  void connect ( const ACE_Addr &remote_sap
               , ACE_SOCK_Stream &new_stream
               , const ACE_Addr &local_sap
               , JAWS_Event_Completer *completer
               , const ACE_Time_Value &timeout
               , void *act = 0
               );

private:

  JAWS_Socket_Impl *impl_;

};


#endif /* JAWS_SOCKET_H */
