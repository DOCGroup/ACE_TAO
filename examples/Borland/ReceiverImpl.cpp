// $Id$
//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "ChatClientWnd.h"
#include "ReceiverImpl.h"
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
void TReceiverImplementation::message (const char* msg)
{
  ::PostMessage (ChatClientWindow->Handle,
                 WM_MESSAGE_RECEIVED,
                 (WPARAM) new String (msg),
                 (LPARAM) 0);
}
//---------------------------------------------------------------------------
void TReceiverImplementation::shutdown ()
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
