// -*- C++ -*-
#include "ace/Event_Handler.h"
#include "ace/Naming_Context.h"
#include <string>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Client_Test : public ACE_Event_Handler
{
public:
  Client_Test ();

  Client_Test (Client_Test const &) = delete;
  Client_Test (Client_Test &&) = delete;
  Client_Test &operator= (Client_Test const &) = delete;
  Client_Test &operator= (Client_Test &&) = delete;

  int open ();
  // Cache reactor and then register self with reactor

  int close ();
  // Close things down and free up resources.

  int handle_input (ACE_HANDLE handle) override;
  // Handle user entered commands

  void list_options ();
  // Print name options

  int bind (std::string const &key,
            std::string const &value,
            std::string const &type = {});
  // Bind a key to a value

  int unbind (std::string const &key);
  // Unbind a name binding

  int rebind (std::string const &key,
              std::string const &value,
              std::string const &type = {});
  // Rebind a name binding

  int find (std::string const &key);
  // Find the value associated with a key

  int list_names (std::string const &pattern);
  // Find all names that match pattern

  int list_values (std::string const &pattern);
  // Find all values that match pattern

  int list_types (std::string const &pattern);
  // Find all types that match pattern

  int list_name_entries (std::string const &pattern);
  // Find all names that match pattern

  int list_value_entries (std::string const &pattern);
  // Find all values that match pattern

  int list_type_entries (std::string const &pattern);
  // Find all types that match pattern

private:
  ACE_Name_Options *name_options_ {};
  // Borrowed from the dynamically loaded ACE_Naming_Context.

  void display_menu ();
  // Display user menu

  int set_proc_local ();
  // Set options to use PROC_LOCAL naming context

  int set_node_local ();
  // Set options to use NODE_LOCAL naming context

  int set_host (std::string const &hostname, int port);
  // Set options to use NET_LOCAL naming context specifying host name
  // and port number
};
