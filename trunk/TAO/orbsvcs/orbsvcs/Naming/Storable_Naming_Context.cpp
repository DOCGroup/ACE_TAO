#include "Storable_Naming_Context.h"
#include "Bindings_Iterator_T.h"

// The following #pragma is needed to disable a warning that occurs
// in MSVC 6 due to the overly long debugging symbols generated for
// the ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<TAO_...> > template
// instance used by some of the methods in this file.
#ifdef _MSC_VER
#  pragma warning(disable: 4786)  /* identifier was truncated to '255'
                                     characters in the browser
                                     information */
#endif  /* _MSC_VER */

#include "ace/Auto_Ptr.h"


const char * TAO_Storable_Naming_Context::root_name_;
ACE_UINT32 TAO_Storable_Naming_Context::gcounter_;
TAO_Storable_Base * TAO_Storable_Naming_Context::gfl_;
int TAO_Storable_Naming_Context::redundant_;

ACE_RCSID (Naming,
           Storable_Naming_Context,
           "$Id$")

int
TAO_Storable_Bindings_Map::unbind (const char *id,
                                   const char *kind)
{
  ACE_TRACE("unbind");
  TAO_Persistent_ExtId name (id, kind);
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

void TAO_Storable_Naming_Context::Write(TAO_Storable_Base& wrtr)
{
  ACE_TRACE("Write");
  TAO_NS_Persistence_Header header;

  header.size (ACE_static_cast(unsigned int,
                               storable_context_->current_size()));
  header.destroyed (destroyed_);

  wrtr << header;

  if( storable_context_->current_size() == 0 )
    return;

  ACE_Hash_Map_Iterator<TAO_Persistent_ExtId,TAO_Persistent_IntId,
                        ACE_Null_Mutex> it = storable_context_->map().begin();
  ACE_Hash_Map_Iterator<TAO_Persistent_ExtId,TAO_Persistent_IntId,
                        ACE_Null_Mutex> itend = storable_context_->map().end();

  ACE_Hash_Map_Entry<TAO_Persistent_ExtId,TAO_Persistent_IntId> ent = *it;

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
    //int i = storable_context_->current_size();

    const char *myid = (*it).ext_id_.id();
    ACE_CString id(myid);
    record.id(id);

    const char *mykind = (*it).ext_id_.kind();
    ACE_CString kind(mykind);
    record.kind(kind);

    ACE_CString name;
    if (bt == CosNaming::nobject)
      {
        name.set((*it).int_id_.ref_);
      }
    else
      {
        CORBA::Object_var obj = orb_->string_to_object((*it).int_id_.ref_);
        PortableServer::ObjectId_var oid = poa_->reference_to_id(obj.in());
        CORBA::String_var nm = PortableServer::ObjectId_to_string(oid.in());
        const char *newname = nm.in();
        name.set(newname);
      }
    record.ref(name);

    wrtr << record;
    it.advance();
  }
}

// Helper function to load a new context into the binding_map
int
TAO_Storable_Naming_Context::load_map(File_Open_Lock_and_Check *flck 
                                      ACE_ENV_ARG_DECL)
{
  ACE_TRACE("load_map");
  // assume file already open for reading
  TAO_Storable_Bindings_Map *bindings_map;

  // create the new bindings map
  ACE_NEW_THROW_EX (bindings_map,
                    TAO_Storable_Bindings_Map (hash_table_size_,orb_),
                    CORBA::NO_MEMORY ());

  // get the data for this bindings map from the file

  TAO_NS_Persistence_Header header;
  TAO_NS_Persistence_Record record;

  // we are only using the size from this header
  flck->peer() >> header;

  // reset the destroyed flag
  this->destroyed_ = header.destroyed();

  // read in the data for the map
  for (unsigned int i=0; i<header.size(); i++)
  {
    flck->peer() >> record;
    if( record.type() == TAO_NS_Persistence_Record::NCONTEXT )
    {
      PortableServer::ObjectId_var id = 
                      PortableServer::string_to_ObjectId(record.ref().c_str());
      const char * intf = interface_->_interface_repository_id();
      CORBA::Object_var objref = poa_->create_reference_with_id (
                                           id.in (),
					   intf );
      bindings_map->bind( record.id().c_str(),
                          record.kind().c_str(),
			  objref.in(),
			  CosNaming::ncontext );
    }
    else
    {
      CORBA::Object_var objref = orb_->string_to_object (record.ref().c_str());
      bindings_map->bind( record.id().c_str(),
                          record.kind().c_str(),
			  objref.in(),
			  CosNaming::nobject );
    }
  }
  storable_context_ = bindings_map;
  context_ = storable_context_;
  return 0;
}

TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::File_Open_Lock_and_Check(
                                 TAO_Storable_Naming_Context * context, 
                                 const char * mode
                                 ACE_ENV_ARG_DECL)
:closed_(1),
 context_(context)
{
  ACE_TRACE("File_Open_Lock_and_Check");
  // We only accept a subset of mode argument, check it
  rwflags_ = 0;
  for( unsigned int i = 0; i<ACE_OS::strlen(mode); i++ ) 
  {
    switch (mode[i])
    {
      case 'r': rwflags_ |= mode_read;
                break;
      case 'w': rwflags_ |= mode_write;
                break;
      case 'c': rwflags_ |= mode_create;
                break;
      default: rwflags_ = -1;
    }
  }
  if( rwflags_ <= 0 ) 
  {
    errno = EINVAL;
    ACE_THROW(CORBA::PERSIST_STORE());
  }

  // build the file name 
  ACE_CString file_name(context->persistence_directory_);
  file_name += "/";
  file_name += context->name_;

  // Create the stream
  fl_ = context->factory_->create_stream(file_name, mode);
  if (TAO_Storable_Naming_Context::redundant_)
  {
    if (fl_->open() != 0)
      {
        delete fl_;
        ACE_THROW(CORBA::PERSIST_STORE());
      } 

    // acquire a lock on it
    if (fl_ -> flock(0, 0, 0) != 0)
      {
         fl_->close();
         delete fl_;
         ACE_THROW(CORBA::INTERNAL());
      }

    // now that the file is successfully opened and locked it must be
    // unlocked/closed before we leave this class
    closed_ = 0;

    if ( ! (rwflags_ & mode_create) )
    {
      // Check if our copy is up to date
      time_t new_last_changed = fl_->last_changed();
      if( new_last_changed > context->last_changed_ )
      {
         context->last_changed_ = new_last_changed;
         // Throw our map away
         delete context->storable_context_;
         // and build a new one from disk
         context->load_map(this ACE_ENV_ARG_PARAMETER);
         ACE_CHECK;
       }
    }
  }
  else if ( ! context->storable_context_ || (rwflags_ & mode_write) )
  {
    if (fl_->open() != 0)
      {
        delete fl_;
        ACE_THROW(CORBA::PERSIST_STORE());
      } 

    // now that the file is successfully opened
    // unlocked/closed before we leave this class
    closed_ = 0;

    if(!context->storable_context_)
    {
      // Load the map from disk
      context->load_map(this ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  }
}

void
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::release(void)
{
  ACE_TRACE("release");
  if ( ! closed_ )
  {
    // If we updated the disk, save the time stamp
    if(TAO_Storable_Naming_Context::redundant_)
    {
      if( rwflags_ & mode_write )
        context_->last_changed_ = fl_->last_changed();
      fl_->funlock(0, 0, 0);
    }
    fl_->close();
    delete fl_;
    closed_ = 1;
  }
}

TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::~File_Open_Lock_and_Check(void)
{
  ACE_TRACE("~File_Open_Lock_and_Check");
  this->release();
}

TAO_Storable_Base &
TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::peer(void)
{
  ACE_TRACE("peer");
  return *fl_;
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
    name_ (poa_id),
    poa_ (PortableServer::POA::_duplicate (poa)),
    factory_(factory),
    persistence_directory_ (ACE_TEXT_ALWAYS_CHAR(persistence_directory)),
    hash_table_size_(hash_table_size),
    last_changed_(0)
{
  ACE_TRACE("TAO_Storable_Naming_Context");
}

TAO_Storable_Naming_Context::~TAO_Storable_Naming_Context (void)
{
  ACE_TRACE("~TAO_Storable_Naming_Context");
  // build the file name
  ACE_CString file_name(persistence_directory_);
  file_name += "/";
  file_name += name_;
  // and delete the file
  TAO_Storable_Base * fl = factory_->create_stream(file_name, "w");
  fl->remove();

}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::make_new_context (
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              size_t context_size,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory,
                              TAO_Storable_Naming_Context **new_context
                              ACE_ENV_ARG_DECL)
{
  ACE_TRACE("make_new_context");
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

  // return the address of the new context object so that caller can finish
  *new_context = context_impl;

  return result._retn ();
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::new_context (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRACE("new_context");
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  {
    // Open the backing file
    File_Open_Lock_and_Check flck(this, "r"
                                       ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

    // Check to make sure this object didn't have <destroy> method
    // invoked on it.
    if (this->destroyed_)
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CosNaming::NamingContext::_nil ());
  }

  TAO_NS_Persistence_Global global;

  // Generate a POA id for the new context.
  if(redundant_)
  {
    // acquire a lock on the file that holds our counter
    if (gfl_->open() != 0)
      {
        delete gfl_;
        ACE_THROW_RETURN(CORBA::PERSIST_STORE(),
                         CosNaming::NamingContext::_nil ());
      }
    if (gfl_ -> flock(0, 0, 0) != 0)
         ACE_THROW_RETURN (CORBA::INTERNAL(),
                           CosNaming::NamingContext::_nil ());
    // get the counter from disk
    *gfl_ >> global;
    gcounter_ = global.counter();
    // use it to generate a new name
  }
  char poa_id[BUFSIZ];
  ACE_OS::sprintf (poa_id,
                   "%s_%d",
                   root_name_,
                   gcounter_++);
  // then save it back on disk
  global.counter(gcounter_);
  *gfl_ << global;
  if(redundant_)
  {
    // and release our lock
    if (gfl_ -> flock(0, 0, 0) != 0)
         ACE_THROW_RETURN(CORBA::INTERNAL(),
                          CosNaming::NamingContext::_nil ());
    gfl_->close();
  }

  // Create a new context.
  TAO_Storable_Naming_Context *new_context = 0;
  CosNaming::NamingContext_var result =
    make_new_context (this->orb_,
                      this->poa_.in (),
                      poa_id,
                      this->storable_context_->total_size (),
                      this->factory_,
                      this->persistence_directory_.c_str (),
                      &new_context
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Since this is a new context, make an empty map in it
  ACE_NEW_THROW_EX (new_context->storable_context_,
                    TAO_Storable_Bindings_Map (hash_table_size_,orb_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());
  new_context->context_ = new_context->storable_context_;

  File_Open_Lock_and_Check flck(new_context, "wc" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());
  new_context->Write(flck.peer());

  return result._retn ();
}

void
TAO_Storable_Naming_Context::rebind (const CosNaming::Name& n,
                                     CORBA::Object_ptr obj
                                     ACE_ENV_ARG_DECL)
{
  ACE_TRACE("rebind");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind (simple_name, obj ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  // If we received a simple name, we need to rebind it in this
  // context.
  else
    {
      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           obj,
                                           CosNaming::nobject);

      // Check for error conditions.
      if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());

      else if (result == -2)
        ACE_THROW (CosNaming::NamingContext::NotFound
                   (CosNaming::NamingContext::not_object, n));

      this->Write(flck.peer());
    }
}

void
TAO_Storable_Naming_Context::bind_context (const CosNaming::Name &n,
                                           CosNaming::NamingContext_ptr nc
                                           ACE_ENV_ARG_DECL)
{
  ACE_TRACE("bine_context");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // Do not allow binding of nil context reference.
  if (CORBA::is_nil (nc))
    ACE_THROW (CORBA::BAD_PARAM ());

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

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

      this->Write(flck.peer());
    }
}

void
TAO_Storable_Naming_Context::rebind_context (const CosNaming::Name &n,
                                         CosNaming::NamingContext_ptr nc
                                         ACE_ENV_ARG_DECL)
{
  ACE_TRACE("rebind_context");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind_context (simple_name, nc ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  // If we received a simple name, we need to rebind it in this
  // context.
  else
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

      this->Write(flck.peer());
    }
}

CORBA::Object_ptr
TAO_Storable_Naming_Context::resolve (const CosNaming::Name& n
                                      ACE_ENV_ARG_DECL)
{
  ACE_TRACE("resolve");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW_RETURN (CosNaming::NamingContext::InvalidName(),
                      CORBA::Object::_nil ());

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, "r"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CORBA::Object::_nil ());

  // Resolve the first component of the name.
  flck.release();

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
  ACE_TRACE("unbind");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

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
    {
      if (this->context_->unbind (n[0].id,
                                  n[0].kind) == -1)
        ACE_THROW (CosNaming::NamingContext::NotFound
                    (CosNaming::NamingContext::missing_node, n));

      this->Write(flck.peer());
    }
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::bind_new_context (const CosNaming::Name& n
                                               ACE_ENV_ARG_DECL)
{
  ACE_TRACE("bind_new_context");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW_RETURN (CosNaming::NamingContext::InvalidName(),
                      CosNaming::NamingContext::_nil ());

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

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNamgin::NamingContext::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CosNaming::NamingContext::_nil ());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the operation on
  // target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      return context->bind_new_context (simple_name ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());
    }
  // If we received a simple name, we need to bind it in this context.
  else 
  {
    // This had been a read on the file so now we are done with it
    flck.release();

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
}

void
TAO_Storable_Naming_Context::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRACE("destroy");
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Open the backing file
  File_Open_Lock_and_Check flck(this, "rw"
                                     ACE_ENV_ARG_PARAMETER);
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

      this->Write(flck.peer());
    }
}


void
TAO_Storable_Naming_Context::bind (const CosNaming::Name& n,
                               CORBA::Object_ptr obj
                               ACE_ENV_ARG_DECL)
{
  ACE_TRACE("bine");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
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

      this->Write(flck.peer());
    }
}


void
TAO_Storable_Naming_Context::list (CORBA::ULong how_many,
                                   CosNaming::BindingList_out &bl,
                                   CosNaming::BindingIterator_out &bi
                                   ACE_ENV_ARG_DECL)
{
  ACE_TRACE("list");
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

  // Open the backing file
  File_Open_Lock_and_Check flck(this, "r"
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // We have the map in memory, let the disk go
  flck.release();

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
    n = ACE_static_cast (CORBA::ULong, this->context_->current_size ());

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
      //  ***  This is a problem.  Is there an exception we can throw? ***
      ACE_THROW (CORBA::NO_IMPLEMENT());

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
                               const ACE_TCHAR *persistence_directory,
                               int use_redundancy
                               ACE_ENV_ARG_DECL)
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
                      context_size,
                      factory,
                      persistence_directory,
                      &new_context
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Now does this already exist on disk?
  ACE_CString file_name(persistence_directory);
  file_name += "/";
  file_name += poa_id;
  TAO_Storable_Base * fl = factory->create_stream(file_name, "r");
  if (fl->exists())
  {
    // Load the map from disk
    File_Open_Lock_and_Check flck(new_context, "r" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());
  }
  else
  {
    // Since this is a new context, make and empty map in it
    ACE_NEW_THROW_EX (new_context->storable_context_,
                      TAO_Storable_Bindings_Map (context_size,orb),
                      CORBA::NO_MEMORY ());
    ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());
    new_context->context_ = new_context->storable_context_;
    File_Open_Lock_and_Check flck(new_context, "wc" ACE_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());
    new_context->Write(flck.peer());
  }

  // build the global file name
  file_name += "_global";

  // Create the stream for the counter used to uniquely creat context names
  gfl_ = factory->create_stream(file_name, "crw");
  if (gfl_->open() != 0)
    {
      delete gfl_;
      ACE_THROW_RETURN(CORBA::PERSIST_STORE(),
                       CosNaming::NamingContext::_nil ());
    }

  // get the counter from disk
  TAO_NS_Persistence_Global global;
  *gfl_ >> global;
  gcounter_ = global.counter();
  if(redundant_) gfl_->close();

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
