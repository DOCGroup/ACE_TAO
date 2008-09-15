//=============================================================================
/**
 *  @file    corbaname_client.cpp
 *
 *  $Id$
 *
 *   This implements a simple CORBA client to tests the
 *   corbaname: style IOR parser
 *
 *
 *  @author  Craig Rodrigues <crodrigu@bbn.com>
 */
//=============================================================================

#include "corbalocC.h"
#include "ace/Log_Msg.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{

  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if(argc < 2 )
        {
          ACE_ERROR_RETURN((LM_ERROR,
                            "Usage:\n  %s [corbaname URL]\n",
                            argv[0]),
                           1);
        }

      // Get an object reference using a corbaname: style URL
      CORBA::Object_var obj =
        orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (argv[1]));

      // Narrow
      corbaloc::Status_var factory =
        corbaloc::Status::_narrow (obj.in ());

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: Object reference <%s> is nil\n",
                             argv[1]),
                            -1);
        }

      // Invoke a request on the server
      CORBA::Boolean ret_value =
        factory->print_status ();

      if (ret_value != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "The server has not been contacted. Error!!\n",
                      0));
        }

      return 0;
    }
  catch (const CORBA::SystemException&)
    {
      //
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client");
    }

  return 0;
}
