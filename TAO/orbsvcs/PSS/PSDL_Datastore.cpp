// $Id$

#include "PSDL_Datastore.h"
#include "PSDL_OctetSeq.h"
#include "PSDL_String.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"

#include "tao/debug.h"
#include "tao/OctetSeqC.h"

#include "ace/Auto_Ptr.h"



TAO_PSDL_Datastore::TAO_PSDL_Datastore ()
  : allocator_ (0),
    obj_ref_map_ (0),
    index_file_ ("Data_Store"),
    base_address_ (ACE_DEFAULT_BASE_ADDR)
{
  this->open (this->index_file_,
              this->base_address_);
}

TAO_PSDL_Datastore::~TAO_PSDL_Datastore (void)
{
  // delete allocator_;
  // ACE_OS::free (const_cast<ACE_TCHAR *> (this->index_file_));
}

int
TAO_PSDL_Datastore::unbind (const char *name)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  TAO_PSDL_String psdl_string (this->allocator_);

  psdl_string = name;

  if (this->obj_ref_map_->unbind (psdl_string, this->allocator_) != 0)
    return -1;

  return 0;
}

int
TAO_PSDL_Datastore::bind (const char *name,
                          const CORBA::OctetSeq &octet_seq)
{
  int result = -1;

  TAO_PSDL_OctetSeq psdl_octet_seq (this->allocator_);

  psdl_octet_seq = octet_seq;

  TAO_PSDL_String psdl_string (this->allocator_);

  psdl_string = name;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  // Do a normal bind.  This will fail if there's already an
  // entry with the same name
  result = this->obj_ref_map_->bind (psdl_string,
                                     psdl_octet_seq,
                                     this->allocator_);

  // if (result == 1): Entry already exists.
  // if (result == -1): Bind failed.
  // if (result == 0) success.
  return result;
}

int
TAO_PSDL_Datastore::find (const char *name,
                          CORBA::OctetSeq &octet_seq)
{
  TAO_PSDL_OctetSeq psdl_seq;

  TAO_PSDL_String psdl_string (this->allocator_);

  psdl_string = CORBA::string_dup (name);

  int result = this->obj_ref_map_->find (psdl_string,
                                         psdl_seq,
                                         this->allocator_);

  if (result == 0)
    {
      octet_seq.replace (psdl_seq.length_,
                         psdl_seq.length_,
                         psdl_seq.buffer_);
    }
  else
    {
      ACE_DEBUG ((LM_ERROR,
                  "Couldnt find the entry in the Database. stopping",
                  -1));
    }

  return result;
}

ACE_Allocator*
TAO_PSDL_Datastore::allocator (void)
{
  return allocator_;
}

int
TAO_PSDL_Datastore::open (const ACE_TCHAR *file_name,
                          void *base_address)
{
  this->base_address_ = base_address;

  this->index_file_ = ACE_OS::strdup (file_name);
  if (this->index_file_ == 0)
    return -1;

  return create_index ();
}

int
TAO_PSDL_Datastore::create_index (void)
{
  // Make sure that the file name is of the legal length.
  if (ACE_OS::strlen (this->index_file_) >= MAXNAMELEN + MAXPATHLEN)
    {
      errno = ENAMETOOLONG;
      return -1;
    }

#if !defined (CHORUS)
  ACE_MMAP_Memory_Pool::OPTIONS options (this->base_address_);
#else
  // Use base address == 0, don't use a fixed address.
  ACE_MMAP_Memory_Pool::OPTIONS options (0,
                                         0,
                                         0,
                                         ACE_CHORUS_LOCAL_NAME_SPACE_T_SIZE);
#endif /* CHORUS */

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

  void *name_obj_map = 0;

  // This is the easy case since if we find hash table in the
  // memory-mapped file we know it's already initialized.
  if (this->allocator_->find (TAO_PERSISTENT_NAME_OBJ_MAP, name_obj_map) == 0)
    {
      this->obj_ref_map_ = static_cast<NAME_OBJ_REF_MAP *> (name_obj_map);
    }
  else
    {
      // Create a new <name_obj_map_> (because we've just created a new
      // memory-mapped file).

      size_t index_size = sizeof (NAME_OBJ_REF_MAP);
      name_obj_map = this->allocator_->malloc (index_size);

      if (name_obj_map == 0
          || create_index_helper (name_obj_map) == -1
          || this->allocator_->bind (TAO_PERSISTENT_NAME_OBJ_MAP,
                                     name_obj_map) == -1)
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
TAO_PSDL_Datastore::create_index_helper (void *buffer)
{
  this->obj_ref_map_ = new(buffer) NAME_OBJ_REF_MAP(this->allocator_) ;
  return 0;
}
