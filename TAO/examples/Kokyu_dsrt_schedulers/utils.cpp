#include "utils.h"
#include "Kokyu/Kokyu.h"
#include "ace/Lock_Adapter_T.h"
#include <cstring>

void
guid_copy (Kokyu::GuidType &lhs, RTScheduling::Current::IdType const &rhs)
{
  lhs.length (rhs.length ());
  std::memcpy (lhs.get_buffer (),
               rhs.get_buffer (),
               rhs.length ());
}

void
guid_copy (RTScheduling::Current::IdType &lhs, Kokyu::GuidType const &rhs)
{
  lhs.length (rhs.length ());
  std::memcpy (lhs.get_buffer (),
               rhs.get_buffer (),
               rhs.length ());
}

