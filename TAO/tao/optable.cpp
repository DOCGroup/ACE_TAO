#include "tao/optable.h"

// destructor
TAO_Operation_Table::~TAO_Operation_Table (void)
{
}

// Template Specialization for char*
int 
ACE_Hash_Map_Manager<const char *, TAO_Skeleton, ACE_SYNCH_RW_MUTEX>::equal (const char *const &id1,
									     const char *const &id2)
{
  return ACE_OS::strcmp (id1, id2) == 0;
}

// Template Specialization for char *
u_long
ACE_Hash_Map_Manager<const char *, TAO_Skeleton, ACE_SYNCH_RW_MUTEX>::hash (const char *const &ext_id)
{
  return ACE::hash_pjw (ext_id);
}

// constructor
TAO_Dynamic_Hash_OpTable::TAO_Dynamic_Hash_OpTable (const TAO_operation_db_entry *db,
						    CORBA_ULong dbsize,
						    CORBA_ULong hashtblsize) 
{
  if (hashtblsize > 0)
    this->hash_.open (hashtblsize);
  // otherwise, some default is chosen by the ACE_Hash_Map_Manager class

  // the job of the constructor is to go thru each entry of the database and
  // bind the operation name to its corresponding skeleton
  for (CORBA_ULong i=0; i < dbsize; i++)
    {
      // XXXASG: what happens if bind fails ???
      (void)this->bind (db[i].opname_, db[i].skel_ptr_);
    }
}

TAO_Dynamic_Hash_OpTable::~TAO_Dynamic_Hash_OpTable (void)
{
  // we need to go thru each entry and free up storage allocated to hold the
  // external ids. In this case, these are strings.
  OP_MAP_MANAGER::ITERATOR iterator (this->hash_);  // initialize an iterator

  for (OP_MAP_MANAGER::ENTRY *entry = 0;
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
TAO_Dynamic_Hash_OpTable::bind (const CORBA_String &opname,
                                const TAO_Skeleton skel_ptr)
{
  return this->hash_.bind (CORBA_string_dup (opname), skel_ptr);
}

int
TAO_Dynamic_Hash_OpTable::find (const CORBA_String &opname,
                                TAO_Skeleton& skel_ptr)
{
  return this->hash_.find ((const char *)opname, skel_ptr);
}

// Linear search strategy
TAO_Linear_OpTable::TAO_Linear_OpTable (const TAO_operation_db_entry *db, CORBA_ULong dbsize)
  : next_ (0),
    tablesize_ (dbsize),
    tbl_ (new TAO_Linear_OpTable_Entry[dbsize])
{
  // the job of the constructor is to go thru each entry of the database and
  // bind the operation name to its corresponding skeleton
  for (CORBA_ULong i=0; i < dbsize; i++)
    {
      // XXXASG: what happens if bind fails ???
      (void)this->bind (db[i].opname_, db[i].skel_ptr_);
    }
}

TAO_Linear_OpTable::~TAO_Linear_OpTable (void)
{
  delete [] this->tbl_;
}

int 
TAO_Linear_OpTable::bind (const CORBA_String &opname,
                          const TAO_Skeleton skel_ptr)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      this->tbl_[i].opname_ = CORBA_string_dup (opname);
      this->tbl_[i].skel_ptr_ = skel_ptr;
      this->next_++;
      return 0; // success
    }
  return -1; // error
}

int
TAO_Linear_OpTable::find (const CORBA_String &opname,
                          TAO_Skeleton& skel_ptr)
{
  ACE_ASSERT (this->next_ <= this->tablesize_);

  for (CORBA_ULong i = 0; i < this->next_; i++)
    if (!ACE_OS::strncmp (this->tbl_[i].opname_, opname, ACE_OS::strlen (opname)))
      {
        skel_ptr = this->tbl_[i].skel_ptr_;
        return 0; // success
      }
  return -1;  // not found
}

// constructor
TAO_Linear_OpTable_Entry::TAO_Linear_OpTable_Entry (void)
{
  opname_ = 0;
  skel_ptr_ = 0;
}

// destructor
TAO_Linear_OpTable_Entry::~TAO_Linear_OpTable_Entry (void)
{
  CORBA_string_free (opname_);
  this->skel_ptr_ = 0;  // cannot delete this as we do not own it
}

// Active Demux search strategy
TAO_Active_Demux_OpTable::TAO_Active_Demux_OpTable (const
						    TAO_operation_db_entry *db,
						    CORBA_ULong dbsize) 
  : next_ (0),
    tablesize_ (dbsize),
    tbl_ (new TAO_Active_Demux_OpTable_Entry[dbsize])
{
  // the job of the constructor is to go thru each entry of the database and
  // bind the operation name to its corresponding skeleton
  for (CORBA_ULong i=0; i < dbsize; i++)
    {
      // XXXASG: what happens if bind fails ???
      (void)this->bind (db[i].opname_, db[i].skel_ptr_);
    }
}

TAO_Active_Demux_OpTable::~TAO_Active_Demux_OpTable (void)
{
  delete [] this->tbl_;
}

int
TAO_Active_Demux_OpTable::bind (const CORBA_String &opname,
				const TAO_Skeleton skel_ptr)
{
  CORBA_ULong i = ACE_OS::atoi (opname);

  if (i < this->tablesize_)
    {
      if (this->tbl_[i].skel_ptr_ != 0)
	{
	  // overwriting previous one
	  return 1;
	}
      else
	{
	  this->tbl_[i].skel_ptr_ = skel_ptr;
	  return 0;
	}
    }
  return -1; // error
}

int
TAO_Active_Demux_OpTable::find (const CORBA_String &opname,
                                TAO_Skeleton& skel_ptr)
{
  CORBA_ULong i = ACE_OS::atoi (opname);

  ACE_ASSERT (i < this->tablesize_);
  skel_ptr = this->tbl_[i].skel_ptr_;
  return 0; //success
}

TAO_Active_Demux_OpTable_Entry::TAO_Active_Demux_OpTable_Entry (void)
{
  this->skel_ptr_ = 0;
}

TAO_Active_Demux_OpTable_Entry::~TAO_Active_Demux_OpTable_Entry (void)
{
  this->skel_ptr_ = 0;  // cannot delete this as we do not own it
}

// constructor
TAO_Operation_Table_Parameters::TAO_Operation_Table_Parameters (void)
  : strategy_ (0),
    type_ (TAO_Operation_Table_Parameters::TAO_DYNAMIC_HASH) // default
{
}

TAO_Operation_Table_Parameters::~TAO_Operation_Table_Parameters (void)
{
}

void 
TAO_Operation_Table_Parameters::lookup_strategy (TAO_Operation_Table_Parameters::DEMUX_STRATEGY s)
{
  this->type_ = s;
}

// get the lookup type
TAO_Operation_Table_Parameters::DEMUX_STRATEGY 
TAO_Operation_Table_Parameters::lookup_strategy (void) const
{
  return this->type_;
}

// set the concrete strategy
void 
TAO_Operation_Table_Parameters::concrete_strategy (TAO_Operation_Table *ot)
{
  this->strategy_ = ot;
}

// return the concrete strategy
TAO_Operation_Table* TAO_Operation_Table_Parameters::concrete_strategy (void)
{
  return this->strategy_;
}

TAO_Operation_Table_Factory::TAO_Operation_Table_Factory (void)
{
}

TAO_Operation_Table_Factory::~TAO_Operation_Table_Factory (void)
{
}

TAO_Operation_Table * 
TAO_Operation_Table_Factory::opname_lookup_strategy (void)
{
  TAO_Operation_Table_Parameters *p = TAO_OP_TABLE_PARAMETERS::instance ();

  return p->concrete_strategy ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator<const char*, TAO_Skeleton, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager<const char*, TAO_Skeleton, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Entry<const char*, TAO_Skeleton>;
template class ACE_Singleton<TAO_Operation_Table_Parameters, ACE_SYNCH_RW_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator<const char*, TAO_Skeleton, ACE_SYNCH_RW_MUTEX>;
#pragma instantiate ACE_Hash_Map_Manager<const char*, TAO_Skeleton, ACE_SYNCH_RW_MUTEX>;
#pragma instantiate ACE_Hash_Map_Entry<const char*, TAO_Skeleton>;
#pragma instantiate ACE_Singleton<TAO_Operation_Table_Parameters, ACE_SYNCH_RW_MUTEX>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
