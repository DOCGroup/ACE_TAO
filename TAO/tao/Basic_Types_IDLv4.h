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
    typedef ACE_CDR::Int8 Int8;
    typedef Int8 &Int8_out;
    struct Int8_tag {};

    typedef ACE_CDR::UInt8 UInt8;
    typedef UInt8 &UInt8_out;
    struct UInt8_tag {};
    //@}
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_TYPES_IDLV4_H */
