/* -*- C++ -*- */
// $Id$
#include "Repository.h"
#include "Options.h"
#include "ace/ACE.h"

ACE_RCSID(ImplRepo_Service, Repository, "$Id$")


  
// Initialize the command_line and working_dir.

Server_Info::Server_Info (const ACE_TString POA_name,
                          const ACE_TString logical_server_name,
                          const ACE_TString startup_command,
                          const ACE_TString working_dir)
: starting_up_ (0),
  logical_server_name_ (logical_server_name),
  POA_name_ (POA_name),
  startup_command_ (startup_command),
  working_dir_ (working_dir),
  host_ (""),
  port_ (0),
  ping_ior_ ("")
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
Server_Info::update_running_info (const ACE_TString host,
                                  const unsigned short port,
                                  const ACE_TString ping_ior)
{
  this->host_ = host;
  this->port_ = port;
  this->ping_ior_ = ping_ior;
}


// Returns startup information.

void
Server_Info::get_startup_info (ACE_TString &logical_server_name,
                               ACE_TString &startup_command,
                               ACE_TString &working_dir)
{
  logical_server_name = this->logical_server_name_;
  startup_command = this->startup_command_;
  working_dir = this->working_dir_;
}


// Returns information about a running instance.

void 
Server_Info::get_running_info (ACE_TString &host,
                               unsigned short &port,
                               ACE_TString &ping_ior)
{
  host = this->host_;
  port = this->port_;
  ping_ior = this->ping_ior_;
}

// Default Constructor

Server_Repository::Server_Repository ()
{
  // Nothing
}


// Add a new server to the Repository

int 
Server_Repository::add (const ACE_TString POA_name,
                        const ACE_TString logical_server_name,
                        const ACE_TString startup_command,
                        const ACE_TString working_dir)
{
  Server_Info *new_server;
  ACE_NEW_RETURN (new_server, 
                  Server_Info (POA_name, logical_server_name, startup_command, working_dir),
                  -1);

  return this->repository_.bind (POA_name, new_server);
}


// Update the associated process information.

int 
Server_Repository::update (const ACE_TString POA_name,
                           const ACE_TString host,
                           const unsigned short port,
                           const ACE_TString ping_ior)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    server->update_running_info (host, port, ping_ior);

  return retval;
}


// Returns information related to startup.

int 
Server_Repository::get_startup_info (const ACE_TString POA_name,
                                     ACE_TString &logical_server_name,
                                     ACE_TString &startup_command,
                                     ACE_TString &working_dir)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    server->get_startup_info (logical_server_name, startup_command, working_dir);

  return retval;
}
  

// Returns information related to a running copy.

int 
Server_Repository::get_running_info (const ACE_TString POA_name,
                                     ACE_TString &host,
                                     unsigned short &port,
                                     ACE_TString &ping_ior)
{
  Server_Info *server;
  int retval = this->repository_.find (POA_name, server);

  // Only fill in data if it was found
  if (retval == 0)
    server->get_running_info (host, port, ping_ior);

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
  return this->repository_.unbind (POA_name);
}


// Returns a new iterator that travels over the repository.

HASH_IR_ITER *
Server_Repository::new_iterator ()
{
  HASH_IR_ITER *hash_iter;
  ACE_NEW_RETURN (hash_iter,
                  Server_Repository::HASH_IR_ITER (this->repository_),
                  0);
  return hash_iter;
}


// Returns the number of entries in the repository.

size_t 
get_repository_size ()
{
  return this->repository_.current_size ();
}
  

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_TString, Repository_Record *, ACE_Hash<ACE_TString>, ACE_Equal_To<ACE_TString>, ACE_Null_Mutex>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

