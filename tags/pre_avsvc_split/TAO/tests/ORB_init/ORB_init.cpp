// -*- C++ -*-
// $Id$

#include "tao/corba.h"

ACE_RCSID(ORB_init, ORB_init, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      const char orbid[] = "my_orb";

      CORBA::ORB_ptr my_orb = CORBA::ORB::_nil();

      {
        CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, orbid, ACE_TRY_ENV);
        ACE_TRY_CHECK;

        my_orb = orb.in();

        // Once we leave this scope, the ORB is released but it should 
        // be possible to obtain the same ORB with another call to
        // CORBA::ORB_init by using the same ORBid argument that was
        // assigned to this ORB.
      }

      // -------------------------------------------------------------
      // Verify that the same ORB is returned from a second call to
      // CORBA::ORB_init() in a different scope when the same ORBid is 
      // used in that scope.
      // -------------------------------------------------------------

      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, orbid, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // This isn't portable, but TAO implements an ORB_ptr as a
      // pointer so we're okay.
      //
      // Check if the ORB returned from the ORB_init() call is the
      // same.
      if (my_orb == orb.in ())
        {
          ACE_DEBUG ((LM_INFO,
                      "\n"
                      "The ORB <%s> was successfully returned from a second\n"
                      "call to CORBA::ORB_init() after it was released in\n"
                      "a previous scope.\n"
                        "\n",
                      orbid));
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "\n"
                             "ORB <%s> was not successfully returned from a\n"
                             "second call to CORBA::ORB_init() despite the\n"
                             "fact it wasn't explicitly destroyed.\n"
                             "\n",
                             orbid),
                            1);
        }

      // -------------------------------------------------------------
      // Now explicitly destroy the ORB with the given ORBid and
      // attempt to initialize a new ORB with the same ORBid.
      // -------------------------------------------------------------

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb = CORBA::ORB_init (argc, argv, orbid, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // This isn't portable, but TAO implements an ORB_ptr as a
      // pointer so we're okay.
      //
      // Check if the ORB returned from the ORB_init() call is the
      // same.
      if (my_orb != orb.in ())
        {
          ACE_DEBUG ((LM_INFO,
                      "\n"
                      "A new ORB with ORBid <%s> was successfully returned\n"
                      "from a second call to CORBA::ORB_init() after the\n"
                      "first ORB with the same ORBid was destroyed.\n"
                      "\n",
                      orbid));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      "\n"
                      "ORB <%s> was not successfully destroyed.\n"
                      "\n",
                      orbid));

          ACE_TRY_THROW (CORBA::INTERNAL ());
        }

      // -------------------------------------------------------------
      // Now try to perform an operation using the destroyed ORB
      // pseudo object.  A CORBA::OBJECT_NOT_EXIST() exception should
      // be thrown.  This also tests whether or not exceptions or the
      // ORB itself break when the last ORB is released.
      // -------------------------------------------------------------
      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // If we get here, then something went wrong.  A
      // CORBA::OBJECT_NOT_EXIST() exception should have been thrown!.
      ACE_ERROR ((LM_ERROR,
                  "\n"
                  "CORBA::OBJECT_NOT_EXIST() exception was not thrown\n"
                  "during attempt to perform an ORB operation using\n"
                  "destroyed ORB <%s>\n"
                  "The CORBA::OBJECT_NOT_EXIST should have been thrown!\n"
                  "\n",
                  orbid));

      ACE_TRY_THROW (CORBA::INTERNAL ());
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, exc)
    {
      // Do something with the exception to make sure it actually
      // exists.  If it doesn't exist then there is something wrong
      // with exception lifetime.
      ACE_DEBUG ((LM_INFO,
                  "\n"
                  "Successfully caught CORBA system exception after the\n"
                  "last ORB was released with the following repository ID:\n"
                  "  %s\n"
                  "This exception was expected.  It is safe to ignore it.\n",
                  exc._id ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught unexpected exception:");


      ACE_DEBUG ((LM_ERROR,
                  "\n"
                  "ORB_init test failed.\n"));

      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_INFO,
              "\n"
              "ORB_init test completed successfully.\n"));

  return 0;
}
