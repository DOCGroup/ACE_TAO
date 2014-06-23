// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/Storable_Naming_Context_Factory.h"
#include "orbsvcs/Naming/Storable_Naming_Context_ReaderWriter.h"
#include "orbsvcs/Naming/Bindings_Iterator_T.h"

#include "tao/debug.h"
#include "tao/Storable_Base.h"
#include "tao/Storable_Factory.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_sys_time.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char * TAO_Storable_Naming_Context::root_name_;
ACE_UINT32 TAO_Storable_Naming_Context::gcounter_;
ACE_Auto_Ptr<TAO::Storable_Base> TAO_Storable_Naming_Context::gfl_;
int TAO_Storable_Naming_Context::redundant_;

TAO_Storable_IntId::TAO_Storable_IntId (void)
  : ref_ (CORBA::string_dup ("")),
    type_ (CosNaming::nobject)
{
}

TAO_Storable_IntId::TAO_Storable_IntId (const char * ior,
                                            CosNaming::BindingType type)
  : ref_ (ior),
    type_ (type)
{
}

TAO_Storable_IntId::TAO_Storable_IntId (const TAO_Storable_IntId &rhs)
{
  ref_ = rhs.ref_;
  type_ = rhs.type_;
}

TAO_Storable_IntId::~TAO_Storable_IntId (void)
{
}

void
TAO_Storable_IntId::operator= (const TAO_Storable_IntId &rhs)
{
  // check for self assignment.
  if (&rhs == this)
    return;

  type_ = rhs.type_;
  ref_ = rhs.ref_;
}

TAO_Storable_ExtId::TAO_Storable_ExtId (void)
  : id_ (CORBA::string_dup ("")),
    kind_ (CORBA::string_dup (""))
{
}

TAO_Storable_ExtId::TAO_Storable_ExtId (const char *id, const char *kind)
  : id_ (id)
  , kind_ (kind)
{
}

TAO_Storable_ExtId::TAO_Storable_ExtId (const TAO_Storable_ExtId &rhs)
{
  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

TAO_Storable_ExtId::~TAO_Storable_ExtId (void)
{
}

void
TAO_Storable_ExtId::operator= (const TAO_Storable_ExtId &rhs)
{
  // Check for self assignment.
  if (&rhs == this)
    return;

  id_ = rhs.id_;
  kind_ = rhs.kind_;
}

bool
TAO_Storable_ExtId::operator== (const TAO_Storable_ExtId &rhs) const
{
  return (ACE_OS::strcmp (id_.in(), rhs.id_.in()) == 0)
    && (ACE_OS::strcmp (kind_.in(), rhs.kind_.in()) == 0);
}

bool
TAO_Storable_ExtId::operator!= (const TAO_Storable_ExtId &rhs) const
{
  return (ACE_OS::strcmp (id_.in(), rhs.id_.in()) != 0)
    || (ACE_OS::strcmp (kind_.in(), rhs.kind_.in()) != 0);
}

u_long
TAO_Storable_ExtId::hash (void) const
{
  // @CJC I wager this could be optimized a bit better, but I'm not
  // sure how much it's called.  At the very least, we could allocate
  // just one ACE_CString, and copy id_ and kind_ into that, rather than
  // taking a double-hit on allocations.
  ACE_CString temp (id_.in());
  temp += kind_.in();

  return temp.hash ();
}

const char *
TAO_Storable_ExtId::id (void)
{
  return id_.in();
}

const char *
TAO_Storable_ExtId::kind (void)
{
  return kind_.in();
}

int
TAO_Storable_Bindings_Map::unbind (const char *id, const char *kind)
{
  ACE_TRACE("unbind");
  TAO_Storable_ExtId name (id, kind);
  return this->map_.unbind (name);
}

int
TAO_Storable_Bindings_Map::bind (const char *id,
                                 const char *kind,
                                 CORBA::Object_ptr obj,
                                 CosNaming::BindingType type)
{
  ACE_TRACE("bind");
  return this->shared_bind (id, kind, obj, type, 0);
}

int
TAO_Storable_Bindings_Map::rebind (const char *id,
                                   const char *kind,
                                   CORBA::Object_ptr obj,
                                   CosNaming::BindingType type)
{
  ACE_TRACE("rebind");
  return this->shared_bind (id, kind, obj, type, 1);
}

int
TAO_Storable_Bindings_Map::find (const char *id,
                                 const char *kind,
                                 CORBA::Object_ptr & obj,
                                 CosNaming::BindingType &type)
{
  ACE_TRACE("find");
  TAO_Storable_ExtId name (id, kind);
  TAO_Storable_IntId entry;

  if (this->map_.find (name, entry) != 0)
    {
      return -1;
    }
  else
    {
      obj = orb_->string_to_object (entry.ref_.in());
      type = entry.type_;

      return 0;
    }
}

TAO_Storable_Bindings_Map::TAO_Storable_Bindings_Map (size_t hash_table_size,
                                                      CORBA::ORB_ptr orb)
  : map_ (hash_table_size),
    orb_(CORBA::ORB::_duplicate (orb))
{
  ACE_TRACE("TAO_Storable_Bindings_Map");
}

TAO_Storable_Bindings_Map::~TAO_Storable_Bindings_Map (void)
{
  ACE_TRACE("~TAO_Storable_Bindings_Map");
}

TAO_Storable_Bindings_Map::HASH_MAP &
TAO_Storable_Bindings_Map::map (void)
{
  ACE_TRACE("map");
  return map_;
}

size_t
TAO_Storable_Bindings_Map::current_size (void)
{
  ACE_TRACE("current_size");
  return map_.current_size ();
}

size_t
TAO_Storable_Bindings_Map::total_size (void)
{
  ACE_TRACE("total_size");
  return map_.total_size ();
}

int
TAO_Storable_Bindings_Map::shared_bind (const char * id,
                                        const char * kind,
                                        CORBA::Object_ptr obj,
                                        CosNaming::BindingType type,
                                        int rebind)
{
  ACE_TRACE("shared_bind");
  TAO_Storable_ExtId new_name (id, kind);
  CORBA::String_var ior = orb_->object_to_string(obj);
  TAO_Storable_IntId new_entry (ior.in(), type);
  TAO_Storable_IntId old_entry;

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

void TAO_Storable_Naming_Context::Write (TAO::Storable_Base& wrtr)
{
  ACE_TRACE("Write");
  TAO_Storable_Naming_Context_ReaderWriter rw(wrtr);
  rw.write(*this);
}

// Helpers function to load a new context into the binding_map
int
TAO_Storable_Naming_Context::load_map (TAO::Storable_Base& storable)
{
  ACE_TRACE("load_map");
  TAO_Storable_Naming_Context_ReaderWriter rw (storable);
  return rw.read (*this);
}

TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::File_Open_Lock_and_Check
(TAO_Storable_Naming_Context * context,
 Method_Type method_type,
 bool force_load)
: TAO::Storable_File_Guard (TAO_Storable_Naming_Context::redundant_),
  context_(context)
{
  try
    {
      this->init_no_load (method_type);
      if (force_load)
        this->reload ();
      else
        {
          if (this->object_obsolete ())
            {
              ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                                        context->lock (),
                                        CORBA::INTERNAL ());
              this->reload ();
            }
        }
    }
  catch (const TAO::Storable_Exception &)
    {
      throw CORBA::INTERNAL ();
    }
}

TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::~File_Open_Lock_and_Check ()
{
  this->release ();

  // Check if a write occurred for this context and
  // notify the context if it did.
  if (context_->write_occurred_ == 1)
    {
      context_->context_written ();
      // We have to make sure we clear the flag
      // for subsequent times through.
      context_->write_occurred_ = 0;
    }
}

bool
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::object_obsolete (void)
{

  // Query the underlying context if it is obsolete with respect
  // to the provided file last-changed time
  return (context_->is_obsolete (fl_->last_changed ()));
}

void
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::mark_object_current (void)
{
  // Reset the stale flag
  context_->stale (false);
  // Set the last update time to the file last update time
  this->set_object_last_changed (fl_->last_changed ());
}

void
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::set_object_last_changed (const time_t & time)
{
  context_->last_changed_ = time;
}

time_t
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::get_object_last_changed ()
{
  return context_->last_changed_;
}

int
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::load_from_stream ()
{
  if (context_ == 0)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) File_Open_Lock_and_Check::load_from_stream -")
                  ACE_TEXT ("null context_ encountered.")));
      throw CORBA::INTERNAL ();
    }
  // Throw our map away
  if (context_->storable_context_)
    {
      delete context_->storable_context_;
      context_->storable_context_ = 0;
    }

  // and build a new one from disk
  return context_->load_map (this->peer());
}

bool
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::is_loaded_from_stream ()
{
  return context_->storable_context_ != 0;
}

TAO::Storable_Base *
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::create_stream (const char * mode)
{
  ACE_CString file_name = context_->context_name_;

  // Create the stream
  return context_->factory_->create_stream(file_name, mode);
}

// Make shortcut to get to Method_Type enums
typedef TAO::Storable_File_Guard SFG;

TAO_Storable_Naming_Context::TAO_Storable_Naming_Context (
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *context_name,
                               TAO_Storable_Naming_Context_Factory *cxt_factory,
                               TAO::Storable_Factory *factory,
                               size_t hash_table_size)
  : TAO_Hash_Naming_Context (poa, context_name),
    counter_ (0),
    storable_context_ (0),
    orb_(CORBA::ORB::_duplicate (orb)),
    context_name_ (context_name),
    poa_ (PortableServer::POA::_duplicate (poa)),
    context_factory_ (cxt_factory),
    factory_ (factory),
    hash_table_size_ (hash_table_size),
    last_changed_ (0),
    last_check_ (0),
    write_occurred_ (0)
{
  ACE_TRACE("TAO_Storable_Naming_Context");
}

TAO_Storable_Naming_Context::~TAO_Storable_Naming_Context (void)
{
  ACE_TRACE("~TAO_Storable_Naming_Context");

  // If we're in the DTOR as a result of the destroy() operation,
  // then we go ahead and delete the file.  Otherwise, we leave the
  // file around because they need to be there for persistence.
  if (this->destroyed_)
    {
      // Make sure we delete the associated stream
      ACE_CString file_name = this->context_name_;

      // Now delete the file
      ACE_Auto_Ptr<TAO::Storable_Base>
        fl (
          this->factory_->create_stream(file_name.c_str(), "r")
          );
      if (fl.get())
        {
          if (TAO_debug_level > 5)
            ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) NameService: removing file %s\n",
                        file_name.fast_rep()));
          fl->remove ();
        }
    }
}

void
TAO_Storable_Naming_Context::context_written (void)
{
  // No-op. Overridden by derived class.
}

bool
TAO_Storable_Naming_Context::is_obsolete (time_t stored_time)
{
  bool result = this->context_ == 0 ||
    stored_time >= this->last_check_;
  this->last_check_ = ACE_OS::gettimeofday ().sec ();
  return result;
}

void
TAO_Storable_Naming_Context::verify_not_destroyed (void)
{
  File_Open_Lock_and_Check flck (this, SFG::ACCESSOR, false);
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();
}

bool
TAO_Storable_Naming_Context::nested_context (const CosNaming::Name &n,
                                             CosNaming::NamingContext_out nc)
{
  CORBA::ULong name_len = n.length ();

  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  if (name_len > 1)
    {
      nc = this->get_context (n);
      return true;
    }
  nc = CosNaming::NamingContext::_nil ();
  return false;
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::make_new_context (
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *context_name,
                              TAO_Storable_Naming_Context_Factory *cxt_factory,
                              TAO::Storable_Factory *pers_factory,
                              TAO_Storable_Naming_Context **new_context)
{
  ACE_TRACE("make_new_context");

  CosNaming::NamingContext_var result;

  // Put together a servant for the new Naming Context.

  TAO_Storable_Naming_Context *context_impl =
    cxt_factory->create_naming_context_impl (orb,
                                             poa,
                                             context_name,
                                             pers_factory);

  if (context_impl == 0)
    throw CORBA::NO_MEMORY ();

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Storable_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();
  PortableServer::ServantBase_var s = context;

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (context_name);

  // If we try to register a naming context that is already registered,
  // the following activation causes a POA::ObjectAlreadyActive exception be
  // thrown which is transmitted as a CORBA::UNKNOWN on the wire. To rectify
  // this problem, we explicitly throw the correct INS exception in
  // this situation.
  try
    {
      poa->activate_object_with_id (id.in (), context);
    }
  catch (const PortableServer::POA::ObjectAlreadyActive&)
    {
      throw CosNaming::NamingContext::AlreadyBound();
    }


  result = context->_this ();

  // return the address of the new context object so that caller can finish
  *new_context = context_impl;

  return result._retn ();
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::new_context (void)
{
  ACE_TRACE ("new_context");

  char object_id[BUFSIZ];
  {
    ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                              this->lock_,
                              CORBA::INTERNAL ());

    File_Open_Lock_and_Check flck (this, SFG::MUTATOR);
    if (this->destroyed_)
      throw CORBA::OBJECT_NOT_EXIST ();

    TAO_NS_Persistence_Global global;
    TAO_Storable_Naming_Context_ReaderWriter rw (*gfl_.get ());

    if (redundant_)
      {
        // acquire a lock on the file that holds our counter
        if (gfl_->open () != 0)
          {
            delete gfl_.release ();
            throw CORBA::PERSIST_STORE ();
          }
        if (gfl_->flock (0, 0, 0) != 0)
          {
            throw CORBA::INTERNAL ();
          }
        // get the counter from disk
        rw.read_global (global);
        gcounter_ = global.counter ();
        // use it to generate a new name
      }

    // Generate an Object id for the new context.
    ACE_OS::sprintf (object_id,
                     "%s_%d",
                     root_name_,
                     gcounter_++);
    // then save it back on disk
    global.counter (gcounter_);
    rw.write_global (global);
    if (redundant_)
      {
        // and release our lock
        if (gfl_->funlock(0, 0, 0) != 0)
          throw CORBA::INTERNAL();
        gfl_->close();
      }
  }

  // Create a new context.
  TAO_Storable_Naming_Context *new_context = 0;
  CosNaming::NamingContext_var result =
    make_new_context (this->orb_.in (),
                      this->poa_.in (),
                      object_id,
                      this->context_factory_,
                      this->factory_,
                      &new_context);

  // Since this is a new context, make an empty map in it
  ACE_NEW_THROW_EX (new_context->storable_context_,
                    TAO_Storable_Bindings_Map (hash_table_size_,orb_.in ()),
                    CORBA::NO_MEMORY ());
  new_context->context_ = new_context->storable_context_;

  File_Open_Lock_and_Check flck(new_context, SFG::CREATE_WITHOUT_FILE);
  new_context->Write(flck.peer());

  return result._retn ();
}

void
TAO_Storable_Naming_Context::rebind (const CosNaming::Name& n,
                                     CORBA::Object_ptr obj)
{
  ACE_TRACE("TAO_Storable_Naming_Context::rebind");
  CosNaming::NamingContext_var context;
  if (this->nested_context (n, context.out ()))
    {
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[n.length () - 1];
      context->rebind (simple_name, obj);
    }
  else
    {
      ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                                this->lock_,
                                CORBA::INTERNAL ());
      File_Open_Lock_and_Check flck (this, SFG::MUTATOR);
      if (this->destroyed_)
        throw CORBA::OBJECT_NOT_EXIST ();

      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           obj,
                                           CosNaming::nobject);

      if (result == -1)
        throw CORBA::INTERNAL ();

      else if (result == -2)
        throw CosNaming::NamingContext::NotFound (
          CosNaming::NamingContext::not_object,
          n);

      this->Write (flck.peer());
    }
}

void
TAO_Storable_Naming_Context::bind_context (const CosNaming::Name &n,
                                           CosNaming::NamingContext_ptr nc)
{
  ACE_TRACE("TAO_Storable_Naming_Context::bind_context");

  if (CORBA::is_nil (nc))
    throw CORBA::BAD_PARAM ();

  CosNaming::NamingContext_var context;
  if (this->nested_context (n, context.out ()))
    {
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[n.length () - 1];
      context->bind_context (simple_name, nc);
    }
  else
    {
      ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                                this->lock_,
                                CORBA::INTERNAL ());
      File_Open_Lock_and_Check flck (this, SFG::MUTATOR);
      if (this->destroyed_)
        throw CORBA::OBJECT_NOT_EXIST ();

      int result = this->context_->bind (n[0].id,
                                         n[0].kind,
                                         nc,
                                         CosNaming::ncontext);
      if (result == 1)
        throw CosNaming::NamingContext::AlreadyBound();

      // Something went wrong with the internal structure
      else if (result == -1)
        throw CORBA::INTERNAL ();

      this->Write(flck.peer());
    }
}

void
TAO_Storable_Naming_Context::rebind_context (const CosNaming::Name &n,
                                             CosNaming::NamingContext_ptr nc)
{
  ACE_TRACE("TAO_Storable_Naming_Context::rebind_context");
  if (CORBA::is_nil (nc))
    throw CORBA::BAD_PARAM ();

  CosNaming::NamingContext_var context;
  if (this->nested_context (n, context.out ()))
    {
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[n.length () - 1];
      context->rebind_context (simple_name, nc);
    }
  else
    {
      ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                                this->lock_,
                                CORBA::INTERNAL ());

      File_Open_Lock_and_Check flck (this, SFG::MUTATOR);
      if (this->destroyed_)
        throw CORBA::OBJECT_NOT_EXIST ();

      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           nc,
                                           CosNaming::ncontext);
      if (result == -1)
        throw CORBA::INTERNAL ();
      else if (result == -2)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);

      this->Write(flck.peer());
    }
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::bind_new_context (const CosNaming::Name& n)
{
  ACE_TRACE("TAO_Storable_Naming_Context::bind_new_context");
  CosNaming::NamingContext_var context;
  if (this->nested_context (n, context.out ()))
    {
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[n.length () - 1];
      return context->bind_new_context (simple_name);
    }

  CosNaming::NamingContext_var result = new_context ();

  try
    {
      bind_context (n, result.in ());
    }
  catch (const CORBA::Exception&)
    {
      try
        {
          result->destroy ();
        }
      catch (const CORBA::Exception&)
        {
          // Do nothing?
        }
      throw;
    }
  return result._retn ();
}

void
TAO_Storable_Naming_Context::unbind (const CosNaming::Name& n)
{
  ACE_TRACE("TAO_Storable_Naming_Context::unbind");
  CosNaming::NamingContext_var context;
  if (this->nested_context (n, context.out ()))
    {
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[n.length () - 1];
      context->unbind (simple_name);
    }
  else
    {
      ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                                this->lock_,
                                CORBA::INTERNAL ());

      File_Open_Lock_and_Check flck (this, SFG::MUTATOR);
      if (this->destroyed_)
        throw CORBA::OBJECT_NOT_EXIST ();

      if (this->context_->unbind (n[0].id,
                                  n[0].kind) == -1)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::missing_node,
          n);

      this->Write(flck.peer());
    }
}

void
TAO_Storable_Naming_Context::bind (const CosNaming::Name& n,
                                   CORBA::Object_ptr obj)
{
  ACE_TRACE("TAO_Storable_Naming_Context::bind");
  CosNaming::NamingContext_var context;
  if (this->nested_context (n, context.out ()))
    {
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[n.length() - 1];
      context->bind (simple_name, obj);
    }
  else
    {
      ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX, ace_mon,
                                this->lock_,
                                CORBA::INTERNAL ());
      File_Open_Lock_and_Check flck(this, SFG::MUTATOR);
      if (this->destroyed_)
        throw CORBA::OBJECT_NOT_EXIST ();

      int result = this->context_->bind (n[0].id,
                                         n[0].kind,
                                         obj,
                                         CosNaming::nobject);
      if (result == 1)
        throw CosNaming::NamingContext::AlreadyBound();
      else if (result == -1)
        throw CORBA::INTERNAL ();

      this->Write (flck.peer());
    }
}

void
TAO_Storable_Naming_Context::destroy (void)
{
  ACE_TRACE("TAO_Storable_Naming_Context::destroy");
  ACE_WRITE_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                            ace_mon,
                            this->lock_,
                            CORBA::INTERNAL ());
  File_Open_Lock_and_Check flck(this, SFG::MUTATOR);
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  if (this->context_->current_size () != 0)
    throw CosNaming::NamingContext::NotEmpty ();

  if (!root ())
    {
      this->destroyed_ = 2;

      PortableServer::POA_var poa = this->_default_POA ();

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id_.fast_rep ());

      poa->deactivate_object (id.in ());

      this->Write(flck.peer());
    }
}

CORBA::Object_ptr
TAO_Storable_Naming_Context::resolve (const CosNaming::Name& n)
{
  ACE_TRACE("TAO_Storable_Naming_Context::resolve");

  CORBA::ULong name_len = n.length ();
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  CosNaming::BindingType type;
  CORBA::Object_var result;
  {
    this->verify_not_destroyed ();

    ACE_READ_GUARD_THROW_EX (TAO_SYNCH_RW_MUTEX, ace_mon, this->lock_,
                             CORBA::INTERNAL ());
    if (this->context_->find (n[0].id,
                              n[0].kind,
                              result.out (),
                              type) == -1)
      {
        throw CosNaming::NamingContext::NotFound
          (CosNaming::NamingContext::missing_node, n);
      }
  }

  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        CosNaming::NamingContext::_nil ();

      if (type == CosNaming::ncontext)
        {
          context = CosNaming::NamingContext::_narrow (result.in ());
        }
      else
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);

      if (CORBA::is_nil (context.in ()))
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);
      else
        {
          CosNaming::Name rest_of_name
            (n.maximum () - 1,
             n.length () - 1,
             const_cast<CosNaming::NameComponent*> (n.get_buffer ()) + 1);

          return context->resolve (rest_of_name);
        }
    }
  return result._retn ();
}

void
TAO_Storable_Naming_Context::list (CORBA::ULong how_many,
                                   CosNaming::BindingList_out &bl,
                                   CosNaming::BindingIterator_out &bi)
{
  ACE_TRACE("TAO_Storable_Naming_Context::list");
  bi = CosNaming::BindingIterator::_nil ();

  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());

  this->verify_not_destroyed ();

  ACE_READ_GUARD_THROW_EX (ACE_SYNCH_RW_MUTEX,
                           ace_mon,
                           this->lock_,
                           CORBA::INTERNAL ());

  HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    HASH_MAP::ITERATOR (storable_context_->map ()),
                    CORBA::NO_MEMORY ());

  ACE_Auto_Basic_Ptr<HASH_MAP::ITERATOR> temp (hash_iter);

  // Silliness below is required because of broken old g++!!!  E.g.,
  // without it, we could have just said HASH_MAP::ITERATOR everywhere we use ITER_DEF.
  typedef ACE_Hash_Map_Manager<TAO_Storable_ExtId,
                               TAO_Storable_IntId,
                               ACE_Null_Mutex>::ITERATOR ITER_DEF;
  typedef ACE_Hash_Map_Manager<TAO_Storable_ExtId,
                               TAO_Storable_IntId,
                               ACE_Null_Mutex>::ENTRY ENTRY_DEF;
  typedef TAO_Bindings_Iterator<ITER_DEF, ENTRY_DEF> ITER_SERVANT;

  CORBA::ULong n = (this->context_->current_size () > how_many) ?
    how_many :
    static_cast<CORBA::ULong> (this->context_->current_size ());
  bl->length (n);

  ENTRY_DEF *hash_entry = 0;
  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (ITER_SERVANT::populate_binding (hash_entry, bl[i]) == 0)
          throw CORBA::NO_MEMORY();
    }

  if (this->context_->current_size () <= how_many)
    return;
  else if (redundant_)
    {
      throw CORBA::NO_IMPLEMENT ();
    }
  else
    {
      ITER_SERVANT *bind_iter = 0;
      ACE_NEW_THROW_EX (bind_iter,
                        ITER_SERVANT (this, hash_iter, this->poa_.in ()),
                        CORBA::NO_MEMORY ());

      temp.release ();
      PortableServer::ServantBase_var svt = bind_iter;

      // Increment reference count on this Naming Context, so it doesn't get
      // deleted before the BindingIterator servant gets deleted.
      interface_->_add_ref ();

      char poa_id[BUFSIZ];
      ACE_OS::snprintf (poa_id,
                        BUFSIZ,
                        "%s_%d",
                        this->poa_id_.c_str (),
                        this->gcounter_++);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (), svt.in());
      CORBA::Object_var obj = this->poa_->id_to_reference (id.in ());
      bi = CosNaming::BindingIterator::_narrow (obj.in());
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include "orbsvcs/Naming/Naming_Service_Container.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CosNaming::NamingContext_ptr TAO_Storable_Naming_Context::recreate_all (
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               size_t context_size,
                               int reentering,
                               TAO_Storable_Naming_Context_Factory *cxt_factory,
                               TAO::Storable_Factory *pers_factory,
                               int use_redundancy)
{
  ACE_TRACE("recreate_all");

  ACE_UNUSED_ARG (reentering);

  // Whether we are redundant is global
  redundant_ = use_redundancy;

  // Save the root name for later use
  root_name_ = poa_id;

  // Create a new context.
  TAO_Storable_Naming_Context *new_context = 0;
  CosNaming::NamingContext_var result =
    make_new_context (orb,
                      poa,
                      poa_id,
                      cxt_factory,
                      pers_factory,
                      &new_context);

  // Now does this already exist on disk?
  ACE_CString file_name = poa_id;
  ACE_Auto_Ptr<TAO::Storable_Base> fl (
    pers_factory->create_stream (file_name.c_str (), "r"));
  if (fl->exists ())
  {
    // Load the map from disk
    File_Open_Lock_and_Check flck (new_context, SFG::CREATE_WITH_FILE);
  }
  else
  {
    // Since this is a new context, make and empty map in it
    ACE_NEW_THROW_EX (new_context->storable_context_,
                      TAO_Storable_Bindings_Map (context_size,orb),
                      CORBA::NO_MEMORY ());
    new_context->context_ = new_context->storable_context_;
    File_Open_Lock_and_Check flck (new_context, SFG::CREATE_WITHOUT_FILE);
    new_context->Write (flck.peer ());
  }

  // build the global file name
  file_name += "_global";

  // Create the stream for the counter used to uniquely create context names
  // Pass false for use_backup since access to this file is not wrapped
  // around a Storable_File_Guard derived class.
  gfl_.reset(pers_factory->
             create_stream (file_name.c_str(),
                            "crw",
                            false));
  if (gfl_->open() != 0)
    {
      delete gfl_.release();
      throw CORBA::PERSIST_STORE();
    }

  // get the counter from disk
  TAO_NS_Persistence_Global global;
  TAO_Storable_Naming_Context_ReaderWriter rw(*gfl_.get());
  rw.read_global(global);
  gcounter_ = global.counter();
  if(redundant_) gfl_->close();

  return result._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
