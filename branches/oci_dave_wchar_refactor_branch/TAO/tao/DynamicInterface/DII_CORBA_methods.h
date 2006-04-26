// -*- C++ -*-

//=============================================================================
/**
 *  @file    DII_CORBA_methods.h
 *
 *  $Id$
 *
 *  Declarations of DII methods in the CORBA namespace.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_DII_CORBA_METHODS_H
#define TAO_DII_CORBA_METHODS_H

#include /**/ "ace/pre.h"

#include "tao/DynamicInterface/dynamicinterface_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_methods.h"

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif

#define TAO_EXPORT_MACRO TAO_DynamicInterface_Export

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class ContextList;
  typedef ContextList *ContextList_ptr;

  class Context;
  typedef Context *Context_ptr;

  class ExceptionList;
  typedef ExceptionList *ExceptionList_ptr;

  class Request;
  typedef Request *Request_ptr;

  TAO_NAMESPACE_INLINE_FUNCTION void release (Context_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Context_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION void release (ContextList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ContextList_ptr );

  TAO_NAMESPACE_INLINE_FUNCTION void release (ExceptionList_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (ExceptionList_ptr);

  TAO_NAMESPACE_INLINE_FUNCTION void release (Request_ptr);
  TAO_NAMESPACE_INLINE_FUNCTION Boolean is_nil (Request_ptr);

}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_DII_CORBA_METHODS_H */
