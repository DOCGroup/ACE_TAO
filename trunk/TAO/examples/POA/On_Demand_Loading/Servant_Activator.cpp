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
//     Implementation of Dir_Service_Activator, which is used by a
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
{
}

// This method associates an servant with the ObjectID.

PortableServer::Servant
Dir_Service_Activator::incarnate (const PortableServer::ObjectId &oid,
                                  PortableServer::POA_ptr poa,
                                  CORBA::Environment &env)
{
  
  // Convert ObjectId to String.
  
  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);

  // Activate and return the servant else exception.
  
  PortableServer::Servant servant = this->activate_servant (s.in (),
                                                            poa);
  if (servant != 0)
    return servant;
  else
    {

      TAO_THROW_ENV_RETURN (CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO), env, 0);
      //*done*
      // @@ Kirthika, can you please make this work for both
      // native and non-native C++ exceptions?  Please see
      // Nanbor if you have any questions.
      // CORBA::Exception *exception = new CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO);
      //env.exception (exception);
      //return 0;
    }
}

// This is the method invoked when the object is deactivated or the
// entire POA is is deactivated or destroyed.

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
  // with Dir_Service object, deactivate it.

  if (remaining_activations == 0)
    deactivate_servant (servant);
}

// This method loads the dynamically linked library which is the
// servant and returns the servant object which is then used for other
// operations in the library.

PortableServer::Servant
Dir_Service_Activator::activate_servant (const char *str, 
                                         PortableServer::POA_ptr poa)
{
  // The string format is dllname:factory_method which needs to be parsed.
  parse_string (str);

  // Now that the library name is available we open the library.  

  // *done*@@
  // Kirthika, make SURE to check return values from function calls
  // like this and return failure results correctly...
  int retval = dll_.open (dllname_);
  if (retval != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p", dll_.error ()),
                      0);

  // The next step is to obtain the symbol for the function that will
  // create the servant object and return it to us.
  Servant_Creator_Prototype servant_creator;
  servant_creator = (Servant_Creator_Prototype) dll_.symbol (create_symbol_.in ());
  
  // Checking whether it is possible to create the servant.
  if (servant_creator == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%s",
                       dll_.error ()),
                      0);
   
  // Now create and return the servant.
  return servant_creator (this->orb_.in (), poa);
}

// This method removes the servant and the dll object associated with
// it after it has performed the opening and symbol obtaining operations.
 
void 
Dir_Service_Activator::deactivate_servant (PortableServer::Servant servant)
{
  // the servant is destroyed.
  delete servant;
}

// The objectID is in a format of library:factory_method which has to
// be parsed and separated into tokens to be used.

void
Dir_Service_Activator::parse_string (const char *s)
{
  // The format of the object library:factory_method.  This string is
  // parsed to obtain the library name and the function name which
  // will create trhe servant and return it to us.

  char str[BUFSIZ],func[BUFSIZ], libname [BUFSIZ];
  char at[2];

  at[0]= ':';
  at[1]= '\0';
  
  //*done*
  // @@ Kirthika, please use the ACE_OS::str*() functions
  // consistently.
  ACE_OS::strcpy (func, "");

  // As strtok () puts a NULL in the position after it gives back a
  // token, we make two copies of the input string.
  ACE_OS::strcpy (str, s);

  // The strtok() method returns the string until ':' i.e. the
  // function name.
  ACE_OS::strcpy (libname, ACE_OS::strtok (str, at));
   
  // Get to ':' and make func point to the next location in the
  // string.
  ACE_OS::strcpy (func, ACE_OS::strchr (s,':') + 1);
  
  // Assign the respective strings obtained.

  this->dllname_ = CORBA::string_dup (libname);
  this->create_symbol_ = CORBA::string_dup (func);

  ACE_DEBUG ((LM_DEBUG,
              "the servant library:%s\n the factory_method:%s\n ", 
              this->dllname_.in (),
              this->create_symbol_.in ()));
}

