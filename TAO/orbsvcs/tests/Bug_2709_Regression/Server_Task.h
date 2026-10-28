#include "ace/Task.h"
#include "ace/Manual_Event.h"
#include "tao/ORB.h"

//////////////////////////////////////////////////////////////////////////

// This task should be run to start Test object implementation
class Server_Task : public ACE_Task_Base
{
public:
  /// Constructor
  Server_Task (ACE_TCHAR const * output,
    CORBA::ORB_ptr sorb,
    ACE_Manual_Event &me,
    ACE_Thread_Manager *thr_mgr);

  /// Thread entry point
  Server_Task (Server_Task const &) = delete;
  Server_Task (Server_Task &&) = delete;
  Server_Task &operator= (Server_Task const &) = delete;
  Server_Task &operator= (Server_Task &&) = delete;

  int svc () override;

private:
  /// Output file for IOR
  ACE_TCHAR const * output_;

  /// Manual event to wake up the main thread to create a client
  /// thread.
  ACE_Manual_Event &me_;

  CORBA::ORB_var sorb_;
};
