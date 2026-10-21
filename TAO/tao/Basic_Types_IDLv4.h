#ifndef TAO_BASIC_TYPES_IDLV4_H
#define TAO_BASIC_TYPES_IDLV4_H

#include /**/ "ace/pre.h"
#include "ace/CDR_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  namespace IDLv4
  {
    /**
     * @name IDL4 Explicit Integer Types
     *
     * These are for the corresponding types defined in IDL4, but they are not
     * standard in CORBA.
     */
    //@{
    using Int8 = ACE_CDR::Int8;
    using Int8_out = Int8 &;
    struct Int8_tag {};

    using UInt8 = ACE_CDR::UInt8;
    using UInt8_out = UInt8 &;
    struct UInt8_tag {};
    //@}
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_TYPES_IDLV4_H */
