// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/IORManipultion
//
//
// = FILENAME
//     IORTest.cpp
//
// = DESCRIPTION
//   This program tests the basic functionality of the 
//   IOR Manipultion interface.
//
// = AUTHOR
//     Fred Kuhns
//
//=========================================================================


#include "tao/corbafwd.h"
#include "tao/ORB.h"
#include "tao/IORManipulation.h"
#include "tao/corbafwd.h"
#include "tao/ORB.h"
#include "tao/IORManipulation.h"
#include "tao/IORS.h"
#include "ace/SString.h"

ACE_RCSID(IORManipluation, IORTest, "$Id$")

main (int argc, char **argv)
{

  int Status = 0;
  CORBA::ORB_var orb_;
  printf ("----------------------------------\n");
  printf ("Running the IORManipulation Tests.\n");

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      // Retrieve the ORB.
      orb_ = CORBA::ORB_init (argc,
                              argv,
                              0,
                              ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // ***********************************************************************

      // Get an object reference for the ORBs IORManipultion object!
      CORBA_Object_ptr IORM =
        orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION,
                                          0,
                                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      TAO_IOP::TAO_IOR_Manipulation_ptr iorm = 
               TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // ***********************************************************************
    
      // Create a few fictitious IORs
      CORBA_Object_ptr name1 = 
        orb_->string_to_object ("iiop://acme.cs.wustl.edu:6060/xyz", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA_Object_ptr name2 =
        orb_->string_to_object ("iiop://tango.cs.wustl.edu:7070/xyz", ACE_TRY_ENV);
      ACE_TRY_CHECK;
      // ***********************************************************************
    
      // Get the string reps for these IORs and show them to the user
      ACE_CString iorm_ior = orb_->object_to_string (IORM, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      printf("\tIORManipultion IOR string = %s\n", iorm_ior.c_str());
    
      ACE_CString name1_ior = orb_->object_to_string (name1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      printf("\tFirst made up IOR = %s\n", name1_ior.c_str());

      ACE_CString name2_ior = orb_->object_to_string (name2, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      printf("\tSecond made up IOR = %s\n", name2_ior.c_str());
    
      // ***********************************************************************
      // Create IOR list for use with merge_iors.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length(2);
      iors [0] = name1;
      iors [1] = name2;
    
      CORBA_Object_ptr merged = iorm->merge_iors (iors, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    
      ACE_CString merged_ior = orb_->object_to_string (merged, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ULong count1 = iorm->get_profile_count (iors [0], ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ULong count2 = iorm->get_profile_count (iors [1], ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::ULong count = iorm->get_profile_count (merged, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count != (count1 + count2))
        printf("**ERROR (merge_profiles): Merged profile count incorrect!\n");

      printf("\tMerged IOR(%d) = %s\n", count, merged_ior.c_str());

      // is_in_ior throws an exception if the intersection of the two
      // IORs is NULL.
      CORBA::ULong in_count = iorm->is_in_ior (merged, name1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count1 != in_count)
        printf ("**ERROR (merge_iors): name1 is_in_ior returned profile count bad (%d)!\n",
                in_count);

      in_count = iorm->is_in_ior (merged, name2, ACE_TRY_ENV); 
      ACE_TRY_CHECK;

      if (count2 != in_count)
        printf ("**ERROR (merge_iors): name2 is_in_ior returned profile count bad (%d)!\n",
                in_count);
      // ***********************************************************************
    
      // Verify ability to remove profiles from an IOR
      // First remove the second IOR from the merged IOR
      CORBA_Object_ptr just1 = iorm->remove_profiles (merged, name2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_CString just1_ior = orb_->object_to_string (just1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      count = iorm->get_profile_count (just1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count1 != count)
        printf ("**ERROR (merge_profiles): removing last IOR after a merge_iors ()\n");

      printf("\tJust 1 IOR(%d) = %s\n", count, just1_ior.c_str ());

      in_count = iorm->is_in_ior (just1, name1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count1 != in_count)
        printf ("**ERROR (merge_profiles): incorrect count returned for is_in_ior\
                \nafter removal (%d)\n", in_count);
      // ***********************************************************************
    
      // Now try the add_profiles interface.
      CORBA_Object_ptr merged2 = iorm->add_profiles (just1, name2, ACE_TRY_ENV);
      ACE_TRY_CHECK;
      
      count = iorm->get_profile_count (merged2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count != (count1 + count2))
        printf("**ERROR: add_profile failed profile count test!\n");

      ACE_CString merged2_ior = orb_->object_to_string (merged2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      printf("\tAdding 2 back in IOR(%d) = %s\n", count, merged2_ior.c_str ());
      
      // is_in_ior throws an exception if the intersection of the two
      // IORs is NULL.
      in_count = iorm->is_in_ior (merged2, name1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count1 != in_count)
        printf ("**ERROR (add_profiles): is_in_ior returned profile count bad (%d)!\n",
                 in_count);

      in_count = iorm->is_in_ior (merged2, name2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count2 != in_count)
        printf ("**ERROR (add_profiles): is_in_ior returned profile count bad!\n");
      // ***********************************************************************
    
      CORBA_Object_ptr just2 = iorm->remove_profiles (merged2, name1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_CString just2_ior = orb_->object_to_string (just2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      count = iorm->get_profile_count (just2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count2 != count)
        printf ("ERROR removing last IOR after a merge_iors ()\n");

      printf("\tJust 2 IOR(%d) = %s\n", count, just2_ior.c_str ());

      in_count = iorm->is_in_ior (just2, name2, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (count2 != in_count)
        printf ("**ERROR (add_profiles): incorrect count returned for is_in_ior after\
                \nremoval\n", in_count);
      // ***********************************************************************
      // all the primary tests worked, set status to 1
      Status = 1;
      // Finally generate an exception and quit.
      // This will generate a NotFound exception.
      in_count = iorm->is_in_ior (just2, name1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

    }
    ACE_CATCH (TAO_IOP::TAO_IOR_Manipulation::EmptyProfileList, userex)
      {
        ACE_UNUSED_ARG (userex);
        printf ("Unexpected EmptyProfileList Exception!\n");
        return -1;
      }
    ACE_CATCH (TAO_IOP::TAO_IOR_Manipulation::NotFound, userex)
      {
        ACE_UNUSED_ARG (userex);
        if (Status == 1)
          {
            Status = 2;
            printf ("Caught NotFound Exception!\n");
          }
        else
          {
            printf ("Unexpected NotFound Exception!\n");
            return -1;
          }
      }
    ACE_CATCH (TAO_IOP::TAO_IOR_Manipulation::Duplicate, userex)
      {
        ACE_UNUSED_ARG (userex);
        printf ("Unexpected Duplicate Exception!\n");
        return -1;
      }
    ACE_CATCH (TAO_IOP::TAO_IOR_Manipulation::Invalid_IOR, userex)
      {
        ACE_UNUSED_ARG (userex);
        printf ("Unexpected Invalid_IOR Exception!\n");
        return -1;
      }
    ACE_CATCH (CORBA::SystemException, sysex)
      {
        ACE_UNUSED_ARG (sysex);
        printf ("Unexpected system Exception!!\n");
        return -1;
      }
    ACE_CATCHANY
      {
        printf("Unexpected ACE_CATCHANY Exception!\n");
        return -1;
      }
  ACE_ENDTRY;

  if (Status == 1)
    {
      // then exception didn't work!
      printf ("**ERROR: Unable to generate the NotFound exception!\n");
      Status = 0;
    }
  if (Status == 0)
    printf ("An ERROR occured during the tests!\n");
  else
    printf ("Tests Successfully Completed!\n");
  printf ("--------------------------------------\n");
      
  return 0;
}
