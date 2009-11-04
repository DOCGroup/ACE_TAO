// $Id$

#ifndef FE_EVENT_HEADER_H
#define FE_EVENT_HEADER_H

#include "fe_obv_header.h"

class AST_EventType;

// FE_EventHeader
// Internal class for FE to describe eventtype headers.
//

class FE_EventHeader : public FE_OBVHeader
{
public:

  FE_EventHeader (UTL_ScopedName *n,
                  UTL_NameList *inherits,
                  UTL_NameList *supports,
                  bool truncatable);
  virtual ~FE_EventHeader (void);
};

#endif /* FE_EVENT_HEADER_H */
