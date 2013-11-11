// -*- C++ -*-

//=============================================================================
/**
 *  @file Async_IOR_Table_Impl.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_ASYNC_IOR_TABLE_IMPL_H
#define TAO_ASYNC_IOR_TABLE_IMPL_H

#include /**/ "ace/pre.h"

#include "tao/IORTable/Async_IORTable.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "tao/LocalObject.h"
#include "ace/SString.h"
#include "tao/Objref_VarOut_T.h"
#include "tao/IORTable/IOR_Table_Impl.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Async_IOR_Table_Impl : public virtual TAO_IOR_Table_Impl
{
public:
  /// Constructor
  TAO_Async_IOR_Table_Impl (void);
  virtual ~TAO_Async_IOR_Table_Impl (void);

  virtual bool async_available (void) { return true; }

  virtual void async_find (::IORTable::Locate_ResponseHandler rh,
                           const char *object_key);

  virtual void set_locator (IORTable::Locator_ptr locator);

private:
  /// The locator
  IORTable::AsyncLocator_var async_locator_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"

#endif /* TAO_ASYNC_IOR_TABLE_IMPL */
