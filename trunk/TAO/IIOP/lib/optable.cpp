#include "optable.h"

TAO_Dynamic_Hash_OpTable::TAO_Dynamic_Hash_OpTable (CORBA_ULong size)
{
  if (size > 0)
    this->hash_.open (size);
  // else we already have a default hash map
}

TAO_Dynamic_Hash_OpTable::~TAO_Dynamic_Hash_OpTable (void)
{
  this->hash_.close ();
}

int 
TAO_Dynamic_Hash_OpTable::bind (const CORBA_String &opname,
                                const TAO_Skeleton skel_ptr)
{
  ACE_CString key (opname);
  return this->hash_.bind (key, skel_ptr);
}

int
TAO_Dynamic_Hash_OpTable::find (const CORBA_String &opname,
                                TAO_Skeleton& skel_ptr)
{
  ACE_CString key (opname);

  return this->hash_.find (key, skel_ptr);
}

// Linear search strategy
TAO_Linear_OpTable::TAO_Linear_OpTable (CORBA_ULong size)
  : next_ (0),
    tablesize_ (size),
    tbl_ (new TAO_Linear_OpTable::Entry[size])
{
}

TAO_Linear_OpTable::~TAO_Linear_OpTable (void)
{
  delete [] this->tbl_;
}

// ****** we should really make sure that the same key doesn't exist
// ******
int 
TAO_Linear_OpTable::bind (const CORBA_String &opname,
                          const TAO_Skeleton skel_ptr)
{
  CORBA_ULong i = this->next_;

  if (i < this->tablesize_)
    {
      this->tbl_[i].opname = opname;
      this->tbl_[i].skel_ptr = skel_ptr;
      this->next_++;
      return 0;
    }
  return -1; // error
}

int
TAO_Linear_OpTable::find (const CORBA_String &opname,
                          TAO_Skeleton& skel_ptr)
{
  ACE_ASSERT (this->next_ <= this->tablesize_);

  for (CORBA_ULong i;
       i < this->next_;
       i++)
    if (!ACE_OS::strncmp (opname, this->tbl_[i].opname, ACE_OS::strlen (opname)))
      {
        skel_ptr = this->tbl_[i].skel_ptr;
        return 1;
      }
  
  return -1;  // not found
}

TAO_Linear_OpTable::Entry::Entry (void)
{
  this->opname = 0;
  this->skel_ptr = 0;
}

TAO_Linear_OpTable::Entry::~Entry (void)
{
  this->opname = 0;
  this->skel_ptr = 0;  // cannot delete this as we do not own it
}

// Active Demux search strategy
TAO_Active_Demux_OpTable::TAO_Active_Demux_OpTable (CORBA_ULong size)
  : next_ (0),
    tablesize_ (size),
    tbl_ (new TAO_Active_Demux_OpTable::Entry[size])
{
}

TAO_Active_Demux_OpTable::~TAO_Active_Demux_OpTable (void)
{
  delete [] this->tbl_;
}

// ****** we should really make sure that the same key doesn't exist ******
int 
TAO_Active_Demux_OpTable::bind (const CORBA_String &opname,
				const TAO_Skeleton skel_ptr)
{
  CORBA_ULong i = ACE_OS::atoi (opname);

  if (i < this->tablesize_)
    {
      this->tbl_[i].skel_ptr = skel_ptr;
      return 0;
    }
  return -1; // error
}

int
TAO_Active_Demux_OpTable::find (const CORBA_String &opname,
                                TAO_Skeleton& skel_ptr)
{
  CORBA_ULong i = ACE_OS::atoi (opname);

  ACE_ASSERT (i <= this->tablesize_);
  skel_ptr = this->tbl_[i].skel_ptr;
  return 1;
}

TAO_Active_Demux_OpTable::Entry::Entry (void)
{
  this->skel_ptr = 0;
}

TAO_Active_Demux_OpTable::Entry::~Entry (void)
{
  this->skel_ptr = 0;  // cannot delete this as we do not own it
}

TAO_Operation_Table_Parameters::TAO_Operation_Table_Parameters (void)
  : type_ (TAO_Operation_Table_Parameters::TAO_DYNAMIC_HASH),
    strategy_ (0)
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

TAO_Operation_Table_Parameters::DEMUX_STRATEGY TAO_Operation_Table_Parameters::lookup_strategy (void) const
{
  return this->type_;
}

void 
TAO_Operation_Table_Parameters::concrete_strategy (TAO_Operation_Table *ot)
{
  this->strategy_ = ot;
}

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
