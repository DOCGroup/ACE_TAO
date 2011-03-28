
//=============================================================================
/**
 *  @file    tests_svc_loader.cpp
 *
 *  $Id$
 *
 * This directory contains a client that checks if a given object
 * reference points to an existing object or not and prints a debug
 * statement to reflect the same. This client is to be used in
 * conjunction with testing the dynamically loadable services. If
 * the service is loaded successfully, the object reference from the
 * server would be a valid one and the corresponding debug statement
 * is printed out. Or viceversa.
 *
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"
#include "ace/CORBA_macros.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv [])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      // There must be at least one argument, the file that has to be
      // retrieved
      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Usage: %s <ior>\n",
                      argv[0]));

          return -1;
        }

      // Use the first argument to create the object reference.
      CORBA::Object_var object =
        orb->string_to_object (argv[1]);

      // Check if this object reference is a valid one..
      CORBA::Boolean not_exists =
        object->_non_existent ();

      if (not_exists)
        {
          // Object reference was not of an existing object
          ACE_DEBUG ((LM_ERROR,
                      "The Object is non existent\n"));
        }
      else
        {
          // The Object exists
          ACE_DEBUG ((LM_DEBUG,
                      "The object exists!!!\n"));
        }

    }
  catch (const CORBA::SystemException&)
    {
      ACE_DEBUG ((LM_ERROR,
                  "CORBA System Exception Raised!\n"));
    }

  return 0;
}
