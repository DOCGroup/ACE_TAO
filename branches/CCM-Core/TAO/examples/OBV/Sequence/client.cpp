// $Id$
//
//===================================================================
//  = LIBRARY
//      TAO/examples/OBV/Sequence
//
//  = FILENAME
//     client.cpp
//
//  = DESCRIPTION
//      A client program for the numerical series module
//
//  = AUTHOR
//     Nanbor Wang
//
//====================================================================

#include "ace/streams.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "seriesC.h"
#include "Registry_i.h"

ACE_RCSID(series, client, "$Id$")

static char *ior = 0;
static char *ior_input_file = 0;
static int do_shutdown = 0;

static int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:f:x");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case 'f':
        ior_input_file = get_opts.optarg;
        break;
      case 'x':
        do_shutdown = 1;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "\nusage:  %s "
                           "-i <ior_input_file> "
                           "-k IOR "
                           "\n",
                           argv [0]),
                          -1);
      }

  if (ior == 0 && ior_input_file == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nPlease specify the IOR or IOR input file"
                       " for the servant"),
                      -1);
  if (ior != 0 && ior_input_file != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "\nPlease specify only an IOR or only an IOR"
                       " input file but not both"),
                      -1);

  // Indicates successful parsing of the command line.
  return 0;
}

int
main (int argc, char **argv)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0,
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

#if 1
      CORBA_ValueFactory_ptr factory = new registry_factory;
      CORBA_ValueFactory_ptr prev_factory =
        orb->register_value_factory (factory->tao_repository_id (),
                                     factory);
      if (prev_factory)
        prev_factory->_remove_ref ();
      factory->_remove_ref ();
#endif /* 0 */

      // Parse the command-line arguments to get the IOR
      parse_args (argc, argv);

      // If ior_input_file exists, Read the file, and get the IOR
      // else, it must have been specified on the command line
      if (ior_input_file != 0)
        {
          ACE_HANDLE input_file = ACE_OS::open (ior_input_file, 0);
          if (input_file == ACE_INVALID_HANDLE)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open input file for reading IOR: %s\n",
                               ior_input_file),
                              -1);
          ACE_Read_Buffer ior_buffer (input_file);
          char *data = ior_buffer.read ();
          if (data == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Unable to read ior\n"),
                              -1);
          ior = ACE_OS::strdup (data);
          ior_buffer.alloc ()-> free (data);
          ACE_OS::close (input_file);
        }

      // Get the object reference with the IOR
      CORBA::Object_var object = orb->string_to_object (ior, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      series_var series_op = series::_narrow (object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      registry_impl reg;

      reg.value (5);

      registry_var retn;

      retn = series_op->cube (&reg, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (retn->value () == 125)
        {
          ACE_DEBUG ((LM_DEBUG, "Sent %d and received %d.  Operation cube succeeded.\n",
                      reg.value (), retn->value ()));
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "Sent %d and received %d.  Operation cube failed.\n",
                      reg.value (), retn->value ()));
        }


      // Now test a sequence of valuetype.
      registries reg_seq (10);

      reg_seq.length (5);

      for (int i = 0; i < 5; i++)
        {
          registry_var tmp = new registry_impl;

          tmp->value (i);

          reg_seq[i] = tmp;
        }

      registries_var ret_seq;

      ret_seq = series_op->sum (reg_seq, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      print_registries (reg_seq);
      print_registries (ret_seq.in ());

      if (do_shutdown)
        series_op->shutdown (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "client");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
