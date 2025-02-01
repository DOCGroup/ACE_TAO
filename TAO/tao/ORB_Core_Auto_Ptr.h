// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORB_Core_Auto_Ptr.h
 *
 *  @author DOC Center - Washington University at St. Louis
 *  @author DOC Laboratory - University of California at Irvine
 */
// ===================================================================

#ifndef TAO_ORB_CORE_AUTO_PTR_H
#define TAO_ORB_CORE_AUTO_PTR_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"
#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;

/**
 * Custom deleter to decrement the refcount when called
 */
struct TAO_Export TAO_ORB_Core_Decr_Refcnt
{
  void operator()(TAO_ORB_Core* core) const;
};

/**
 * TAO_ORB_Core_Auto_Ptr will decrement the refcount when going our of scope
 * using std::unique_ptr and a custom deleter
 */
using TAO_ORB_Core_Auto_Ptr = std::unique_ptr<TAO_ORB_Core, TAO_ORB_Core_Decr_Refcnt>;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_ORB_CORE_AUTO_PTR_H */
