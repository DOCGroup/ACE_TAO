// $Id$

#ifndef ACE_ADAPTER_SYSTEM_TIME_H
#define ACE_ADAPTER_SYSTEM_TIME_H
#include "ace/pre.h"

#include "../../System_Time.h"

class ACE_System_Time_W : public ACE_System_Time
{
public:
  ACE_System_Time_W (const wchar_t *poolname = 0)
    : ACE_System_Time (ACE_TEXT_WCHAR_TO_CHAR (poolname))
  {}

  ~ACE_System_Time_W (void)
  {}
};

#include "ace/post.h"
#endif /* ACE_ADAPTER_SYSTEM_TIME_H */
