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
          ACE_ERROR_RETURN ((LM_ERROR,
                             "\n"
                             "ORB <%s> was not successfully destroyed.\n"
                             "\n",
                             orbid),
                            1);
        }

      // -------------------------------------------------------------
      // Now throw a bogus exception to test whether or not exceptions 
      // break when the last ORB is released.
      // -------------------------------------------------------------
      ACE_TRY_THROW (CORBA::UNKNOWN);
    }
  ACE_CATCH (CORBA::UNKNOWN, exc)
    {
      ACE_PRINT_EXCEPTION (exc,
                  "\n"
                  "Successfully caught CORBA system exception \"UNKNOWN\"\n"
                  "after the last ORB was released.\n"
                  "\n"
                  "The exception follows.  It is safe to ignore it.\n");
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught unexpected exception:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_INFO,
              "\n"
              "ORB_init test completed successfully.\n"));

  return 0;
}
