#include "objtable.hh"

TAO_Dynamic_Hash_ObjTable::TAO_Dynamic_Hash_ObjTable(CORBA_ULong size)
{
  if (size > 0)
    this->hash_.open(size);
  // else we already have a default hash map
}

TAO_Dynamic_Hash_ObjTable::~TAO_Dynamic_Hash_ObjTable()
{
  this->hash_.close();
}

void TAO_Dynamic_Hash_ObjTable::register_obj(const CORBA_OctetSeq &key, const
					     CORBA_Object_ptr &obj)
{
  ACE_CString objkey ((char *)key.buffer);
  (void)this->hash_.bind(objkey, obj);
  // we need error handling here
}

CORBA_Object_ptr TAO_Dynamic_Hash_ObjTable::lookup(const CORBA_OctetSeq &key)
{
  ACE_CString objkey ((char *)key.buffer);
  CORBA_Object_ptr obj = 0;

  if (!this->hash_.find(objkey, obj)){
    return obj;
  }
  return 0; // otherwise
}

// Linear search strategy
TAO_Linear_ObjTable::TAO_Linear_ObjTable(CORBA_ULong size)
  : next_(0),
    tablesize_(size),
    tbl_(new TAO_Linear_ObjTable::Entry[size])
{
}

TAO_Linear_ObjTable::~TAO_Linear_ObjTable()
{
  delete [] this->tbl_;
}

// ****** we should really make sure that the same key doesn't exist ******
void TAO_Linear_ObjTable::register_obj(const CORBA_OctetSeq &key, const CORBA_Object_ptr &obj)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      this->tbl_[i].obj = obj;
      this->tbl_[i].key.buffer = new CORBA_Octet [key.length];
      this->tbl_[i].key.length = this->tbl_[i].key.maximum = key.length;
      ACE_OS::memcpy(this->tbl_[i].key.buffer, key.buffer, key.length);

      this->next_++;
    }
}

CORBA_Object_ptr TAO_Linear_ObjTable::lookup(const CORBA_OctetSeq &key)
{
  CORBA_ULong i;

  ACE_ASSERT(this->next_ <= this->tablesize_);
  i=0;
  while ( i < this->next_) 
    {
      if (!ACE_OS::memcmp(key.buffer, this->tbl_[i].key.buffer, key.length))
	{
	  return this->tbl_[i].obj;
	}
      i++;
    }
  return 0;  // not found
}

TAO_Linear_ObjTable::Entry::Entry()
{
  this->key.buffer = 0;
  this->key.length = this->key.maximum = 0;
  this->obj = 0;
}

TAO_Linear_ObjTable::Entry::~Entry()
{
  if (this->key.buffer)
    delete [] this->key.buffer;
  this->key.length = this->key.maximum = 0;
  this->obj = 0;  // cannot delete this as we do not own it
}

// Active Demux search strategy
TAO_Active_Demux_ObjTable::TAO_Active_Demux_ObjTable(CORBA_ULong size)
  : next_(0),
    tablesize_(size),
    tbl_(new TAO_Active_Demux_ObjTable::Entry[size])
{
}

TAO_Active_Demux_ObjTable::~TAO_Active_Demux_ObjTable()
{
  delete [] this->tbl_;
}

// ****** we should really make sure that the same key doesn't exist ******
void TAO_Active_Demux_ObjTable::register_obj(const CORBA_OctetSeq &key, const CORBA_Object_ptr &obj)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      this->tbl_[i].obj = obj;
      this->next_++;
    }
}

CORBA_Object_ptr TAO_Active_Demux_ObjTable::lookup(const CORBA_OctetSeq &key)
{
  CORBA_ULong i = ACE_OS::atoi((char *)key.buffer);

  ACE_ASSERT(i <= this->tablesize_);
  return this->tbl_[i].obj;
}

TAO_Active_Demux_ObjTable::Entry::Entry()
{
  this->obj = 0;
}

TAO_Active_Demux_ObjTable::Entry::~Entry()
{
  this->obj = 0;  // cannot delete this as we do not own it
}

