//=============================================================================
/**
 *  @file   Iterator.cpp
 *
 *  $Id$
 *
 *  @brief  This file declares ImR's iterator.
 *
 *  @author Darrell Brunsch <brunsch@cs.wustl.edu>
 */
//=============================================================================

#include "Iterator.h"

// Plain old constructor

ImR_Iterator::ImR_Iterator (Server_Repository::HASH_IMR_ITER *iterator,
                            PortableServer::POA_ptr poa)
  : iterator_ (iterator),
    poa_ (poa)
{
  // Nothing
}


// Destructor

ImR_Iterator::~ImR_Iterator ()
{
  delete iterator_;
}


// Returns the next list of up to <how_many> servers.  If empty, will return
// false.

CORBA::Boolean
ImR_Iterator::next_n (CORBA::ULong how_many,
                      ImplementationRepository::ServerInformationList_out server_list
                      ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_NEW_THROW_EX (server_list,
                    ImplementationRepository::ServerInformationList (0),
                    CORBA::NO_MEMORY ());

  // If there are no more bindings...
  if (this->iterator_->done ())
    return 0; // Return false

  // Initially assume that iterator has the requested number of
  // bindings.
  server_list->length (how_many);

  Server_Repository::HASH_IMR_ENTRY *server_entry;

  // Iterate and populate the BindingList.

  for (CORBA::ULong i = 0; i < how_many; i++)
    {
      this->iterator_->next (server_entry);

      ACE_TString logical, server, command_line, working_directory, location, server_ior;
      ImplementationRepository::EnvironmentList environment_vars;
      ImplementationRepository::ActivationMode activation =
        ImplementationRepository::NORMAL;

      server_entry->int_id_->get_running_info (location, server_ior);
      server_entry->int_id_->get_startup_info (logical,
                                               command_line,
                                               environment_vars,
                                               working_directory,
                                               activation);

      server_list[i].logical_server = CORBA::string_dup (logical.c_str ());
      server_list[i].server = CORBA::string_dup (server_entry->ext_id_.c_str ());
      server_list[i].startup.command_line = CORBA::string_dup (command_line.c_str ());
      server_list[i].startup.environment = environment_vars;
      server_list[i].startup.working_directory = CORBA::string_dup (working_directory.c_str ());
      server_list[i].startup.activation = activation;
      server_list[i].location = CORBA::string_dup (location.c_str ());

      if (this->iterator_->advance () == 0)
        {
          // If no more servers left, reset length to the actual
          // number servers and get out of the loop.
          server_list->length (i + 1);
          break;
        }
    }
  return 1;
}


// Destroys the iterator.

void
ImR_Iterator::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
   ACE_THROW_SPEC ((CORBA::SystemException))
{
}
