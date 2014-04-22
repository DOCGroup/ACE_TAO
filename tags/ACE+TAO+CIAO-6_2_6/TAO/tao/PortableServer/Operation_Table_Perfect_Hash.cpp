// $Id$

#include "tao/PortableServer/Operation_Table_Perfect_Hash.h"
#include "tao/Timeprobe.h"
#include "ace/Log_Msg.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Operation_Table_Timeprobe_Description[] =
  {
    "TAO_Perfect_Hash_OpTable::find - start",
    "TAO_Perfect_Hash_OpTable::find - end",
  };

enum
  {
    // Timeprobe description table start key
    TAO_PERFECT_HASH_OPTABLE_FIND_START = 606,
    TAO_PERFECT_HASH_OPTABLE_FIND_END,
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Operation_Table_Timeprobe_Description,
                                  TAO_PERFECT_HASH_OPTABLE_FIND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  TAO_operation_db_entry const * const entry = lookup (opname,
                                                       length);
  if (entry == 0)
    {
      skelfunc = 0; // insure that somebody can't call a wrong function!
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_Perfect_Hash_OpTable:find for ")
                         ACE_TEXT ("operation '%C' (length=%d) failed\n"),
                         opname ? opname : "<null string>", length),
                        -1);
    }

  // Valid entry. Figure out the skel_ptr.
  skelfunc = entry->skel_ptr;

  return 0;
}

int
TAO_Perfect_Hash_OpTable::find (const char *opname,
                                TAO_Collocated_Skeleton &skelfunc,
                                TAO::Collocation_Strategy st,
                                const unsigned int length)
{
  ACE_FUNCTION_TIMEPROBE (TAO_PERFECT_HASH_OPTABLE_FIND_START);

  TAO_operation_db_entry const * const entry = lookup (opname, length);
  if (entry == 0)
    {
      skelfunc = 0; // insure that somebody can't call a wrong function!
      TAOLIB_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("TAO_Perfect_Hash_OpTable:find for ")
                         ACE_TEXT ("operation '%C' (length=%d) failed\n"),
                         opname ? opname : "<null string>", length),
                        -1);
    }

  switch (st)
    {
    case TAO::TAO_CS_DIRECT_STRATEGY:
      skelfunc = entry->direct_skel_ptr;
      break;
    default:
      return -1;
    }

  return 0;
}

int
TAO_Perfect_Hash_OpTable::bind (const char *, const TAO::Operation_Skeletons)
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
