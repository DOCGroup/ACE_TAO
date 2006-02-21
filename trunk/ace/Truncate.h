// -*- C++ -*-
//=============================================================================
/**
 * @file Truncate.h
 *
 * $Id$
 *
 * @author Steve Huston <shuston@riverace.com>
 */
//=============================================================================
#ifndef ACE_TRUNCATE_H
#define ACE_TRUNCATE_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_Utils
{
/**
 * @class Truncate
 *
 * @brief Helper function to truncate an integral value to an int.
 *        Very useful since ACE methods return int very often and the value's
 *        source is often a different-size integral type, such as size_t.
 *        This function hides the truncation logic and resolves compiler
 *        diagnostics.
 */
template<typename X>
ACE_NAMESPACE_INLINE_FUNCTION int Truncate (const X& val);

// Specialize one for size_t to alleviate the explicit instantiation pain.
template<>
ACE_NAMESPACE_INLINE_FUNCTION int Truncate<size_t> (const size_t& val);

} // namespace ACE_Utils

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined(__ACE_INLINE__)
# include "ace/Truncate.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* ACE_TRUNCATE_H*/
