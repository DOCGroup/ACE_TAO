// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/AVStreams
//
// = FILENAME
//    server.cpp
//
// = DESCRIPTION 
//    Test server for the AVStreams demo
//   
// = AUTHORS
//   Sumedh Mungee <sumedh@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/AVStreamsC.h"
#include "ace/Get_Opt.h"

char *basic_stream_ctrl_key = 0;

int
parse_args (int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, "k:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
	basic_stream_ctrl_key = 
          ACE_OS::strdup (get_opts.optarg);
	break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-k basic_stream_ctrl_key]"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}


int
main (int argc, char **argv)
{
  if (parse_args (argc, argv) == -1)
    return -1;

  TAO_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            0,
                                            TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      CORBA::Object_var basic_stream_ctrl_object =
        orb->string_to_object (basic_stream_ctrl_key, 
                               TAO_TRY_ENV);
      TAO_CHECK_ENV;

      Basic_StreamCtrl_var basic_stream_ctrl =
        AVStreams::Basic_StreamCtrl::_narrow (basic_stream_ctrl_object,
                                              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (CORBA::is_nil (basic_stream_ctrl.in ()))
	{
	  ACE_ERROR_RETURN ((LM_ERROR,
			     "invalid key <%s>\n",
			     basic_stream_ctrl_key),
			    -1);
	}

      ACE_DEBUG ((LM_DEBUG, "Object received OK\n"));
      basic_stream_ctrl->push_event ("Hello world\n", 
                                     TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }

  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("AVStreams: client: ");
      return -1;
    }
  TAO_ENDTRY;
      

}
