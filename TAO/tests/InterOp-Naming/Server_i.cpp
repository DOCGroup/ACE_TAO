#include "Server_i.h"
#include "tao/debug.h"
#include "tao/IORTable/IORTable.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"

#include <memory>

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
  std::string ins;
};

// Parse the command-line arguments and set options.
static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("do:ni:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':
        ++TAO_debug_level;
        break;
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'n':
        break;
      case 'i':
        options.ins = ACE_TEXT_ALWAYS_CHAR (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s [-d] [-o <ior_output_file>] [-n] "
                                     "[-i <object_name>]\n"),
                           argv [0]),
                          -1);
      }

  return 0;
}

// Add the ObjectID:IOR mapping to the IOR table of the ORB.
int
Server_i::add_IOR_to_table (char const *ior, std::string const &ins)
{
  try
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Adding (KEY:IOR) %C:%C\n"),
                    ins.c_str (),
                    ior));

      CORBA::Object_var const table_object (
        this->orb_manager_.orb ()->resolve_initial_references ("IORTable"));
      IORTable::Table_var const adapter (
        IORTable::Table::_narrow (table_object.in ()));

      adapter->bind (ins.c_str (), ior);
    }
  catch (CORBA::SystemException const &ex)
    {
      ex._tao_print_exception ("Exception caught in add_IOR_to_table");
      return -1;
    }

  return 0;
}

// Initialize the server.
int
Server_i::init (int argc, ACE_TCHAR *argv[])
{
  // Initialize the ORB and create a child POA under the root POA.
  int const result = this->orb_manager_.init_child_poa (argc, argv, "child_poa");

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "init_child_poa"), -1);

  Server_Options options {};
  if (parse_args (argc, argv, options) != 0)
    return -1;

  try
    {
      CORBA::String_var const ior (
        this->orb_manager_.activate_under_child_poa ("INS_servant", &this->servant_));

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The IOR is: <%C>\n"), ior.in ()));

      if (!options.ins.empty () && this->add_IOR_to_table (ior.in (), options.ins) != 0)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("test_for_ins (): failed\n")), -1);

      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> ior_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!ior_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Unable to open %s for writing: %p\n"),
                               options.ior_output_file),
                              -1);
          if (ACE_OS::fprintf (ior_file.get (), "%s", ior.in ()) < 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Unable to write %s: %p\n"),
                               options.ior_output_file),
                              -1);
        }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception in activation of POA");
      return -1;
    }

  return 0;
}

int
Server_i::run ()
{
  // Run the main event loop for the ORB.
  int const result = this->orb_manager_.run ();

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Server_i::run")), -1);

  return 0;
}
