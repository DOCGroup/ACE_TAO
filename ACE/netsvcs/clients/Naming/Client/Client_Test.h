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

  /// Caches the reactor and registers this object with it.
  int open ();

  /// Closes things down and frees resources.
  int close ();

  /// Handles user-entered commands.
  int handle_input (ACE_HANDLE handle) override;

  /// Prints the available name options.
  void list_options ();

  /// Binds a key to a value.
  int bind (std::string const &key,
            std::string const &value,
            std::string const &type = {});

  /// Removes a name binding.
  int unbind (std::string const &key);

  /// Rebinds a name to a value.
  int rebind (std::string const &key,
              std::string const &value,
              std::string const &type = {});

  /// Finds the value associated with a key.
  int find (std::string const &key);

  /// Lists names that match a pattern.
  int list_names (std::string const &pattern);

  /// Lists values that match a pattern.
  int list_values (std::string const &pattern);

  /// Lists types that match a pattern.
  int list_types (std::string const &pattern);

  /// Lists names that match a pattern.
  int list_name_entries (std::string const &pattern);

  /// Lists values that match a pattern.
  int list_value_entries (std::string const &pattern);

  /// Lists types that match a pattern.
  int list_type_entries (std::string const &pattern);

private:
  /// Borrowed from the dynamically loaded ACE_Naming_Context.
  ACE_Name_Options *name_options_ {};

  /// Displays the user menu.
  void display_menu ();

  /// Selects the PROC_LOCAL naming context.
  int set_proc_local ();

  /// Selects the NODE_LOCAL naming context.
  int set_node_local ();

  /// Selects the NET_LOCAL naming context for a host and port.
  int set_host (std::string const &hostname, int port);
};
