#include "objtable.h"

// Template Specialization for char*
int ACE_Hash_Map_Manager<const char*, CORBA_Object_ptr,ACE_SYNCH_RW_MUTEX>::equal(const char* const&id1,
												                                  const char* const&id2)
{
   return strcmp(id1, id2) == 0;
}

// Template Specialization for char*
size_t ACE_Hash_Map_Manager<const char*, CORBA_Object_ptr,ACE_SYNCH_RW_MUTEX>::hash(const char* const&ext_id)
{
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
  this->hash_.close ();
}

int
TAO_Dynamic_Hash_ObjTable::bind (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr obj)
{
  ACE_CString objkey ((char *) key.buffer, key.length);

  return this->hash_.bind (objkey.rep(), obj);
}

int
TAO_Dynamic_Hash_ObjTable::find (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr &obj)
{
  ACE_CString objkey ((char *) key.buffer, key.length);
  return this->hash_.find (objkey.rep(), obj);
}

// Linear search strategy.
TAO_Linear_ObjTable::TAO_Linear_ObjTable (CORBA_ULong size)
  : next_ (0),
    tablesize_ (size),
    tbl_ (new TAO_Linear_ObjTable_Entry[size])
{
}

TAO_Linear_ObjTable::~TAO_Linear_ObjTable (void)
{
  delete [] this->tbl_;
}

// ****** we should really make sure that the same key doesn't exist
// ******

int
TAO_Linear_ObjTable::bind (const CORBA_OctetSeq &key,
			   CORBA_Object_ptr obj)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      this->tbl_[i].obj = obj;
      this->tbl_[i].key.buffer = new CORBA_Octet [key.length];
      this->tbl_[i].key.length = this->tbl_[i].key.maximum = key.length;
      ACE_OS::memcpy (this->tbl_[i].key.buffer, key.buffer, key.length);

      this->next_++;
      return 0;
    }
  return -1; // error
}

int
TAO_Linear_ObjTable::find (const CORBA_OctetSeq &key,
			   CORBA_Object_ptr &obj)
{


  ACE_ASSERT (this->next_ <= this->tablesize_);

  for (CORBA_ULong i = 0;
       i < this->next_;
       i++)
    if (!ACE_OS::memcmp (key.buffer, this->tbl_[i].key.buffer, key.length))
      {
	obj = this->tbl_[i].obj;
	return 1;
      }

  return -1;  // not found
}

TAO_Linear_ObjTable_Entry::TAO_Linear_ObjTable_Entry(void)
{
  this->key.buffer = 0;
  this->key.length = this->key.maximum = 0;
  this->obj = 0;
}

TAO_Linear_ObjTable_Entry::~TAO_Linear_ObjTable_Entry ()
{
  delete [] this->key.buffer;
  this->key.length = this->key.maximum = 0;
  this->obj = 0;  // cannot delete this as we do not own it
}

// Active Demux search strategy
TAO_Active_Demux_ObjTable::TAO_Active_Demux_ObjTable (CORBA_ULong size)
  : next_ (0),
    tablesize_ (size),
    tbl_ (new TAO_Active_Demux_ObjTable_Entry[size])
{
}

TAO_Active_Demux_ObjTable::~TAO_Active_Demux_ObjTable ()
{
  delete [] this->tbl_;
}

// ****** we should really make sure that the same key doesn't exist ******
int
TAO_Active_Demux_ObjTable::bind (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr obj)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      this->tbl_[i].obj = obj;
      this->next_++;
      return 0;
    }
  return -1; // error
}

int
TAO_Active_Demux_ObjTable::find (const CORBA_OctetSeq &key,
				 CORBA_Object_ptr& obj)
{
  CORBA_ULong i = ACE_OS::atoi ((char *)key.buffer);

  ACE_ASSERT (i <= this->tablesize_);
  obj = this->tbl_[i].obj;
  return 1;
}

TAO_Active_Demux_ObjTable_Entry::TAO_Active_Demux_ObjTable_Entry (void)
{
  this->obj = 0;
}

TAO_Active_Demux_ObjTable_Entry::~TAO_Active_Demux_ObjTable_Entry (void)
{
  this->obj = 0;  // cannot delete this as we do not own it
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Hash_Map_Manager<ACE_CString, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<ACE_CString, CORBA_Object_ptr>;
template class ACE_Hash_Map_Manager<char const*, CORBA_Object_ptr, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<char const*, CORBA_Object_ptr>;
template class ACE_Guard<ACE_SYNCH_RW_MUTEX>;
template class ACE_Read_Guard<ACE_SYNCH_RW_MUTEX>;
template class ACE_Write_Guard<ACE_SYNCH_RW_MUTEX>;
#endif
