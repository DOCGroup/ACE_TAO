// $Id$

#include "Operation_Table.h"
#include "tao/Timeprobe.h"
#include "tao/ORB.h"

ACE_RCSID(PortableServer,
          Operation_Table,
          "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Operation_Table_Timeprobe_Description[] =
  {
    "TAO_Dynamic_Hash_OpTable::find - start",
    "TAO_Dynamic_Hash_OpTable::find - end",

    "TAO_Linear_Search_OpTable::find - start",
    "TAO_Linear_Search_OpTable::find - end",

    "TAO_Active_Demux_OpTable::find - start",
    "TAO_Active_Demux_OpTable::find - end",

    "TAO_Perfect_Hash_OpTable::find - start",
    "TAO_Perfect_Hash_OpTable::find - end",

    "TAO_Binary_Search_OpTable::find - start",
    "TAO_Binary_Search_OpTable::find - end"
  };

enum
  {
    // Timeprobe description table start key
    TAO_DYNAMIC_HASH_OPTABLE_FIND_START = 600,
    TAO_DYNAMIC_HASH_OPTABLE_FIND_END,

    TAO_LINEAR_SEARCH_OPTABLE_FIND_START,
    TAO_LINEAR_SEARCH_OPTABLE_FIND_END,

    TAO_ACTIVE_DEMUX_OPTABLE_FIND_START,
    TAO_ACTIVE_DEMUX_OPTABLE_FIND_END,

    TAO_PERFECT_HASH_OPTABLE_FIND_START,
    TAO_PERFECT_HASH_OPTABLE_FIND_END,

    TAO_BINARY_SEARCH_OPTABLE_FIND_START,
    TAO_BINARY_SEARCH_OPTABLE_FIND_END
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Operation_Table_Timeprobe_Description,
                                  TAO_DYNAMIC_HASH_OPTABLE_FIND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

// destructor
TAO_Operation_Table::~TAO_Operation_Table (void)
{
}

// constructor
TAO_Dynamic_Hash_OpTable::TAO_Dynamic_Hash_OpTable (const TAO_operation_db_entry *db,
                                                    CORBA::ULong dbsize,
                                                    CORBA::ULong hashtblsize,
                                                    ACE_Allocator *alloc)
  : hash_ (hashtblsize, alloc)
{
  // Iterate thru each entry in the database and bind the operation
  // name to its corresponding skeleton.
  for (CORBA::ULong i = 0; i < dbsize; i++)
    {
      TAO::Operation_Skeletons s;
      s.skel_ptr_ = db[i].skel_ptr_;
      s.thruPOA_skel_ptr_ = db[i].thruPOA_skel_ptr_;
      s.direct_skel_ptr_ = db[i].direct_skel_ptr_;

      // @@ (ASG): what happens if bind fails ???
      if (this->bind (db[i].opname_, s) == -1)
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) %p\n"),
                    ACE_TEXT ("bind failed")));
    }
}

TAO_Dynamic_Hash_OpTable::~TAO_Dynamic_Hash_OpTable (void)
{
  // Initialize an iterator.  We need to go thru each entry and free
  // up storage allocated to hold the external ids.  In this case,
  // these are strings.
  OP_MAP_MANAGER::ITERATOR iterator (this->hash_);

  for (OP_MAP_MANAGER::ENTRY *entry = 0;
       iterator.next (entry) != 0;
       iterator.advance ())
    {
      // We had allocated memory and stored the string. So we free the
      // memory.
      CORBA::string_free ((char *) entry->ext_id_);
      entry->ext_id_ = 0;
    }
}

int
TAO_Dynamic_Hash_OpTable::bind (const char *opname,
                                const TAO::Operation_Skeletons skel_ptr)
{
  return this->hash_.bind (CORBA::string_dup (opname),
                           skel_ptr);
}

int
TAO_Dynamic_Hash_OpTable::find (const char *opname,
                                TAO_Skeleton& skel_ptr,
                                const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_DYNAMIC_HASH_OPTABLE_FIND_START);
  TAO::Operation_Skeletons s;

  int retval =
    this->hash_.find ((const char *)opname,
                      s);

  if (retval != -1)
    {
      skel_ptr = s.skel_ptr_;
    }

  return retval;
}

int
TAO_Dynamic_Hash_OpTable::find (const char *opname,
                                TAO_Collocated_Skeleton& skel_ptr,
                                TAO::Collocation_Strategy s,
                                const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_DYNAMIC_HASH_OPTABLE_FIND_START);

  TAO::Operation_Skeletons skel;

  int retval =
    this->hash_.find ((const char *)opname, skel);

  if (retval != -1)
    {
      switch (s)
        {
        case TAO::TAO_CS_THRU_POA_STRATEGY:
          skel_ptr = skel.thruPOA_skel_ptr_;
          break;
        case TAO::TAO_CS_DIRECT_STRATEGY:
          skel_ptr = skel.direct_skel_ptr_;
          break;
        default:
          return -1;
        }
    }

  return retval;
}

/***************************************************************/

TAO_Linear_Search_OpTable::TAO_Linear_Search_OpTable (void)
{
}

TAO_Linear_Search_OpTable::~TAO_Linear_Search_OpTable (void)
{
}

int
TAO_Linear_Search_OpTable::bind (const char *,
                                 const TAO::Operation_Skeletons )
{
  return 0;
}

int
TAO_Linear_Search_OpTable::find (const char *opname,
                                 TAO_Skeleton& skelfunc,
                                 const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_LINEAR_SEARCH_OPTABLE_FIND_START);

  const TAO_operation_db_entry *entry = lookup (opname);
  if (entry == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO_Linear_Search_Table:find failed\n")),
                      -1);

  // Valid entry. Figure out the skel_ptr.
  skelfunc = entry->skel_ptr_;

  return 0;
}


int
TAO_Linear_Search_OpTable::find (const char *opname,
                                 TAO_Collocated_Skeleton &skelfunc,
                                 TAO::Collocation_Strategy st,
                                 const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_LINEAR_SEARCH_OPTABLE_FIND_START);

  const TAO_operation_db_entry *entry = lookup (opname);
  if (entry == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO_Linear_Search_Table:find failed\n")),
                      -1);

  switch (st)
    {
    case TAO::TAO_CS_THRU_POA_STRATEGY:
      skelfunc = entry->thruPOA_skel_ptr_;
      break;
    case TAO::TAO_CS_DIRECT_STRATEGY:
      skelfunc = entry->direct_skel_ptr_;
      break;
    default:
      return -1;
    }

  return 0;
}

/*********************************************************************/
TAO_Active_Demux_OpTable::TAO_Active_Demux_OpTable (const
                                                    TAO_operation_db_entry *db,
                                                    CORBA::ULong dbsize)
  : next_ (0),
    tablesize_ (dbsize),
    tbl_ (0)
{
  ACE_NEW (tbl_,
           TAO_Active_Demux_OpTable_Entry[dbsize]);

  // The job of the constructor is to go thru each entry of the
  // database and bind the operation name to its corresponding
  // skeleton.
  for (CORBA::ULong i=0; i < dbsize; i++)
    {
      TAO::Operation_Skeletons s;
      s.skel_ptr_ = db[i].skel_ptr_;
      s.thruPOA_skel_ptr_ = db[i].thruPOA_skel_ptr_;
      s.direct_skel_ptr_ = db[i].direct_skel_ptr_;

      // @@ (ASG): what happens if bind fails ???
      (void) this->bind (db[i].opname_, s);
    }
}

TAO_Active_Demux_OpTable::~TAO_Active_Demux_OpTable (void)
{
  delete [] this->tbl_;
}

int
TAO_Active_Demux_OpTable::bind (const char *opname,
                                const TAO::Operation_Skeletons skel_ptr)
{
  CORBA::ULong i = ACE_OS::atoi (opname);

  if (i < this->tablesize_)
    {
      if (this->tbl_[i].op_skel_ptr_.skel_ptr_ != 0)
        // overwriting previous one
        return 1;
      else
        {
          this->tbl_[i].op_skel_ptr_ = skel_ptr;
          return 0;
        }
    }
  return -1; // error
}

int
TAO_Active_Demux_OpTable::find (const char *opname,
                                TAO_Skeleton& skel_ptr,
                                const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_ACTIVE_DEMUX_OPTABLE_FIND_START);

  CORBA::ULong i = ACE_OS::atoi (opname);

  ACE_ASSERT (i < this->tablesize_);
  skel_ptr = this->tbl_[i].op_skel_ptr_.skel_ptr_;
  return 0; //success
}


int
TAO_Active_Demux_OpTable::find (const char *opname,
                                TAO_Collocated_Skeleton& skel_ptr,
                                TAO::Collocation_Strategy st,
                                const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_ACTIVE_DEMUX_OPTABLE_FIND_START);

  CORBA::ULong i = ACE_OS::atoi (opname);

  ACE_ASSERT (i < this->tablesize_);

  switch (st)
    {
    case TAO::TAO_CS_THRU_POA_STRATEGY:
      skel_ptr = this->tbl_[i].op_skel_ptr_.thruPOA_skel_ptr_;
      break;
    case TAO::TAO_CS_DIRECT_STRATEGY:
      skel_ptr = this->tbl_[i].op_skel_ptr_.direct_skel_ptr_;
      break;
    default:
      return -1;
   }

  return 0; //success
}
TAO_Active_Demux_OpTable_Entry::TAO_Active_Demux_OpTable_Entry (void)
{
}

TAO_Active_Demux_OpTable_Entry::~TAO_Active_Demux_OpTable_Entry (void)
{
}

/**************************************************/
// Do nothing constructor.
TAO_Perfect_Hash_OpTable::TAO_Perfect_Hash_OpTable (void)
{
}

// Do nothing destrctor.
TAO_Perfect_Hash_OpTable::~TAO_Perfect_Hash_OpTable (void)
{
}

int
TAO_Perfect_Hash_OpTable::find (const char *opname,
                                TAO_Skeleton &skelfunc,
                                const unsigned int length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_PERFECT_HASH_OPTABLE_FIND_START);

  const TAO_operation_db_entry *entry = lookup (opname,
                                                length);
  if (entry == 0)
    {
      skelfunc = 0; // insure that somebody can't call a wrong function!
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_Perfect_Hash_OpTable:find for ")
                         ACE_TEXT ("operation '%s' (length=%d) failed\n"),
                         opname ? opname : "<null string>", length),
                        -1);
    }

  // Valid entry. Figure out the skel_ptr.
  skelfunc = entry->skel_ptr_;

  return 0;
}

int
TAO_Perfect_Hash_OpTable::find (const char *opname,
                                TAO_Collocated_Skeleton &skelfunc,
                                TAO::Collocation_Strategy st,
                                const unsigned int length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_PERFECT_HASH_OPTABLE_FIND_START);

  const TAO_operation_db_entry *entry = lookup (opname,
                                                length);
  if (entry == 0)
    {
      skelfunc = 0; // insure that somebody can't call a wrong function!
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_Perfect_Hash_OpTable:find for ")
                         ACE_TEXT ("operation '%s' (length=%d) failed\n"),
                         opname ? opname : "<null string>", length),
                        -1);
    }

  switch (st)
    {
    case TAO::TAO_CS_THRU_POA_STRATEGY:
      skelfunc = entry->thruPOA_skel_ptr_;
      break;
    case TAO::TAO_CS_DIRECT_STRATEGY:
      skelfunc = entry->direct_skel_ptr_;
      break;
    default:
      return -1;
    }

  return 0;
}

int
TAO_Perfect_Hash_OpTable::bind (const char *,
                                const TAO::Operation_Skeletons)
{
  return 0;
}

/*****************************************************************/
TAO_Binary_Search_OpTable::TAO_Binary_Search_OpTable (void)
{
}

TAO_Binary_Search_OpTable::~TAO_Binary_Search_OpTable (void)
{
}

int
TAO_Binary_Search_OpTable::find (const char *opname,
                                 TAO_Skeleton &skelfunc,
                                 const unsigned int /* length */)
{
  ACE_FUNCTION_TIMEPROBE (TAO_BINARY_SEARCH_OPTABLE_FIND_START);

  const TAO_operation_db_entry *entry = lookup (opname);

  if (entry == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO_Binary_Search_Table:find failed\n")),
                      -1);
  // Valid entry. Figure out the skel_ptr.
  skelfunc = entry->skel_ptr_;

  return 0;
}


int
TAO_Binary_Search_OpTable::find (const char *opname,
                                 TAO_Collocated_Skeleton &skelfunc,
                                 TAO::Collocation_Strategy st,
                                 const unsigned int /* length */)
{
  ACE_FUNCTION_TIMEPROBE (TAO_BINARY_SEARCH_OPTABLE_FIND_START);

  const TAO_operation_db_entry *entry = lookup (opname);

  if (entry == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO_Binary_Search_Table:find failed\n")),
                      -1);

  switch (st)
    {
    case TAO::TAO_CS_THRU_POA_STRATEGY:
      skelfunc = entry->thruPOA_skel_ptr_;
      break;
    case TAO::TAO_CS_DIRECT_STRATEGY:
      skelfunc = entry->direct_skel_ptr_;
      break;
    default:
      return -1;
    }

  return 0;
}

int
TAO_Binary_Search_OpTable::bind (const char *,
                                const TAO::Operation_Skeletons )
{
  return 0;
}

/******************************************************************/
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
TAO_Operation_Table*
TAO_Operation_Table_Parameters::concrete_strategy (void)
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
  TAO_Operation_Table_Parameters *p =
    TAO_OP_TABLE_PARAMETERS::instance ();

  return p->concrete_strategy ();
}

/**************************************************************/
TAO::Operation_Skeletons::Operation_Skeletons (void)
  : skel_ptr_ (0)
    , thruPOA_skel_ptr_ (0)
    , direct_skel_ptr_ (0)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Iterator_Base_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<const char *, TAO_Skeleton>;
template class TAO_Singleton<TAO_Operation_Table_Parameters, TAO_SYNCH_RECURSIVE_MUTEX>;
#elif defined (ACE_HAS_GNU_REPO)
// This is necessary with g++ 2.91.66 to avoid a couple of strange
// unresolved ACE_Hash_Map_Entry symbols.  (Strange because c++filt
// can't demangle them.)
template class ACE_Hash_Map_Entry<char const *, void (*)(CORBA_ServerRequest &, void *, void * ACE_ENV_ARG_DECL_NOT_USED)>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<const char *, TAO_Skeleton, ACE_Hash<const char *>, ACE_Equal_To<const char *>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<const char *, TAO_Skeleton>
#pragma instantiate TAO_Singleton<TAO_Operation_Table_Parameters, TAO_SYNCH_RECURSIVE_MUTEX>

#elif defined (__GNUC__) && defined (__hpux)
template class TAO_Singleton<TAO_Operation_Table_Parameters,TAO_SYNCH_RECURSIVE_MUTEX>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
