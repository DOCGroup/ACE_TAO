// $Id$
//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "ReceiverImpl.h"
#include "ChatClientWnd.h"
//---------------------------------------------------------------------------
TReceiverImplementation::TReceiverImplementation ()
  : orb_ (0)
{
}
//---------------------------------------------------------------------------
TReceiverImplementation::~TReceiverImplementation ()
{
}
//---------------------------------------------------------------------------
void TReceiverImplementation::message (const char* msg) throw (CORBA::SystemException)
{
  ::PostMessage (ChatClientWindow->Handle,
                 WM_MESSAGE_RECEIVED,
                 (WPARAM) new String (msg),
                 (LPARAM) 0);
}
//---------------------------------------------------------------------------
void TReceiverImplementation::shutdown () throw (CORBA::SystemException)
{
  // Instruct the ORB to shutdown.
  orb_->shutdown (0);
}
//---------------------------------------------------------------------------
void TReceiverImplementation::orb (CORBA::ORB_ptr o)
{
  orb_ = CORBA::ORB::_duplicate (o);
}
//---------------------------------------------------------------------------
