// $Id$

#include "tao/corba.h"
#include "tao/IFR_Client/IFR_BasicC.h"

int main(int argc, char** argv)
{  
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb_ = CORBA::ORB_init (argc, argv, 0 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var object =
        orb_->resolve_initial_references ("InterfaceRepository" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Repository_var repo_ = CORBA::Repository::_narrow (object.in ()
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      
    
      CORBA::ContainedSeq_var interfaces =
                  repo_->contents (   CORBA::dk_Interface,    // Any type of contained object.
                                      1                       // Exclude parents of interfaces.
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::ULong length = interfaces->length ();
      
      if (length != 1)
      {
        ACE_DEBUG ((LM_DEBUG, "CLIENT (%P): Incorrect number of elements in IFR - length is %d\n", length));
        return -1;
      }

      CORBA::ULong first_one = 0;
      
      CORBA::InterfaceDef_var the_interface =
            CORBA::InterfaceDef::_narrow (interfaces[first_one].in ()
                                          ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var name = the_interface->name(); 
      
      if (ACE_OS::strcmp (name.in(), "int"))
      {
        ACE_DEBUG ((LM_DEBUG, "CLIENT (%P): Interface name has been munged - it is now %s\n",
                      name.in()));
        return -1;    
      }
    
      
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception ...");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}
