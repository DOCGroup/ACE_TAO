// -*- C++ -*-
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Reactor.h"
#include "ace/Naming_Context.h"
#include "ace/OS_NS_stdio.h"
#include <memory>
#include <string>

class Dump_Restore : public ACE_Event_Handler
{
public:
  enum class Operation_Type
    {
      BIND,
      UNBIND,
      REBIND
    };
  Dump_Restore (int argc, ACE_TCHAR *argv[]);
  // Initialize name options and naming context

  ~Dump_Restore ();

  int handle_input (ACE_HANDLE handle) override;
  // Handle user entered commands

  Dump_Restore (Dump_Restore const &) = delete;
  Dump_Restore (Dump_Restore &&) = delete;
  Dump_Restore &operator= (Dump_Restore const &) = delete;
  Dump_Restore &operator= (Dump_Restore &&) = delete;

  void dump ();

private:
  std::basic_string<ACE_TCHAR> hostname_;
  // Cache the hostname and port number for remote case

  void display_menu ();
  // Display user menu.

  int set_proc_local ();
  // Set options to use PROC_LOCAL naming context.

  int set_node_local ();
  // Set options to use NODE_LOCAL naming context.

  int set_host (ACE_TCHAR const *hostname,
                int port);
  // Set options to use NET_LOCAL naming context specifying host name
  // and port number.

  int quit ();
  // Gracefully exit.

  int populate (Dump_Restore::Operation_Type op);

  int doit (Dump_Restore::Operation_Type op,
            std::string const &name,
            std::string const &value,
            std::string const &type = {});
  int bind (std::string const &key,
            std::string const &value,
            std::string const &type = {});
  int unbind (std::string const &key);
  int rebind (std::string const &key,
              std::string const &value,
              std::string const &type = {});

  std::string filename_;
  std::string dump_filename_;

  u_short port_ {};
  // port server is listening on

  std::unique_ptr<ACE_Naming_Context> ns_context_;
  // Current naming context

  ACE_Naming_Context::Context_Scope_Type ns_scope_ {ACE_Naming_Context::PROC_LOCAL};
  // Defines the scope of the naming context

  std::unique_ptr<FILE, int (*) (FILE *)> infile_ {nullptr, ACE_OS::fclose};
  // input file

  ACE_Name_Options *name_options_ {};
  // Borrowed from ns_context_.
};
