// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Storable_Naming_Context.cpp
//
// = AUTHOR
//    Bruce Trask <trask_b@ociweb.com>
//    Chanaka Liyanaarachchi <chanaka@ociweb.com>
//
// ============================================================================

#include "ace/Auto_Ptr.h"
#include "Storable_Naming_Context.h"
#include "Bindings_Iterator_T.h"

CosNaming::NamingContext_ptr TAO_Storable_Naming_Context::root_context_;

ACE_RCSID(Naming, Storable_Naming_Context, "Storable_Naming_Context.cpp,v 1.16 1999/12/16 23:40:07 othman Exp")

int
TAO_Storable_Bindings_Map::unbind (const char *id,
                                   const char *kind)
{
  TAO_Persistent_ExtId name (id, kind);
  return this->map_.unbind (name);
}

int
TAO_Storable_Bindings_Map::bind (const char *id,
                                 const char *kind,
                                 CORBA::Object_ptr obj,
                                 CosNaming::BindingType type)
{
  return this->shared_bind (id, kind, obj, type, 0);
}

int
TAO_Storable_Bindings_Map::rebind (const char *id,
                                   const char *kind,
                                   CORBA::Object_ptr obj,
                                   CosNaming::BindingType type)
{
  return this->shared_bind (id, kind, obj, type, 1);
}

int
TAO_Storable_Bindings_Map::find (const char *id,
                                 const char *kind,
                                 CORBA::Object_ptr & obj,
                                 CosNaming::BindingType &type)
{
  TAO_Persistent_ExtId name (id, kind);
  TAO_Persistent_IntId entry;

  if (this->map_.find (name,
                       entry) != 0)
    {
      return -1;
    }
  else
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      obj = orb_->string_to_object (entry.ref_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      type = entry.type_;

      return 0;
    }
}

TAO_Storable_Bindings_Map::TAO_Storable_Bindings_Map (size_t hash_table_size)
  : map_ (hash_table_size)
{
}

TAO_Storable_Bindings_Map::TAO_Storable_Bindings_Map (size_t hash_table_size, CORBA::ORB_ptr orb, const char* poa_id,PortableServer::POA_ptr poa)
  : map_ (hash_table_size),
    orb_ (CORBA::ORB::_duplicate (orb)),
    name_ (poa_id),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Storable_Bindings_Map::~TAO_Storable_Bindings_Map (void)
{
}

TAO_Storable_Bindings_Map::HASH_MAP &
TAO_Storable_Bindings_Map::map (void)
{
  return map_;
}

size_t
TAO_Storable_Bindings_Map::current_size (void)
{
  return map_.current_size ();
}

size_t
TAO_Storable_Bindings_Map::total_size (void)
{
  return map_.total_size ();
}

int
TAO_Storable_Bindings_Map::shared_bind (const char * id,
                                        const char * kind,
                                        CORBA::Object_ptr obj,
                                        CosNaming::BindingType type,
                                        int rebind)
{
  TAO_Persistent_ExtId new_name (CORBA::string_dup(id), CORBA::string_dup(kind));
  TAO_Persistent_IntId new_entry (orb_->object_to_string(obj), type);
  TAO_Persistent_IntId old_entry;

  if (rebind == 0)
    {
      // Do a normal bind.
      return this->map_.bind (new_name, new_entry);
    }
  else
    // Rebind.
    {
      // Check that types of old and new entries match.
      if (this->map_.find (new_name,
                           old_entry) == 0
          && type != old_entry.type_)
        return -2;

      else
        return this->map_.rebind (new_name, new_entry);
    }
}

const char * TAO_Storable_Bindings_Map::name()
{
  return name_.rep();
}

void TAO_Storable_Bindings_Map::Write(TAO_Writer_Base& wrtr)
{
  ACE_Hash_Map_Iterator<TAO_Persistent_ExtId,TAO_Persistent_IntId, ACE_Null_Mutex> it = map_.begin();
  ACE_Hash_Map_Iterator<TAO_Persistent_ExtId,TAO_Persistent_IntId, ACE_Null_Mutex> itend = map_.end();

  if (it == itend)
  {
    wrtr.delete_bindings_ = 1;
    return;
  }

  ACE_Hash_Map_Entry<TAO_Persistent_ExtId,TAO_Persistent_IntId> ent = *it;

  const char *my_name = name_.rep();

  TAO_NS_Persistence_Header header;

  ACE_CString name(my_name);
  header.context_name (name);
  header.size ((const unsigned short)map_.current_size());

  wrtr << header;

  while (!(it == itend))
  {
    TAO_NS_Persistence_Record record;

    CosNaming::BindingType bt = (*it).int_id_.type_;
    if (bt ==  CosNaming::ncontext)
      record.type(TAO_NS_Persistence_Record::NCONTEXT);
    else if (bt == CosNaming::nobject)
      record.type(TAO_NS_Persistence_Record::OBJREF);
    //else
    //there shouldn't be any other, can there be ??
    //ignore for now

    // todo - are we using this i ??
    //int i = map_.current_size();

    const char *myid = (*it).ext_id_.id();
    ACE_CString id(myid);
    record.id(id);

    const char *mykind = (*it).ext_id_.kind();
    ACE_CString kind(mykind);
    record.kind(kind);

    ACE_CString ior((*it).int_id_.ref_);
    record.ior(ior);

    if (bt == 1)
      {
        CORBA::Object_var obj = orb_->string_to_object((*it).int_id_.ref_);
        PortableServer::ObjectId_var oid = poa_->reference_to_id(obj.in());
        CORBA::String_var nm = PortableServer::ObjectId_to_string(oid.in());
        const char *newname = nm.in();
        ACE_CString name(newname);
        record.context_binding(name);
      }
    wrtr << record;
    it.advance();
  }
}

TAO_Storable_Naming_Context::TAO_Storable_Naming_Context (
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
                               size_t hash_table_size)
  : TAO_Hash_Naming_Context (poa,
                             poa_id),
    counter_ (0),
    storable_context_ (0),
    orb_(CORBA::ORB::_duplicate (orb)),
    factory_(factory),
    persistence_directory_ (persistence_directory)
{
  ACE_NEW (this->storable_context_,
           TAO_Storable_Bindings_Map (hash_table_size,orb,poa_id,poa));
  context_ = storable_context_;
}

TAO_Storable_Naming_Context::TAO_Storable_Naming_Context (
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              TAO_Storable_Bindings_Map *bindings_map,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory,
                              size_t)
  : TAO_Hash_Naming_Context (poa, poa_id),
    counter_ (0),
    storable_context_ (0),
    orb_(CORBA::ORB::_duplicate (orb)),
    factory_(factory),
    persistence_directory_ (persistence_directory)
{
  this->storable_context_ = bindings_map;
  context_ = storable_context_;
}

TAO_Storable_Naming_Context::~TAO_Storable_Naming_Context (void)
{
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::make_new_context (
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              size_t context_size,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory
                              ACE_ENV_ARG_DECL)
{
  // Store the stub we will return here.
  CosNaming::NamingContext_var result;

  // Put together a servant for the new Naming Context.

  TAO_Storable_Naming_Context *context_impl = 0;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Storable_Naming_Context (orb,
                                                 poa,
                                                 poa_id,
                                                 factory,
                                                 persistence_directory,
                                                 context_size),
                                                 CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();
  PortableServer::ServantBase_var s = context;

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (poa_id);

  // If we try to register a naming context that is already registered,
  // the following activation causes a POA::ObjectAlreadyActive exception be
  // thrown which is transmitted as a CORBA::UNKNOWN on the wire. To rectify
  // this problem, we explicitly throw the correct INS exception in
  // this situation.
  ACE_TRY
    {
      poa->activate_object_with_id (id.in (),
                                    context
                                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (PortableServer::POA::ObjectAlreadyActive, ex)
    {
      ACE_THROW_RETURN (CosNaming::NamingContext::AlreadyBound(),
                        CosNaming::NamingContext::_nil ());
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (result._retn ());

  result = context->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::new_context (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CosNaming::NamingContext::_nil ());

  // Generate a POA id for the new context.
  char poa_id[BUFSIZ];
  ACE_OS::sprintf (poa_id,
                   "%s_%d",
                   this->poa_id_.c_str (),
                   this->counter_++);

  // Create a new context.
  CosNaming::NamingContext_var result =
    make_new_context (this->orb_,
                      this->poa_.in (),
                      poa_id,
                      this->storable_context_->total_size (),
                      this->factory_,
                      this->persistence_directory_.c_str ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

void
TAO_Storable_Naming_Context::rebind (const CosNaming::Name& n,
                                     CORBA::Object_ptr obj
                                     ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
                  get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind (simple_name, obj ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    // If we received a simple name, we need to rebind it in this
    // context.
    {
      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           obj,
                                           CosNaming::nobject);


      ACE_CString writer_name(this->persistence_directory_);
      writer_name += "/";
      writer_name += ACE_static_cast(TAO_Storable_Bindings_Map*,
                                              this->context_)->name();
      //TAO_Writer_Base *wrtr = factory_->create_writer(writer_name.c_str());
      TAO_Writer_Base *wrtr = factory_->create_writer();
      if (wrtr->open (writer_name.c_str()) != 0)
        {
          delete wrtr;
          ACE_THROW (CORBA::PERSIST_STORE ());
        }

      ACE_static_cast (TAO_Storable_Bindings_Map*,
                       this->context_)->Write(*wrtr);
      wrtr->close();
      delete wrtr;

      // Check for error conditions.
      if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());

      else if (result == -2)
        ACE_THROW (CosNaming::NamingContext::NotFound
                   (CosNaming::NamingContext::not_object, n));
    }
}

void
TAO_Storable_Naming_Context::bind_context (const CosNaming::Name &n,
                                           CosNaming::NamingContext_ptr nc
                                           ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Do not allow binding of nil context reference.
  if (CORBA::is_nil (nc))
    ACE_THROW (CORBA::BAD_PARAM ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->bind_context (simple_name, nc ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  // If we received a simple name, we need to bind it in this context.
  else
    {
      // Try binding the name.
      int result = this->context_->bind (n[0].id,
                                         n[0].kind,
                                         nc,
                                         CosNaming::ncontext);
      if (result == 1)
        ACE_THROW (CosNaming::NamingContext::AlreadyBound());

      // Something went wrong with the internal structure
      else if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());

      ACE_CString writer_name(this->persistence_directory_);
      writer_name += "/";
      writer_name += ACE_static_cast(TAO_Storable_Bindings_Map*,
                                              this->context_)->name();

      //TAO_Writer_Base *wrtr = factory_->create_writer(writer_name.c_str());
      TAO_Writer_Base *wrtr = factory_->create_writer();
      if (wrtr->open (writer_name.c_str()) != 0)
        {
          delete wrtr;
          ACE_THROW (CORBA::PERSIST_STORE ());
        }

      ACE_static_cast(TAO_Storable_Bindings_Map*,
                      this->context_)->Write(*wrtr);
      wrtr->close();
      delete wrtr;
    }
}

void
TAO_Storable_Naming_Context::rebind_context (const CosNaming::Name &n,
                                         CosNaming::NamingContext_ptr nc
                                         ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind_context (simple_name, nc ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    // If we received a simple name, we need to rebind it in this
    // context.
    {
      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           nc,
                                           CosNaming::ncontext);
      // Check for error conditions.
      if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());

      else if (result == -2)
        ACE_THROW (CosNaming::NamingContext::NotFound
                   (CosNaming::NamingContext::not_context, n));

      ACE_CString writer_name(this->persistence_directory_);
      writer_name += "/";
      writer_name += ACE_static_cast(TAO_Storable_Bindings_Map*,
                                              this->context_)->name();
      //TAO_Writer_Base *wrtr = factory_->create_writer(writer_name.c_str());
      TAO_Writer_Base *wrtr = factory_->create_writer();
      if (wrtr->open (writer_name.c_str()) != 0)
        {
          delete wrtr;
          ACE_THROW (CORBA::PERSIST_STORE ());
        }

      ACE_static_cast(TAO_Storable_Bindings_Map*,
                      this->context_)->Write(*wrtr);
      wrtr->close();
      delete wrtr;
    }
}

CORBA::Object_ptr
TAO_Storable_Naming_Context::resolve (const CosNaming::Name& n
                                      ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CORBA::Object::_nil ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW_RETURN (CosNaming::NamingContext::InvalidName(),
                      CORBA::Object::_nil ());

  // Resolve the first component of the name.

  // Stores the binding type for the first name component.
  CosNaming::BindingType type;
  // Stores the object reference bound to the first name component.
  CORBA::Object_ptr obj = CORBA::Object::_nil ();

  if (this->context_->find (n[0].id,
                            n[0].kind,
                            obj,
                            type) == -1)
    ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                      (CosNaming::NamingContext::missing_node, n),
                      CORBA::Object::_nil ());

  // Store the value in var to avoid memory leaks.
  CORBA::Object_var result = obj;

  // If the name we have to resolve is a compound name, we need to
  // resolve it recursively.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        CosNaming::NamingContext::_nil ();

      if (type == CosNaming::ncontext)
        {
          // Narrow to NamingContext.
          context = CosNaming::NamingContext::_narrow (result.in ()
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (result._retn ());
        }
      else
        // The first name component wasn't bound to a NamingContext.
        ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                          (CosNaming::NamingContext::not_context,
                           n),
                          CORBA::Object::_nil ());

      // If narrow failed...
      if (CORBA::is_nil (context.in ()))
        ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                          (CosNaming::NamingContext::not_context, n),
                          CORBA::Object::_nil ());
      else
        {
          // Successfully resolved the first name component, need to
          // recursively call resolve on <n> without the first component.

          // We need a name just like <n> but without the first
          // component.  Instead of copying data we can reuse <n>'s
          // buffer since we will only be using it for 'in' parameters
          // (no modifications).
          CosNaming::Name rest_of_name
            (n.maximum () - 1,
             n.length () - 1,
             ACE_const_cast (CosNaming::NameComponent*,
                             n.get_buffer ()) + 1);

          // If there are any exceptions, they will propagate up.
          return context->resolve (rest_of_name
                                   ACE_ENV_ARG_PARAMETER);
        }
    }
  // If the name we had to resolve was simple, we just need to return
  // the result.
  return result._retn ();
}

void
TAO_Storable_Naming_Context::unbind (const CosNaming::Name& n
                                 ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->unbind (simple_name ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  // If we received a simple name, we need to unbind it in this
  // context.
  else
    if (this->context_->unbind (n[0].id,
                                n[0].kind) == -1)
      ACE_THROW (CosNaming::NamingContext::NotFound
                 (CosNaming::NamingContext::missing_node, n));

    ACE_CString writer_name(this->persistence_directory_);
    writer_name += "/";
    writer_name += ACE_static_cast(TAO_Storable_Bindings_Map*,
                                            this->context_)->name();
    //TAO_Writer_Base *wrtr = factory_->create_writer(writer_name.c_str());
    TAO_Writer_Base *wrtr = factory_->create_writer();
    if (wrtr->open (writer_name.c_str()) != 0)
      {
        delete wrtr;
        ACE_THROW (CORBA::PERSIST_STORE ());
      }

    ACE_static_cast(TAO_Storable_Bindings_Map*,
                    this->context_)->Write(*wrtr);
    wrtr->close();
    delete wrtr;
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::bind_new_context (const CosNaming::Name& n
                                               ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CosNaming::NamingContext::_nil ());

  // Stores our new Naming Context.
  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  // Create new context.
  result = new_context (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Bind the new context to the name.
  ACE_TRY
    {
      bind_context (n,
                    result.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      {
        ACE_DECLARE_NEW_CORBA_ENV;
        ACE_TRY_EX(DESTROY)
          {
            result->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK_EX(DESTROY);
          }
        ACE_CATCHANY
          {
            // Do nothing?
          }
        ACE_ENDTRY;
      }
      // Re-raise the exception in bind_context()
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

void
TAO_Storable_Naming_Context::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  if (this->context_->current_size () != 0)
    ACE_THROW (CosNaming::NamingContext::NotEmpty());

  // Destroy is a no-op on a root context.
  if (root ())
    return;

  else
    {
      this->destroyed_ = 2;

      // Remove self from POA.  Because of reference counting, the POA
      // will automatically delete the servant when all pending requests
      // on this servant are complete.

      PortableServer::POA_var poa =
        this->_default_POA ();

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id_.fast_rep ());

      ACE_CHECK;

      poa->deactivate_object (id.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}


void
TAO_Storable_Naming_Context::bind (const CosNaming::Name& n,
                               CORBA::Object_ptr obj
                               ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        this->get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->bind (simple_name, obj ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  // If we received a simple name, we need to bind it in this context.
  else
    {
      // Try binding the name.
      int result = this->context_->bind (n[0].id,
                                         n[0].kind,
                                         obj,
                                         CosNaming::nobject);
      if (result == 1)
        ACE_THROW (CosNaming::NamingContext::AlreadyBound());

      // Something went wrong with the internal structure
      else if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());

      ACE_CString writer_name(this->persistence_directory_);
      writer_name += "/";
      writer_name += ACE_static_cast(TAO_Storable_Bindings_Map*,
                                              this->context_)->name();
      //TAO_Writer_Base *wrtr = factory_->create_writer((char*)(writer_name.c_str()));
      TAO_Writer_Base *wrtr = factory_->create_writer();
      if (wrtr->open (writer_name.c_str()) != 0)
        {
          delete wrtr;
          ACE_THROW (CORBA::PERSIST_STORE ());
        }

      ACE_static_cast(TAO_Storable_Bindings_Map*,
                      this->context_)->Write(*wrtr);

      wrtr->close();
      delete wrtr;
    }
}


void
TAO_Storable_Naming_Context::list (CORBA::ULong how_many,
                                   CosNaming::BindingList_out &bl,
                                   CosNaming::BindingIterator_out &bi
                                   ACE_ENV_ARG_DECL)
{
  // Allocate nil out parameters in case we won't be able to complete
  // the operation.
  bi = CosNaming::BindingIterator::_nil ();
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Obtain a lock before we proceed with the operation.
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Dynamically allocate iterator for traversing the underlying hash map.
  HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    HASH_MAP::ITERATOR (storable_context_->map ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Store <hash_iter temporarily in auto pointer, in case we'll have
  // some failures and throw an exception.
  ACE_Auto_Basic_Ptr<HASH_MAP::ITERATOR> temp (hash_iter);

  // Silliness below is required because of broken old g++!!!  E.g.,
  // without it, we could have just said HASH_MAP::ITERATOR everywhere we use ITER_DEF.
  typedef ACE_Hash_Map_Manager<TAO_Persistent_ExtId,
                               TAO_Persistent_IntId,
                               ACE_Null_Mutex>::ITERATOR ITER_DEF;
  typedef ACE_Hash_Map_Manager<TAO_Persistent_ExtId,
                               TAO_Persistent_IntId,
                               ACE_Null_Mutex>::ENTRY ENTRY_DEF;

  // Typedef to the type of BindingIterator servant for ease of use.
  typedef TAO_Bindings_Iterator<ITER_DEF, ENTRY_DEF> ITER_SERVANT;

  // A pointer to BindingIterator servant.
  ITER_SERVANT *bind_iter = 0;

  // Number of bindings that will go into the BindingList <bl>.
  CORBA::ULong n;

  // Calculate number of bindings that will go into <bl>.
  if (this->context_->current_size () > how_many)
    n = how_many;
  else
    n = this->context_->current_size ();

  // Use the hash map iterator to populate <bl> with bindings.
  bl->length (n);

  ENTRY_DEF *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (ITER_SERVANT::populate_binding (hash_entry, bl[i]) == 0)
          ACE_THROW (CORBA::NO_MEMORY());
    }

  // Now we are done with the BindingsList, and we can follow up on
  // the BindingIterator business.

  // If we do not need to pass back BindingIterator.
  if (this->context_->current_size () <= how_many)
    return;
  else
    {
      // Create a BindingIterator for return.
      ACE_NEW_THROW_EX (bind_iter,
                        ITER_SERVANT (this, hash_iter,
                                      this->poa_.in (), this->lock_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      // Release <hash_iter> from auto pointer, and start using
      // reference counting to control our servant.
      temp.release ();
      PortableServer::ServantBase_var iter = bind_iter;

      // Increment reference count on this Naming Context, so it doesn't get
      // deleted before the BindingIterator servant gets deleted.
      interface_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Register with the POA.
      char poa_id[BUFSIZ];
      ACE_OS::sprintf (poa_id,
                       "%s_%d",
                       this->poa_id_.c_str (),
                       this->counter_++);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (),
                                           bind_iter
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      bi = bind_iter->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

#include "Naming_Service_Container.h"
CosNaming::NamingContext_ptr TAO_Storable_Naming_Context::recreate_all(
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               size_t context_size,
                               int reentering,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory
                               ACE_ENV_ARG_DECL)
{

  ACE_UNUSED_ARG (reentering);

  ACE_Unbounded_List<ACE_CString> context_id_coll;
  ACE_Unbounded_List<ACE_CString> context_kind_coll;
  ACE_Unbounded_List<ACE_CString> context_binding_coll;
  ACE_Unbounded_List<ACE_CString> context_IOR_coll;
  ACE_Unbounded_List<ACE_CString> ref_id_coll;
  ACE_Unbounded_List<ACE_CString> ref_kind_coll;
  ACE_Unbounded_List<ACE_CString> ref_IOR_coll;
  ACE_Unbounded_List<ACE_CString> ref_type_coll;

  ACE_CString reader_name(persistence_directory);
  reader_name += "/";
  reader_name += poa_id;

  TAO_Reader_Base *rdr;

  // This is the call the to the abstract factory
  //rdr = factory->create_reader(ACE_const_cast (char *, reader_name.c_str()));
  rdr = factory->create_reader();
  int retVal = rdr->open(ACE_const_cast (char *, reader_name.c_str()));

  if (retVal != 0)
    {
      // This means that there was no stored state and so need to make the context.
      // This can happen if a naming context with no entries existed at the time the
      // INS crashed.
      return TAO_Storable_Naming_Context::make_new_context (
                                              orb,
                                              poa,
                                              poa_id,
                                              context_size,
                                              factory,
                                              persistence_directory
                                              ACE_ENV_ARG_PARAMETER);

    }

  TAO_Storable_Bindings_Map *bindings_map;

  // create the new bindings map
  ACE_NEW_THROW_EX (bindings_map,
                    TAO_Storable_Bindings_Map (context_size,orb,poa_id,poa),
                    CORBA::NO_MEMORY ());

  // get the data for this bindings map from the file

  TAO_NS_Persistence_Header header;
  TAO_NS_Persistence_Record record;

  *rdr >> header;
  unsigned int max_count = 0;

  // read in the data
  for (unsigned int i = 0; i < header.size() ; i++)
  {
    *rdr >> record;
    TAO_NS_Persistence_Record::Record_Type type = record.type();

    if (type == TAO_NS_Persistence_Record::NCONTEXT)
    {
      ACE_CString context_id = record.id();
      context_id_coll.insert(context_id);

      ACE_CString context_kind = record.kind();
      context_kind_coll.insert(context_kind);
      //size_t sz = context_kind_coll.size();

      ACE_CString context_IOR = record.ior();
      context_IOR_coll.insert(context_IOR);

      ACE_CString context_binding = record.context_binding();
      context_binding_coll.insert(context_binding);

      // Lets rip the context_binding apart to reverse engineer
      // the counter used to generate it.  We'll save the max
      // count we see for later use to initialize this context
      int underscore_pos = context_binding.rfind('_');
      if (underscore_pos != ACE_CString::npos)
        {
          ACE_CString count_str = context_binding.substring (
                                             underscore_pos + 1);
          unsigned int count = ACE_OS::atoi (count_str.c_str());
          if (count >= max_count)
            {
              max_count = count + 1;
            }
        }
    }

    else if (type == TAO_NS_Persistence_Record::OBJREF)
    {
      ref_type_coll.insert(ACE_CString("nobject"));

      ACE_CString ref_id = record.id();
      ref_id_coll.insert(ref_id);

      ACE_CString ref_kind = record.kind();
      ref_kind_coll.insert(ref_kind);
      //size_t sz = ref_kind_coll.size();

      ACE_CString ref_IOR = record.ior();
      ref_IOR_coll.insert(ref_IOR);
    }
  }

  // now put this data into the bindings map. Let's bind the nobject
  //references first
  ACE_Unbounded_List<ACE_CString>::iterator id_set_iter(ref_id_coll);
  ACE_Unbounded_List<ACE_CString>::iterator IOR_set_iter(ref_IOR_coll);
  IOR_set_iter = ref_IOR_coll.begin();
  ACE_Unbounded_List<ACE_CString>::iterator kind_set_iter(ref_kind_coll);
  kind_set_iter = ref_kind_coll.begin();
  //vector<ACE_CString>::iterator kind_set_iter = ref_kind_coll.begin();
  for (id_set_iter = ref_id_coll.begin();
       id_set_iter != ref_id_coll.end();
       ++id_set_iter,++IOR_set_iter,++kind_set_iter)
  {
    CORBA::Object_var obj_ref = orb->string_to_object((*IOR_set_iter).c_str());
    bindings_map->bind((*id_set_iter).c_str(),
                       (*kind_set_iter).c_str(),obj_ref.in(),
                       (CosNaming::BindingType)0);
  }

  // Now bind the ncontexts
  ACE_Unbounded_List<ACE_CString>::iterator
                  context_id_set_iter(context_id_coll);
  ACE_Unbounded_List<ACE_CString>::iterator
                  context_IOR_set_iter(context_IOR_coll);
  context_IOR_set_iter = context_IOR_coll.begin();
  ACE_Unbounded_List<ACE_CString>::iterator
                  context_kind_set_iter(context_kind_coll);
  context_kind_set_iter = context_kind_coll.begin();
  for (context_id_set_iter = context_id_coll.begin();
       context_id_set_iter != context_id_coll.end();
       ++context_id_set_iter,++context_IOR_set_iter,++context_kind_set_iter)
  {
    CORBA::Object_var obj_ref = orb->string_to_object(
                                      (*context_IOR_set_iter).c_str());
    bindings_map->bind((*context_id_set_iter).c_str(),
                       (*context_kind_set_iter).c_str(),
                       obj_ref.in(),
                       (CosNaming::BindingType)1);
  }

  // now create the context
  // This is basically a call to make_new_context except we are creating
  // the context impl with the bindings map already created and we are passing
  // it into the overloaded constructor.

  // Store the stub we will return here.
  CosNaming::NamingContext_var result;

  // Put together a servant for the new Naming Context.
  TAO_Storable_Naming_Context *context_impl = 0;
  // create an impl that takes a bindings map in its constructor
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Storable_Naming_Context (orb,
                                                 poa,
                                                 poa_id,
                                                 bindings_map,
                                                 factory,
                                                 persistence_directory,
                                                 context_size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // We need to ensure that we set the counter for this context
  // higher than the highest count we see in the contexts contained
  // within it.  Otherwise we will end up with object id collisions
  // if we add a new context after a reload.
  if (context_impl->counter_ < max_count)
    {
      context_impl->counter_ = max_count;
    }

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();
  PortableServer::ServantBase_var s = context;

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (poa_id);

  poa->activate_object_with_id (id.in (),
                                context
                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (result._retn ());

  result = context->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Now we need to recursively read in the other context's contents
  ACE_Unbounded_List<ACE_CString>::iterator context_binding_set_iter(context_binding_coll);
  context_binding_set_iter = context_binding_coll.begin();
  for (; context_binding_set_iter != context_binding_coll.end();
       ++context_binding_set_iter)
  {
    CosNaming::NamingContext_var temp =
             TAO_Storable_Naming_Context::recreate_all(
                                       orb,
                                       poa,
                                       (*context_binding_set_iter).c_str(),
                                       context_size,
                                       1,
                                       factory,
                                       persistence_directory
                                       ACE_ENV_ARG_PARAMETER);
  }
  rdr->close();
  delete rdr; // Should be scoped instead
  return result._retn ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Unbounded_List_Iterator<ACE_CString>;
template class ACE_Unbounded_List<ACE_CString>;
template class ACE_NS_Node<ACE_CString>;
template class ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Unbounded_List_Iterator<ACE_CString>
#pragma instantiate ACE_Unbounded_List<ACE_CString>
#pragma instantiate ACE_NS_Node<ACE_CString>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context>
#endif
