// $Id$

#include "fe_event_header.h"

FE_EventHeader::FE_EventHeader (UTL_ScopedName *n,
                                UTL_NameList *inherits,
                                UTL_NameList *supports,
                                bool truncatable)
  : FE_OBVHeader (n,
                  inherits,
                  supports,
                  truncatable,
                  true)
{
}

FE_EventHeader::~FE_EventHeader (void)
{
}

