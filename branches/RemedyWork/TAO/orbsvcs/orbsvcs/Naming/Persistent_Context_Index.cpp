// $Id$

#include "orbsvcs/Naming/Persistent_Context_Index.h"
#include "orbsvcs/Naming/Persistent_Naming_Context.h"

#include "tao/debug.h"

#include "ace/Auto_Ptr.h"
#include "ace/OS_NS_unistd.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_Persistent_Context_Index::unbind (const char *poa_id)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  TAO_Persistent_Index_ExtId name (poa_id);
  TAO_Persistent_Index_IntId entry;
  if (this->index_->unbind (name, entry, this->allocator_) != 0)
    return -1;
  else
    {
      // Free up the memory we allocated in bind().
      this->allocator_->free ((void *) (entry.counter_));
      return 0;
    }
}

int
TAO_Persistent_Context_Index::bind (const char *poa_id,
                                    ACE_UINT32 *&counter,
                                    TAO_Persistent_Context_Index::CONTEXT *hash_map)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  // Allocate memory for items to be stored in the table.
  size_t poa_id_len = ACE_OS::strlen (poa_id) + 1;
  size_t counter_len = sizeof (ACE_UINT32);
  char *ptr = (char *) this->allocator_->malloc (poa_id_len + counter_len);

  if (ptr == 0)
    return -1;
  else
    {
      // Populate memory with data.
      counter = reinterpret_cast<ACE_UINT32 *> (ptr);
      *counter = 0;
      char * poa_id_ptr = ptr + counter_len;
      ACE_OS::strcpy (poa_id_ptr, poa_id);

      TAO_Persistent_Index_ExtId name (poa_id_ptr);
      TAO_Persistent_Index_IntId entry (counter, hash_map);
      int result = -1;

      // Do a normal bind.  This will fail if there's already an
      // entry with the same name.
      result = this->index_->bind (name, entry, this->allocator_);

      if (result == 1)
        {
          // Entry already existed so bind failed. Free our dynamically
          // allocated memory.
          this->allocator_->free ((void *) ptr);
          return result;
        }

      if (result == -1)
        // Free our dynamically allocated memory.
        this->allocator_->free ((void *) ptr);
      else
        // If bind() succeed, it will automatically sync
        // up the map manager entry.  However, we must sync up our
        // name/value memory.
        this->allocator_->sync (ptr, poa_id_len + counter_len);

      return result;
    }
}

TAO_Persistent_Context_Index::TAO_Persistent_Context_Index
  (CORBA::ORB_ptr orb,
   PortableServer::POA_ptr poa)
  : allocator_ (0),
    index_ (0),
    index_file_ (0),
    base_address_ (0),
    orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}

TAO_Persistent_Context_Index::~TAO_Persistent_Context_Index (void)
{
  delete allocator_;
  ACE_OS::free (reinterpret_cast<void *> (const_cast<ACE_TCHAR *> (index_file_)));
}

ACE_Allocator*
TAO_Persistent_Context_Index::allocator (void)
{
  return allocator_;
}

CosNaming::NamingContext_ptr
TAO_Persistent_Context_Index::root_context (void)
{
  return CosNaming::NamingContext::_duplicate (root_context_.in ());
}

CORBA::ORB_ptr
TAO_Persistent_Context_Index::orb (void)
{
  return orb_.in ();
}

int
TAO_Persistent_Context_Index::open (const ACE_TCHAR *file_name,
                                    void *base_address)
{
  this->base_address_ = base_address;

  index_file_ = ACE_OS::strdup (file_name);
  if (index_file_ == 0)
    return -1;

  return create_index ();
}

int
TAO_Persistent_Context_Index::init (size_t context_size)
{
  // Note: in case of an early exit from this (or helper) function due
  // to an error condition, we rely on POA to clean up all of the servants
  // already registered with it.

  // Return value of this function (necessary to keep compilers quiet).
  int status = 0;

  if (index_->current_size () == 0)
    // CASE 1:there are no Naming Contexts registered.  We need to create
    // one.
    {

      this->root_context_ =
        TAO_Persistent_Naming_Context::make_new_context (poa_.in (),
                                                         TAO_ROOT_NAMING_CONTEXT,
                                                         context_size,
                                                         this);
    }

  else
    // CASE 2:Recreate all Naming Contexts.
    status = recreate_all ();

  return status;
}

int
TAO_Persistent_Context_Index::recreate_all (void)
{
  CONTEXT_INDEX::ITERATOR *index_iter = 0;

  ACE_NEW_RETURN (index_iter,
                  (CONTEXT_INDEX::ITERATOR) (*index_),
                  -1);

  ACE_Auto_Basic_Ptr<CONTEXT_INDEX::ITERATOR> it (index_iter);

  // Because of broken old g++!!!
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId>  IND_DEF;

  IND_DEF::ENTRY *entry = 0;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Starting to recreate Naming Contexts from the file...\n"));

  // For each entry in <index_>, create a Naming Context servant.
  do
    {
      index_iter->next (entry);

      // Put together a servant for the new Naming Context.

      TAO_Persistent_Naming_Context *context_impl = 0;
      ACE_NEW_RETURN (context_impl,
                      TAO_Persistent_Naming_Context (poa_.in (),
                                                     entry->ext_id_.poa_id_,
                                                     this,
                                                     entry->int_id_.hash_map_,
                                                     entry->int_id_.counter_),
                  -1);


      // Put <context_impl> into the auto pointer temporarily, in case next
      // allocation fails.
      ACE_Auto_Basic_Ptr<TAO_Persistent_Naming_Context> temp (context_impl);

      TAO_Naming_Context *context = 0;
      ACE_NEW_RETURN (context,
                      TAO_Naming_Context (context_impl),
                      -1);

      // Let <implementation> know about it's <interface>.
      context_impl->interface (context);

      // Release auto pointer and start using reference counting to
      // control our servant.
      temp.release ();
      PortableServer::ServantBase_var s = context;

      // Register with the POA.
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (entry->ext_id_.poa_id_);

      this->poa_->activate_object_with_id (id.in (),
                                           context);

      CosNaming::NamingContext_var result = context->_this ();

      // If this is the root Naming Context, take a note of it.
      if (context_impl->root ())
          this->root_context_= result._retn ();

    } while (index_iter->advance ());

  return 0;
}

int
TAO_Persistent_Context_Index::create_index (void)
{
  // Make sure that the file name is of the legal length.
  if (ACE_OS::strlen (index_file_) >= MAXNAMELEN + MAXPATHLEN)
    {
      errno = ENAMETOOLONG;
      return -1;
    }

  ACE_MMAP_Memory_Pool::OPTIONS options (base_address_);

  // Create the allocator with the appropriate options.  The name used
  // for  the lock is the same as one used for the file.
  ACE_NEW_RETURN (this->allocator_,
                  ALLOCATOR (this->index_file_,
                             this->index_file_,
                             &options),
                  -1);

#if !defined (ACE_LACKS_ACCESS)
  // Now check if the backing store has been created successfully.
  if (ACE_OS::access (this->index_file_, F_OK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "create_index\n"),
                      -1);
#endif /* ACE_LACKS_ACCESS */

  void *context_index = 0;

  // This is the easy case since if we find hash table in the
  // memory-mapped file we know it's already initialized.
  if (this->allocator_->find (TAO_NAMING_CONTEXT_INDEX, context_index) == 0)
    this->index_ = (CONTEXT_INDEX *) context_index;

  // Create a new <index_> (because we've just created a new
  // memory-mapped file).
  else
    {
      size_t index_size = sizeof (CONTEXT_INDEX);
      context_index = this->allocator_->malloc (index_size);

      if (context_index == 0
          || create_index_helper (context_index) == -1
          || this->allocator_->bind (TAO_NAMING_CONTEXT_INDEX,
                                     context_index) == -1)
        {
          // Attempt to clean up.
          ACE_ERROR ((LM_ERROR,
                      "create_index\n"));
          this->allocator_->remove ();
          return -1;
        }
    }
  return 0;
}

int
TAO_Persistent_Context_Index::create_index_helper (void *buffer)
{
  this->index_ = new (buffer) CONTEXT_INDEX (this->allocator_);
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
