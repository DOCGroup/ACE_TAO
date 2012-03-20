// -*- C++ -*-

// ===================================================================
/**
 *  @file   Service_Context_Handler_Registry.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_SERVICE_CONTEXT_HANDLER_REGISTRY_H
#define TAO_SERVICE_CONTEXT_HANDLER_REGISTRY_H

#include /**/ "ace/pre.h"

#include "tao/IOPC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Array_Map.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport;
class TAO_Service_Context_Handler;
class TAO_Operation_Details;
class TAO_Target_Specification;
class TAO_Stub;
class TAO_ServerRequest;

class TAO_Export TAO_Service_Context_Registry
{
  public:
    /// Destructor

    ~TAO_Service_Context_Registry (void);

    /**
     * Bind a new entry in the registry
     */
    int bind (IOP::ServiceId id, TAO_Service_Context_Handler* handler);

    /**
     * Retrieve the entry related to @a id
     */
    TAO_Service_Context_Handler* operator[] (IOP::ServiceId id);

    int process_service_contexts (
      IOP::ServiceContextList &sc,
      TAO_Transport &transport,
      TAO_ServerRequest *request);

    int generate_service_context (
      TAO_Stub *stub,
      TAO_Transport& transport,
      TAO_Operation_Details &opdetails,
      TAO_Target_Specification &spec,
      TAO_OutputCDR &msg);

  private:
    typedef ACE_Array_Map<IOP::ServiceId,
                          TAO_Service_Context_Handler*> Table;
    typedef Table::key_type   key_type;
    typedef Table::data_type  data_type;
    typedef Table::value_type value_type;
    typedef Table::size_type  size_type;
    typedef value_type *      iterator;

    Table registry_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_SERVICE_CONTEXT_HANDLER_REGISTRY_H */
