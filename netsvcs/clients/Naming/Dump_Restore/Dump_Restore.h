/* -*- C++ -*- */
// $Id$

#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Reactor.h"
#include "ace/Naming_Context.h"

class Dump_Restore : public ACE_Event_Handler
{
public:
  enum Operation_Type
    {
      BIND,
      UNBIND,
      REBIND
    };
  Dump_Restore (int argc, char *argv[]);
  // Initialize name options and naming context

  ~Dump_Restore (void);

  virtual int handle_input (ACE_HANDLE handle);
  // Handle user entered commands

  void dump (void);

private:
  char hostname_[MAXHOSTNAMELEN + 1];
  // Cache the hostname and port number for remote case

  void display_menu (void);
  // Display user menu.

  int set_proc_local (void);
  // Set options to use PROC_LOCAL naming context.

  int set_node_local (void);
  // Set options to use NODE_LOCAL naming context.

  int set_host (char* hostname, int port);
  // Set options to use NET_LOCAL naming context specifying host name
  // and port number.

  int quit (void);
  // Gracefully exit.

  int populate (Dump_Restore::Operation_Type op);

  int doit (Dump_Restore::Operation_Type op,
            char *name,
            char *value,
            char *type = "");
  int bind (char* key, char* value, char* type = "");
  int unbind (char* key);
  int rebind (char* key, char* value, char* type = "");

  char filename_[MAXPATHLEN + 1];
  char dump_filename_[MAXPATHLEN + 1];

  u_short port_;
  // port server is listening on

  ACE_Naming_Context *ns_context_;
  // Current naming context

  ACE_Naming_Context::Context_Scope_Type ns_scope_;
  // Defines the scope of the naming context

  FILE *infile_;
  // input file

  ACE_Name_Options *name_options_;
  // Name Options associated with the Naming Context
};
