/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Adapter.h
//
// = AUTHOR
//    Carlos O'Ryan (coryan@uci.edu)
//
// ============================================================================

#ifndef TAO_ADAPTER_H
#define TAO_ADAPTER_H
#include "ace/pre.h"

#include "tao/corbafwd.h"
#include "tao/Exception.h"
#include "ace/Service_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORB_Core;
class TAO_Stub;
class TAO_MProfile;
class TAO_ServerRequest;

class TAO_Export TAO_Adapter
{
public:
  virtual ~TAO_Adapter (void);

  virtual void open (CORBA::Environment &) = 0;
  // Initialize the Adapter

  virtual void close (int wait_for_completion,
                      CORBA::Environment &) = 0;
  // The ORB is shutting down, destroy any resources attached to this
  // adapter.

  virtual void check_close (int wait_for_completion,
                            CORBA::Environment &) = 0;
  // Check if the adapter can be closed in the current context, raise
  // an exception if not.

  virtual int priority (void) const = 0;
  // Return the priority assigned to this adapter.
  // Adapters at higher priority are used first, the first adapter
  // that matches a key is used to dispatch a request.

  virtual int dispatch (TAO_ObjectKey &key,
                        TAO_ServerRequest &request,
                        void *context, /* unused? */
                        CORBA::Object_out forward_to,
                        CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException)) = 0;
  // Return the status....
  enum {
    DS_OK,
    // The operation was successfully dispatched, an exception may
    // have been raised, but that is a correct execution too.

    DS_FAILED,
    // There was a problem in dispatching the operation.

    DS_MISMATCHED_KEY,
    // The key is not in the right format for this Adapter, try the
    // next one.

    DS_FORWARD
    // Forward the request to another object reference, this decouples
    // the ORB from the PortableServer::ForwardRequest exception
  };

  virtual const char *name (void) const = 0;
  // Return the name, i.e. the object id used to resolve it in the
  // ORB.

  virtual CORBA::Object_ptr root (void) = 0;
  // Rerturn the root of the Object Adapter.
  // Each adapter defines its own IDL interface accessed through the
  // method above.

  virtual CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                                      const TAO_MProfile &) = 0;
  // Create a collocated object using the given profile and stub.
};

// ****************************************************************

// @@ Move to its own file

class TAO_Export TAO_Adapter_Registry
{
public:
  TAO_Adapter_Registry (TAO_ORB_Core *orb_core);

  ~TAO_Adapter_Registry (void);
  // Close the

  void close (int wait_for_completion,
              CORBA::Environment &ACE_TRY_ENV);
  // Close each of of the Adapters and then cleanup the Registry.
  // It is possible that an Adapter will reject a close() call if it
  // is invoked in an innapropriate context (think shutting down the
  // POA while performing an upcall).

  void check_close (int wait_for_completion,
                    CORBA::Environment &ACE_TRY_ENV);
  // Verify if the close() call can be invoked in the current
  // context for *all* adapters.
  // Raise the right exception if not.

  void insert (TAO_Adapter *adapter,
               CORBA::Environment &ACE_TRY_ENV);
  // Insert a new adapter into the registry.

  void dispatch (TAO_ObjectKey &key,
                 TAO_ServerRequest &request,
                 void *context, /* unused? */
                 CORBA::Object_out forward_to,
                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Dispatch the request to all the adapters.
  // It tries the adapters ordered by priority, stopping when the
  // adapter returns a status different from DS_MISMATCHED_KEY

  CORBA::Object_ptr create_collocated_object (TAO_Stub *,
                                              const TAO_MProfile &);
  // Create a collocated object using the given profile and stub.

  TAO_Adapter *find_adapter (const char *name) const;
  // Fetch the adapter named <name>

private:
  TAO_ORB_Core *orb_core_;
  // The ORB Core

  size_t adapters_capacity_;
  size_t adapters_count_;
  TAO_Adapter **adapters_;
  // A simple array of adapters.
};

// ****************************************************************

class TAO_Export TAO_Adapter_Factory : public ACE_Service_Object
{
public:
  virtual ~TAO_Adapter_Factory (void);
  // Destructor

  virtual TAO_Adapter *create (TAO_ORB_Core *orb_core) = 0;
  // Create a new adapter
};

#if defined (__ACE_INLINE__)
# include "Adapter.i"
#endif /* __ACE_INLINE__ */

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_OBJECT_ADAPTER_H */
