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

#include "orbsvcs/AV/AVStreams_i.h"
/* #include "tao/tao.h" */
#include "tao/tao_util.h"

int
main (int argc, char ** argv)
{
  TAO_TRY
    {
      TAO_ORB_Manager m;

      m.init (argc, 
              argv,
              TAO_TRY_ENV);
      TAO_CHECK_ENV;

      TAO_MMDevice *mmdevice_impl = new TAO_MMDevice;
      TAO_CHECK_ENV;

      CORBA::String_var s;
      s = m.activate (mmdevice_impl,
                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      ACE_DEBUG ((LM_DEBUG,
                  "\nThe IOR is: <%s>\n", 
                  s.in ()));

      m.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, sysex)
    {
      TAO_TRY_ENV.print_exception ("System Exception");
      return -1;
    }
  TAO_CATCH (CORBA::UserException, userex)
    {
      TAO_TRY_ENV.print_exception ("User Exception");
      return -1;
    }
  TAO_ENDTRY;
  ACE_DEBUG ((LM_DEBUG, "\nServer is terminating"));
  return 0;
}

