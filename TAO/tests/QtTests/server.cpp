// $Id$

#include "test_i.h"
#include "ace/Get_Opt.h"

// who defines index macro?
#ifdef index
#undef index
#endif
#include "tao/QtResource/QtResource_Loader.h"
#include <qlcdnumber.h>
#include <qvbox.h>
#include <qslider.h>
#include "ace/OS_NS_stdio.h"


const ACE_TCHAR *ior_output_file = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
          // ignore the first unknown option
          return 0;
//         ACE_ERROR_RETURN ((LM_ERROR,
//                            "usage:  %s "
//                            "-o <iorfile>"
//                            "\n",
//                            argv [0]),
//                           -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  // We do the command line parsing first
  if (parse_args (argc, argv) != 0)
    return 1;

  // Qt specific stuff for running with TAO...
  QApplication app (argc, argv);
  TAO::QtResource_Loader qt_resources (&app);

  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      // Create the Qt stuff..
      // Instantiate the LCD_Display implementation class
      LCD_Display_imp display_impl (orb.in ());

      PortableServer::ObjectId_var id =
        root_poa->activate_object (&display_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      LCD_Display_var server =
        LCD_Display::_narrow (object.in ());

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
        orb->object_to_string (server.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

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

      poa_manager->activate ();

      // We choose to run the main Qt event loop..
      app.exec ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
  return 0;
}
