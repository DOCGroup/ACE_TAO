#include "ace/String_Base_Const.h"
#include <limits>

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_String_Base_Const::size_type const ACE_String_Base_Const::npos =
  std::numeric_limits<ACE_String_Base_Const::size_type>::max ();

ACE_END_VERSIONED_NAMESPACE_DECL
