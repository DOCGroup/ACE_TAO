/* -*- C++ -*- */
// $Id$
#include "Repository.h"
#include "Options.h"
#include "ace/ACE.h"

ACE_RCSID(ImplRepo_Service, Repository, "$Id$")

// Some helper functions
const char *
convert_mode_to_str (Server_Info::ActivationMode mode)
{
  if (mode == Server_Info::NORMAL)
    return "NORMAL";
  else if (mode == Server_Info::MANUAL)
    return "MANUAL";
  else if (mode == Server_Info::PER_CLIENT)
    return "PER_CLIENT";
  else if (mode == Server_Info::AUTO_START)
    return "AUTO_START";

  // If all else fails, we are normal
  return "NORMAL";
}

Server_Info::ActivationMode 
convert_str_to_mode (const char *str)
{
  if (ACE_OS::strcasecmp (str, "NORMAL") == 0)
    return Server_Info::NORMAL;
  else if (ACE_OS::strcasecmp (str, "MANUAL") == 0)
    return Server_Info::MANUAL;
  else if (ACE_OS::strcasecmp (str, "PER_CLIENT") == 0)
    return Server_Info::PER_CLIENT;
  else if (ACE_OS::strcasecmp (str, "AUTO_START") == 0)
    return Server_Info::AUTO_START;

  // If all else fails, we are normal
  return Server_Info::NORMAL;
}


// Initialize the command_line and working_dir.

Server_Info::Server_Info (const ACE_TString POA_name,
                          const ACE_TString logical_server_name,
                          const ACE_TString startup_command,
                          const ACE_TString working_dir,
                          const ActivationMode activation)
: starting_up_ (0),
  logical_server_name_ (logical_server_name),
  POA_name_ (POA_name),
  startup_command_ (startup_command),
  working_dir_ (working_dir),
  location_ (""),
  server_object_ior_ (""),
  activation_ (activation)
{
  // Nothing
}


Server_Info::~Server_Info ()
{
  // Nothing
}


// Updates information that is relevant only when an instance
// of the server is running.

void
Server_Info::update_running_info (const ACE_TString location,
                                  const ACE_TString server_object_ior)
{
  this->location_ = location;
  this->server_object_ior_ = server_object_ior;
}


// Returns startup information.

void
Server_Info::get_startup_info (ACE_TString &logical_server_name,
                               ACE_TString &startup_command,
                               ACE_TString &working_dir,
                               ActivationMode &activation)
{
  logical_server_name = this->logical_server_name_;
  startup_command = this->startup_command_;
  working_dir = this->working_dir_;
  activation = this->activation_;
}


// Returns information about a running instance.

void
Server_Info::get_running_info (ACE_TString &location,
                               ACE_TString &server_object_ior)
{
  location = this->location_;
  server_object_ior = this->server_object_ior_;
}


// Default Constructor

Server_Repository::Server_Repository ()
: config_ (0)
{
  // Nothing
}


// Destructor

Server_Repository::~Server_Repository ()
{
  delete this->config_;
}


// Initialize the the configuration class.

int
Server_Repository::init (ACE_Configuration* config)
{
  delete this->config_;
  this->config_ = config;
  
  // iterate through the list of registered servers and register them
  this->config_->open_section(config_->root_section(), "Servers", 1, this->servers_);
  int index = 0;
  ACE_TString name;
  
  while (this->config_->enumerate_sections (servers_, index, name) == 0)
    {
      ACE_TString logical, startup, working_dir, activation_str;
      Server_Info::ActivationMode activation;
      ACE_Configuration_Section_Key server_key;
      int error = 0;
    
      error += this->config_->open_section (this->servers_, name.c_str(), 0, server_key);
      error += this->config_->get_string_value (server_key, "LogicalServer", logical);
      error += this->config_->get_string_value (server_key, "StartupCommand", startup);
      error += this->config_->get_string_value (server_key, "WorkingDir", working_dir);
      error += this->config_->get_string_value (server_key, "Activation", activation_str);

      activation = convert_str_to_mode (activation_str.c_str ());

      if(error)
        ACE_DEBUG ((LM_DEBUG, "Error reading configuration data for service '%s', skipping\n", name.rep()));
      else
        this->add (name, logical, startup, working_dir, activation);

      index++;
    }
  return 0;
}


// Add a new server to the Repository

int
Server_Repository::add (const ACE_TString POA_name,
                        const ACE_TString logical_server_name,
                        const ACE_TString startup_command,
                        const ACE_TString working_dir,
                        const Server_Info::ActivationMode activation)
{
  // Add this to the persistent configuration
  ACE_Configuration_Section_Key server;
  this->config_->open_section (this->servers_, POA_name.c_str(), 1, server);
  this->config_->set_string_value (server, "LogicalServer", logical_server_name);
  this->config_->set_string_value (server, "StartupCommand", startup_command);
  this->config_->set_string_value (server, "WorkingDir", working_dir);
  this->config_->set_string_value (server, "Activation", convert_mode_to_str (activation));

  Server_Info *new_server;
  ACE_NEW_RETURN (new_server,
                  Server_Info (POA_name, 
                               logical_server_name, 
                               startup_command, 
                               working_dir, 
                               activation),
                  -1);

  return this->repository_.bind (POA_name, new_server);
}


// Update the associated process information.

int
Server_Repository::update (const ACE_TString POA_name,
                           const ACE_TString location,
                           const ACE_TString server_object_ior)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    server->update_running_info (location, server_object_ior);

  return retval;
}


// Returns information related to startup.

int
Server_Repository::get_startup_info (const ACE_TString POA_name,
                                     ACE_TString &logical_server_name,
                                     ACE_TString &startup_command,
                                     ACE_TString &working_dir,
                                     Server_Info::ActivationMode &activation)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    server->get_startup_info (logical_server_name, startup_command, working_dir, activation);

  return retval;
}


// Returns information related to a running copy.

int
Server_Repository::get_running_info (const ACE_TString POA_name,
                                     ACE_TString &location,
                                     ACE_TString &server_object_ior)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    server->get_running_info (location, server_object_ior);

  return retval;
}


// Checks the starting_up_ variable in the Server_Info and
// returns the previous value or -1 if the POA_name wasn't found

int
Server_Repository::starting_up (const ACE_TString POA_name, int new_value)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
  {
    retval = server->starting_up_;
    server->starting_up_ = new_value;
  }

  return retval;
}


// Same as above but does not alter the value

int
Server_Repository::starting_up (const ACE_TString POA_name)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    retval = server->starting_up_;

  return retval;
}


// Removes the server from the Repository.

int
Server_Repository::remove (const ACE_TString POA_name)
{
  // Remove the persistent configuration information
  this->config_->remove_section (this->servers_, POA_name.c_str(), 1);

  return this->repository_.unbind (POA_name);
}


// Returns a new iterator that travels over the repository.

Server_Repository::HASH_IMR_ITER *
Server_Repository::new_iterator ()
{
  HASH_IMR_ITER *hash_iter;
  ACE_NEW_RETURN (hash_iter,
                  Server_Repository::HASH_IMR_ITER (this->repository_),
                  0);
  return hash_iter;
}


// Returns the number of entries in the repository.

size_t
Server_Repository::get_repository_size ()
{
  return this->repository_.current_size ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<ACE_TString, Server_Info *>;
template class ACE_Hash_Map_Manager_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;

#if defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x500)
// This template is already defined in TAO, but Sun/CC 5.0 is broken
template class ACE_Equal_To<ACE_CString>;
#endif /* __SUNPRO_CC */
// Instantiate for ACE_WString because ACE_TString can be either
// ACE_CString or ACE_WString.
template class ACE_Equal_To<ACE_WString>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<ACE_TString, Server_Info *>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_TString, Server_Info *,ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>

#if defined(__SUNPRO_CC) && (__SUNPRO_CC == 0x500)
// This template is already defined in TAO, but Sun/CC 5.0 is broken
#pragma instantiate ACE_Equal_To<ACE_CString>
#endif /* __SUNPRO_CC */
// Instantiate for ACE_WString because ACE_TString can be either
// ACE_CString or ACE_WString.
#pragma instantiate ACE_Equal_To<ACE_WString>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
