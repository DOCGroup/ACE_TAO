/**
 * @file idl_features.h
 *
 * Get information about what IDL features the IDL compiler has, similarly to
 * the "version" standard header in C++.
 *
 * In C++ it can be used like this if you want to be compatible with older
 * versions of TAO:
 *
 * #include "tao/orbconf.h"
 * #if defined TAO_HAS_IDL_FEATURES && TAO_HAS_IDL_FEATURES
 * // Use this macro if you are using something that's not the max possible
 * // version.
 * #  define TAO_IDL_IDL_VERSION 0x30000
 * #  include "tao/idl_features.h"
 * #  if defined TAO_IDL_HAS_FEATURE && TAO_IDL_HAS_FEATURE
 * #    define USE_FEATURE
 * #  endif
 * #endif
 *
 * In IDL it can be used like this if you want to be compatible with older
 * versions of TAO:
 *
 * #if defined __TAO_IDL_FEATURES
 * #  include __TAO_IDL_FEATURES
 * #  if defined TAO_IDL_HAS_FEATURE && TAO_IDL_HAS_FEATURE
 * #    define USE_FEATURE
 * #  endif
 * #endif
 *
 * Note that support for annotations and anonymous types in IDL4 predate this
 * file, so they are a potential special case depending on what ACE/TAO is
 * being used.
 */

#ifndef TAO_IDL_FEATURES_H
#define TAO_IDL_FEATURES_H

#ifndef TAO_IDL_IDL_VERSION
#  ifdef __TAO_IDL_IDL_VERSION
#    define TAO_IDL_IDL_VERSION __TAO_IDL_IDL_VERSION
#  else
#    define TAO_IDL_IDL_VERSION 0xffffffff
#  endif
#endif

#ifndef TAO_IDL_HAS_ANNOTATIONS
#  define TAO_IDL_HAS_ANNOTATIONS TAO_IDL_IDL_VERSION >= 0x40000
#endif

#ifndef TAO_IDL_HAS_ANONYMOUS_TYPES
#  define TAO_IDL_HAS_ANONYMOUS_TYPES TAO_IDL_IDL_VERSION >= 0x40000
#endif

#ifndef TAO_IDL_HAS_EXPLICIT_INTS
#  define TAO_IDL_HAS_EXPLICIT_INTS TAO_IDL_IDL_VERSION >= 0x40000
#endif

#ifndef TAO_IDL_HAS_OCTET_AND_WCHAR_UNION_DISCS
#  define TAO_IDL_HAS_OCTET_AND_WCHAR_UNION_DISCS 0
#endif

#ifndef TAO_IDL_HAS_STRUCT_INHERITANCE
#  define TAO_IDL_HAS_STRUCT_INHERITANCE 0
#endif

#ifndef TAO_IDL_HAS_MAP
#  define TAO_IDL_HAS_MAP 0
#endif

#ifndef TAO_IDL_HAS_BITSET
#  define TAO_IDL_HAS_BITSET 0
#endif

#ifndef TAO_IDL_HAS_BITMASK
#  define TAO_IDL_HAS_BITMASK 0
#endif

#endif
