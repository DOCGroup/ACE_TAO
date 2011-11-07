//$Id$
#include "Messenger_i.h"

#include "ace/Auto_Ptr.h"

#include "tao/IORTable/IORTable.h"

#include <string>
#include <vector>

class TestServer
{
public:
  TestServer (CORBA::ORB_ptr, int argc, ACE_TCHAR *argv[]);

  ~TestServer();

  void run();

private:
  int parseCommands (int argc, ACE_TCHAR *argv[]);
  bool verifyEnvironment() const;
  bool registerWithManager();
  void buildObjects();
  void pause(int milliseconds);

  int   serverID_;
  int   serverInstanceID_;
  bool  useIORTable_;
  bool  writeIORFile_;
  bool  retryQuery_;
  ACE_TCHAR  pauseType_;
  int   startupPause_;
  int   objActivatePause_;
  int   activatePause_;
  int   runPause_;
  int   numPOAS_;
  int   numObjsPerPOA_;
  int   useItLoseItSecs_;

  std::string managerIor_;
  std::string outputIor_;
  std::string baseDir_;
  std::string expectedDir_;
  std::vector<std::pair<std::string, std::string> > expectedEnv_;
  PortableServer::Servant_var<Messenger_i> servant_;

  CORBA::ORB_var                  orb_;
  IORTable::Table_var             iorTable_;
  PortableServer::POA_var         root_;
  PortableServer::POAManager_var  mgr_;
};
