//$Id$
#include "tao/ORB.h"

#include "ace/Task.h"

#include <vector>
#include <string>
#include "ace/Capabilities.h"

class TestClient : public ACE_Task_Base, public ACE_Capabilities
{
public:
  TestClient (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[]);

  virtual ~TestClient();

  void run();

private:
  int parseCommands (int argc, ACE_TCHAR *argv[]);
  void buildIORList();
  void pause(int milliseconds);
  virtual int svc();

  CORBA::ORB_var     orb_;
  std::vector<ACE_TString> iors_;

  ACE_TCHAR          pauseType_;
  int                startupPause_;
  int                threadCount_;
  int                instance_;
  int                iterations_;
  int                requestCount_;
  bool               randomRequests_;
  bool               shutdownOrb_;
  bool               expectHolding_;
  bool               expectNoProfile_;
  const ACE_TCHAR    *iorFile_;
};
