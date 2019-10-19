//---------------------------------------------------------------------------
#ifndef ORBThreadH
#define ORBThreadH
//---------------------------------------------------------------------------
#include "tao/ORB.h"
//---------------------------------------------------------------------------
class PACKAGE TORBThread : public TThread
{
  // Class for running the orb in a separate thread.
public:
  // Constructor.
  __fastcall TORBThread (CORBA::ORB_ptr orb);

  // Destructor.
  __fastcall ~TORBThread ();

protected:
  void __fastcall Execute ();
  // Perform the thread's work.

private:
  CORBA::ORB_var orb_;
  // Reference to the orb.
};
//---------------------------------------------------------------------------
#endif
