// $Id$

#include "ace/Service_Config.h"
#include "ace/Read_Buffer.h"
#include "ace/Thread_Manager.h"
#include "ace/streams.h"  /* Because dump () uses ofstream. */
#include "Dump_Restore.h"

ACE_RCSID(Dump_Restore, Dump_Restore, "$Id$")

Dump_Restore::Dump_Restore (int argc, char *argv[])
  : infile_ (0)
{
  ACE_NEW (this->ns_context_, ACE_Naming_Context);

  // Cache the name options
  this->name_options_ = this->ns_context_->name_options ();
  this->name_options_->parse_args (argc, argv);

  //determine name context
  if (ACE_OS::strcmp (this->name_options_->nameserver_host (), "localhost") == 0)
    {
      if (ns_context_->open (ACE_Naming_Context::PROC_LOCAL) == -1)
        ACE_ERROR ( (LM_ERROR, "%p\n", "ns_context_->open"));
    }
  else
    {
      // Don't really need to do this but it's a hack to fix
      // the problme of Display () not printing the right hostname
      ACE_OS::strcpy (this->hostname_,
                      this->name_options_->nameserver_host ());
      this->port_ = this->name_options_->nameserver_port ();

      if (this->ns_context_->open (ACE_Naming_Context::NET_LOCAL) == -1)
        ACE_ERROR ((LM_ERROR, "%p\n", "ns_context_->open"));
    }

  this->display_menu ();

  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR, "%p\n", "register_stdin_handler"));
}

Dump_Restore::~Dump_Restore (void)
{
  // Deregister this handler with the ACE_Reactor.
  ACE_Reactor::instance ()->remove_handler
    (ACE_STDIN,
     ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK);

  ACE_OS::fclose (this->infile_);
}

int
Dump_Restore::handle_input (ACE_HANDLE)
{
  char option[BUFSIZ];
  char buf1[BUFSIZ];
  u_short port;

  if (::scanf ("%s", option) <= 0)
    {
      ACE_DEBUG ((LM_ERROR, "try again\n"));
      return 0;
    }

  switch (option[0])
    {
    case 'P' :
    case 'p' :
      set_proc_local ();
      break;
    case 'N' :
    case 'n' :
      set_node_local ();
      break;
    case 'H' :
    case 'h' :
      if (::scanf ("%s %hu", buf1, &port) <= 0)
        break;
      set_host (buf1, port);
      break;
    case 'F':
    case 'f':
      if (::scanf ("%s", filename_) <= 0)
        break;
      if (this->infile_)
        ACE_OS::fclose (this->infile_);
      this->infile_ = fopen(filename_,"r");
      break;
    case 'B' :
    case 'b' :
      populate (Dump_Restore::BIND);
      break;
    case 'U' :
    case 'u' :
      populate (Dump_Restore::UNBIND);
      break;
    case 'R' :
    case 'r' :
      populate (Dump_Restore::REBIND);
      break;
    case 'D':
    case 'd':
      if (::scanf ("%s", dump_filename_) <= 0)
        break;
      this->dump ();
      break;
    case 'Q' :
    case 'q' :
      quit ();
      break;
    default :
      ACE_DEBUG ((LM_DEBUG, "Unrecognized command.\n"));
    }

  display_menu ();
  return 0;
}

void
Dump_Restore::display_menu (void)
{
  ACE_DEBUG ((LM_DEBUG, "\n"));
  ACE_DEBUG ((LM_DEBUG, "Name Service Main Menu\n"));
  ACE_DEBUG ((LM_DEBUG, "----------------------\n"));

  // Check if using local name space or remote name space
  if (ACE_OS::strcmp (this->name_options_->nameserver_host (), "localhost") == 0)
    {
      if (this->ns_scope_ == ACE_Naming_Context::PROC_LOCAL)
        ACE_DEBUG ((LM_DEBUG, "  *** Using Process Local Database ***\n\n"));
      else
        ACE_DEBUG ((LM_DEBUG, "  *** Using Node Local Database ***\n\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "  Hostname: %s\n", this->hostname_));
      ACE_DEBUG ((LM_DEBUG, "  Port Number: %d\n", this->port_));
    }

  if (this->infile_)
    ACE_DEBUG ((LM_DEBUG, "Input File: %s\n", this->filename_));
  else
    ACE_DEBUG ((LM_DEBUG, "** No Input File Specified **\n"));

  ACE_DEBUG ((LM_DEBUG, "<P> Use Process Local Database\n"));
  ACE_DEBUG ((LM_DEBUG, "<N> Use Node Local Database\n"));
  ACE_DEBUG ((LM_DEBUG, "<H> Set Remote Name server <host> and <port>\n"));
  ACE_DEBUG ((LM_DEBUG, "<F> Set Input File <file name>\n"));
  ACE_DEBUG ((LM_DEBUG, "<B> Bind\n"));
  ACE_DEBUG ((LM_DEBUG, "<U> Unbind\n"));
  ACE_DEBUG ((LM_DEBUG, "<R> Rebind\n"));
  ACE_DEBUG ((LM_DEBUG, "<D> Dump <file name>\n"));
  ACE_DEBUG ((LM_DEBUG, "<Q> or ^C (exit) \n"));
}


int
Dump_Restore::set_proc_local (void)
{
  // Set Name Options
  this->name_options_->nameserver_host ("localhost");
  this->name_options_->nameserver_port (0);

  // Set Naming Context scope
  this->ns_scope_ = ACE_Naming_Context::PROC_LOCAL;

  // Remove old naming context
  delete this->ns_context_;

  // Create new Naming Context
  ACE_NEW_RETURN (this->ns_context_, ACE_Naming_Context, -1);

  if (this->ns_context_->open (ACE_Naming_Context::PROC_LOCAL) == -1)
    ACE_ERROR_RETURN ( (LM_ERROR, "%p\n", "ns_context_->open"), -1);

  return 0;
}

int
Dump_Restore::set_node_local (void)
{
  // Set Name Options
  this->name_options_->nameserver_host ("localhost");
  this->name_options_->nameserver_port (0);

  // Set Naming Context scope
  this->ns_scope_ = ACE_Naming_Context::NODE_LOCAL;

  // Remove old naming context
  delete this->ns_context_;

  // Create new Naming Context
  ACE_NEW_RETURN (this->ns_context_, ACE_Naming_Context, -1);

  if (ns_context_->open (ACE_Naming_Context::NODE_LOCAL) == -1)
    ACE_ERROR_RETURN ( (LM_ERROR, "%p\n", "ns_context_->open"), -1);
  return 0;
}

int
Dump_Restore::set_host (char* hostname, int port)
{
  // Set Name Options
  this->name_options_->nameserver_host (hostname);
  this->name_options_->nameserver_port (port);

  // don't really need to do this but it's a hack to fix
  // the problme of Display () not printing the right hostname
  ACE_OS::strcpy (this->hostname_, hostname);
  this->port_ = port;
  this->ns_scope_ = ACE_Naming_Context::NET_LOCAL;

  // remove old naming context
  delete this->ns_context_;

  // Create new Naming Context
  ACE_NEW_RETURN (this->ns_context_, ACE_Naming_Context, -1);

  // assume net_local context
  if (ns_context_->open (ACE_Naming_Context::NET_LOCAL) == -1)
    ACE_ERROR_RETURN ( (LM_ERROR, "%p\n", "ns_context_->open"), -1);

  return 0;
}

int
Dump_Restore::doit (Dump_Restore::Operation_Type op,
                    char *name,
                    char* value,
                    char* type)
{
  int result = -1;

  switch (op)
    {
    case Dump_Restore::BIND:
      {
        result = this->bind (name, value, type);
        break;
      }
    case Dump_Restore::UNBIND:
      {
        result = this->unbind (name);
        break;
      }
    case Dump_Restore::REBIND:
      {
        result = this->rebind (name, value, type);
        break;
      }
    }

  return result;
}

int
Dump_Restore::populate (Dump_Restore::Operation_Type op)
{
  if (this->infile_)
    {
      int result = -1;
      enum State { NAME, VALUE, TYPE };

      State state = NAME;
      // reset file pointer
      ACE_OS::rewind (this->infile_);

      ACE_Allocator *allocator = ACE_Allocator::instance ();
      ACE_Read_Buffer read_buffer (this->infile_, 0, allocator);

      for (char *temp; (temp = read_buffer.read ('\n')) != 0; )
        {
          char *name = 0;
          char *actual_name = 0;
          char *value = 0;
          char *actual_value = 0;
          char *type = 0;
          char *actual_type = 0;

          switch (state)
            {
            case NAME:
              name = temp;
              ACE_OS::strtok (name, "=");
              actual_name = ACE_OS::strtok (0, "=");
              state = VALUE;
              break;
            case VALUE:
              value = temp;
              ACE_OS::strtok (value, "=");
              actual_value = ACE_OS::strtok (0, "=");
              state = TYPE;
              break;
            case TYPE:
              type = temp;
              ACE_OS::strtok (type, "=");
              actual_type = ACE_OS::strtok (0, "=");

              if (actual_type)
                result = this->doit (op,
                                     actual_name,
                                     actual_value,
                                     actual_type);
              else
                result = this->doit (op,
                                     actual_name,
                                     actual_value);
              if (name)
                allocator->free(name);
              if (value)
                allocator->free(value);
              if (type)
                allocator->free(type);
              state = NAME;
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "populate"), -1);
              /* NOTREACHED */
            }
        }

      return result;
    }
  else
    return -1;
}

int
Dump_Restore::bind (char* key, char* value, char* type)
{
  int result = ns_context_->bind (key, value, type);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ns_context_->bind"), -1);
  else if (result == 1)
    ACE_ERROR_RETURN ((LM_ERROR, "%s%s%s\n", "key <", key, "> already bound"), 1);
  return 0;
}

int
Dump_Restore::unbind (char* key)
{
  int result = ns_context_->unbind (key);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ns_context_->unbind"), -1);

  return 0;
}

int
Dump_Restore::rebind (char* key, char* value, char* type)
{
  if (ns_context_->rebind (key, value, type) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ns_context_->rebind"), -1);

  return 0;
}

int
Dump_Restore::quit (void)
{
  return ACE_OS::kill (ACE_OS::getpid (), SIGINT);
}

void
Dump_Restore::dump (void)
{
  ofstream output_file (dump_filename_);

  ostream *orig_stream = ACE_Log_Msg::instance ()->msg_ostream ();
  ACE_Log_Msg::instance ()->msg_ostream (&output_file);
  ACE_Log_Msg::instance ()->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER );
  ACE_Log_Msg::instance ()->set_flags (ACE_Log_Msg::OSTREAM);

  ns_context_->dump ();

  ACE_Log_Msg::instance ()->msg_ostream (orig_stream);
  ACE_Log_Msg::instance ()->clr_flags (ACE_Log_Msg::STDERR);
}
