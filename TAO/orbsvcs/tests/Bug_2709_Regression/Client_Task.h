// $Id$

#include "ace/Task.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"

//////////////////////////////////////////////////////////////////////////

/// Implement a Task to run the client as a thread
class Client_Task : public ACE_Task_Base
{
public:

  /// Constructor
  Client_Task (CORBA::ORB_ptr corb,
    ACE_Thread_Manager *thr_mgr,
    const ACE_CString& collocated_ior_file,
    const ACE_CString& server_ior_file);

  CORBA::Object_ptr make_iogr (
     const char* domain_id,
     CORBA::ULongLong group_id,
     CORBA::ULong group_version);

  /// Thread entry point
  int svc (void);

private:
  CORBA::ORB_var corb_;

  TAO_IOP::TAO_IOR_Manipulation_var iorm_;

  const ACE_CString& collocated_ior_file_;

  const ACE_CString& server_ior_file_;
};
