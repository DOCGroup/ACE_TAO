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
  TORBThread (TORBThread const&) = delete;
  TORBThread (TORBThread&&) = delete;
  TORBThread& operator= (TORBThread const&) = delete;
  TORBThread& operator= (TORBThread&&) = delete;

  // Constructor.
  explicit __fastcall TORBThread (CORBA::ORB_ptr orb);

  // Destructor.
  __fastcall ~TORBThread () override;

protected:
  // Perform the thread's work.
  void __fastcall Execute () override;

private:
  // Reference to the orb.
  CORBA::ORB_var orb_ {};
};
//---------------------------------------------------------------------------
#endif
