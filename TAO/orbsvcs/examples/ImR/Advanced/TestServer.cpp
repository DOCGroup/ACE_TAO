#include "TestServer.h"

#include "ManagerC.h"

#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/Root_POA.h"

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_ctype.h"
#include "ace/os_include/os_ctype.h"
#include "ace/Get_Opt.h"

#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

using std::string;

// Does this function already exist?
// Is there a path class?

string normalizePath (string const &dir, char delim = '/', bool toLower = false);
string getWorkingPath (char delim = '/', bool toLower = false);

string getWorkingPath (char delim, bool toLower)
{
  char buffer[PATH_MAX + 2] {};
  ACE_OS::getcwd (buffer, PATH_MAX);
  return normalizePath (buffer, delim, toLower);
}

string normalizePath (string const &dir, char delim, bool toLower)
{
  if (dir.empty ())
    {
      return string {};
    }

  char buffer[PATH_MAX + 2] {};

  string::size_type i {};

  for (; ACE_OS::ace_isspace (dir[i]); ++i)
    {
      // No action.
    }

  string::size_type const begin = i;

  int j {};

  for (; i < dir.size (); ++i)
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
              buffer[j] = delim;
              ++j;
            }
        }
      else if (dir[i] == '.')
        {
          // If specifying current, skip
          if (dir[i + 1] == '\\' || dir[i + 1] == '/' || dir[i + 1] == '\0')
            {
              if (i == begin) // relative start
                {
                  string const curDir = getWorkingPath ();
                  ACE_OS::strncpy (buffer, curDir.c_str (), sizeof (buffer));
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
                  string const curDir = getWorkingPath ();
                  ACE_OS::strncpy (buffer, curDir.c_str (), sizeof (buffer));
                  j = curDir.length();
                }

              int k = j;

              for (; buffer[k] != delim; k--)
                {
                  // No action.
                }

              k--;

              for (; buffer[k] != delim; k--)
                {
                  // No action.
                }

              j = (k + 1);
              i += 2;
            }
          else
            {
              if (toLower)
                {
                  buffer[j] = ACE_OS::ace_tolower (dir[i]);
                }
              else
                {
                  buffer[j] = dir[i];
                }

              ++j;
            }
        }
      // normal character
      else
        {
          if (toLower)
            {
              buffer[j] = ACE_OS::ace_tolower (dir[i]);
            }
          else
            {
              buffer[j] = dir[i];
            }

          ++j;
        }
    }

  if (buffer[j - 1] == delim)
    {
      j--;
    }

  buffer[j] ='\0';
  return string(buffer);
}

TestServer::TestServer (CORBA::ORB_ptr orb, int argc, ACE_TCHAR *argv[])
  : orb_ (CORBA::ORB::_duplicate (orb))
{
  parseCommands (argc, argv);

  this->verifyEnvironment ();

  // We really do not want the current directory set by the
  // activator. base_dir is where all the operation files are.
  if (!options_.base_dir.empty ())
  {
    ACE_OS::chdir (options_.base_dir.c_str ());
  }
}

//  TestServer::parseCommands
//  Reads params from command line
//
int TestServer::parseCommands (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("w:e:d:t:o:s:c:a:r:p:n:x:z:q:b:m:i:"));
  int c {};
  while ((c = get_opts ()) != -1)
  {
    switch (c)
    {
    case 'w':
      options_.expected_dir = normalizePath (ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ()));
      break;

    case 'e':
      {
        string name = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        string::size_type const i = name.find_first_of ('=');
        string const value = name.substr (i + 1);
        name.resize(i);
        options_.expected_env.emplace_back (name, value);
        break;
      }

    case 'd':
      options_.server_id = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 't':
      {
        ACE_TCHAR const *opt = get_opts.opt_arg ();
        options_.use_ior_table = (opt && opt[0] != '0');
        break;
      }

    case 'q':
      {
        ACE_TCHAR const *opt = get_opts.opt_arg ();
        options_.retry_query = (opt && opt[0] != '0');
        break;
      }

    case 'o':
      {
        ACE_TCHAR const *opt = get_opts.opt_arg ();
        options_.write_ior_file = (opt && opt[0] != '0');
        break;
      }

    case 's':
      options_.startup_pause = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'c':
      options_.object_activate_pause = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'a':
      options_.activate_pause = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'r':
      options_.run_pause = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'p':
      options_.num_poas = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'n':
      options_.num_objects_per_poa = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'x':
      options_.use_it_lose_it_secs = ACE_OS::atoi (get_opts.opt_arg ());
      break;

    case 'z':
      options_.pause_type = get_opts.opt_arg ()[0];
      break;

    case 'b':
      options_.base_dir = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
      break;

    case 'm':
      options_.manager_ior = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
      break;

    case 'i':
      options_.output_ior = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
      break;

    case '?':
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("usage:  %s\n")
                         ACE_TEXT ("\t-d <startup pause in milliseconds>\n")
                         ACE_TEXT ("\t-t 'use IOR Table'\n")
                         ACE_TEXT ("\t-o 'write the IOR file'\n")
                         ACE_TEXT ("\t-s <startup pause in milliseconds>\n")
                         ACE_TEXT ("\t-a <activation pause in milliseconds>\n")
                         ACE_TEXT ("\t-r <run pause in milliseconds>\n")
                         ACE_TEXT ("\t-p <number of POAS>\n")
                         ACE_TEXT ("\t-n <number of objects per POA>\n")
                         ACE_TEXT ("\t-x <number of seconds from last hit till exit>\n")
                         ACE_TEXT ("\t-e <expected_env_var_name>=<value> 'can be repeated'\n")
                         ACE_TEXT ("\t-w <expected_working_directory>\n\n"),
                         argv[0]),
                        -1);
    }
  }
  return 0;
}

//  TestServer::verifyEnvironment
//  Checks to see if the activator setup requested env vars and current directory
//
bool TestServer::verifyEnvironment () const
{
  bool err {};

  // Check paths
  string const currentDir = getWorkingPath ();
  if (!options_.expected_dir.empty () && currentDir != options_.expected_dir)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Error: directory paths (%C,%C) do not match.\n"),
                currentDir.c_str (), options_.expected_dir.c_str ()));
    err = true;
  }

  // Check env vars
  for (size_t i {}; i < options_.expected_env.size (); ++i)
  {
    char const *realValue = std::getenv (options_.expected_env[i].first.c_str ());
    if (realValue == nullptr)
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Error, env variable '%C' not found\n"),
                  options_.expected_env[i].first.c_str ()));
      err = true;
    }
    else if (options_.expected_env[i].second != realValue)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Error, env variable '%C' values (%C,%C) do not match.\n"),
                  options_.expected_env[i].first.c_str (), realValue,
                  options_.expected_env[i].second.c_str ()));
      err = true;
    }
  }

  return err;
}

//  TestServer::pause
//  Pause processing by either sleep or timed run-loop
//
void TestServer::pause (int milliseconds)
{
  if (milliseconds > 0)
  {
    if (options_.pause_type == 's')
    {
      ACE_OS::sleep (ACE_Time_Value (0, milliseconds * 1000));
    }
    else if (options_.pause_type == 'r')
    {
      ACE_Time_Value tv (0, milliseconds * 1000);
      orb_->run (tv);
    }
    else
    {
      ACE_ASSERT (options_.pause_type == 'r');
    }
  }
}

void TestServer::run ()
{
  pause (options_.startup_pause);

  // FUZZ: disable check_for_missing_rir_env
  CORBA::Object_var obj = orb_->resolve_initial_references ("RootPOA");
  root_ = PortableServer::POA::_narrow (obj.in ());
  mgr_ = root_->the_POAManager ();

  if (!registerWithManager ())
    return;

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Server (%d.%d) started.\n"),
              options_.server_id, options_.server_instance_id));

  if (options_.use_ior_table)
  {
    // FUZZ: disable check_for_missing_rir_env
    CORBA::Object_var obj = orb_->resolve_initial_references ("IORTable");
    iorTable_ = IORTable::Table::_narrow (obj.in ());
  }

  servant_ = new Messenger_i (orb_.in (), options_.server_instance_id);

  buildObjects ();

  pause (options_.activate_pause);
  mgr_->activate ();

  if (options_.use_it_lose_it_secs > 0)
  {
    pause (options_.run_pause);
    do
    {
      ACE_Time_Value tv (options_.use_it_lose_it_secs);
      orb_->run (tv);
    } while (orb_->orb_core ()->has_shutdown () == 0
             && servant_->acknowledgeHit ());
  }

  if (orb_->orb_core ()->has_shutdown () != 0)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Server (%d.%d) ended.\n"),
                options_.server_id, options_.server_instance_id));
  }
  else
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Server (%d.%d) self terminated.\n"),
                options_.server_id, options_.server_instance_id));
  }
}

//  TestServer::registerServer
//  This will register the server with the manager.
//
bool TestServer::registerWithManager ()
{
  //  So-far, the use cases dictate writting IOR
  // responsibility is exclusive of registering
  if (!options_.write_ior_file)
  {
    // Get the manager's ref
    CORBA::Object_var obj = orb_->string_to_object (options_.manager_ior.c_str ());
    if (CORBA::is_nil (obj.in ()))
    {
      std::cerr << "Server Error: Could not get Manager IOR." << std::endl;
      return false;
    }
    Manager_var manager = Manager::_narrow (obj.in ());
    if (CORBA::is_nil (manager.in ())) {
      std::cerr << "Server Error: IOR was not a Manager object reference." << std::endl;
      return false;
    }

    // If this is a query process
    if (options_.retry_query)
    {
      CORBA::Long const diff = manager->endRetry ();
      if (diff != 0)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Server Error: Not all retry attempts were made.\n")));
      }
      return false;
    }
    else
    {
      options_.server_instance_id = manager->registerServer ();
      if (options_.server_instance_id == -1)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Server (%d,%d) could not register.\n"),
                    options_.server_id, options_.server_instance_id));
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
void TestServer::buildObjects ()
{
  // Append to existing file
  std::ofstream iorFile (options_.output_ior.c_str (), std::ios::app);

  // Create number of requested POAS
  for (int i {}; i < options_.num_poas; ++i)
  {
    CORBA::PolicyList policies(3);
    policies.length (3);
    policies[0] = root_->create_id_assignment_policy (PortableServer::USER_ID);
    policies[1] = root_->create_id_uniqueness_policy (PortableServer::MULTIPLE_ID);
    policies[2] = root_->create_lifespan_policy (PortableServer::PERSISTENT);

    // Create a POA
    string poaName;
    {
      std::stringstream poaStream;
      poaStream << "POA_" << options_.server_id << "_" << (i + 1);
      poaName = poaStream.str ();
    }

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Creating POA: %C\n"), poaName.c_str ()));

    PortableServer::POA_var sub_poa = root_->create_POA (poaName.c_str (), mgr_.in (), policies);

    pause (options_.object_activate_pause);
    // Create number of requested object references
    for (int j {}; j < options_.num_objects_per_poa; ++j)
    {
      // Create object reference and activate it
      string objName;
      {
        std::stringstream objStream;
        objStream << "OBJ_" << options_.server_id << "_" << (i + 1) << "_" << (j + 1);
        objName = objStream.str ();
      }
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("* Activating Obj: %C\n"), objName.c_str ()));

      PortableServer::ObjectId_var oid = PortableServer::string_to_ObjectId (objName.c_str ());
      sub_poa->activate_object_with_id (oid.in (), servant_.in ());

      // Output the stringified ID to the file for the client
      if (!options_.use_ior_table)
      {
        if (options_.write_ior_file)
        {
          // Write out IOR
          CORBA::Object_var obj = sub_poa->id_to_reference (oid.in ());
          CORBA::String_var ior = orb_->object_to_string (obj.in ());
          iorFile << ior.in () << std::endl;
        }
      }
      else  // use IOR table
      {
        // Write out corbaloc
        TAO_Root_POA *tmp_poa = dynamic_cast<TAO_Root_POA*> (sub_poa.in ());
        // Make entry into ior table using the non IMRified object ref.
        CORBA::Object_var obj = tmp_poa->id_to_reference_i (oid.in (), false);
        CORBA::String_var ior = orb_->object_to_string (obj.in ());
        string const key = poaName + "/" + objName;
        iorTable_->bind (key.c_str (), ior.in ());

        if (options_.write_ior_file)
        {
          // Use the IMRified object ref to create the corbaloc.
          CORBA::Object_var obj = sub_poa->id_to_reference (oid.in ());

          // Create a corbaloc
          TAO_Profile *profile = obj->_stubobj ()->profile_in_use ();
          CORBA::String_var temp = profile->to_string ();
          string corbaloc = temp.in ();
          corbaloc.resize (corbaloc.find_first_of (profile->object_key_delimiter ()) + 1);
          corbaloc += key;

          // Write out corbaloc
          iorFile << corbaloc.c_str () << std::endl;
        }
      }
    }
  }
}
