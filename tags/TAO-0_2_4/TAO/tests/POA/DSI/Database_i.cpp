// $Id$

#include "Database_i.h"

ACE_RCSID(DSI, Database_i, "$Id$")

DatabaseImpl::Simpler_Malloc::Simpler_Malloc (void)
  : MALLOC (ACE_DEFAULT_BACKING_STORE)
{
}

DatabaseImpl::Entry::Entry (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

DatabaseImpl::Entry::~Entry (void)
{
}

void 
DatabaseImpl::Entry::invoke (CORBA::ServerRequest_ptr request,
                             CORBA::Environment &env)
{
  // Get the POA Current object reference
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("POACurrent");

  // Narrow the object reference to a POA Current reference
  PortableServer::Current_var poa_current = PortableServer::Current::_narrow (obj.in (), env);
  if (env.exception () != 0)
    {
      env.print_exception ("PortableServer::Current::_narrow");
      return;
    }
  
  // The servant determines the key associated with thex database entry
  // represented by self
  PortableServer::ObjectId_var oid = poa_current->get_object_id (env);
  if (env.exception () != 0)
    return;

  // Now convert the id into a string
  CORBA::String_var key = PortableServer::ObjectId_to_string (oid.in ());
  
  // Get the operation name for this request
  const char *operation = request->operation ();
  
  if (ACE_OS::strcmp (operation, "_is_a") == 0)
    this->_is_a (request, env);
  else
    {
      env.exception (new CORBA::NO_IMPLEMENT (CORBA::COMPLETED_NO));
      return;
    }    
}

void 
DatabaseImpl::Entry::_is_a (CORBA::ServerRequest_ptr request,
                            CORBA::Environment &env)
{
  CORBA::NVList_ptr list;
  this->orb_->create_list (1, list);

  char *value = 0;
  CORBA::Any any_1 (CORBA::_tc_string, &value);

  CORBA::NamedValue_ptr named_value_1 = list->add_value ("value", 
                                                         any_1, 
                                                         CORBA::ARG_IN, 
                                                         env);
  if (env.exception () != 0)
    return;

  request->arguments (list,
                      env);
  if (env.exception () != 0)
    return;

  CORBA::Boolean result;
  if (!ACE_OS::strcmp (value, "IDL:Database/Employee:1.0") ||
      !ACE_OS::strcmp (value, "IDL:Database/Entry:1.0") ||
      !ACE_OS::strcmp (value, CORBA::_tc_Object->id (env)))
    result = CORBA::B_TRUE;
  else
    result = CORBA::B_FALSE;
  
  CORBA::Any result_any;
  CORBA::Any::from_boolean from_boolean (result);
  result_any <<= from_boolean;

  request->set_result (result_any, env);
}

CORBA::RepositoryId 
DatabaseImpl::Entry::_primary_interface (const PortableServer::ObjectId &oid,
                                         PortableServer::POA_ptr poa,
                                         CORBA::Environment &env)
{
  return 0;
}

PortableServer::POA_ptr 
DatabaseImpl::Entry::_default_POA (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

DatabaseImpl::Agent::Agent (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    common_servant_ (orb, poa)
{
  CORBA::Environment env;

  this->poa_->set_servant (&this->common_servant_, env);
  if (env.exception () != 0)
    {
      ACE_ERROR ((LM_ERROR, "PortableServer::POA::set_servant\n"));
      ACE_OS::exit (-1);
    }
}

DatabaseImpl::Agent::~Agent (void)
{
}

Database::Entry_ptr
DatabaseImpl::Agent::create_entry (const char *key,
                                   const char *entry_type,
                                   const Database::NVPairSequence &initial_attributes,
                                   CORBA::Environment &env)
{
  // Create a new entry in the database:  
  if (ACE_OS::strcmp (entry_type, "Employee") != 0 ||
      initial_attributes.length () != 2)
    {
      CORBA::Exception *exception = new Database::Unknown_Type (entry_type);
      env.exception (exception);
      return Database::Entry::_nil ();
    }
  
  char *name = 0;
  CORBA::Long id = 0;

  const Database::NamedValue &first = initial_attributes[0];    
  const Database::NamedValue &second = initial_attributes[1];    
  if (ACE_OS::strcmp (first.name.in (), "name") != 0 ||
      ACE_OS::strcmp (second.name.in (), "id") != 0)
    {
      CORBA::Exception *exception = new Database::Unknown_Type (entry_type);
      env.exception (exception);
      return Database::Entry::_nil ();
    }
      
  first.value >>= name;
  second.value >>= id;

  Employee *new_employee = new Employee (name, id);
  DATABASE::instance ()->bind (key, new_employee);

  // creates a reference to the CORBA object used to encapsulate
  // access to the new entry in the database.  There is an interface
  // for each entry type:
  PortableServer::ObjectId_var obj_id = PortableServer::string_to_ObjectId (key);
  CORBA::String_var repository_id = DatabaseImpl::entry_type_to_repository_id ("Entry");
  CORBA::Object_var obj = this->poa_->create_reference_with_id (obj_id.in (),
                                                                repository_id.in (),
                                                                env);
  Database::Entry_var entry = Database::Entry::_narrow (obj, env);
  if (env.exception () != 0)
    return Database::Entry::_nil ();

  return entry._retn ();
}

Database::Entry_ptr 
DatabaseImpl::Agent::find_entry (const char *key,
                                 const char *entry_type,
                                 CORBA::Environment &env)
{
  if (ACE_OS::strcmp (entry_type, "Employee") != 0)
    {
      CORBA::Exception *exception = new Database::Unknown_Type (entry_type);
      env.exception (exception);
      return Database::Entry::_nil ();
    }
  
  void *temp;
  if (DATABASE::instance ()->find (key, temp) == 0)
    {
      Employee *employee = (Employee *) temp;

      // creates a reference to the CORBA object used to encapsulate
      // access to the new entry in the database.  There is an interface
      // for each entry type:
      PortableServer::ObjectId_var obj_id = PortableServer::string_to_ObjectId (key);
      CORBA::String_var repository_id = DatabaseImpl::entry_type_to_repository_id ("Entry");
      CORBA::Object_var obj = this->poa_->create_reference_with_id (obj_id.in (),
                                                                    repository_id.in (),
                                                                    env);
      Database::Entry_var entry = Database::Entry::_narrow (obj, env);
      if (env.exception () != 0)
        return Database::Entry::_nil ();

      return entry._retn ();
    }

  else
    {
      CORBA::Exception *exception = new Database::Unknown_Key (key);
      env.exception (exception);
      return Database::Entry::_nil ();
    }
}

void 
DatabaseImpl::Agent::destroy_entry (const char *key,
                                    const char *entry_type,
                                    CORBA::Environment &env)
{
  if (ACE_OS::strcmp (entry_type, "Employee") != 0)
    {
      CORBA::Exception *exception = new Database::Unknown_Type (entry_type);
      env.exception (exception);
      return;
    }
  
  void *temp;
  if (DATABASE::instance ()->unbind (key, temp) == 0)
    {
      Employee *employee = (Employee *) temp;
      delete employee;
    }
  else
    {
      CORBA::Exception *exception = new Database::Unknown_Key (key);
      env.exception (exception);
      return;
    }
}

PortableServer::POA_ptr 
DatabaseImpl::Agent::_default_POA (CORBA::Environment &env)
{
  ACE_UNUSED_ARG (env);
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

char *
DatabaseImpl::entry_type_to_repository_id (const char *entry_type)
{
  static const char *prefix = "IDL:Database/";
  static int prefix_length = ACE_OS::strlen (prefix); 
  static const char *suffix = ":1.0";
  static int suffix_length = ACE_OS::strlen (prefix); 

  int len = 
    prefix_length + 
    ACE_OS::strlen (entry_type) + 
    suffix_length +     
    1;

  char *result = CORBA::string_alloc (len);
  ACE_OS::sprintf (result,
                   "%s%s%s",
                   prefix,
                   entry_type,
                   suffix);
  return result;
}

DatabaseImpl::Employee::Employee (const char* name, 
                                  CORBA::Long id) 
  : id_ (id),
    name_ (0)
{
  this->name (name);
}

DatabaseImpl::Employee::~Employee (void) 
{ 
  DATABASE::instance ()->free (this->name_); 
}

const char *
DatabaseImpl::Employee::name (void) const
{ 
  return this->name_; 
}

void 
DatabaseImpl::Employee::name (const char* name)
{
  DATABASE::instance ()->free (this->name_);
  this->name_ = (char *) DATABASE::instance ()->malloc (ACE_OS::strlen (name) + 1);
  ACE_OS::strcpy (this->name_, name);
}

CORBA::Long 
DatabaseImpl::Employee::id (void) const 
{ 
  return this->id_; 
}

void 
DatabaseImpl::Employee::id (CORBA::Long id)
{ 
  this->id_ = id; 
}

void *
DatabaseImpl::Employee::operator new (size_t size)
{
  return DATABASE::instance ()->malloc (size);
}

void 
DatabaseImpl::Employee::operator delete (void *pointer) 
{ 
  DATABASE::instance ()->free (pointer); 
}
