#ifndef TAO_IMR_ADVANCED_TEST_CLIENT_H
#define TAO_IMR_ADVANCED_TEST_CLIENT_H

#include "tao/ORB.h"

#include "ace/Task.h"

#include <vector>

#include "ace/Capabilities.h"

class TestClient : public ACE_Task_Base, public ACE_Capabilities
{
public:
  TestClient (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[]);

  ~TestClient () override = default;

  bool run ();

  TestClient (TestClient const &) = delete;
  TestClient (TestClient &&) = delete;
  TestClient &operator= (TestClient const &) = delete;
  TestClient &operator= (TestClient &&) = delete;

private:
  struct Options
  {
    ACE_TCHAR pause_type {ACE_TEXT ('s')};
    int startup_pause {};
    int thread_count {};
    int iterations {};
    int request_count {};
    bool random_requests {};
    bool shutdown_orb {};
    bool expect_holding {};
    bool expect_no_profile {};
    ACE_TString ior_file {ACE_TEXT ("imr_test.ior")};
  };

  int parseCommands (int argc, ACE_TCHAR *argv[]);
  bool buildIORList ();
  void pause (int milliseconds);
  int svc () override;

  CORBA::ORB_var orb_;
  std::vector<ACE_TString> iors_;
  Options options_;
  int instance_ {};
};

#endif /* TAO_IMR_ADVANCED_TEST_CLIENT_H */
