// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO/tests/POA/On_Demand_Loading
//
// = FILENAME
//     Servant_Locator.cpp
//
// = DESCRIPTION
//     Implementation of MyFooServantLocator class, used with a POA
//     having a NON_RETAIN policy.
//
// = AUTHOR
//     Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "Servant_Locator.h"
#include "MyFooServant.h"

ACE_RCSID(On_Demand_Activation, Servant_Locator, "$Id$")

// Initialization.

MyFooServantLocator::MyFooServantLocator (CORBA::ORB_ptr orb)
  : counter_ (0),
    orb_ (CORBA::ORB::_duplicate (orb))
{
}

// This method associates an servant with the ObjectID.

PortableServer::Servant
MyFooServantLocator::preinvoke (const PortableServer::ObjectId &oid,
                                PortableServer::POA_ptr poa,
                                const char * /* operation */,
                                PortableServer::ServantLocator::Cookie &cookie,
                                CORBA::Environment &env)
{
  // Convert ObjectID to String.

  CORBA::String_var s = PortableServer::ObjectId_to_string (oid);
  // If ObjectID string has a Foo Substring create and return a
  // MyFooServant.

  PortableServer::Servant servant = this->invoke_servant (s.in (),
                                                         poa,
                                                         27);
  if (servant != 0)
    {
      // Return the servant as the cookie , used as a check when
      // postinvoke is called on this MyFooServantLocator.
      
      cookie = servant;
      return servant;
    }
  else
    {
      TAO_THROW_ENV_RETURN (CORBA::OBJECT_NOT_EXIST (CORBA::COMPLETED_NO), env, 0);
    }
}

// Since the servant gets invoked per operation, the servant has to be destroyed per operation too.
// This is accomplished in the postinvoke method.

void
MyFooServantLocator::postinvoke (const PortableServer::ObjectId & /* oid */,
                                 PortableServer::POA_ptr /* poa */,
                                 const char * /* operation */,
                                 PortableServer::ServantLocator::Cookie cookie,
                                 PortableServer::Servant servant,
                                 CORBA::Environment &/* env */)
{
  // Check the passed servant with the cookie.

  PortableServer::Servant my_servant = (PortableServer::Servant) cookie;
  ACE_ASSERT (servant == my_servant);
  delete servant;

  // To avoid warning about unused variable with ACE_NDEBUG.
  ACE_UNUSED_ARG (my_servant);
}

// This method loads the dynamically linked library which is the
// servant and returns the servant object which is then used for other
// operations in the library.

PortableServer::Servant
MyFooServantLocator::invoke_servant (const char *str, 
                                     PortableServer::POA_ptr poa,
                                     long value)
{
  // The string format is dllname:factory_method which needs to be parsed.
  parse_string (str);

  // Now that the library name is available we open the library.  

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
  return servant_creator (this->orb_.in (), poa, value);
}


// The objectID is in a format of library:factory_method which has to
// be parsed and separated into tokens to be used.

void
MyFooServantLocator::parse_string (const char *s)
{
  // The format of the object library:factory_method.  This string is
  // parsed to obtain the library name and the function name which
  // will create trhe servant and return it to us.

  char str[BUFSIZ],func[BUFSIZ], libname [BUFSIZ];
  char at[2];

  at[0]= ':';
  at[1]= '\0';
  
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

// This method returns an ObjectId when given an library name and the factory method
// to be invoked in the library.The format of the ObjectId is libname:factory_method.

PortableServer::ObjectId_var
MyFooServantLocator::create_objectId (const char *libname, const char *factory_method)
{
  char format_string [BUFSIZ];
  
  ACE_OS::strcpy (format_string, libname);
  ACE_OS::strcat (format_string, ":");
  ACE_OS::strcat (format_string, factory_method);

  // The object ID is created.

  PortableServer::ObjectId_var oid =
    PortableServer::string_to_ObjectId ( format_string);

  return oid;
}
