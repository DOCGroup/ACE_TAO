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
  // Perform the thread's work.
  void __fastcall Execute ();

private:
  // Reference to the orb.
  CORBA::ORB_var orb_;
};
//---------------------------------------------------------------------------
#endif
