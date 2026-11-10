//=============================================================================
/**
 *  @file     client.cpp
 *
 *    A client program for the Multiple Inheritance module
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================
#include <memory>

#include "Multiple_InheritanceC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Read_Buffer.h"
#include "ace/SString.h"

struct Client_Options
{
  ACE_TCHAR const *ior {};
  ACE_TCHAR const *ior_input_file {};
};

static int
parse_args (int argc, ACE_TCHAR **argv, Client_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:f:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        options.ior = get_opts.opt_arg ();
        break;
      case 'f':
        options.ior_input_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("\nusage: %s ")
                           ACE_TEXT ("-f <ior_input_file> or -k IOR\n"),
                           argv [0]),
                          -1);
      }

  if (options.ior == nullptr && options.ior_input_file == nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nPlease specify the IOR or IOR input file"
                       " for the servant"),
                      -1);
  if (options.ior != nullptr && options.ior_input_file != nullptr)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nPlease specify only an IOR or only an IOR"
                       " input file but not both"),
                      -1);

  // Indicates successful parsing of the command line.
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Client_Options options;
  try
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Parse the command-line arguments to get the IOR
      if (parse_args (argc, argv, options) != 0)
        return 1;

      // If ior_input_file exists, Read the file, and get the IOR
      // else, it must have been specified on the command line
      ACE_TString ior;
      if (options.ior != nullptr)
        ior = options.ior;
      if (options.ior_input_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> input_file (
            ACE_OS::fopen (options.ior_input_file, ACE_TEXT ("r")), ACE_OS::fclose);
          if (!input_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open input file for reading IOR: %s\n"),
                               options.ior_input_file),
                              -1);
          ACE_Read_Buffer ior_buffer (input_file.get ());
          char * const data = ior_buffer.read ();
          if (data == nullptr)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Unable to read ior\n")),
                              -1);
          ior = ACE_TEXT_CHAR_TO_TCHAR (data);
          ior_buffer.alloc ()->free (data);
        }

      // Get the object reference with the IOR
      CORBA::Object_var object = orb->string_to_object (ior.c_str ());

      CORBA::String_var string;

      A_var a = A::_narrow (object.in ());

      intB_var b = intB::_narrow (a.in ());

      C_var c = C::_narrow (a.in ());

      D_var d = D::_narrow (c.in ());

      string = a->method1 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = b->method1 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = b->method2 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = c->method1 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = c->method3 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = d->method1 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = d->method2 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = d->method3 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));

      string = d->method4 ();
      ACE_DEBUG ((LM_DEBUG, "%C\n", string.in ()));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("client");
      return 1;
    }

  return 0;
}
