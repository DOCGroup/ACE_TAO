// $Id$

//========================================================================
/**
 * @file  IORTest.cpp
 *
 * This program tests the basic functionality of the IORManipulation
 * interface.
 *
 * @author Fred Kuhns
 */
//=========================================================================


#include "tao/corbafwd.h"
#include "tao/ORB.h"
#include "tao/Environment.h"
#include "tao/IORManipulation/IORManip_Loader.h"


ACE_RCSID (IORManipluation,
           IORTest,
           "$Id$")

int
main (int argc, char *argv[])
{
  // The variable is declared volatile because, some compilers
  // optimizes this the wrong way. A case is point is the CC compiler
  // on IRIX.
  volatile int Status = 0;

  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "Running the IORManipulation Tests.\n"));

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Retrieve the ORB.
      CORBA::ORB_var orb_ = CORBA::ORB_init (argc,
                                             argv,
                                             ""
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **********************************************************************

      // Get an object reference for the ORBs IORManipulation object!
      CORBA::Object_var IORM =
        orb_->resolve_initial_references ("IORManipulation"
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      TAO_IOP::TAO_IOR_Manipulation_var iorm =
        TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ()
                                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **********************************************************************

      // Create a few fictitious IORs
      CORBA::Object_var name1 =
        orb_->string_to_object ("corbaloc:iiop:macarena.cs.wustl.edu:6060/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      CORBA::Object_var name2 =
        orb_->string_to_object ("corbaloc:iiop:tango.cs.wustl.edu:7070/xyz"
                                ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // **********************************************************************

      CORBA::String_var name1_ior =
        orb_->object_to_string (name1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "\tFirst made up IOR = %s\n", name1_ior.in ()));

      CORBA::String_var name2_ior =
        orb_->object_to_string (name2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG, "\tSecond made up IOR = %s\n", name2_ior.in ()));

      // **********************************************************************

      // Create IOR list for use with merge_iors.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length (2);
      iors [0] = name1;
      iors [1] = name2;

      CORBA::Object_var merged = iorm->merge_iors (iors ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var merged_ior =
        orb_->object_to_string (merged.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong count1 = iorm->get_profile_count (iors [0] ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong count2 = iorm->get_profile_count (iors [1] ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong count = iorm->get_profile_count (merged.in ()
                                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count != (count1 + count2))
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (merge_profiles): "
                    "Merged profile count incorrect!\n"));

      ACE_DEBUG ((LM_DEBUG, "\tMerged IOR(%d) = %s\n",
                  count,
                  merged_ior.in ()));

      // is_in_ior throws an exception if the intersection of the two
      // IORs is NULL.
      CORBA::ULong in_count = iorm->is_in_ior (merged.in (),
                                               name1.in ()
                                               ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count1 != in_count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (merge_iors): name1 is_in_ior returned profile "
                    "count bad (%d)!\n",
                    in_count));

      in_count = iorm->is_in_ior (merged.in (),
                                  name2.in ()
                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count2 != in_count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (merge_iors): name2 is_in_ior returned profile "
                    "count bad (%d)!\n",
                    in_count));

      // **********************************************************************

      // Verify ability to remove profiles from an IOR
      // First remove the second IOR from the merged IOR
      CORBA::Object_var just1 =
        iorm->remove_profiles (merged.in (), name2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var just1_ior =
        orb_->object_to_string (just1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      count = iorm->get_profile_count (just1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count1 != count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (merge_profiles): "
                    "removing last IOR after a merge_iors ()\n"));

      ACE_DEBUG ((LM_DEBUG,
                  "\tJust 1 IOR(%d) = %s\n",
                  count,
                  just1_ior.in ()));

      in_count = iorm->is_in_ior (just1.in (), name1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count1 != in_count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (merge_profiles): incorrect count returned for "
                    "is_in_ior"
                    "\nafter removal (%d)\n",
                    in_count));
      // **********************************************************************

      // Now try the add_profiles interface.
      CORBA::Object_var merged2 =
        iorm->add_profiles (just1.in (), name2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      count = iorm->get_profile_count (merged2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count != (count1 + count2))
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR: add_profile failed profile count test!\n"));

      CORBA::String_var merged2_ior =
        orb_->object_to_string (merged2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\tAdding 2 back in IOR(%d) = %s\n",
                  count,
                  merged2_ior.in ()));

      // is_in_ior throws an exception if the intersection of the two
      // IORs is NULL.
      in_count = iorm->is_in_ior (merged2.in (), name1. in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count1 != in_count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (add_profiles): is_in_ior returned profile "
                    "count bad (%d)!\n",
                    in_count));

      in_count = iorm->is_in_ior (merged2.in (), name2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count2 != in_count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (add_profiles): is_in_ior returned "
                    "profile count bad!\n"));

      // **********************************************************************

      CORBA::Object_var just2 =
        iorm->remove_profiles (merged2.in (), name1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var just2_ior =
        orb_->object_to_string (just2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      count = iorm->get_profile_count (just2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count2 != count)
        ACE_DEBUG ((LM_ERROR,
                    "ERROR removing last IOR after a merge_iors ()\n"));

      ACE_DEBUG ((LM_DEBUG, "\tJust 2 IOR(%d) = %s\n",
                  count,
                  just2_ior.in ()));

      in_count = iorm->is_in_ior (just2.in (), name2.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (count2 != in_count)
        ACE_DEBUG ((LM_ERROR,
                    "**ERROR (add_profiles): incorrect count (%d) "
                    "returned for is_in_ior after removal\n",
                    in_count));

      // all the primary tests worked, set status to 1
      Status = 1;
      // Finally generate an exception and quit.
      // This will generate a NotFound exception.
      in_count = iorm->is_in_ior (just2.in (), name1.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCH (TAO_IOP::EmptyProfileList, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                           "Unexpected EmptyProfileList Exception!\n");
      return -1;
    }
  ACE_CATCH (TAO_IOP::NotFound, userex)
    {
      if (Status == 1)
        {
          Status = 2;

          // @@ Don't use ACE_PRINT_EXCEPTION here since it will print
          //    "EXCEPTION" somewhere in the output which will make
          //    our auto-compile/test scripts think that an unexpected
          //    exception occurred.  Instead, simply print the
          //    exception ID.
          //       -Ossama
          ACE_DEBUG ((LM_DEBUG,
                      "Caught <TAO_IOP::TAO_IOR_Manipulation::NotFound> exception.\n"
                      "This exception was expected.\n\n"));
        }
      else
        {
          ACE_PRINT_EXCEPTION (userex,
                               "Unexpected NotFound Exception!\n");
          return -1;
        }
    }
  ACE_CATCH (TAO_IOP::Duplicate, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                           "Unexpected Duplicate Exception!\n");
      return -1;
    }
  ACE_CATCH (TAO_IOP::Invalid_IOR, userex)
    {
      ACE_PRINT_EXCEPTION (userex,
                           "Unexpected Invalid_IOR Exception!\n");
      return -1;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex,
                           "Unexpected system Exception!!\n");
      return -1;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Unexpected ACE_CATCHANY Exception!\n");
      return -1;
    }
  ACE_ENDTRY;

  if (Status == 1)
    {
      // then exception didn't work!
      ACE_DEBUG ((LM_ERROR,
                  "**ERROR: Unable to generate the NotFound exception!\n"));
      Status = 0;
    }
  if (Status == 0)
    ACE_DEBUG ((LM_DEBUG, "An ERROR occured during the tests!\n"));
  else
    ACE_DEBUG ((LM_DEBUG, "IORManipulation Tests Successfully Completed!\n"));
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));

  return 0;
}
