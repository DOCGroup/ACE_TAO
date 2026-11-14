#include "Database_i.h"

#include "tao/DynamicInterface/Server_Request.h"

#include "tao/AnyTypeCode/NVList.h"
#include "tao/AnyTypeCode/TypeCode.h"

#include <cstring>
#include <memory>
#include <string>

DatabaseImpl::Simpler_Database_Malloc::~Simpler_Database_Malloc ()
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
  char const * const operation =
    request->operation ();

  if (std::strcmp (operation, "_is_a") == 0)
    {
      this->is_a (request);
    }
  else
    throw CORBA::NO_IMPLEMENT ();
}

void
DatabaseImpl::Entry::is_a (CORBA::ServerRequest_ptr request)
{
  CORBA::NVList_ptr list {};
  this->orb_->create_list (0, list);

  CORBA::Any any_1;
  any_1._tao_set_typecode (CORBA::_tc_string);

  list->add_value ("value",
                   any_1,
                   CORBA::ARG_IN);

  request->arguments (list);

  CORBA::NamedValue_ptr nv = list->item (0);

  CORBA::Any_ptr const ap = nv->value ();
  char const *value {};
  *ap >>= value;

  char const * const object_id =
    CORBA::_tc_Object->id ();

  CORBA::Boolean const result =
    std::strcmp (value, "IDL:Database/Employee:1.0") == 0
    || std::strcmp (value, "IDL:Database/Entry:1.0") == 0
    || std::strcmp (value, object_id) == 0;

  CORBA::Any result_any;
  CORBA::Any::from_boolean from_boolean (result);
  result_any <<= from_boolean;

  request->set_result (result_any);
}

CORBA::RepositoryId
DatabaseImpl::Entry::_primary_interface (PortableServer::ObjectId const & /*oid*/,
                                         PortableServer::POA_ptr)
{
  return {};
}

PortableServer::POA_ptr
DatabaseImpl::Entry::_default_POA ()
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

Database::Entry_ptr
DatabaseImpl::Agent::create_entry (char const *key,
                                   char const *entry_type,
                                   Database::NVPairSequence const &initial_attributes)
{
  // Create a new entry in the database:
  if (std::strcmp (entry_type, "Employee") != 0
      || initial_attributes.length () != 2)
    throw Database::Unknown_Type ();

  char const *name {};
  CORBA::Long id {};

  Database::NamedValue const &first =
    initial_attributes[0];
  Database::NamedValue const &second =
    initial_attributes[1];

  if (std::strcmp (first.name.in (), "name") != 0
      || std::strcmp (second.name.in (), "id") != 0)
    throw Database::Unknown_Type ();

  first.value >>= name;
  second.value >>= id;

  Employee *new_employee {};
  // This attempts to create a new Employee and throws an exception
  // and returns a null value if it fails
  ACE_NEW_THROW_EX (new_employee,
                    Employee (name, id),
                    CORBA::NO_MEMORY ());
  std::unique_ptr<Employee> employee (new_employee);

  if (DATABASE::instance ()->bind (key, new_employee) != 0)
    throw Database::Duplicate_Key (key);
  employee.release ();

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
DatabaseImpl::Agent::find_entry (char const *key,
                                 char const *entry_type)
{
  if (std::strcmp (entry_type, "Employee") != 0)
    throw Database::Unknown_Type ();

  void *temp {};
  Database::Entry_var entry;
  if (DATABASE::instance ()->find (key, temp) == 0)
    {
      // The map owns this object; this pointer is only a temporary borrowed view.
      Employee * const employee = static_cast<Employee *> (temp);
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
DatabaseImpl::Agent::destroy_entry (char const *key,
                                    char const *entry_type)
{
  if (std::strcmp (entry_type, "Employee") != 0)
    throw Database::Unknown_Type ();

  void *temp {};
  if (DATABASE::instance ()->unbind (key, temp) == 0)
    {
      std::unique_ptr<Employee> employee (static_cast<Employee *> (temp));
      ACE_DEBUG ((LM_DEBUG,
                  "Employee with key = %C will be removed from the database: "
                  "name = %C and id = %d\n",
                  key,
                  employee->name (),
                  employee->id ()));

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
DatabaseImpl::Agent::shutdown ()
{
  this->orb_->shutdown ();
}

PortableServer::POA_ptr
DatabaseImpl::Agent::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

char *
DatabaseImpl::entry_type_to_repository_id (char const *entry_type)
{
  static constexpr char prefix[] = "IDL:Database/";
  static constexpr char suffix[] = ":1.0";
  std::string const repository_id = std::string (prefix) + entry_type + suffix;
  return CORBA::string_dup (repository_id.c_str ());
}

DatabaseImpl::Employee::Employee (char const *name,
                                  CORBA::Long id)
  : id_ (id),
    name_ {}
{
  this->name (name);
}

DatabaseImpl::Employee::~Employee ()
{
  DATABASE::instance ()->free (this->name_);
}

char const *
DatabaseImpl::Employee::name () const
{
  return this->name_;
}

void
DatabaseImpl::Employee::name (char const *name)
{
  char * const new_name = static_cast<char *> (DATABASE::instance ()->malloc (std::strlen (name) + 1));
  if (new_name == nullptr)
    throw CORBA::NO_MEMORY ();
  std::strcpy (new_name, name);
  DATABASE::instance ()->free (this->name_);
  this->name_ = new_name;
}

CORBA::Long
DatabaseImpl::Employee::id () const
{
  return this->id_;
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

  /// Overloaded new operator, nothrow_t variant.
void *
DatabaseImpl::Employee::operator new (size_t size, std::nothrow_t const &)
{
  return DATABASE::instance ()->malloc (size);
}

void
DatabaseImpl::Employee::operator delete (void *ptr, std::nothrow_t const &) noexcept
{
  DATABASE::instance ()->free (ptr);
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_Singleton, DatabaseImpl::Simpler_Database_Malloc,  ACE_Null_Mutex);
