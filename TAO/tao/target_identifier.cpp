//$Id$
#include "tao/target_identifier.h"

#if !defined (__ACE_INLINE__)
#include "target_identfier.i"
#endif /* !defined INLINE */

TAO_Target_Specification::TAO_Target_Specification (void)
  :specifier_ (TAO_Target_Specification::Key_Addr),
   profile_index_ (0)
{
  //no-op
}
