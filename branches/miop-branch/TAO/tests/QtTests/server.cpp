// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"


ACE_RCSID (QtTests, server, "$Id$")

#if !defined (ACE_HAS_QT)

int
main (int, char *[])
{
  ACE_ERROR ((LM_INFO,
              "Qt not supported on this platform\n"));
  return 0;
}

#else

#include "tao/qt_resource.h"
#include <qlcdnumber.h>
#include <qvbox.h>
#include <qslider.h>


const char *ior_output_file = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  // We do the command line parsing first
  if (parse_args (argc, argv) != 0)
    return 1;

  // Qt specific stuff for running with TAO...
  QApplication app (argc, argv);
  TAO_QtResource_Factory::set_context (&app);

  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA" TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create the Qt stuff..
      // Instantiate the LCD_Display implementation class
      LCD_Display_imp display_impl (orb.in ());

      LCD_Display_var server =
        display_impl._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create the LCD after the QVbox is created.
      QVBox box;

      box.resize (145, 100);
      QLCDNumber lcd (2, &box, "lcd_display");

      // Connect the signal from the hosted servant with the public
      // SLOT method display () for the LCD Widget.

      QObject::connect (&display_impl,
                        SIGNAL (set_value (int)),
                        &lcd,
                        SLOT (display (int)));

      app.setMainWidget(&box);
      box.show ();

      // End of QT specific stuff..

      CORBA::String_var ior =
        orb->object_to_string (server.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
      {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
      }

      poa_manager->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // We choose to run the main Qt event loop..
      app.exec ();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

#endif /* ACE_HAS_QT */
