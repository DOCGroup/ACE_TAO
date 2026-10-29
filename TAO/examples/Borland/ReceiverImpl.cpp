//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "ChatClientWnd.h"
#include "ReceiverImpl.h"

#include <memory>
//---------------------------------------------------------------------------
void TReceiverImplementation::message (char const *msg)
{
  std::unique_ptr<String> message = std::make_unique<String> (msg);
  if (::PostMessage (ChatClientWindow->Handle,
                     WM_MESSAGE_RECEIVED,
                     reinterpret_cast<WPARAM> (message.get ()),
                     0))
    message.release ();
}
//---------------------------------------------------------------------------
void TReceiverImplementation::shutdown ()
{
  // Instruct the ORB to shutdown.
  orb_->shutdown (false);
}
//---------------------------------------------------------------------------
void TReceiverImplementation::orb (CORBA::ORB_ptr o)
{
  orb_ = CORBA::ORB::_duplicate (o);
}
//---------------------------------------------------------------------------
