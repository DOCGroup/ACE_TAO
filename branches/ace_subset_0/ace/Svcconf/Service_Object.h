/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Service_Object.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_SERVICE_OBJECT_H
#define ACE_SERVICE_OBJECT_H
#include "ace/pre.h"

#include "ace/Svcconf/Shared_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler/Event_Handler.h"

#define ACE_Component ACE_Service_Object
/**
 * @class ACE_Service_Object
 *
 * @brief Provide the abstract base class common to all service
 * implementations.
 *
 * Classes that inherit from <ACE_Service_Objects> are capable
 * of being registered with the <ACE_Reactor> (due to the
 * <ACE_Event_Handler>, as well as being dynamically linked by
 * the <ACE_Service_Config> (due to the <ACE_Shared_Object>).
 */
class ACE_Export ACE_Service_Object : public ACE_Event_Handler, public ACE_Shared_Object
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  ACE_Service_Object (ACE_Reactor * = 0);

  /// Destructor.
  virtual ~ACE_Service_Object (void);

    /// Temporarily disable a service without removing it completely.
  virtual int suspend (void);

    /// Re-enable a previously suspended service.
  virtual int resume (void);
};

// Forward decl.
class ACE_Service_Type_Impl;

/**
 * @class ACE_Service_Type
 *
 * @brief Keeps track of information related to the various
 * <ACE_Service_Type_Impl> subclasses.
 *
 * This class acts as the interface of the "Bridge" pattern.
 */
class ACE_Export ACE_Service_Type
{
public:
  enum
  {
    /// Delete the payload object.
    DELETE_OBJ = 1,

    /// Delete the enclosing object.
    DELETE_THIS = 2
  };

  // = Initialization and termination methods.
  ACE_Service_Type (const ACE_TCHAR *n,
                    ACE_Service_Type_Impl *o,
                    const ACE_SHLIB_HANDLE handle,
                    int active);
  ~ACE_Service_Type (void);

  const ACE_TCHAR *name (void) const;
  void name (const ACE_TCHAR *);

  const ACE_Service_Type_Impl *type (void) const;
  void type (const ACE_Service_Type_Impl *,
             int active = 1);

  ACE_SHLIB_HANDLE handle (void) const;
  void handle (const ACE_SHLIB_HANDLE);

  void suspend (void) const;
  void resume (void) const;
  int  active (void) const;
  void active (int);

  /// Calls <fini> on <type_>
  void fini (void);

  /// Check if the service has been fini'ed.
  int fini_called (void) const;

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Humanly readible name of svc.
  const ACE_TCHAR *name_;

  /// Pointer to C++ object that implements the svc.
  const ACE_Service_Type_Impl *type_;

  /// Handle to shared object file (non-zero if dynamically linked).
  ACE_SHLIB_HANDLE handle_;

  /// 1 if svc is currently active, otherwise 0.
  int active_;

  /// 1 if <fini> on <type_> has already been called, otherwise 0.
  int fini_already_called_;
};

/**
 * @class ACE_Service_Object_Ptr
 *
 * @brief This is a smart pointer that holds onto the associated
 * <ACE_Service_Object> * until the current scope is left, at
 * which point the object's <fini> hook is called and the
 * service_object_ gets deleted.
 *
 * This class is similar to the Standard C++ Library class
 * <auto_ptr>.  It is used in conjunction with statically linked
 * <ACE_Service_Objects>, as shown in the
 * ./netsvcs/server/main.cpp example.
 */
class ACE_Export ACE_Service_Object_Ptr
{
public:
  // = Initialization and termination methods.
  /// Acquire ownership of the <so>.
  ACE_Service_Object_Ptr (ACE_Service_Object *so);

  /// Release the held <ACE_Service_Object> by calling its <fini> hook.
  ~ACE_Service_Object_Ptr (void);

  /// Smart pointer to access the underlying <ACE_Service_Object>.
  ACE_Service_Object *operator-> ();

private:
  /// Holds the service object until we're done.
  ACE_Service_Object *service_object_;
};

#if defined (__ACE_INLINE__)
#include "ace/Svcconf/Service_Object.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* ACE_SERVICE_OBJECT_H */
