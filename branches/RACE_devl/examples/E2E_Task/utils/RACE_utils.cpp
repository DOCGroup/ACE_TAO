// $Id$

#include "RACE_utils.h"
#include "ace/Env_Value_T.h"

/// This should really be an anonymous namespace, but some compilers
/// still don't support this features.  Therefore, just use a long
/// namespace name here.
namespace race_anonymous_namespace
{
	int debug_level = -1;
}

int
CIAO::RACE::debug_level (void)
{
	if (race_anonymous_namespace::debug_level == -1)
	{
		// Initialize the thing.
		ACE_Env_Value<int> envar ("RACE_DEBUG_LEVEL", 1);
		race_anonymous_namespace::debug_level = envar;
	}

	return race_anonymous_namespace::debug_level;
}
