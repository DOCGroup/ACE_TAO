// $Id$

#include "tao/PortableServer/Operation_Table_Dynamic_Hash.h"
#include "tao/Timeprobe.h"
#include "ace/Log_Msg.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Operation_Table_Timeprobe_Description[] =
  {
    "TAO_Dynamic_Hash_OpTable::find - start",
    "TAO_Dynamic_Hash_OpTable::find - end",
  };

enum
  {
    // Timeprobe description table start key
    TAO_DYNAMIC_HASH_OPTABLE_FIND_START = 600,
    TAO_DYNAMIC_HASH_OPTABLE_FIND_END,
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Operation_Table_Timeprobe_Description,
                                  TAO_DYNAMIC_HASH_OPTABLE_FIND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// constructor
TAO_Dynamic_Hash_OpTable::TAO_Dynamic_Hash_OpTable (
  TAO_operation_db_entry const * db,
  CORBA::ULong dbsize,
  CORBA::ULong hashtblsize,
  ACE_Allocator *alloc)
  : hash_ (hashtblsize, alloc)
{
  // Iterate thru each entry in the database and bind the operation
  // name to its corresponding skeleton.
  for (CORBA::ULong i = 0; i < dbsize; ++i)
    {
      TAO::Operation_Skeletons s;
      s.skel_ptr = db[i].skel_ptr;
      s.thruPOA_skel_ptr = db[i].skel_ptr;
      s.direct_skel_ptr = db[i].direct_skel_ptr;

      // @@ (ASG): what happens if bind fails ???
      if (this->bind (db[i].opname, s) == -1)
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
      skel_ptr = s.skel_ptr;
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
        case TAO::TAO_CS_DIRECT_STRATEGY:
          skel_ptr = skel.direct_skel_ptr;
          break;
        default:
          return -1;
        }
    }

  return retval;
}

TAO_END_VERSIONED_NAMESPACE_DECL
