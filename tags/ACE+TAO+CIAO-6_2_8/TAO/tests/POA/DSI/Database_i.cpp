// $Id$

#include "Database_i.h"

#include "tao/DynamicInterface/Server_Request.h"

#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/Null_Mutex.h"

DatabaseImpl::Simpler_Database_Malloc::Simpler_Database_Malloc (void)
  //  : DATABASE_MALLOC ()
{
}

DatabaseImpl::Simpler_Database_Malloc::~Simpler_Database_Malloc (void)
{
  this->remove ();
}

DatabaseImpl::Entry::Entry (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
  // Get the POA Current object reference
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("POACurrent");

  // Narrow the object reference to a POA Current reference
  this->poa_current_ =
    PortableServer::Current::_narrow (obj.in ());
}

DatabaseImpl::Entry::~Entry (void)
{
}

void
DatabaseImpl::Entry::invoke (CORBA::ServerRequest_ptr request)
{
  // The servant determines the key associated with the database
  // entry represented by self.
  PortableServer::ObjectId_var oid =
    this->poa_current_->get_object_id ();

  // Now convert the id into a string
  CORBA::String_var key =
    PortableServer::ObjectId_to_string (oid.in ());

  // Get the operation name for this request
  const char *operation =
    request->operation ();

  if (ACE_OS::strcmp (operation,
                      "_is_a") == 0)
    {
      this->is_a (request);
    }
  else
    throw CORBA::NO_IMPLEMENT ();
}

void
DatabaseImpl::Entry::is_a (CORBA::ServerRequest_ptr request)
{
  CORBA::NVList_ptr list;
  this->orb_->create_list (0, list);

  CORBA::Any any_1;
  any_1._tao_set_typecode (CORBA::_tc_string);

  list->add_value ("value",
                   any_1,
                   CORBA::ARG_IN);

  request->arguments (list);

  CORBA::NamedValue_ptr nv = list->item (0);

  CORBA::Any_ptr ap = nv->value ();
  const char *value;
  *ap >>= value;

  const char *object_id =
    CORBA::_tc_Object->id ();

  CORBA::Boolean result;
  if (ACE_OS::strcmp (value, "IDL:Database/Employee:1.0") == 0
      || ACE_OS::strcmp (value, "IDL:Database/Entry:1.0") == 0
      || ACE_OS::strcmp (value, object_id) == 0)
    result = 1;
  else
    result = 0;

  CORBA::Any result_any;
  CORBA::Any::from_boolean from_boolean (result);
  result_any <<= from_boolean;

  request->set_result (result_any);
}

CORBA::RepositoryId
DatabaseImpl::Entry::_primary_interface (const PortableServer::ObjectId &/*oid*/,
                                         PortableServer::POA_ptr)
{
  return 0;
}

PortableServer::POA_ptr
DatabaseImpl::Entry::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

DatabaseImpl::Agent::Agent (CORBA::ORB_ptr orb,
                            PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    common_servant_ (orb,
                     poa)
{

  this->poa_->set_servant (&this->common_servant_);
}

DatabaseImpl::Agent::~Agent (void)
{
}

Database::Entry_ptr
DatabaseImpl::Agent::create_entry (const char *key,
                                   const char *entry_type,
                                   const Database::NVPairSequence &initial_attributes)
{
  // Create a new entry in the database:
  if (ACE_OS::strcmp (entry_type, "Employee") != 0
      || initial_attributes.length () != 2)
    throw Database::Unknown_Type ();

  const char *name = 0;
  CORBA::Long id = 0;

  const Database::NamedValue &first =
    initial_attributes[0];
  const Database::NamedValue &second =
    initial_attributes[1];

  if (ACE_OS::strcmp (first.name.in (), "name") != 0
      || ACE_OS::strcmp (second.name.in (), "id") != 0)
    throw Database::Unknown_Type ();

  first.value >>= name;
  second.value >>= id;

  Employee *new_employee;
  // This attempts to create a new Employee and throws an exception
  // and returns a null value if it fails
  ACE_NEW_THROW_EX (new_employee,
                    Employee (name, id),
                    CORBA::NO_MEMORY ());

  // @@ Should check the return value here and throw an exception if
  // it fails.
  DATABASE::instance ()->bind (key,
                               new_employee);

  ACE_DEBUG ((LM_DEBUG,
              "New employee created with name = %C and id = %d\n",
              name,
              id));

  // Creates a reference to the CORBA object used to encapsulate
  // access to the new entry in the database.  There is an interface
  // for each entry type:
  PortableServer::ObjectId_var obj_id =
    PortableServer::string_to_ObjectId (key);
  CORBA::String_var repository_id =
    DatabaseImpl::entry_type_to_repository_id ("Entry");

  CORBA::Object_var obj =
    this->poa_->create_reference_with_id (obj_id.in (),
                                          repository_id.in ());

  Database::Entry_var entry = Database::Entry::_narrow (obj.in ());

  return entry._retn ();
}

Database::Entry_ptr
DatabaseImpl::Agent::find_entry (const char *key,
                                 const char *entry_type)
{
  if (ACE_OS::strcmp (entry_type,
                      "Employee") != 0)
    throw Database::Unknown_Type ();

  void *temp = 0;
  Database::Entry_var entry;
  if (DATABASE::instance ()->find (key, temp) == 0)
    {
      Employee *employee =
        reinterpret_cast<Employee *> (temp);
      ACE_DEBUG ((LM_DEBUG,
                  "Employee with key = %C found: name = %C and id = %d\n",
                  key,
                  employee->name (),
                  employee->id ()));

      // Creates a reference to the CORBA object used to encapsulate
      // access to the new entry in the database.  There is an
      // interface for each entry type:
      PortableServer::ObjectId_var obj_id =
        PortableServer::string_to_ObjectId (key);
      CORBA::String_var repository_id =
        DatabaseImpl::entry_type_to_repository_id ("Entry");
      CORBA::Object_var obj =
        this->poa_->create_reference_with_id (obj_id.in (),
                                              repository_id.in ());

      entry = Database::Entry::_narrow (obj.in ());
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Employee with key = %C not found\n",
                  key));

      throw Database::Not_Found ();
    }

  return entry._retn ();
}

void
DatabaseImpl::Agent::destroy_entry (const char *key,
                                    const char *entry_type)
{
  if (ACE_OS::strcmp (entry_type, "Employee") != 0)
    throw Database::Unknown_Type ();

  void *temp = 0;
  if (DATABASE::instance ()->unbind (key, temp) == 0)
    {
      Employee *employee =
        reinterpret_cast<Employee *> (temp);
      ACE_DEBUG ((LM_DEBUG,
                  "Employee with key = %C will be removed from the database: "
                  "name = %C and id = %d\n",
                  key,
                  employee->name (),
                  employee->id ()));

      delete employee;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Employee with key = %C not found\n",
                  key));

      throw Database::Unknown_Key ();
    }
}

void
DatabaseImpl::Agent::shutdown (void)
{
  this->orb_->shutdown ();
}

PortableServer::POA_ptr
DatabaseImpl::Agent::_default_POA (void)
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

  this->name_ =
    (char *) DATABASE::instance ()->malloc (ACE_OS::strlen (name) + 1);

  ACE_OS::strcpy (this->name_,
                  name);
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

#if defined (ACE_HAS_NEW_NOTHROW)
  /// Overloaded new operator, nothrow_t variant.
void *
DatabaseImpl::Employee::operator new (size_t size, const ACE_nothrow_t &)
{
  return DATABASE::instance ()->malloc (size);
}

#if !defined (ACE_LACKS_PLACEMENT_OPERATOR_DELETE)
void
DatabaseImpl::Employee::operator delete (void *ptr, const ACE_nothrow_t&) throw ()
{
  DATABASE::instance ()->free (ptr);
}
#endif /* ACE_LACKS_PLACEMENT_OPERATOR_DELETE */

#endif /* ACE_HAS_NEW_NOTHROW */

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<DatabaseImpl::Simpler_Database_Malloc, ACE_Null_Mutex> *ACE_Singleton<DatabaseImpl::Simpler_Database_Malloc, ACE_Null_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
