// $Id$
// ============================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Activator.cpp
//
// = DESCRIPTION
//     Implementation of Dir_Service_Activator , which is used by a
//     POA with a RETAIN policy.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "Servant_Activator.h"
#include "Dir_Service_i.h"

ACE_RCSID(On_Demand_Loading, Servant_Activator, "$Id$")

// Initialization.

Dir_Service_Activator::Dir_Service_Activator (CORBA::ORB_ptr orb)
  :  orb_ (CORBA::ORB::_duplicate (orb))
{}

// This method associates an seravnt with the ObjectID.

PortableServer::Servant
Dir_Service_Activator::incarnate (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr poa,
                                  CORBA::Environment &env)
{
  // Convert ObjectId to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  // Activate and return the servant else exception.
  
  PortableServer::Servant servant = activate_servant (s.in (), poa);
  if (servant !=0)
      return servant;
  else
    {
      CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      env.exception (exception);
      return 0;
    }
}

// This is the method is invoked when the object is deactivated or the entire POA is 
// is deactivated or destroyed.

void
Dir_Service_Activator::etherealize (const PortableServer::ObjectId &oid,
                                    PortableServer::POA_ptr poa,
                                    PortableServer::Servant servant,
                                    CORBA::Boolean cleanup_in_progress,
                                    CORBA::Boolean remaining_activations,
                                    CORBA::Environment &env)
{
  ACE_UNUSED_ARG (oid);
  ACE_UNUSED_ARG (poa);
  ACE_UNUSED_ARG (cleanup_in_progress);
  ACE_UNUSED_ARG (env);

  // If there are no remaining activations i.e ObjectIds associated
  // with Dir_Service object deactivate it.

  if (remaining_activations == 0)
    deactivate_servant (servant);
 
}


// This method loads the  dynamically linked library which is the servant and returns
// the servant object which is then used for other operations in the library.

PortableServer::Servant
Dir_Service_Activator::activate_servant (const char *str, 
                                         PortableServer::POA_ptr poa)
{
  // The string format is function@dll which needs to be parsed.
  parse_string (str);

  // Now that the library name is available we try to create an dll object
  // which also opens the library on creation.
  ACE_NEW_RETURN (dll_,
                  ACE_DLL (dllname_.in ()),
                  0);

  // The next step is to obtain the symbol for the function whihc will create
  // the servant object and return it to us.
  Servant_Creator_Prototype servant_creator;
  servant_creator = (Servant_Creator_Prototype) dll_->symbol (create_symbol_.in ());
  
  // Checking whether it is possible to create the servant.
  if (servant_creator == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                       dll_->error ()),
                      0);
   
  // Now create and return the servant.
  return servant_creator (this->orb_.in (), poa);
  
 }

// This method removes the servant and the dll object associated with it with performed the
// opening and symbol obtaining operations.
 
void 
Dir_Service_Activator::deactivate_servant (PortableServer::Servant servant)
{
  // the servant is destroyed.
  delete servant;

  // The library loaded is now closed and destroyed.
  delete dll_;
  
}

// The objectID is in a format of factory_method@library which has to be parsed and separated
// into tokens to be used.

void
Dir_Service_Activator::parse_string (const char *s)
{
  // The format of the object name: function@dll 
  // This string is parsed to obatin the library name and the
  // function name which will create trhe servant and return it to us.

  char str[BUFSIZ],func[BUFSIZ], libname [BUFSIZ];
  char at[2];

  at[0]= '@';
  at[1]= '\0';
  strcpy (func, "");
  // As strtok () puts a NULL in the position after it gives back a token,
  // we make two copies of the input string.
  strcpy (str, s);

  // The strtok() method returns the string until '@' i.e. the function name.
   ACE_OS::strcpy (func, ACE_OS::strtok(str, at));
   
   // Get to '@' and make the libname point to the next location in the string.
   ACE_OS::strcpy (libname, ACE_OS::strchr (s,'@') + 1);
  
   // Assign the respective strings obtained.

   this->dllname_ = CORBA::string_dup (libname);
   this->create_symbol_ = CORBA::string_dup (func);

   ACE_DEBUG ((LM_DEBUG,
               "the servant library:%s\n the factory_method:%s\n ", 
               this->dllname_.in (),
               this->create_symbol_.in ()));
  
  
}

