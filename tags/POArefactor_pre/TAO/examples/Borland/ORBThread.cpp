// $Id$
//---------------------------------------------------------------------------
// To be able to handle CORBA requests an application usually calls orb->run().
// To integrate this into VCL applications we will run the orb in a separate
// thread. The ability to run the orb in a thread other than the main thread is
// a non-standard extension to CORBA provided by TAO. If you would rather use
// strictly standard CORBA calls, you need to add the code:
//
//   if (orb->work_pending())
//     orb->perform_work();
//
// somewhere into your application's event loop.
//
// Note that a "pure" client application (one that does not handle requests
// from any other application) need not do either of these things.
//---------------------------------------------------------------------------
#include "pch.h"
#pragma hdrstop
#include "ORBThread.h"
//---------------------------------------------------------------------------
__fastcall TORBThread::TORBThread (CORBA::ORB_ptr orb)
   : TThread (true),
     orb_ (CORBA::ORB::_duplicate (orb))
{
  Resume ();
}
//---------------------------------------------------------------------------
__fastcall TORBThread::~TORBThread ()
{
  try
    {
      orb_->shutdown (0);
    }
  catch (CORBA::Exception&)
    {
    }

  WaitFor ();
}
//---------------------------------------------------------------------------
void __fastcall TORBThread::Execute ()
{
  try
    {
      orb_->run ();
    }
  catch (CORBA::Exception& e)
    {
      ShowMessage (e._rep_id ());
      Application->Terminate ();
    }
}
//---------------------------------------------------------------------------
