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
                             CORBA::Environment &ACE_TRY_ENV)
{
  // Get the POA Current object reference
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("POACurrent");

  ACE_TRY
    {
      // Narrow the object reference to a POA Current reference
      PortableServer::Current_var poa_current =
        PortableServer::Current::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // The servant determines the key associated with thex database entry
      // represented by self
      PortableServer::ObjectId_var oid =
        poa_current->get_object_id (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now convert the id into a string
      CORBA::String_var key =
        PortableServer::ObjectId_to_string (oid.in ());

      // Get the operation name for this request
      const char *operation = request->operation ();

      if (ACE_OS::strcmp (operation, "_is_a") == 0)
        this->is_a (request, ACE_TRY_ENV);

      else
        {
          ACE_THROW (CORBA::NO_IMPLEMENT (TAO_DEFAULT_MINOR_CODE,
                                          CORBA::COMPLETED_NO));
        }
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

void
DatabaseImpl::Entry::is_a (CORBA::ServerRequest_ptr request,
                            CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::NVList_ptr list;
  this->orb_->create_list (0, list);

  ACE_TRY
    {
      CORBA::Any any_1 (CORBA::_tc_string);

      list->add_value ("value",
                       any_1,
                       CORBA::ARG_IN,
                       ACE_TRY_ENV);
      ACE_TRY_CHECK;

      request->arguments (list,
                          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      char *value;
      CORBA::Any_ptr ap = list->item (0, ACE_TRY_ENV)->value ();
      *ap >>= value;

      CORBA::Boolean result;
      if (!ACE_OS::strcmp (value, "IDL:Database/Employee:1.0") ||
          !ACE_OS::strcmp (value, "IDL:Database/Entry:1.0") ||
          !ACE_OS::strcmp (value, CORBA::_tc_Object->id (ACE_TRY_ENV)))
        result = 1;
      else
        result = 0;

      CORBA::Any result_any;
      CORBA::Any::from_boolean from_boolean (result);
      result_any <<= from_boolean;

      request->set_result (result_any, ACE_TRY_ENV);
    }
  ACE_CATCHANY
    {
      return;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

CORBA::RepositoryId
DatabaseImpl::Entry::_primary_interface (const PortableServer::ObjectId &/*oid*/,
                                         PortableServer::POA_ptr ,
                                         CORBA::Environment &)
{
  return 0;
}

PortableServer::POA_ptr
DatabaseImpl::Entry::_default_POA (CORBA::Environment &)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

DatabaseImpl::Agent::Agent (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    common_servant_ (orb, poa)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  this->poa_->set_servant (&this->common_servant_, ACE_TRY_ENV);
  ACE_CHECK;
}

DatabaseImpl::Agent::~Agent (void)
{
}

Database::Entry_ptr
DatabaseImpl::Agent::create_entry (const char *key,
                                   const char *entry_type,
                                   const Database::NVPairSequence &initial_attributes,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  // Create a new entry in the database:
  if (ACE_OS::strcmp (entry_type, "Employee") != 0 ||
      initial_attributes.length () != 2)
    {
      ACE_THROW_RETURN (Database::Unknown_Type (),
                        Database::Entry::_nil ());
    }

  char *name = 0;
  CORBA::Long id = 0;

  const Database::NamedValue &first = initial_attributes[0];
  const Database::NamedValue &second = initial_attributes[1];
  if (ACE_OS::strcmp (first.name.in (), "name") != 0 ||
      ACE_OS::strcmp (second.name.in (), "id") != 0)
    {
      ACE_THROW_RETURN (Database::Unknown_Type (),
                        Database::Entry::_nil ());
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
                                                                ACE_TRY_ENV);
  ACE_CHECK_RETURN (Database::Entry::_nil ()) ;
  Database::Entry_var entry = Database::Entry::_narrow (obj.in (),
                                                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (Database::Entry::_nil ());

  return entry._retn ();
}

Database::Entry_ptr
DatabaseImpl::Agent::find_entry (const char *key,
                                 const char *entry_type,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  if (ACE_OS::strcmp (entry_type, "Employee") != 0)
    {
      ACE_THROW_RETURN (Database::Unknown_Type (),
                        Database::Entry::_nil ());
    }

  void *temp;
  Database::Entry_var entry;
  if (DATABASE::instance ()->find (key, temp) == 0)
    {
      //      Employee *employee = (Employee *) temp;

      // creates a reference to the CORBA object used to encapsulate
      // access to the new entry in the database.  There is an interface
      // for each entry type:
      PortableServer::ObjectId_var obj_id = PortableServer::string_to_ObjectId (key);
      CORBA::String_var repository_id = DatabaseImpl::entry_type_to_repository_id ("Entry");
      CORBA::Object_var obj = this->poa_->create_reference_with_id (obj_id.in (),
                                                                    repository_id.in (),
                                                                    ACE_TRY_ENV);
      ACE_CHECK_RETURN (Database::Entry::_nil ());

      entry = Database::Entry::_narrow (obj.in (),
                                        ACE_TRY_ENV);
      ACE_CHECK_RETURN (Database::Entry::_nil ());
    }
  else
    {
      ACE_THROW_RETURN (Database::Unknown_Key (),
                        Database::Entry::_nil ());
    }
  return entry._retn ();
}

void
DatabaseImpl::Agent::destroy_entry (const char *key,
                                    const char *entry_type,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  if (ACE_OS::strcmp (entry_type, "Employee") != 0)
    {
      ACE_THROW (Database::Unknown_Type ());
    }

  void *temp;
  if (DATABASE::instance ()->unbind (key, temp) == 0)
    {
      Employee *employee = (Employee *) temp;
      delete employee;
    }
  else
    {
      ACE_THROW (Database::Unknown_Key ());
    }
}

PortableServer::POA_ptr
DatabaseImpl::Agent::_default_POA (CORBA::Environment &)
{
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Malloc_Iterator<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>;
template class ACE_Singleton<DatabaseImpl::Simpler_Malloc, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Malloc<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Malloc_Iterator<ACE_MMAP_MEMORY_POOL, ACE_Null_Mutex>
#pragma instantiate ACE_Singleton<DatabaseImpl::Simpler_Malloc, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
