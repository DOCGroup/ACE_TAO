#include "Server_Info.h"


ACE_RCSID (ImplRepo_Service,
           Server_Info,
           "$Id$")


Server_Info::Server_Info (
    const ACE_CString POA_name,
    const ACE_CString logical_server_name,
    const ACE_CString startup_command,
    const ImplementationRepository::EnvironmentList environment_vars,
    const ACE_CString working_dir,
    const ImplementationRepository::ActivationMode activation)
  : starting_up_ (0),
    logical_server_name_ (logical_server_name),
    POA_name_ (POA_name),
    startup_command_ (startup_command),
    environment_vars_ (environment_vars),
    working_dir_ (working_dir),
    location_ (""),
    server_object_ior_ (""),
    activation_ (activation)
{
  // Initialize the command_line and working_dir.
}


Server_Info::~Server_Info (void)
{
}


// Updates information that is relevant only when an instance
// of the server is running.
void
Server_Info::update_running_info (const ACE_CString location,
                                  const ACE_CString server_object_ior)
{
  this->location_ = location;
  this->server_object_ior_ = server_object_ior;
  this->starting_up_ = 0;
}


void
Server_Info::get_startup_info (
    ACE_CString &logical_server_name,
    ACE_CString &startup_command,
    ImplementationRepository::EnvironmentList &environment_vars,
    ACE_CString &working_dir,
    ImplementationRepository::ActivationMode &activation)
{
  // Returns startup information.
  logical_server_name = this->logical_server_name_;
  startup_command = this->startup_command_;
  environment_vars = this->environment_vars_;
  working_dir = this->working_dir_;
  activation = this->activation_;
}

void
Server_Info::get_running_info (ACE_CString &location,
                               ACE_CString &server_object_ior)
{
  // Returns information about a running instance.
  location = this->location_;
  server_object_ior = this->server_object_ior_;
}
