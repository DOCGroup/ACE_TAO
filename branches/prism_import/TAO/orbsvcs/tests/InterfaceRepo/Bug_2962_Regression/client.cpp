// -*- C++ -*-
// $Id$
// The run_test.pl perl script will check for the expected output.

#include "ace/config.h"
#include "tao/IFR_Client/IFR_ComponentsC.h"

const char *ifr_ior= "file://ifr.ior";
const char *member_type_id= "IDL:arrayOfStruct:1.0";

int main (int argc, char **argv)
{
    CORBA::ORB_ptr the_orb = CORBA::ORB_init (argc, argv);

    CORBA::Object_var obj = the_orb->string_to_object (ifr_ior);
    if (CORBA::is_nil (obj.in()))
    {
      ACE_DEBUG ((LM_DEBUG, "Obtained nil reference to Object for interface repository\n" ));
      return 1;
    }
    CORBA::ComponentIR::Repository_var ifr;
    try
    {
      ifr = CORBA::ComponentIR::Repository::_narrow (obj.in());
    }
    catch (CORBA::SystemException &)
    {
      ACE_DEBUG ((LM_DEBUG, "exception catched .. narrowing IFR object\n" ));
      return 1;
    }
    if (CORBA::is_nil (ifr.in()))
    {
      ACE_DEBUG ((LM_DEBUG, "Can't narrow to IFR\n" ));
      return 1;
    }

    ACE_DEBUG ((LM_DEBUG, "Looking up %s", member_type_id ));
    CORBA::Contained_var
      dependency = ifr->lookup_id (member_type_id);
    if (CORBA::is_nil (dependency.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, " *** failed ***\n" ));
      return 1;
    }

    CORBA::AliasDef_var
        the_alias = CORBA::AliasDef::_narrow (dependency.in ());
    if (CORBA::is_nil (the_alias.in ()))
    {
      ACE_DEBUG ((LM_DEBUG, " *** not an alias ***\n" ));
      return 1;
    }

    CORBA::IDLType_var
      orig_type = the_alias->original_type_def ();
    CORBA::TypeCode_var
      tc_content = orig_type->type ();
    const CORBA::ULong
      length= tc_content->length ();
    const char
      *name= tc_content->content_type ()->name ();
    ACE_DEBUG ((LM_DEBUG,
                " = %s [%u]\n",
                name,
                length ));

    if (2u == length)
    {
      ACE_DEBUG ((LM_DEBUG, "Correct result, test passed\n" ));
    }
    else
    {
      ACE_DEBUG ((LM_DEBUG, "Error: Should have been %s [2]\n", name ));
      return 2;
    }

    return 0;
}
