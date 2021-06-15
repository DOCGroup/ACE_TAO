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

    typedef ACE_CDR::UInt8 UInt8;
    typedef UInt8 &UInt8_out;

    typedef ACE_CDR::Int16 Int16;
    typedef Int16 &Int16_out;

    typedef ACE_CDR::UInt16 UInt16;
    typedef UInt16 &UInt16_out;

    typedef ACE_CDR::Int32 Int32;
    typedef Int32 &Int32_out;

    typedef ACE_CDR::UInt32 UInt32;
    typedef UInt32 &UInt32_out;

    typedef ACE_CDR::Int64 Int64;
    typedef Int64 &Int64_out;

    typedef ACE_CDR::UInt64 UInt64;
    typedef UInt64 &UInt64_out;
    //@}
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_BASIC_TYPES_IDLV4_H */
