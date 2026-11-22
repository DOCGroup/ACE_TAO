// -*- C++ -*-

/**
 *  @file    SystemExceptionA.h
 *
 *  SystemException Any operator and typecode decls.
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */

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
  using TypeCode_ptr = TypeCode *;

  class Any;
  using Any_ptr = Any *;

#define TAO_SYSTEM_EXCEPTION(name) \
  TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, \
                                           const CORBA::name &); \
  TAO_AnyTypeCode_Export void operator<<= (CORBA::Any &, \
                                           CORBA::name *); \
  TAO_AnyTypeCode_Export CORBA::Boolean operator>>= (const CORBA::Any &, \
                                                     const CORBA::name *&); \
  extern TAO_AnyTypeCode_Export TypeCode_ptr const _tc_ ## name

  /// the unknown exception
  TAO_SYSTEM_EXCEPTION(UNKNOWN);
  /// an invalid parameter was passed
  TAO_SYSTEM_EXCEPTION(BAD_PARAM);
  /// memory allocation failure
  TAO_SYSTEM_EXCEPTION(NO_MEMORY);
  /// violated implementation limit
  TAO_SYSTEM_EXCEPTION(IMP_LIMIT);
  /// communication failure
  TAO_SYSTEM_EXCEPTION(COMM_FAILURE);
  /// invalid object reference
  TAO_SYSTEM_EXCEPTION(INV_OBJREF);
  /// no such object
  TAO_SYSTEM_EXCEPTION(OBJECT_NOT_EXIST);
  /// no permission for operation
  TAO_SYSTEM_EXCEPTION(NO_PERMISSION);
  /// ORB internal error
  TAO_SYSTEM_EXCEPTION(INTERNAL);
  /// error marshaling param/result
  TAO_SYSTEM_EXCEPTION(MARSHAL);
  /// ORB initialization failure
  TAO_SYSTEM_EXCEPTION(INITIALIZE);
  /// implementation unavailable
  TAO_SYSTEM_EXCEPTION(NO_IMPLEMENT);
  /// bad typecode
  TAO_SYSTEM_EXCEPTION(BAD_TYPECODE);
  /// invalid operation
  TAO_SYSTEM_EXCEPTION(BAD_OPERATION);
  /// out of resources for request
  TAO_SYSTEM_EXCEPTION(NO_RESOURCES);
  /// response not yet available
  TAO_SYSTEM_EXCEPTION(NO_RESPONSE);
  /// persistent storage failure
  TAO_SYSTEM_EXCEPTION(PERSIST_STORE);
  /// routine invocations out of order
  TAO_SYSTEM_EXCEPTION(BAD_INV_ORDER);
  /// transient error, try again later
  TAO_SYSTEM_EXCEPTION(TRANSIENT);
  /// cannot free memory
  TAO_SYSTEM_EXCEPTION(FREE_MEM);
  /// invalid identifier syntax
  TAO_SYSTEM_EXCEPTION(INV_IDENT);
  /// invalid flag was specified
  TAO_SYSTEM_EXCEPTION(INV_FLAG);
  /// interface repository unavailable
  TAO_SYSTEM_EXCEPTION(INTF_REPOS);
  /// error processing context object
  TAO_SYSTEM_EXCEPTION(BAD_CONTEXT);
  /// object adapter failure
  TAO_SYSTEM_EXCEPTION(OBJ_ADAPTER);
  /// data conversion error
  TAO_SYSTEM_EXCEPTION(DATA_CONVERSION);
  /// invalid policies present
  TAO_SYSTEM_EXCEPTION(INV_POLICY);
  /// rebind needed
  TAO_SYSTEM_EXCEPTION(REBIND);
  /// operation timed out
  TAO_SYSTEM_EXCEPTION(TIMEOUT);
  /// no transaction
  TAO_SYSTEM_EXCEPTION(TRANSACTION_UNAVAILABLE);
  /// invalid transaction mode
  TAO_SYSTEM_EXCEPTION(TRANSACTION_MODE);
  /// operation needs transaction
  TAO_SYSTEM_EXCEPTION(TRANSACTION_REQUIRED);
  /// operation was a no-op
  TAO_SYSTEM_EXCEPTION(TRANSACTION_ROLLEDBACK);
  /// invalid TP context passed
  TAO_SYSTEM_EXCEPTION(INVALID_TRANSACTION);
  /// incompatible code set
  TAO_SYSTEM_EXCEPTION(CODESET_INCOMPATIBLE);
  /// bad quality of service
  TAO_SYSTEM_EXCEPTION(BAD_QOS);
  TAO_SYSTEM_EXCEPTION(INVALID_ACTIVITY);
  TAO_SYSTEM_EXCEPTION(ACTIVITY_COMPLETED);
  TAO_SYSTEM_EXCEPTION(ACTIVITY_REQUIRED);
  TAO_SYSTEM_EXCEPTION(THREAD_CANCELLED);

#undef TAO_SYSTEM_EXCEPTION

} // End CORBA namespace

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_SYSTEMEXCEPTIONA_H */


