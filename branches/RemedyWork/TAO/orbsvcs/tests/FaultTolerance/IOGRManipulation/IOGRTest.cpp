
//=============================================================================
/**
 *  @file     IOGRTest.cpp
 *
 *  $Id$
 *
 * This program tests the basic functionality FT IOGR implementation
 *
 *
 *  @author  Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#include "tao/ORB.h"
#include "orbsvcs/FT_CORBA_ORBC.h"
#include "orbsvcs/FaultTolerance/FT_IOGR_Property.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{

  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));
  ACE_DEBUG ((LM_DEBUG, "Running the IOGRManipulation Tests.\n"));

  try
    {
      // Retrieve the ORB.
      CORBA::ORB_var orb_ = CORBA::ORB_init (argc, argv);
      // **********************************************************************

      // Get an object reference for the ORBs IORManipulation object!
      CORBA::Object_var IORM =
        orb_->resolve_initial_references (TAO_OBJID_IORMANIPULATION, 0);

      TAO_IOP::TAO_IOR_Manipulation_var iorm =
               TAO_IOP::TAO_IOR_Manipulation::_narrow (IORM.in ());
      // **********************************************************************

      // Create a few fictitious IORs
      CORBA::Object_var name1 =
        orb_->string_to_object ("iiop://acme.cs.wustl.edu:6060/xyz");
      CORBA::Object_var name2 =
        orb_->string_to_object ("iiop://tango.cs.wustl.edu:7070/xyz");

      // **********************************************************************
      // Create IOR list for use with merge_iors.
      TAO_IOP::TAO_IOR_Manipulation::IORList iors (2);
      iors.length (2);
      iors [0] = name1;
      iors [1] = name2;
      // **********************************************************************

      CORBA::Object_var merged = iorm->merge_iors (iors);

      // Check for set and get primaries
      // Make a dummy property set
      FT::TagFTGroupTaggedComponent ft_tag_component;
      TAO_FT_IOGR_Property prop (ft_tag_component);

      CORBA::Boolean retval =
        iorm->set_primary (&prop, name2.in (), merged.in ());

      if (retval != 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tThe primary has been set\n")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\tError in setting primary\n")));
          return -1;
        }

      // Check whether a primary has been set
      retval = iorm->is_primary_set (&prop, merged.in ());

      if (retval)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tis_primary_set () returned true\n")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\tis_primary_set () returned false\n")));

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\tSo Exiting\n")));
          return -1;
        }

      // Get the primary
      CORBA::Object_var prim = iorm->get_primary (&prop, merged.in ());

      // Check whether we got back the right primary
      if (prim->_is_equivalent (name2.in ()))
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tWe got the right primary back\n")));
        }
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\tWe have a problem in getting the right primary\n")));
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("\tSo exiting\n")));
          return -1;
        }
      // **********************************************************************
      // Set properties
      // Property values

      // Major and Minor revision numbers
      ft_tag_component.component_version.major = (CORBA::Octet) 1;
      ft_tag_component.component_version.minor = (CORBA::Octet) 0;

      // Domain id
      const char *id = "iogr_regression";
      ft_tag_component.group_domain_id = id;

      // Object group id
      ft_tag_component.object_group_id = (CORBA::ULongLong) 10;

      // Version
      ft_tag_component.object_group_ref_version = (CORBA::ULong) 5;

      // Set the property
      retval = iorm->set_property (&prop, merged.in ());

      /// Extract the property
      FT::TagFTGroupTaggedComponent ftc;
      TAO_FT_IOGR_Property tmp_prop;

      retval = tmp_prop.get_tagged_component (merged.in (), ftc);


      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Testing for tagged component\n"));

      if ((ftc.object_group_ref_version != 5) &&
          (ftc.object_group_id != 10))
        ACE_ERROR ((LM_ERROR,
                    "%P|%t) Not working right\n"));

      if (retval)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("\tWe have set the property\n")));

      orb_->destroy ();
    }
  catch (const TAO_IOP::NotFound& userex)
    {
      userex._tao_print_exception (
        ACE_TEXT ("Unexpected NotFound Exception!\n"));
      return -1;
    }
  catch (const TAO_IOP::Duplicate& userex)
    {
      userex._tao_print_exception ("Unexpected Duplicate Exception!\n");
      return -1;
    }
  catch (const TAO_IOP::Invalid_IOR& userex)
    {
      userex._tao_print_exception ("Unexpected Invalid_IOR Exception!\n");
      return -1;
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("Unexpected system Exception!!\n");
      return -1;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unexpected CORBA Exception!\n");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, "IORManipulation Tests Successfully Completed!\n"));
  ACE_DEBUG ((LM_DEBUG, "---------------------------------------------\n"));

  return 0;
}
