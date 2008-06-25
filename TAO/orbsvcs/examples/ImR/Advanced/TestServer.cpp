//$Id$
#include "TestServer.h"

#include "ManagerC.h"

#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"
#include "ace/os_include/os_ctype.h"
#include "ace/Get_Opt.h"

#include <sstream>
#include <utility>

using std::string;

// Does this function already exist?
// Is there a path class?

string normalizePath(const string& dir, char delim = '/', bool toLower = false);
string getWorkingPath(char delim = '/', bool toLower = false);

string deleteLeaf(const string& dir)
{
  string temp = dir;
  temp.resize(dir.find_last_of("/\\"));
  return temp;
}

string getWorkingPath(char delim, bool toLower)
{
  char buffer[PATH_MAX + 2];
  ACE_OS::getcwd(buffer, PATH_MAX);
  return normalizePath(buffer, delim, toLower);
}

string normalizePath(const string& dir, char delim, bool toLower)
{
  if (dir.empty() == true) return string();
  char buffer[PATH_MAX + 2];

  string::size_type i = 0;
 
  for (; ACE_OS::ace_isspace(dir[i]); i++);

  string::size_type begin = i;

  int j = 0;
  for (; i < dir.size(); i++)
  {
    if (dir[i] == '\\' || dir[i] == '/')
    {
      // skip redundant
      if (dir[i + 1] == '\\' || dir[i + 1] == '/')
      {
      }
      // Convert to proper delim
      else
      {
        buffer[j] = delim; j++;
      }
    }
    else if (dir[i] == '.')
    {
      // If specifying current, skip
      if (dir[i + 1] == '\\' || dir[i + 1] == '/' || dir[i + 1] == '\0')
      {
        if (i == begin) // relative start
        {
          string curDir = getWorkingPath();
          ACE_OS::strncpy(buffer, curDir.c_str(), sizeof(buffer));
          j = curDir.length();
        }
        else
        {
          i += 2;
        }
      }
      // If specifying parent, go back
      else if (dir[i + 1] == '.')
      {
        if (i == begin) // relative start
        {
          string curDir = getWorkingPath();
          ACE_OS::strncpy(buffer, curDir.c_str(), sizeof(buffer));
          j = curDir.length();
        }
        int k = j;
        for (; buffer[k] != delim; k--);
        k--;
        for (; buffer[k] != delim; k--);
        j = (k + 1);
        i+=2;
      }
      else
      {
        if (toLower)
          buffer[j] = ACE_OS::ace_tolower(dir[i]);
        else
          buffer[j] = dir[i];
        j++;
      }
    }
    // normal character
    else
    {
      if (toLower)
        buffer[j] = ACE_OS::ace_tolower(dir[i]);
      else
        buffer[j] = dir[i];
      j++;
    }
  }

  if (buffer[j - 1] == delim) j--;
  buffer[j] ='\0';
  return string(buffer);
}

TestServer::TestServer (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[])
: serverID_(1)
, serverInstanceID_(-1)
, useIORTable_(false)
, writeIORFile_(false)
, retryQuery_(false)
, pauseType_('s')
, startupPause_(0)
, objActivatePause_(0)
, activatePause_(0)
, runPause_(0)
, numPOAS_(0)
, numObjsPerPOA_(0)
, useItLoseItSecs_(0)
, orb_(CORBA::ORB::_duplicate(orb))
, iorTable_(IORTable::Table::_nil())
, root_(PortableServer::POA::_nil())
, mgr_(PortableServer::POAManager::_nil())
{
  parseCommands (argc, argv);

  verifyEnvironment();

  // We really do not want the current directory set by the
  // activator. baseDir_ is where all the operation files are.
  if (baseDir_.empty() == false)
  {
    ACE_OS::chdir(baseDir_.c_str());
  }
}

TestServer::~TestServer()
{
  root_->destroy(1, 1);
}

//  TestServer::parseCommands
//  Reads params from command line
//
int TestServer::parseCommands (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "w:e:d:t:o:s:c:a:r:p:n:x:z:q:b:");
  int c;
  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
    case 'w':
      expectedDir_ = normalizePath(get_opts.opt_arg());
      break;

    case 'e':
      {
        string name = get_opts.opt_arg();
        string::size_type i = name.find_first_of("=");
        string value = name.substr(i +  1, name.length() - 1);
        name.resize(i);
        expectedEnv_.push_back(std::make_pair(name, value));
        break;
      }

    case 'd':
      serverID_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 't':
      {
        const char* opt = get_opts.opt_arg();
        useIORTable_ = (opt && opt[0] != '0');
        break;
      }

    case 'q':
      {
        const char* opt = get_opts.opt_arg();
        retryQuery_ = (opt && opt[0] != '0');
        break;
      }

    case 'o':
      {
        const char* opt = get_opts.opt_arg();
        writeIORFile_ = (opt && opt[0] != '0');
        break;
      }

    case 's':
      startupPause_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'c':
      objActivatePause_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'a':
      activatePause_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'r':
      runPause_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'p':
      numPOAS_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'n':
      numObjsPerPOA_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'x':
      useItLoseItSecs_ = ACE_OS::atoi(get_opts.opt_arg());
      break;

    case 'z':
      pauseType_ = get_opts.opt_arg()[0];
      break;

    case 'b':
      baseDir_ =  get_opts.opt_arg();
      break;

    case '?':
    default:
      ACE_ERROR_RETURN((LM_ERROR,
        "usage:  %s\n"
        "\t-d <startup pause in milliseconds>\n"
        "\t-t 'use IOR Table'\n"
        "\t-o 'write the IOR file'\n"
        "\t-s <startup pause in milliseconds>\n"
        "\t-a <activation pause in milliseconds>\n"
        "\t-r <run pause in milliseconds>\n"
        "\t-p <number of POAS>\n"
        "\t-n <number of objects per POA>\n"
        "\t-x <number of seconds from last hit till exit>\n"
        "\t-e <expected_env_var_name>=<value> 'can be repeated'\n"
        "\t-w <expected_working_directory>\n"
        "\n",
        argv[0]),
        -1);
    }
  }
  return 0;
}

//  TestServer::verifyEnvironment
//  Checks to see if the activator setup requested env vars and current directory
//
bool TestServer::verifyEnvironment() const
{
  bool err = false;

  // Check paths
  string currentDir = getWorkingPath();
  if (expectedDir_.empty() == false && currentDir != expectedDir_)
  {
    ACE_DEBUG((LM_DEBUG, "Error: directory paths (%s,%s) do not match.\n",
               ACE_TEXT_CHAR_TO_TCHAR (currentDir.c_str()), ACE_TEXT_CHAR_TO_TCHAR (expectedDir_.c_str())));
    err |= true;
  }

  // Check env vars
  for (string::size_type i = 0; i < expectedEnv_.size(); i++)
  {
    const char* realValue = ACE_OS::getenv(expectedEnv_[i].first.c_str()) ;
    if (realValue == 0)
    {
      ACE_DEBUG((LM_DEBUG, "Error, env variable '%s' not found\n",
                 ACE_TEXT_CHAR_TO_TCHAR (expectedEnv_[i].first.c_str())));
      err |= true;
    }
    else if (expectedEnv_[i].second != realValue)
    {
      ACE_DEBUG((LM_DEBUG, "Error, env variable '%s' values (%s,%s) do not match.\n",
                 ACE_TEXT_CHAR_TO_TCHAR (expectedEnv_[i].first.c_str()), ACE_TEXT_CHAR_TO_TCHAR (realValue), ACE_TEXT_CHAR_TO_TCHAR (expectedEnv_[i].second.c_str())));
      err |= true;
    }
  }

  return (err ? 1 : 0);
}

//  TestServer::pause
//  Pause processing by either sleep or timed run-loop
//
void TestServer::pause(int milliseconds)
{
  if (milliseconds > 0)
  {
    if (pauseType_ == 's')
    {
      ACE_OS::sleep(ACE_Time_Value(0, milliseconds * 1000));
    }
    else if (pauseType_ == 'r')
    {
      ACE_Time_Value tv(0, milliseconds * 1000);
      orb_->run(tv);
    }
    else
    {
      ACE_ASSERT(pauseType_ == 'r');
    }
  }
}

void TestServer::run()
{
  pause(startupPause_);

  // FUZZ: disable check_for_missing_rir_env
  CORBA::Object_var obj = orb_->resolve_initial_references("RootPOA");
  root_ = PortableServer::POA::_narrow(obj.in());
  mgr_ = root_->the_POAManager();

  if (registerWithManager() == false)
    return;

  ACE_DEBUG((LM_DEBUG, "* Server (%d.%d) started.\n",
             serverID_, serverInstanceID_));

  if (useIORTable_ == true)
  {
    // FUZZ: disable check_for_missing_rir_env
    CORBA::Object_var obj = orb_->resolve_initial_references("IORTable");
    iorTable_ = IORTable::Table::_narrow(obj.in());
  }

  servant_.reset(new Messenger_i(orb_.in(), serverInstanceID_));

  buildObjects();

  pause(activatePause_);
  mgr_->activate();

  if (useItLoseItSecs_ > 0)
  {
    pause(runPause_);
    do
    {
      ACE_Time_Value tv(useItLoseItSecs_);
      orb_->run(tv);
    } while (orb_->orb_core()->has_shutdown() == 0
      && servant_->acknowledgeHit());
  }

  if (orb_->orb_core()->has_shutdown() != 0)
  {
    ACE_DEBUG((LM_DEBUG, "* Server (%d.%d) ended.\n",
               serverID_, serverInstanceID_));
  }
  else
  {
    ACE_DEBUG((LM_DEBUG, "* Server (%d.%d) self terminated.\n",
               serverID_, serverInstanceID_));
  }
}

//  TestServer::registerServer
//  This will register the server with the manager.
//
bool TestServer::registerWithManager()
{
  //  So-far, the use cases dictate writting IOR
  // responsibility is exclusive of registering
  if (writeIORFile_ == false)
  {
    // Get the manager's ref
    CORBA::Object_var obj = orb_->string_to_object("file://manager.ior");
    if (CORBA::is_nil(obj.in()))
    {
      cerr << "Server Error: Could not get Manager IOR." << endl;
      return false;
    }
    Manager_var manager = Manager::_narrow(obj.in());
    if (CORBA::is_nil(manager.in())) {
      cerr << "Server Error: IOR was not a Manager object reference." << endl;
      return false;
    }

    // If this is a query process
    if (retryQuery_ == true)
    {
      int diff = manager->endRetry();
      if (diff != 0)
      {
        ACE_DEBUG((LM_DEBUG, "* Server Error: Not all retry attempts were made.\n"));
      }
      return false;
    }
    else // Get the server ID
    {
      serverInstanceID_  = manager->registerServer();
      if (serverInstanceID_ == -1)
      {
        ACE_DEBUG((LM_DEBUG, "* Server (%d,%d) could not register.\n",
                   serverID_, serverInstanceID_));
        return false;
      }
    }
  }
  return true;
}

//  TestServer::buildObjects
//  Builds all the objects required by the client and server
//  Writes IORs if requested
//
void TestServer::buildObjects()
{
  // Append to existing file
  ofstream iorFile("imr_test.ior", ios::app);

  // Create number of requested POAS
  for (int i = 0; i < numPOAS_; i++)
  {
    CORBA::PolicyList policies(3);
    policies.length(3);
    policies[0] = root_->create_id_assignment_policy(PortableServer::USER_ID);
    policies[1] = root_->create_id_uniqueness_policy(PortableServer::MULTIPLE_ID);
    policies[2] = root_->create_lifespan_policy(PortableServer::PERSISTENT);

    // Create a POA
    string poaName;
    {
      std::stringstream poaStream;
      poaStream << "POA_" << serverID_ << "_" << (i + 1);
      poaName = poaStream.str();
    }

    ACE_DEBUG((LM_DEBUG, "* Creating POA: %s\n", ACE_TEXT_CHAR_TO_TCHAR (poaName.c_str())));

    PortableServer::POA_var sub_poa = root_->create_POA(poaName.c_str(), mgr_.in(), policies);

    pause(objActivatePause_);
    // Create number of requested object references
    for (int j = 0; j < numObjsPerPOA_; j++)
    {
      // Create object reference and activate it
      string objName;
      {
        std::stringstream objStream;
        objStream << "OBJ_" << serverID_ << "_" << (i + 1) << "_" << (j + 1);
        objName = objStream.str();
      }
      ACE_DEBUG((LM_DEBUG, "* Activating Obj: %s\n", ACE_TEXT_CHAR_TO_TCHAR (objName.c_str())));

      PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId(objName.c_str());
      sub_poa->activate_object_with_id(oid.in(), servant_.get());

      // Output the stringified ID to the file for the client
      if (! useIORTable_)
      {
        if (writeIORFile_)
        {
          // Write out IOR
          CORBA::Object_var obj = sub_poa->id_to_reference(oid.in());
          CORBA::String_var ior = orb_->object_to_string(obj.in());
          iorFile << ior.in () << endl;
        }
      }
      else  // use IOR table
      {
        // Write out corbaloc
        TAO_Root_POA* tmp_poa = dynamic_cast<TAO_Root_POA*>(sub_poa.in());
        // Make entry into ior table using the non IMRified object ref.
        CORBA::Object_var obj = tmp_poa->id_to_reference_i(oid.in(), false);
        CORBA::String_var ior = orb_->object_to_string(obj.in());
        string key = (poaName + "/" + objName);
        iorTable_->bind(key.c_str(), ior.in());

        if (writeIORFile_)
        {
          // Use the IMRified object ref to create the corbaloc.
          CORBA::Object_var obj = sub_poa->id_to_reference(oid.in());

          // Create a corbaloc
          TAO_Profile* profile = obj->_stubobj()->profile_in_use();
          CORBA::String_var temp = profile->to_string();
          string corbaloc = temp.in();
          corbaloc.resize(corbaloc.find_first_of(profile->object_key_delimiter()) + 1);
          corbaloc += key;

          // Write out corbaloc
          iorFile << corbaloc.c_str() << endl;
        }
      }
    }
  }
}
