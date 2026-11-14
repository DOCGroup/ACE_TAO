#include "ace/Service_Config.h"
#include "ace/Naming_Context.h"
#include "ace/Dynamic_Service.h"
#include "ace/Reactor.h"
#include "ace/Thread_Manager.h"
#include "ace/Malloc_Base.h"

#include "Client_Test.h"

#include <cctype>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

// Get the instance of Name_Service using Dynamic_Service

//inline Name_Service *
//NAME_SERVICE ()

static ACE_Naming_Context *
NAMING_CONTEXT ()
{
  return ACE_Dynamic_Service<ACE_Naming_Context>::instance ("ACE_Naming_Context");
}

static void
free_naming_context_result (char *result)
{
#if defined (ACE_HAS_ALLOC_HOOKS)
  ACE_Allocator::instance ()->free (result);
#else
  delete [] result;
#endif
}

using Naming_Context_String = std::unique_ptr<char, decltype (&free_naming_context_result)>;

Client_Test::Client_Test ()
{
  ACE_DEBUG ((LM_DEBUG,
              "Client_Test::Client_Test\n"));
}

int
Client_Test::open ()
{
  // Cache the name options.
  this->name_options_ = NAMING_CONTEXT ()->name_options ();

  this->display_menu ();

  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "register_stdin_handler"),
                      -1);
  return 0;
}

int
Client_Test::close ()
{
  // Deregister this handler with the ACE_Reactor.
  return ACE_Reactor::instance ()->remove_handler
    (ACE_STDIN,
     ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK);
}

int
Client_Test::handle_input (ACE_HANDLE)
{
  std::string command_line;
  if (!std::getline (std::cin, command_line))
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Try again!\n",
                       "Client_Test::handle_input"),
                      0);

  std::istringstream input (command_line);
  std::string option;
  if (!(input >> option))
    {
      this->display_menu ();
      return 0;
    }

  int result {-1};
  char const command = static_cast<char> (
    std::tolower (static_cast<unsigned char> (option.front ())));

  switch (command)
    {
    case 'p':
      result = this->set_proc_local ();
      break;
    case 'n':
      result = this->set_node_local ();
      break;
    case 'h':
      {
        std::string hostname;
        int port {};
        if (!(input >> hostname >> port))
          break;
        result = this->set_host (hostname, port);
        break;
      }
    case 'b':
      {
        std::string key;
        std::string value;
        std::string type;
        if (input >> key >> value)
          {
            input >> type;
            result = this->bind (key, value, type);
          }
        else
          ACE_ERROR ((LM_ERROR,
                      "Bind Failed! Key and value not entered.\n"));
        break;
      }
    case 'u':
      {
        std::string key;
        if (!(input >> key))
          break;
        result = this->unbind (key);
        break;
      }
    case 'r':
      {
        std::string key;
        std::string value;
        std::string type;
        if (input >> key >> value)
          {
            input >> type;
            result = this->rebind (key, value, type);
          }
        else
          ACE_ERROR ((LM_ERROR,
                      "Rebind Failed! Key and value not entered.\n"));
        break;
      }
    case 'f':
    case 'j':
    case 'k':
    case 'l':
    case 'c':
    case 'd':
    case 'e':
      {
        std::string key_pattern;
        if (!(input >> key_pattern))
          break;
        switch (command)
          {
          case 'f':
            result = this->find (key_pattern);
            break;
          case 'j':
            result = this->list_names (key_pattern);
            break;
          case 'k':
            result = this->list_values (key_pattern);
            break;
          case 'l':
            result = this->list_types (key_pattern);
            break;
          case 'c':
            result = this->list_name_entries (key_pattern);
            break;
          case 'd':
            result = this->list_value_entries (key_pattern);
            break;
          case 'e':
            result = this->list_type_entries (key_pattern);
            break;
          }
        break;
      }
    case 'q':
      result = -1;
      break;
    default:
      ACE_DEBUG ((LM_DEBUG,
                  "Unrecognized command.\n"));
    }

  if (result == -1)
    ACE_Reactor::instance ()->end_reactor_event_loop ();
  else
    this->display_menu ();
  return result;
}

void
Client_Test::display_menu ()
{
  ACE_DEBUG ((LM_DEBUG, "\n"));
  this->list_options ();
  ACE_DEBUG ((LM_DEBUG, "         Name Service Main Menu\n"));
  ACE_DEBUG ((LM_DEBUG, "         ----------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "<P> Use Process Local Database\n"));
  ACE_DEBUG ((LM_DEBUG, "<N> Use Node Local Database\n"));
  ACE_DEBUG ((LM_DEBUG, "<H> Set Remote Name server <host> and <port>\n\n"));
  ACE_DEBUG ((LM_DEBUG, "<B> Bind <key> <value> [<type>]\n"));
  ACE_DEBUG ((LM_DEBUG, "<U> Unbind <key>\n"));
  ACE_DEBUG ((LM_DEBUG, "<R> Rebind <key> <value> [<type>]\n"));
  ACE_DEBUG ((LM_DEBUG, "<F> Find <key>\n"));
  ACE_DEBUG ((LM_DEBUG, "<J> Lookup keys matching <pattern>\n"));
  ACE_DEBUG ((LM_DEBUG, "<K> Lookup values matching <pattern>\n"));
  ACE_DEBUG ((LM_DEBUG, "<L> Lookup types matching <pattern>\n"));
  ACE_DEBUG ((LM_DEBUG, "<C> Complete lookup keys matching <pattern>\n"));
  ACE_DEBUG ((LM_DEBUG, "<D> Complete lookup values matching <pattern>\n"));
  ACE_DEBUG ((LM_DEBUG, "<E> Complete lookup types matching <pattern>\n"));

  ACE_DEBUG ((LM_DEBUG, "<Q> or ^C (exit)\n"));
}

void
Client_Test::list_options ()
{
  switch (this->name_options_->context ())
    {
    case ACE_Naming_Context::PROC_LOCAL:
      ACE_DEBUG ((LM_DEBUG,
                  "  *** Using Process Local Database\n"));
      break;
    case ACE_Naming_Context::NODE_LOCAL:
      ACE_DEBUG ((LM_DEBUG,
                  "  *** Using Node Local Database\n"));
      break;
    case ACE_Naming_Context::NET_LOCAL:
      ACE_DEBUG ((LM_DEBUG,
                  "  *** Hostname: %s\n",
                  this->name_options_->nameserver_host ()));
      ACE_DEBUG ((LM_DEBUG,
                  "  *** Port Number: %d\n",
                  this->name_options_->nameserver_port ()));
      break;
    default:
      ACE_ERROR ((LM_ERROR, "ERROR: shouldn't occur!\n"));
      break;
    }
  ACE_DEBUG ((LM_DEBUG,
              "  *** Namespace directory is %s ***\n",
              this->name_options_->namespace_dir ()));
}

int
Client_Test::set_proc_local ()
{
  // Close down original name space
  NAMING_CONTEXT ()->close ();
  this->name_options_->nameserver_host (ACE_TEXT ("localhost"));
  this->name_options_->context (ACE_Naming_Context::PROC_LOCAL);
  return NAMING_CONTEXT ()->open (ACE_Naming_Context::PROC_LOCAL);
}

int
Client_Test::set_node_local ()
{
  // Close down original name space
  NAMING_CONTEXT ()->close ();
  this->name_options_->nameserver_host (ACE_TEXT ("localhost"));
  this->name_options_->context (ACE_Naming_Context::NODE_LOCAL);
  return NAMING_CONTEXT ()->open (ACE_Naming_Context::NODE_LOCAL);
}

int
Client_Test::set_host (std::string const &hostname, int port)
{
  // Close down original name space
  NAMING_CONTEXT ()->close ();

  this->name_options_->context (ACE_Naming_Context::NET_LOCAL);
  // Set Name Options
  this->name_options_->nameserver_host (ACE_TEXT_CHAR_TO_TCHAR (hostname.c_str ()));
  this->name_options_->nameserver_port (port);

  return NAMING_CONTEXT ()->open (ACE_Naming_Context::NET_LOCAL);
}

int
Client_Test::bind (std::string const &key,
                   std::string const &value,
                   std::string const &type)
{
  if (NAMING_CONTEXT ()->bind (key.c_str (), value.c_str (), type.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Bind failed! Key %s exists\n",
                       "Client_Test::bind",
                       key.c_str ()),
                      0);
  return 0;
}

int
Client_Test::unbind (std::string const &key)
{
  if (NAMING_CONTEXT ()->unbind (key.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Unbind failed! Key %s not found\n",
                       "Client_Test::unbind",
                       key.c_str ()),
                      0);
  return 0;
}

int
Client_Test::rebind (std::string const &key,
                     std::string const &value,
                     std::string const &type)
{
  int const result = NAMING_CONTEXT ()->rebind (key.c_str (),
                                               value.c_str (),
                                               type.c_str ());
  return result == 1 ? 0 : result;
}

int
Client_Test::list_names (std::string const &pattern)
{
  ACE_PWSTRING_SET set;

  if (NAMING_CONTEXT ()->list_names (set, pattern.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Pattern matching failed!\n",
                       "Client_Test::list_names"),
                      0);
  else
    {
      ACE_PWSTRING_ITERATOR set_iterator (set);

      for (ACE_NS_WString *name {};
           set_iterator.next (name) != 0;
           set_iterator.advance ())
        {
          Naming_Context_String name_rep {name->char_rep (), free_naming_context_result};
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      name_rep ? name_rep.get () : ""));
        }
    }
  return 0;
}

int
Client_Test::list_values (std::string const &pattern)
{
  ACE_PWSTRING_SET set;

  if (NAMING_CONTEXT ()->list_values (set, pattern.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Pattern matching failed!\n",
                       "Client_Test::list_values"),
                      0);
  else
    {
      ACE_PWSTRING_ITERATOR set_iterator (set);

      for (ACE_NS_WString *value {};
           set_iterator.next (value) != 0;
           set_iterator.advance ())
        {
          Naming_Context_String value_rep {value->char_rep (), free_naming_context_result};
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      value_rep ? value_rep.get () : ""));
        }
    }
  return 0;
}

int
Client_Test::list_types (std::string const &pattern)
{
  ACE_PWSTRING_SET set;

  if (NAMING_CONTEXT ()->list_types (set, pattern.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Pattern matching failed!\n",
                       "Client_Test::list_types"),
                      0);
  else
    {
      ACE_PWSTRING_ITERATOR set_iterator (set);

      for (ACE_NS_WString *type {};
           set_iterator.next (type) != 0;
           set_iterator.advance ())
        {
          Naming_Context_String type_rep {type->char_rep (), free_naming_context_result};
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      type_rep ? type_rep.get () : ""));
        }
    }
  return 0;
}

int
Client_Test::list_name_entries (std::string const &pattern)
{
  ACE_BINDING_SET set;

  if (NAMING_CONTEXT ()->list_name_entries (set, pattern.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Pattern matching failed!\n",
                       "Client_Test::list_names"),
                      0);
  else
    {
      ACE_BINDING_ITERATOR set_iterator (set);

      for (ACE_Name_Binding *entry {};
           set_iterator.next (entry) != 0;
           set_iterator.advance ())
        {
          Naming_Context_String name_rep {entry->name_.char_rep (), free_naming_context_result};
          Naming_Context_String value_rep {entry->value_.char_rep (), free_naming_context_result};
          ACE_DEBUG ((LM_DEBUG,
                      "%s\t",
                      name_rep ? name_rep.get () : ""));
          ACE_DEBUG ((LM_DEBUG,
                      "%s\t",
                      value_rep ? value_rep.get () : ""));
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      entry->type_ ? entry->type_ : ""));
        }
    }
  return 0;
}

int
Client_Test::list_value_entries (std::string const &pattern)
{
  ACE_BINDING_SET set;

  if (NAMING_CONTEXT ()->list_value_entries (set, pattern.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Pattern matching failed!\n",
                       "Client_Test::list_values"),
                      0);
  else
    {
      ACE_BINDING_ITERATOR set_iterator (set);
      for (ACE_Name_Binding *entry {};
           set_iterator.next (entry) != 0;
           set_iterator.advance ())
        {
          Naming_Context_String name_rep {entry->name_.char_rep (), free_naming_context_result};
          Naming_Context_String value_rep {entry->value_.char_rep (), free_naming_context_result};
          ACE_DEBUG ((LM_DEBUG,
                      "%s\t",
                      name_rep ? name_rep.get () : ""));
          ACE_DEBUG ((LM_DEBUG,
                      "%s\t",
                      value_rep ? value_rep.get () : ""));
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      entry->type_ ? entry->type_ : ""));
        }
    }
  return 0;
}

int
Client_Test::list_type_entries (std::string const &pattern)
{
  ACE_BINDING_SET set;

  if (NAMING_CONTEXT ()->list_type_entries (set, pattern.c_str ()) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Pattern matching failed!\n",
                       "Client_Test::list_types"),
                      0);
  else
    {
      ACE_BINDING_ITERATOR set_iterator (set);

      for (ACE_Name_Binding *entry {};
           set_iterator.next (entry) != 0;
           set_iterator.advance ())
        {
          Naming_Context_String name_rep {entry->name_.char_rep (), free_naming_context_result};
          Naming_Context_String value_rep {entry->value_.char_rep (), free_naming_context_result};
          ACE_DEBUG ((LM_DEBUG,
                      "%s\t",
                      name_rep ? name_rep.get () : ""));
          ACE_DEBUG ((LM_DEBUG,
                      "%s\t",
                      value_rep ? value_rep.get () : ""));
          ACE_DEBUG ((LM_DEBUG,
                      "%s\n",
                      entry->type_ ? entry->type_ : ""));
        }
    }
  return 0;
}

int
Client_Test::find (std::string const &key)
{
  char *value {};
  char *type {};

  int const result = NAMING_CONTEXT ()->resolve (key.c_str (), value, type);
  Naming_Context_String value_owner {value, free_naming_context_result};
  Naming_Context_String type_owner {type, free_naming_context_result};

  if (result != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p Find failed! Key %s not found\n",
                       "Client_Test::list_find",
                       key.c_str ()),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              "Binding for %s : value = %s\ttype = %s\n",
              key.c_str (),
              value_owner.get (),
              type_owner ? type_owner.get () : ""));
  return 0;
}

