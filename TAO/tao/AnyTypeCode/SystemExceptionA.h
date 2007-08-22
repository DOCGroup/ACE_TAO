// -*- C++ -*-

//=============================================================================
/**
 *  @file    SystemExceptionA.h
 *
 *  $Id$
 *
 *  SystemException Any operator and typecode decls.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SYSTEMEXCEPTIONA_H
#define TAO_SYSTEMEXCEPTIONA_H

#include /**/ "ace/pre.h"
#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"
#include "tao/AnyTypeCode/AnyTypeCode_methods.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace CORBA
{
  class TypeCode;
  typedef TypeCode *TypeCode_ptr;

  class Any;
  typedef Any *Any_ptr;

#define TAO_SYSTEM_EXCEPTION(name) \
  TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, \
                                           const CORBA::name &); \
  TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, \
                                           CORBA::name *); \
  TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &, \
                                                     const CORBA::name *&); \
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_ ## name

  TAO_SYSTEM_EXCEPTION(UNKNOWN);          // the unknown exception
  TAO_SYSTEM_EXCEPTION(BAD_PARAM);        // an invalid parameter was passed
  TAO_SYSTEM_EXCEPTION(NO_MEMORY);        // memory allocation failure
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT);        // violated implementation limit
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE);     // communication failure
  TAO_SYSTEM_EXCEPTION(INV_OBJREF);       // invalid object reference
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST); // no such object
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION);    // no permission for operation
  TAO_SYSTEM_EXCEPTION(INTERNAL);         // ORB internal error
  TAO_SYSTEM_EXCEPTION(MARSHAL);          // error marshaling param/result
  TAO_SYSTEM_EXCEPTION(INITIALIZE);       // ORB initialization failure
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT);     // implementation unavailable
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE);     // bad typecode
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION);    // invalid operation
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES);     // out of resources for request
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE);      // response not yet available
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE);    // persistent storage failure
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER);    // routine invocations out of order
  TAO_SYSTEM_EXCEPTION(TRANSIENT);        // transient error, try again later
  TAO_SYSTEM_EXCEPTION(FREE_MEM);         // cannot free memory
  TAO_SYSTEM_EXCEPTION(INV_IDENT);        // invalid identifier syntax
  TAO_SYSTEM_EXCEPTION(INV_FLAG);         // invalid flag was specified
  TAO_SYSTEM_EXCEPTION(INTF_REPOS);       // interface repository unavailable
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT);      // error processing context object
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER);      // object adapter failure
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION);  // data conversion error
  TAO_SYSTEM_EXCEPTION(INV_POLICY);       // invalid policies present
  TAO_SYSTEM_EXCEPTION(REBIND);           // rebind needed
  TAO_SYSTEM_EXCEPTION(TIMEOUT);          // operation timed out
  TAO_SYSTEM_EXCEPTION(TRANSACTION_UNAVAILABLE); // no transaction
  TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE);        // invalid transaction mode
  TAO_SYSTEM_EXCEPTION(TRANSACTION_REQUIRED);    // operation needs transaction
  TAO_SYSTEM_EXCEPTION(TRANSACTION_ROLLEDBACK);  // operation was a no-op
  TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION);     // invalid TP context passed
  TAO_SYSTEM_EXCEPTION(CODESET_INCOMPATIBLE);    // incompatible code set
  TAO_SYSTEM_EXCEPTION(BAD_QOS);          // bad quality of service
  TAO_SYSTEM_EXCEPTION(INVALID_ACTIVITY);
  TAO_SYSTEM_EXCEPTION(ACTIVITY_COMPLETED);
  TAO_SYSTEM_EXCEPTION(ACTIVITY_REQUIRED);
  TAO_SYSTEM_EXCEPTION(THREAD_CANCELLED);

#undef TAO_SYSTEM_EXCEPTION

} // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SYSTEMEXCEPTIONA_H */


