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
    ACE_TString const & collocated_ior_file,
    ACE_TString const & server_ior_file);

  Client_Task (Client_Task const &) = delete;
  Client_Task (Client_Task &&) = delete;
  Client_Task &operator= (Client_Task const &) = delete;
  Client_Task &operator= (Client_Task &&) = delete;

  CORBA::Object_ptr make_iogr (
     char const * domain_id,
     CORBA::ULongLong group_id,
     CORBA::ULong group_version);

  /// Thread entry point
  int svc () override;

private:
  CORBA::ORB_var corb_;

  TAO_IOP::TAO_IOR_Manipulation_var iorm_;

  ACE_TString const & collocated_ior_file_;

  ACE_TString const & server_ior_file_;
};
