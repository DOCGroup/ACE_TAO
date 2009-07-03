// $Id$

#include "fe_event_header.h"

ACE_RCSID (fe,
           fe_event_header,
           "$Id$")

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

