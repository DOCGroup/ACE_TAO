#include "params.h"
#include "connect.h"
#include "objtable.h"

#if !defined(__ACE_INLINE__)
#  include "params.i"
#endif

#if defined(ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Singleton<TAO_OA_Parameters, ACE_SYNCH_MUTEX>;
#endif
