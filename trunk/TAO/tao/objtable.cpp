#include "tao/objtable.h"

// Template Specialization for char*. Needed for the dynamic hash lookup
int 
ACE_Hash_Map_Manager<const char *, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>::equal (const char *const &id1,
										 const char *const &id2)
{
  // do a string compare
  return ACE_OS::strcmp (id1, id2) == 0;
}

// Template Specialization for char *
u_long
ACE_Hash_Map_Manager<const char *, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>::hash (const char *const &ext_id)
{
  // Use the hash_pjw hash function available in the ACE library
  return ACE::hash_pjw (ext_id);
}

TAO_Dynamic_Hash_ObjTable::TAO_Dynamic_Hash_ObjTable (CORBA_ULong size)
{
  if (size > 0)
    this->hash_.open (size);
  // else we already have a default hash map
}

TAO_Dynamic_Hash_ObjTable::~TAO_Dynamic_Hash_ObjTable (void)
{
  // we need to go thru each entry and free the space taken up by the strings
  OBJ_MAP_MANAGER::ITERATOR iterator (this->hash_);  // initialize an iterator

  for (OBJ_MAP_MANAGER::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance ())
    {
      CORBA_string_free ((char *)entry->ext_id_); // we had allocated memory and stored
					  // the string. So we free the memory
      entry->int_id_ = 0;  // we do not own this. So we just set it to 0
    }

  this->hash_.close ();
}

int
TAO_Dynamic_Hash_ObjTable::bind (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr obj)
{
  // the key is an octet sequence. Hence, we cannot simply cast the buffer to a
  // char* as it may result in an arbitrary name. Hence we must first convert
  // it to a string and then save a copy of the string in the table.
  ACE_CString objkey ((char *)key.buffer, key.length);
  return this->hash_.bind (CORBA_string_dup (objkey.rep ()), obj);
}

int
TAO_Dynamic_Hash_ObjTable::find (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr &obj)
{
  // the key is an octet sequence. Hence, we cannot simply cast the buffer to a
  // char* as it may result in an arbitrary name due to absence of a NULL
  // terminating character. Hence we must first convert it to a string of the
  // specified length.
  ACE_CString objkey ((char *)key.buffer, key.length);
  return this->hash_.find (objkey.rep(), obj); // no string_dup necessary here
}

// Linear search strategy
TAO_Linear_ObjTable::TAO_Linear_ObjTable (CORBA_ULong size)
  : next_ (0),
    tablesize_ (size),
    tbl_ (new TAO_Linear_ObjTable_Entry[size])
{
}

TAO_Linear_ObjTable::~TAO_Linear_ObjTable (void)
{
  delete [] this->tbl_; // this will delete each entry
}

int
TAO_Linear_ObjTable::bind (const CORBA_OctetSeq &key,
			   CORBA_Object_ptr obj)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      // store the string and the corresponding object pointer
      this->tbl_[i].opname_ = CORBA_string_alloc (key.length); // allocates one
							       // more
      ACE_OS::memset (this->tbl_[i].opname_, '\0', key.length+1);
      ACE_OS::strncpy (this->tbl_[i].opname_, (char *)key.buffer, key.length);
      this->tbl_[i].obj_ = obj;
      this->next_++; // point to the next available slot
      return 0; // success
    }

  return -1; // error, size exceeded
}

// find if the key exists
int
TAO_Linear_ObjTable::find (const CORBA_OctetSeq &key,
			   CORBA_Object_ptr &obj)
{
  ACE_ASSERT (this->next_ <= this->tablesize_);

  for (CORBA_ULong i = 0; i < this->next_; i++)
    {
      // linearly search thru the table
      if (!ACE_OS::strncmp (this->tbl_[i].opname_, (char *)key.buffer, key.length))
	{
	  // keys match. Return the object pointer
	  obj = this->tbl_[i].obj_;
	  return 0; // success
	}
    }
  return -1;  // not found
}

// constructor
TAO_Linear_ObjTable_Entry::TAO_Linear_ObjTable_Entry (void)
{
  this->opname_ = 0;
  this->obj_ = 0;
}

TAO_Linear_ObjTable_Entry::~TAO_Linear_ObjTable_Entry (void)
{
  CORBA_string_free (this->opname_); // reclaim space consumed by the string
  this->obj_ = 0;  // cannot delete this as we do not own it
}

// Active Demux search strategy
// constructor
TAO_Active_Demux_ObjTable::TAO_Active_Demux_ObjTable (CORBA_ULong size)
  : next_ (0),
    tablesize_ (size),
    tbl_ (new TAO_Active_Demux_ObjTable_Entry[size])
{
}

// destructor
TAO_Active_Demux_ObjTable::~TAO_Active_Demux_ObjTable ()
{
  delete [] this->tbl_;
}


// bind the object based on the key
int
TAO_Active_Demux_ObjTable::bind (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr obj)
{
  // The active demux strategy works on the assumption that the key is a
  // stringified form of an index into the table
  CORBA_ULong i = ACE_OS::atoi ((char *)key.buffer);

  if (i < this->tablesize_)
    {
      if (this->tbl_[i].obj_ != 0)
	{
	  // we are trying to overwrite a previous entry
	  return 1; // duplicate
	}
      else
	{
	  this->tbl_[i].obj_ = obj;
	  return 0;
	}
    }
  return -1; // error
}

int
TAO_Active_Demux_ObjTable::find (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr& obj)
{
  CORBA_ULong i = ACE_OS::atoi ((char *)key.buffer);

  ACE_ASSERT (i < this->tablesize_); // cannot be equal to
  obj = this->tbl_[i].obj_;
  return 0; // success
}

TAO_Active_Demux_ObjTable_Entry::TAO_Active_Demux_ObjTable_Entry (void)
{
  this->obj_ = 0;
}

TAO_Active_Demux_ObjTable_Entry::~TAO_Active_Demux_ObjTable_Entry (void)
{
  this->obj_ = 0;  // cannot delete this as we do not own it
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator<char const*, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<char const*, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<char const*, CORBA_Object_ptr>;
template class ACE_Guard<ACE_SYNCH_RW_MUTEX>;
template class ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator<char const*, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>;
#pragma instantiate ACE_Hash_Map_Manager<char const*, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>;
#pragma instantiate ACE_Hash_Map_Entry<char const*, CORBA_Object_ptr>;
#pragma instantiate ACE_Guard<ACE_SYNCH_RW_MUTEX>;
#pragma instantiate ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>;
#pragma instantiate ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
