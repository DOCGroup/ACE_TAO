#include "LB_server.h"
#include "Basic.h"

ACE_RCSID (GenericFactory,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  // @@ Jai, why do you have this ACE_TRY block?  None of your code
  //    below uses it.  That is why you're getting an "unused
  //    ACE_TRY_FLAG" warning.
  ACE_TRY_NEW_ENV
    {
      const char *loc1 = "MyLocation 1";
      const char *loc2 = "MyLocation 2";

      LB_Basic_Test basic_test (argc, argv);

      if (basic_test.start_orb_and_poa () == -1)
        return 1;

      if (basic_test.create_object_group () == -1)
        return 1;

      // @@ You've enabled reference counting in this servant but you
      //    instantiate the servant on the stack!  Either disable
      //    reference counting by not inheriting from
      //    PortableServer::RefCountServantBase or instantiate these
      //    servants on the heap, and store the servant pointers in a
      //    PortableServer::ServantBase_var to eventually transfer
      //    ownership to the POA.  For example:
      //
      //         Basic * servant;
      //         ACE_NEW_THROW_EX (servant,
      //                           Basic (basic_test.orb ()),
      //                           CORBA::NO_MEMORY ());
      //         ACE_TRY_CHECK;
      //
      //         PortableServer::ServantBase_var safe_servant = servant;
      //
      //    When safe_servant goes out of scope, the reference count
      //    is decreased by one.  However, when the servant was
      //    activated it's reference count was incremented by one by
      //    the POA.  The net effect is that the POA ends up managing
      //    servant's memory.
      //
      //    If you really intended for the servants to be reference
      //    counted, you would have noticed this problem had you made
      //    the servant's destructor protected.  Doing so would have
      //    caused the compiler to choke on the below code since the
      //    it would attempt to generate code that calls a protected
      //    destructor, as if it was public.
      //
      //    Decide if you want to reference count the servant or not.
      //    As things are now, the POA will probably seg fault when
      //    attempt to clean up its resources.
      Basic basic_servant1 (basic_test.orb ());
      Basic basic_servant2 (basic_test.orb ());

      if (basic_test.register_servant (&basic_servant1, loc1) == -1)
        return 1;

      if (basic_test.register_servant (&basic_servant2, loc2) == -1)
        return 1;

      // @@ Jai, where's your orb->run() call?  You're client can't
      //    make invocations on the servants without it!  Presumably,
      //    the code should look like this:
      //
      //    basic_test.orb ()->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      //    ACE_TRY_CHECK;
      //
      //    Note that you'll need the ACE_TRY_NEW_ENV block if you run
      //    the ORB here.
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server exception");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
