#include "ace/INet/IOS_util.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace IOS
  {
    std::ostream Null::out_stream_ (nullptr);
    std::istream Null::in_stream_ (nullptr);
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL
