#ifndef TAO_IMR_ADVANCED_TEST_SERVER_H
#define TAO_IMR_ADVANCED_TEST_SERVER_H

#include "Messenger_i.h"

#include "tao/IORTable/IORTable.h"

#include <string>
#include <utility>
#include <vector>

class TestServer
{
public:
  TestServer (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[]);

  ~TestServer () = default;

  void run ();

  TestServer (TestServer const &) = delete;
  TestServer (TestServer &&) = delete;
  TestServer &operator= (TestServer const &) = delete;
  TestServer &operator= (TestServer &&) = delete;

private:
  struct Options
  {
    int server_id {1};
    int server_instance_id {-1};
    bool use_ior_table {};
    bool write_ior_file {};
    bool retry_query {};
    ACE_TCHAR pause_type {ACE_TEXT ('s')};
    int startup_pause {};
    int object_activate_pause {};
    int activate_pause {};
    int run_pause {};
    int num_poas {};
    int num_objects_per_poa {};
    int use_it_lose_it_secs {};
    std::string manager_ior {"file://manager.ior"};
    std::string output_ior {"imr_test.ior"};
    std::string base_dir;
    std::string expected_dir;
    std::vector<std::pair<std::string, std::string>> expected_env;
  };

  int parseCommands (int argc, ACE_TCHAR *argv[]);
  bool verifyEnvironment () const;
  bool registerWithManager ();
  void buildObjects ();
  void pause (int milliseconds);

  Options options_;
  PortableServer::Servant_var<Messenger_i> servant_;

  CORBA::ORB_var orb_;
  IORTable::Table_var iorTable_ {IORTable::Table::_nil ()};
  PortableServer::POA_var root_ {PortableServer::POA::_nil ()};
  PortableServer::POAManager_var mgr_ {PortableServer::POAManager::_nil ()};
};

#endif /* TAO_IMR_ADVANCED_TEST_SERVER_H */
