// $Id$


#define JAWS_BUILD_DLL
#include "jaws3/Datagram.h"


JAWS_Datagram::JAWS_Datagram (JAWS_Datagram_Impl *impl = 0)
  : impl_ (impl)
{
}

void
JAWS_Datagram::accept ( const ACE_Addr &local_sap
                      , ACE_SOCK_Dgram &new_dgram
                      , JAWS_Event_Completer *completer
                      , void *act
                      )
{
  this->impl_->accept (local_sap, new_dgram, completer, act);
}

void
JAWS_Datagram::connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Dgram &new_dgram
                       , JAWS_Event_Completer *completer
                       , void *act = 0
                       )
{
  this->impl_->connect (remote_sap, new_dgram, completer, act);
}

void
JAWS_Datagram::connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Dgram &new_dgram
                       , const ACE_Addr &local_sap
                       , JAWS_Event_Completer *completer
                       , void *act = 0
                       )
{
  this->impl_->connect (remote_sap, new_dgram, local_sap, completer, act);
}

void
JAWS_Datagram::accept ( const ACE_Addr &local_sap
                      , ACE_SOCK_Dgram &new_dgram
                      , JAWS_Event_Completer *completer
                      , const ACE_Time_Value &timeout
                      , void *act = 0
                      )
{
  this->impl_->accept (local_sap, new_dgram, completer, timeout, act);
}

void
JAWS_Datagram::connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Dgram &new_dgram
                       , JAWS_Event_Completer *completer
                       , const ACE_Time_Value &timeout
                       , void *act = 0
                       )
{
  this->impl_->connect (remote_sap, new_dgram, completer, timeout, act);
}

void
JAWS_Datagram::connect ( const ACE_Addr &remote_sap
                       , ACE_SOCK_Dgram &new_dgram
                       , const ACE_Addr &local_sap
                       , JAWS_Event_Completer *completer
                       , const ACE_Time_Value &timeout
                       , void *act = 0
                       )
{
  this->impl_->connect ( remote_sap
                       , new_dgram
                       , local_sap
                       , completer
                       , timeout
                       , act
                       );
}

