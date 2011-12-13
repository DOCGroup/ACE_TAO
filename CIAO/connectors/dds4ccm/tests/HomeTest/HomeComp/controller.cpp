// $Id$

//==============================================================
/**
 * @file controller.cpp
 *
 * This is a controller that interact with the Home-component implementation.
 * This test uses the explicit factory
 * operation in the home interface to create a connector component
 * instance, run it for a while, and destroy the component instance.
 */
//==============================================================

#include "HomeT_HomeCC.h"
#include "Connector/HomeT_ConnectorEC.h"
#include "ace/streams.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *home_comp_ior_ = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:of"));
  int c;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
           break;
        case 'f':
           break;
        case 'k':
         home_comp_ior_ = get_opts.opt_arg ();
          break;
        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "usage:  %s\n"
                             "-k <HomeComponent IOR> (default is file://HomeC.ior)\n"
                             "\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (home_comp_ior_ == 0)
    {
      home_comp_ior_ = ACE_TEXT("file://HomeC.ior");
    }

  return 0;
}


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
         {
           return -1;
         }

      // Resolve Home interface
      CORBA::Object_var obj
        = orb->string_to_object (home_comp_ior_);

      ConnHome_var home_comp
        = ConnHome::_narrow (obj.in ());
      if (CORBA::is_nil (home_comp.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire ConnHome objref\n"), -1);

      // starting Connector component
      ACE_DEBUG ((LM_DEBUG, "Controller: Start create Connector component\n"));

      CORBA::Object_var tmp  = home_comp->new_ConnComp ();
      ConnComp::DDS_Event_var ce = ConnComp::DDS_Event::_narrow (tmp.in ());

      if (CORBA::is_nil (ce.in ()))
        ACE_ERROR((LM_ERROR, "Unable to start create Connector component\n"));

      ACE_DEBUG ((LM_DEBUG, "Controller: Started Connector component\n"));

      // Place to plug in some connector settings
      ce->topic_name ("Shapes");
      ce->configuration_complete();
      // activate not possible
      //ce->ccm_activate ();

      ACE_OS::sleep (5);

      ce->remove ();

      ACE_DEBUG ((LM_DEBUG, "Controller: removed Connector component\n"));

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception\n");
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Uncaught CORBA exception\n"),
                        1);
    }

  return 0;
}
