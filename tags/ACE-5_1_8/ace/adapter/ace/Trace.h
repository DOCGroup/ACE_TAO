// $Id$

#ifndef ACE_ADAPTER_TRACE_H
#define ACE_ADAPTER_TRACE_H
#include "ace/pre.h"

#include "../../Trace.h"

class ACE_Trace_W : public ACE_Trace
{
public:
  ACE_Trace_W (const wchar_t *n,
               int line = 0,
               const wchar_t *file = L"")
    : ACE_Trace (ACE_TEXT_WCHAR_TO_CHAR (n), line, ACE_TEXT_WCHAR_TO_CHAR (file))
  {}
                             

  ~ACE_Trace_W (void)
  {}
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_TRACE_H */
