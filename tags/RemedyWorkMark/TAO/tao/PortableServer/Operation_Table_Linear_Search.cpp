// $Id$

#include "tao/PortableServer/Operation_Table_Linear_Search.h"
#include "tao/Timeprobe.h"
#include "ace/Log_Msg.h"

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Operation_Table_Timeprobe_Description[] =
  {
    "TAO_Linear_Search_OpTable::find - start",
    "TAO_Linear_Search_OpTable::find - end",
  };

enum
  {
    // Timeprobe description table start key
    TAO_LINEAR_SEARCH_OPTABLE_FIND_START = 602,
    TAO_LINEAR_SEARCH_OPTABLE_FIND_END,
  };

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Operation_Table_Timeprobe_Description,
                                  TAO_LINEAR_SEARCH_OPTABLE_FIND_START);

#endif /* ACE_ENABLE_TIMEPROBES */

/***************************************************************/

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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

  TAO_operation_db_entry const * const entry = lookup (opname);
  if (entry == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO_Linear_Search_Table:find failed\n")),
                      -1);

  // Valid entry. Figure out the skel_ptr.
  skelfunc = entry->skel_ptr;

  return 0;
}


int
TAO_Linear_Search_OpTable::find (const char *opname,
                                 TAO_Collocated_Skeleton &skelfunc,
                                 TAO::Collocation_Strategy st,
                                 const unsigned int )
{
  ACE_FUNCTION_TIMEPROBE (TAO_LINEAR_SEARCH_OPTABLE_FIND_START);

  TAO_operation_db_entry const * const entry = lookup (opname);
  if (entry == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("TAO_Linear_Search_Table:find failed\n")),
                      -1);

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

TAO_END_VERSIONED_NAMESPACE_DECL
