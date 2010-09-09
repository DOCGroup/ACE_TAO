#include "orbsvcs/Naming/Storable_Naming_Context.h"
#include "orbsvcs/Naming/Bindings_Iterator_T.h"

#include "tao/debug.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_stdio.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

const char * TAO_Storable_Naming_Context::root_name_;
ACE_UINT32 TAO_Storable_Naming_Context::gcounter_;
ACE_Auto_Ptr<TAO_Storable_Base> TAO_Storable_Naming_Context::gfl_;
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

void TAO_Storable_Naming_Context::Write(TAO_Storable_Base& wrtr)
{
  ACE_TRACE("Write");
  TAO_NS_Persistence_Header header;

  header.size (static_cast<unsigned int> (storable_context_->current_size()));
  header.destroyed (destroyed_);

  wrtr << header;

  if (0u == header.size ())
    return;

  ACE_Hash_Map_Iterator<TAO_Storable_ExtId,TAO_Storable_IntId,
                        ACE_Null_Mutex> it = storable_context_->map().begin();
  ACE_Hash_Map_Iterator<TAO_Storable_ExtId,TAO_Storable_IntId,
                        ACE_Null_Mutex> itend = storable_context_->map().end();

  ACE_Hash_Map_Entry<TAO_Storable_ExtId,TAO_Storable_IntId> ent = *it;

  while (!(it == itend))
  {
    TAO_NS_Persistence_Record record;

    ACE_CString name;
    CosNaming::BindingType bt = (*it).int_id_.type_;
    if (bt ==  CosNaming::ncontext)
      {
        CORBA::Object_var
          obj = orb_->string_to_object ((*it).int_id_.ref_.in ());
        if (obj->_is_collocated ())
          {
            // This is a local (i.e. non federated context) we therefore
            // store only the ObjectID (persistence filename) for the object.

            // The driving force behind storing ObjectIDs rather than IORs for
            // local contexts is to provide for a redundant naming service.
            // That is, a naming service that runs simultaneously on multiple
            // machines sharing a file system. It allows multiple redundant
            // copies to be started and stopped independently.
            // The original target platform was Tru64 Clusters where there was
            // a cluster address. In that scenario, clients may get different
            // servers on each request, hence the requirement to keep
            // synchronized to the disk. It also works on non-cluster system
            // where the client picks one of the redundant servers and uses it,
            // while other systems can pick different servers. (However in this
            // scenario, if a server fails and a client must pick a new server,
            // that client may not use any saved context IORs, instead starting
            // from the root to resolve names. So this latter mode is not quite
            // transparent to clients.) [Rich Seibel (seibel_r) of ociweb.com]

            PortableServer::ObjectId_var
              oid = poa_->reference_to_id (obj.in ());
            CORBA::String_var
              nm = PortableServer::ObjectId_to_string (oid.in ());
            const char
              *newname = nm.in ();
            name.set (newname); // The local ObjectID (persistance filename)
            record.type (TAO_NS_Persistence_Record::LOCAL_NCONTEXT);
          }
        else
          {
            // Since this is a foreign (federated) context, we can not store
            // the objectID (because it isn't in our storage), if we did, when
            // we restore, we would end up either not finding a permanent
            // record (and thus ending up incorrectly assuming the context was
            // destroyed) or loading another context altogether (just because
            // the contexts shares its objectID filename which is very likely).
            // [Simon Massey  (sma) of prismtech.com]

            name.set ((*it).int_id_.ref_.in ()); // The federated context IOR
            record.type (TAO_NS_Persistence_Record::REMOTE_NCONTEXT);
          }
      }
    else // if (bt == CosNaming::nobject) // shouldn't be any other, can there?
      {
        name.set ((*it).int_id_.ref_.in ()); // The non-context object IOR
        record.type (TAO_NS_Persistence_Record::OBJREF);
      }
    record.ref(name);

    const char *myid = (*it).ext_id_.id();
    ACE_CString id(myid);
    record.id(id);

    const char *mykind = (*it).ext_id_.kind();
    ACE_CString kind(mykind);
    record.kind(kind);

    wrtr << record;
    it.advance();
  }
}

// Helper function to load a new context into the binding_map
int
TAO_Storable_Naming_Context::load_map(File_Open_Lock_and_Check *flck)
{
  ACE_TRACE("load_map");
  // assume file already open for reading
  TAO_Storable_Bindings_Map *bindings_map;

  // create the new bindings map
  ACE_NEW_THROW_EX (bindings_map,
                    TAO_Storable_Bindings_Map (hash_table_size_,orb_.in()),
                    CORBA::NO_MEMORY ());

  // get the data for this bindings map from the file

  TAO_NS_Persistence_Header header;
  TAO_NS_Persistence_Record record;

  // we are only using the size from this header
  flck->peer() >> header;
  if (!flck->peer ().good ())
    {
      flck->peer ().clear ();
      throw CORBA::INTERNAL ();
    }

  // reset the destroyed flag
  this->destroyed_ = header.destroyed();

  // read in the data for the map
  for (unsigned int i= 0u; i<header.size(); ++i)
    {
      flck->peer() >> record;
      if (!flck->peer ().good ())
        {
          flck->peer ().clear ();
          throw CORBA::INTERNAL ();
        }

      if (TAO_NS_Persistence_Record::LOCAL_NCONTEXT == record.type ())
        {
          PortableServer::ObjectId_var
            id = PortableServer::string_to_ObjectId (record.ref ().c_str ());
          const char
            *intf = interface_->_interface_repository_id ();
          CORBA::Object_var
            objref = poa_->create_reference_with_id (id.in (), intf);
          bindings_map->bind ( record.id ().c_str (),
                               record.kind ().c_str (),
                               objref.in (),
                               CosNaming::ncontext );
        }
      else
        {
          CORBA::Object_var
            objref = orb_->string_to_object (record.ref ().c_str ());
          bindings_map->bind ( record.id ().c_str (),
                               record.kind ().c_str (),
                               objref.in (),
                               ((TAO_NS_Persistence_Record::REMOTE_NCONTEXT == record.type ())
                                ? CosNaming::ncontext    // REMOTE_NCONTEXT
                                : CosNaming::nobject )); // OBJREF
        }
    }
  storable_context_ = bindings_map;
  context_ = storable_context_;
  return 0;
}

TAO_Storable_Naming_Context::
File_Open_Lock_and_Check::File_Open_Lock_and_Check(
                                 TAO_Storable_Naming_Context * context,
                                 const char * mode)
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
    throw CORBA::PERSIST_STORE();
  }

  // build the file name
  ACE_CString file_name(context->persistence_directory_);
  file_name += "/";
  file_name += context->name_;

  // Create the stream
  fl_ = context->factory_->create_stream(file_name, ACE_TEXT_CHAR_TO_TCHAR(mode));
  if (TAO_Storable_Naming_Context::redundant_)
  {
    if (fl_->open() != 0)
      {
        delete fl_;
        throw CORBA::PERSIST_STORE();
      }

    // acquire a lock on it
    if (fl_ -> flock(0, 0, 0) != 0)
      {
         fl_->close();
         delete fl_;
         throw CORBA::INTERNAL();
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
         context->load_map(this);
       }
    }
  }
  else if ( ! context->storable_context_ || (rwflags_ & mode_write) )
  {
    if (fl_->open() != 0)
      {
        delete fl_;
        throw CORBA::PERSIST_STORE();
      }

    // now that the file is successfully opened
    // unlocked/closed before we leave this class
    closed_ = 0;

    if(!context->storable_context_)
    {
      // Load the map from disk
      context->load_map(this);
    }
  }
  else
    {
      // Need to insure that fl_ gets deleted
      delete fl_;
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
TAO_Storable_Naming_Context::File_Open_Lock_and_Check::peer(void)
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

  // If we're in the DTOR as a result of the destroy() operation,
  // then we go ahead and delete the file.  Otherwise, we leave the
  // file around because they need to be there for persistence.
  if (this->destroyed_)
    {
      // Make sure we delete the associated stream
      ACE_CString file_name (this->persistence_directory_);
      file_name += "/";
      file_name += this->name_;

      // Now delete the file
      ACE_Auto_Ptr<TAO_Storable_Base>
        fl (
          this->factory_->create_stream(file_name.c_str(),
                                        ACE_TEXT("r"))
          );
      if (fl.get())
        {
          if (TAO_debug_level > 5)
            ACE_DEBUG ((LM_DEBUG, "(%P|%t) NameService: removing file %s\n",
                        file_name.fast_rep()));
          fl->remove ();
        }
    }
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::make_new_context (
                              CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr poa,
                              const char *poa_id,
                              size_t context_size,
                              TAO_Naming_Service_Persistence_Factory *factory,
                              const ACE_TCHAR *persistence_directory,
                              TAO_Storable_Naming_Context **new_context)
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
    PortableServer::string_to_ObjectId (poa_id);

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
  ACE_TRACE("new_context");
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  {
    // Open the backing file
    File_Open_Lock_and_Check flck(this, "r");

    // Check to make sure this object didn't have <destroy> method
    // invoked on it.
    if (this->destroyed_)
      throw CORBA::OBJECT_NOT_EXIST ();
  }

  TAO_NS_Persistence_Global global;

  // Generate a POA id for the new context.
  if(redundant_)
  {
    // acquire a lock on the file that holds our counter
    if (gfl_->open() != 0)
      {
        delete gfl_.release();
        throw CORBA::PERSIST_STORE();
      }
    if (gfl_ -> flock(0, 0, 0) != 0)
         throw CORBA::INTERNAL();
    // get the counter from disk
    *gfl_.get() >> global;
    if (!gfl_.get ()->good () &&
        gfl_.get ()->rdstate () != TAO_Storable_Base::eofbit)
      {
        gfl_.get ()->clear ();
        throw CORBA::INTERNAL ();
      }
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
  *gfl_.get() << global;
  if(redundant_)
  {
    // and release our lock
    if (gfl_ -> flock(0, 0, 0) != 0)
         throw CORBA::INTERNAL();
    gfl_->close();
  }

  // Create a new context.
  TAO_Storable_Naming_Context *new_context = 0;
  CosNaming::NamingContext_var result =
    make_new_context (this->orb_.in (),
                      this->poa_.in (),
                      poa_id,
                      this->storable_context_->total_size (),
                      this->factory_,
                      ACE_TEXT_CHAR_TO_TCHAR (this->persistence_directory_.c_str ()),
                      &new_context);

  // Since this is a new context, make an empty map in it
  ACE_NEW_THROW_EX (new_context->storable_context_,
                    TAO_Storable_Bindings_Map (hash_table_size_,orb_.in ()),
                    CORBA::NO_MEMORY ());
  new_context->context_ = new_context->storable_context_;

  File_Open_Lock_and_Check flck(new_context, "wc");
  new_context->Write(flck.peer());

  return result._retn ();
}

void
TAO_Storable_Naming_Context::rebind (const CosNaming::Name& n,
                                     CORBA::Object_ptr obj)
{
  ACE_TRACE("rebind");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind (simple_name, obj);
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
        throw CORBA::INTERNAL ();

      else if (result == -2)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_object,
          n);

      this->Write(flck.peer());
    }
}

void
TAO_Storable_Naming_Context::bind_context (const CosNaming::Name &n,
                                           CosNaming::NamingContext_ptr nc)
{
  ACE_TRACE("TAO_Storable_Naming_Context::bind_context");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // Do not allow binding of nil context reference.
  if (CORBA::is_nil (nc))
    throw CORBA::BAD_PARAM ();

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->bind_context (simple_name, nc);
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
  ACE_TRACE("rebind_context");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind_context (simple_name, nc);
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
        throw CORBA::INTERNAL ();
      else if (result == -2)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);

      this->Write(flck.peer());
    }
}

CORBA::Object_ptr
TAO_Storable_Naming_Context::resolve (const CosNaming::Name& n)
{
  ACE_TRACE("resolve");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, "r");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

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
    throw CosNaming::NamingContext::NotFound(
      CosNaming::NamingContext::missing_node,
      n);

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
          context = CosNaming::NamingContext::_narrow (result.in ());
        }
      else
        // The first name component wasn't bound to a NamingContext.
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);

      // If narrow failed...
      if (CORBA::is_nil (context.in ()))
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);
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
             const_cast<CosNaming::NameComponent*> (n.get_buffer ()) + 1);

          // If there are any exceptions, they will propagate up.
          return context->resolve (rest_of_name);
        }
    }
  // If the name we had to resolve was simple, we just need to return
  // the result.
  return result._retn ();
}

void
TAO_Storable_Naming_Context::unbind (const CosNaming::Name& n)
{
  ACE_TRACE("unbind");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->unbind (simple_name);
    }
  // If we received a simple name, we need to unbind it in this
  // context.
  else
    {
      if (this->context_->unbind (n[0].id,
                                  n[0].kind) == -1)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::missing_node,
          n);

      this->Write(flck.peer());
    }
}

CosNaming::NamingContext_ptr
TAO_Storable_Naming_Context::bind_new_context (const CosNaming::Name& n)
{
  ACE_TRACE("bind_new_context");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the operation on
  // target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      return context->bind_new_context (simple_name);
    }
  // If we received a simple name, we need to bind it in this context.

  // This had been a read on the file so now we are done with it
  flck.release();

  // Stores our new Naming Context.
  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  // Create new context.
  result = new_context ();

  // Bind the new context to the name.
  try
    {
      bind_context (n,
                    result.in ());
    }
  catch (const CORBA::Exception&)
    {
      {
        try
          {
            result->destroy ();
          }
        catch (const CORBA::Exception&)
          {
            // Do nothing?
          }
      }
      // Re-raise the exception in bind_context()
      throw;
    }
  return result._retn ();
}

void
TAO_Storable_Naming_Context::destroy (void)
{
  ACE_TRACE("destroy");
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  if (this->context_->current_size () != 0)
    throw CosNaming::NamingContext::NotEmpty();

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


      poa->deactivate_object (id.in ());

      this->Write(flck.peer());
    }
}


void
TAO_Storable_Naming_Context::bind (const CosNaming::Name& n,
                               CORBA::Object_ptr obj)
{
  ACE_TRACE("TAO_Storable_Naming_Context::bind");
  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // we didn't need a lock to check the input arg, but now we do
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, name_len > 1 ? "r" : "rw");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding
  // on target context.
  if (name_len > 1)
    {
      // This had been a read on the file so now we are done with it
      flck.release();

      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->bind (simple_name, obj);
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
        throw CosNaming::NamingContext::AlreadyBound();

      // Something went wrong with the internal structure
      else if (result == -1)
        throw CORBA::INTERNAL ();

      this->Write(flck.peer());
    }
}


void
TAO_Storable_Naming_Context::list (CORBA::ULong how_many,
                                   CosNaming::BindingList_out &bl,
                                   CosNaming::BindingIterator_out &bi)
{
  ACE_TRACE("list");
  // Allocate nil out parameters in case we won't be able to complete
  // the operation.
  bi = CosNaming::BindingIterator::_nil ();
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());

  // Obtain a lock before we proceed with the operation.
  ACE_GUARD_THROW_EX (ACE_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Open the backing file
  File_Open_Lock_and_Check flck(this, "r");

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // We have the map in memory, let the disk go
  flck.release();

  // Dynamically allocate iterator for traversing the underlying hash map.
  HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    HASH_MAP::ITERATOR (storable_context_->map ()),
                    CORBA::NO_MEMORY ());

  // Store <hash_iter temporarily in auto pointer, in case we'll have
  // some failures and throw an exception.
  ACE_Auto_Basic_Ptr<HASH_MAP::ITERATOR> temp (hash_iter);

  // Silliness below is required because of broken old g++!!!  E.g.,
  // without it, we could have just said HASH_MAP::ITERATOR everywhere we use ITER_DEF.
  typedef ACE_Hash_Map_Manager<TAO_Storable_ExtId,
                               TAO_Storable_IntId,
                               ACE_Null_Mutex>::ITERATOR ITER_DEF;
  typedef ACE_Hash_Map_Manager<TAO_Storable_ExtId,
                               TAO_Storable_IntId,
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
    n = static_cast<CORBA::ULong> (this->context_->current_size ());

  // Use the hash map iterator to populate <bl> with bindings.
  bl->length (n);

  ENTRY_DEF *hash_entry = 0;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (ITER_SERVANT::populate_binding (hash_entry, bl[i]) == 0)
          throw CORBA::NO_MEMORY();
    }

  // Now we are done with the BindingsList, and we can follow up on
  // the BindingIterator business.

  // If we do not need to pass back BindingIterator.
  if (this->context_->current_size () <= how_many)
    return;
  else if (redundant_)
    {
      //  ***  This is a problem.  Is there an exception we can throw? ***
      ACE_UNUSED_ARG (bind_iter);
      throw CORBA::NO_IMPLEMENT ();
    }
  else
    {
      // Create a BindingIterator for return.
      ACE_NEW_THROW_EX (bind_iter,
                        ITER_SERVANT (this, hash_iter,
                                      this->poa_.in (), this->lock_),
                        CORBA::NO_MEMORY ());

      // Release <hash_iter> from auto pointer, and start using
      // reference counting to control our servant.
      temp.release ();
      PortableServer::ServantBase_var iter = bind_iter;

      // Increment reference count on this Naming Context, so it doesn't get
      // deleted before the BindingIterator servant gets deleted.
      interface_->_add_ref ();

      // Register with the POA.
      // Is an ACE_UINT32 enough?
      char poa_id[BUFSIZ];
      ACE_OS::snprintf (poa_id,
                        BUFSIZ,
                        "%s_%d",
                        this->poa_id_.c_str (),
                        this->gcounter_++);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (),
                                           bind_iter);
      bi = bind_iter->_this ();
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include "orbsvcs/Naming/Naming_Service_Container.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

CosNaming::NamingContext_ptr TAO_Storable_Naming_Context::recreate_all(
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               const char *poa_id,
                               size_t context_size,
                               int reentering,
                               TAO_Naming_Service_Persistence_Factory *factory,
                               const ACE_TCHAR *persistence_directory,
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
                      context_size,
                      factory,
                      persistence_directory,
                      &new_context);

  // Now does this already exist on disk?
  ACE_TString file_name(persistence_directory);
  file_name += ACE_TEXT("/");
  file_name += ACE_TEXT_CHAR_TO_TCHAR(poa_id);
  ACE_Auto_Ptr<TAO_Storable_Base> fl (factory->create_stream(ACE_TEXT_ALWAYS_CHAR(file_name.c_str()), ACE_TEXT("r")));
  if (fl->exists())
  {
    // Load the map from disk
    File_Open_Lock_and_Check flck(new_context, "r");
  }
  else
  {
    // Since this is a new context, make and empty map in it
    ACE_NEW_THROW_EX (new_context->storable_context_,
                      TAO_Storable_Bindings_Map (context_size,orb),
                      CORBA::NO_MEMORY ());
    new_context->context_ = new_context->storable_context_;
    File_Open_Lock_and_Check flck(new_context, "wc");
    new_context->Write(flck.peer());
  }

  // build the global file name
  file_name += ACE_TEXT("_global");

  // Create the stream for the counter used to uniquely creat context names
  gfl_.reset(factory->create_stream(ACE_TEXT_ALWAYS_CHAR(file_name.c_str()), ACE_TEXT("crw")));
  if (gfl_->open() != 0)
    {
      delete gfl_.release();
      throw CORBA::PERSIST_STORE();
    }

  // get the counter from disk
  TAO_NS_Persistence_Global global;
  *gfl_.get() >> global;
  if (!gfl_.get ()->good () &&
      gfl_.get ()->rdstate () != TAO_Storable_Base::eofbit)
    {
      gfl_.get ()->clear ();
      throw CORBA::INTERNAL ();
    }
  gcounter_ = global.counter();
  if(redundant_) gfl_->close();

  return result._retn ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
