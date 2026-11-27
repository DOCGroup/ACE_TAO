#ifndef RT_SCHEDULING_GUID_UTILS_H
#define RT_SCHEDULING_GUID_UTILS_H

#include "tao/RTScheduling/RTScheduler.h"
#include "tao/SystemException.h"

#include <cstring>

template <typename Sequence>
inline size_t
rtscheduling_guid_value (Sequence const &guid)
{
  if (guid.length () != sizeof (size_t))
    {
      throw CORBA::BAD_PARAM ();
    }

  size_t value {};
  std::memcpy (&value, guid.get_buffer (), sizeof (value));
  return value;
}

#endif
