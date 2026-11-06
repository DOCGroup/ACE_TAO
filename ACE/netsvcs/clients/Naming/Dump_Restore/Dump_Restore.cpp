#include "ace/Malloc_Base.h"
#include "ace/Read_Buffer.h"
#include "ace/Thread_Manager.h"

// FUZZ: disable check_for_streams_include
#include "ace/streams.h"  /* Because dump () uses ofstream. */

#include "Dump_Restore.h"
#include "ace/OS_NS_signal.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_unistd.h"

#include <cctype>
#include <iostream>
#include <limits>
#include <sstream>

static void
free_read_buffer (char *buffer)
{
  ACE_Allocator::instance ()->free (buffer);
}

using Read_Buffer_String = std::unique_ptr<char, decltype (&free_read_buffer)>;

Dump_Restore::Dump_Restore (int argc, ACE_TCHAR *argv[])
  : ns_context_ (std::make_unique<ACE_Naming_Context> ())
{
  // Cache the name options
  this->name_options_ = this->ns_context_->name_options ();
  this->name_options_->parse_args (argc, argv);

  //determine name context
  if (ACE_OS::strcmp (this->name_options_->nameserver_host (),
                      ACE_TEXT ("localhost")) == 0)
    {
      if (ns_context_->open (ACE_Naming_Context::PROC_LOCAL) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("ns_context_->open")));
    }
  else
    {
      // Cache the host name for display.
      this->hostname_ = this->name_options_->nameserver_host ();
      this->port_ =
        this->name_options_->nameserver_port ();

      if (this->ns_context_->open (ACE_Naming_Context::NET_LOCAL) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("ns_context_->open")));
    }

  this->display_menu ();

  if (ACE_Event_Handler::register_stdin_handler (this,
                                                 ACE_Reactor::instance (),
                                                 ACE_Thread_Manager::instance ()) == -1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("register_stdin_handler")));
}

Dump_Restore::~Dump_Restore ()
{
  // Deregister this handler with the ACE_Reactor.
  ACE_Reactor::instance ()->remove_handler
    (ACE_STDIN,
     ACE_Event_Handler::DONT_CALL | ACE_Event_Handler::READ_MASK);
}

int
Dump_Restore::handle_input (ACE_HANDLE)
{
  std::string command_line;
  if (!std::getline (std::cin, command_line))
    {
      ACE_DEBUG ((LM_ERROR,
                  ACE_TEXT ("try again\n")));
      return 0;
    }

  std::istringstream input (command_line);
  std::string option;
  if (!(input >> option))
    {
      this->display_menu ();
      return 0;
    }

  char const command = static_cast<char> (
    std::tolower (static_cast<unsigned char> (option.front ())));
  switch (command)
    {
    case 'p':
      this->set_proc_local ();
      break;
    case 'n':
      this->set_node_local ();
      break;
    case 'h':
      {
        std::string hostname;
        unsigned int port {};
        if (input >> hostname >> port
            && port <= std::numeric_limits<u_short>::max ())
          this->set_host (ACE_TEXT_CHAR_TO_TCHAR (hostname.c_str ()),
                          static_cast<u_short> (port));
        break;
      }
    case 'f':
      {
        if (!(input >> this->filename_))
          break;
        this->infile_.reset (ACE_OS::fopen (ACE_TEXT_CHAR_TO_TCHAR (this->filename_.c_str ()),
                                            ACE_TEXT ("r")));
        if (!this->infile_)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("Unable to open input file %C\n"),
                      this->filename_.c_str ()));
        break;
      }
    case 'b':
      this->populate (Operation_Type::BIND);
      break;
    case 'u':
      this->populate (Operation_Type::UNBIND);
      break;
    case 'r':
      this->populate (Operation_Type::REBIND);
      break;
    case 'd':
      if (input >> this->dump_filename_)
        this->dump ();
      break;
    case 'q':
      this->quit ();
      break;
    default:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Unrecognized command.\n")));
    }

  this->display_menu ();
  return 0;
}

void
Dump_Restore::display_menu ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Name Service Main Menu\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("----------------------\n")));

  // Check if using local name space or remote name space
  if (ACE_OS::strcmp (this->name_options_->nameserver_host (),
                      ACE_TEXT ("localhost")) == 0)
    {
      if (this->ns_scope_ == ACE_Naming_Context::PROC_LOCAL)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("  *** Using Process Local Database ***\n\n")));
      else
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("  *** Using Node Local Database ***\n\n")));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("  Hostname: %s\n"),
                  this->hostname_.c_str ()));
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("  Port Number: %d\n"),
                  this->port_));
    }

  if (this->infile_)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Input File: %C\n"),
                this->filename_.c_str ()));
  else
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("** No Input File Specified **\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<P> Use Process Local Database\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<N> Use Node Local Database\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<H> Set Remote Name server <host> and <port>\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<F> Set Input File <file name>\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<B> Bind\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<U> Unbind\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<R> Rebind\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<D> Dump <file name>\n")));
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("<Q> or ^C (exit)\n")));
}

int
Dump_Restore::set_proc_local ()
{
  // Set Naming Context scope
  this->ns_scope_ =
    ACE_Naming_Context::PROC_LOCAL;

  // Remove old naming context
  this->ns_context_ = std::make_unique<ACE_Naming_Context> ();
  this->name_options_ = this->ns_context_->name_options ();
  this->name_options_->nameserver_host (ACE_TEXT ("localhost"));
  this->name_options_->nameserver_port (0);
  this->name_options_->context (ACE_Naming_Context::PROC_LOCAL);

  if (this->ns_context_->open (ACE_Naming_Context::PROC_LOCAL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ns_context_->open")),
                      -1);

  return 0;
}

int
Dump_Restore::set_node_local ()
{
  // Set Naming Context scope
  this->ns_scope_ = ACE_Naming_Context::NODE_LOCAL;

  // Remove old naming context
  this->ns_context_ = std::make_unique<ACE_Naming_Context> ();
  this->name_options_ = this->ns_context_->name_options ();
  this->name_options_->nameserver_host (ACE_TEXT ("localhost"));
  this->name_options_->nameserver_port (0);
  this->name_options_->context (ACE_Naming_Context::NODE_LOCAL);

  if (ns_context_->open (ACE_Naming_Context::NODE_LOCAL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ns_context_->open")),
                      -1);
  return 0;
}

int
Dump_Restore::set_host (ACE_TCHAR const *hostname,
                        int port)
{
  this->hostname_ = hostname;
  this->port_ = port;
  this->ns_scope_ = ACE_Naming_Context::NET_LOCAL;

  // remove old naming context
  this->ns_context_ = std::make_unique<ACE_Naming_Context> ();
  this->name_options_ = this->ns_context_->name_options ();
  this->name_options_->nameserver_host (hostname);
  this->name_options_->nameserver_port (port);
  this->name_options_->context (ACE_Naming_Context::NET_LOCAL);

  // assume net_local context
  if (ns_context_->open (ACE_Naming_Context::NET_LOCAL) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ns_context_->open")),
                      -1);
  return 0;
}

int
Dump_Restore::doit (Dump_Restore::Operation_Type op,
                    std::string const &name,
                    std::string const &value,
                    std::string const &type)
{
  switch (op)
    {
    case Operation_Type::BIND:
      return this->bind (name, value, type);
    case Operation_Type::UNBIND:
      return this->unbind (name);
    case Operation_Type::REBIND:
      return this->rebind (name, value, type);
    }
  return -1;
}

int
Dump_Restore::populate (Dump_Restore::Operation_Type op)
{
  if (this->infile_)
    {
      enum class State { NAME, VALUE, TYPE };
      State state = State::NAME;
      ACE_OS::rewind (this->infile_.get ());

      ACE_Read_Buffer read_buffer (this->infile_.get (),
                                   false,
                                   ACE_Allocator::instance ());
      std::string name;
      std::string value;
      std::string type;

      for (char *line {};
           (line = read_buffer.read ('\n')) != nullptr;)
        {
          Read_Buffer_String line_owner {line, free_read_buffer};
          std::string line_text {line_owner.get ()};
          if (!line_text.empty () && line_text.back () == '\r')
            line_text.pop_back ();

          std::string::size_type const separator = line_text.find ('=');
          if (separator == std::string::npos)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Invalid input line: %C\n"),
                               line_text.c_str ()),
                              -1);

          std::string const field = line_text.substr (0, separator);
          std::string const field_value = line_text.substr (separator + 1);

          switch (state)
            {
            case State::NAME:
              if (field != "name")
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("Expected name field, got %C\n"),
                                   field.c_str ()),
                                  -1);
              name = field_value;
              state = State::VALUE;
              break;
            case State::VALUE:
              if (field != "value")
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("Expected value field, got %C\n"),
                                   field.c_str ()),
                                  -1);
              value = field_value;
              state = State::TYPE;
              break;
            case State::TYPE:
              if (field != "type")
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("Expected type field, got %C\n"),
                                   field.c_str ()),
                                  -1);
              type = field_value;
              if (this->doit (op, name, value, type) != 0)
                return -1;
              name.clear ();
              value.clear ();
              type.clear ();
              state = State::NAME;
              break;
            }
        }

      if (state != State::NAME)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Incomplete name binding in input file\n")),
                          -1);
      return 0;
    }
  return -1;
}

int
Dump_Restore::bind (std::string const &key,
                    std::string const &value,
                    std::string const &type)
{
  int const result = this->ns_context_->bind (key.c_str (),
                                              value.c_str (),
                                              type.c_str ());
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ns_context_->bind")),
                      -1);
  else if (result == 1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%s%s%s\n"),
                       ACE_TEXT ("key <"),
                       ACE_TEXT_CHAR_TO_TCHAR (key.c_str ()),
                       ACE_TEXT ("> already bound")),
                      1);
  return 0;
}

int
Dump_Restore::unbind (std::string const &key)
{
  int const result = this->ns_context_->unbind (key.c_str ());

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ns_context_->unbind")),
                      -1);
  return 0;
}

int
Dump_Restore::rebind (std::string const &key,
                      std::string const &value,
                      std::string const &type)
{
  if (this->ns_context_->rebind (key.c_str (),
                                 value.c_str (),
                                 type.c_str ()) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ns_context_->rebind")),
                      -1);
  return 0;
}

int
Dump_Restore::quit ()
{
  return ACE_OS::kill (ACE_OS::getpid (), SIGINT);
}

void
Dump_Restore::dump ()
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
