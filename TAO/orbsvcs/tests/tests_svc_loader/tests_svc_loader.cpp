
//=============================================================================
/**
 *  @file    tests_svc_loader.cpp
 *
 * This directory contains a client that checks if a given object
 * reference points to an existing object or not and prints a debug
 * statement to reflect the same. This client is to be used in
 * conjunction with testing the dynamically loadable services. If
 * the service is loaded successfully, the object reference from the
 * server would be a valid one and the corresponding debug statement
 * is printed out. Or viceversa.
 *
 *  @author  Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================


#include "tao/ORB.h"
#include "tao/Object.h"
#include "tao/SystemException.h"

#include "ace/Log_Msg.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // First initialize the ORB, that will remove some arguments...
      CORBA::ORB_var const orb =
        CORBA::ORB_init (argc, argv);

      // There must be at least one argument, the file that has to be
      // retrieved
      if (argc < 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Usage: %s <ior>\n"),
                      argv[0]));

          return 1;
        }

      // Use the first argument to create the object reference.
      CORBA::Object_var const object =
        orb->string_to_object (argv[1]);

      // Check if this object reference is a valid one..
      CORBA::Boolean const not_exists =
        object->_non_existent ();

      if (not_exists)
        {
          // Object reference was not of an existing object
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("The Object is non existent\n")));
        }
      else
        {
          // The Object exists
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("The object exists!!!\n")));
        }

    }
  catch (CORBA::SystemException const &)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("CORBA System Exception Raised!\n")));
      return 1;
    }

  return 0;
}
