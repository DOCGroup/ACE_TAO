#include "Null_Fragmentation_Strategy.h"

TAO_Null_Fragmentation_Strategy::~TAO_Null_Fragmentation_Strategy ()
{
}

int
TAO_Null_Fragmentation_Strategy::fragment (TAO_OutputCDR &,
                                           ACE_CDR::ULong,
                                           ACE_CDR::ULong)
{
  return 0;
}
