/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Service_Object.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_SERVICE_OBJECT_H
#define ACE_SERVICE_OBJECT_H

#include "ace/Shared_Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

class ACE_Export ACE_Service_Object : public ACE_Event_Handler, public ACE_Shared_Object
{
  // = TITLE
  //     Provide the abstract base class common to all service
  //     implementations.
  //
  // = DESCRIPTION
  //     Classes that inherit from <ACE_Service_Objects> are capable
  //     of being registered with the <ACE_Reactor> (due to the
  //     <ACE_Event_Handler>, as well as being dynamically linked by
  //     the <ACE_Service_Config> (due to the <ACE_Shared_Object>).
public:
  // = Initialization and termination methods.
  ACE_Service_Object (ACE_Reactor * = 0);
  // Constructor.

  virtual ~ACE_Service_Object (void);
  // Destructor.

  virtual int suspend (void);
    // Temporarily disable a service without removing it completely.

  virtual int resume (void);
    // Re-enable a previously suspended service.
};

// Forward decl.
class ACE_Service_Type_Impl;

class ACE_Export ACE_Service_Type
{
  // = TITLE
  //      Keeps track of information related to the various
  //      <ACE_Service_Type_Impl> subclasses.
  //
  // = DESCRIPTION
  //      This class acts as the interface of the "Bridge" pattern.
public:
  enum
  {
    DELETE_OBJ = 1,
    // Delete the payload object.

    DELETE_THIS = 2
    // Delete the enclosing object.
  };

  // = Initialization and termination methods.
  ACE_Service_Type (const ASYS_TCHAR *n,
                    ACE_Service_Type_Impl *o,
                    const ACE_SHLIB_HANDLE handle,
                    int active);
  ~ACE_Service_Type (void);

  const ASYS_TCHAR *name (void) const;
  void name (const ASYS_TCHAR *);
  const char *chname (void) const;

  const ACE_Service_Type_Impl *type (void) const;
  void type (const ACE_Service_Type_Impl *,
             int active = 1);

  ACE_SHLIB_HANDLE handle (void) const;
  void handle (const ACE_SHLIB_HANDLE);

  void suspend (void) const;
  void resume (void) const;
  int  active (void) const;
  void active (int);

  void fini (void);
  // Calls fini() on <type_>

  int fini_called (void) const;
  // Check if the service has been fini'ed.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const ASYS_TCHAR *name_;
  // Humanly readible name of svc.

#if defined (ACE_HAS_MOSTLY_UNICODE_APIS)
  char *chname_;
  // This interface is used to pass char name when instantiate
  // ACE_Parse_Node.
#endif /* ACE_HAS_MOSTLY_UNICODE_APIS */

  const ACE_Service_Type_Impl *type_;
  // Pointer to C++ object that implements the svc.

  ACE_SHLIB_HANDLE handle_;
  // Handle to shared object file (non-zero if dynamically linked).

  int active_;
  // 1 if svc is currently active, otherwise 0.

  int fini_already_called_;
  // 1 if fini() on <type_> has already been called, otherwise 0.
};

class ACE_Export ACE_Service_Object_Ptr
{
  // = TITLE
  //     This is a smart pointer that holds onto the associated
  //     <ACE_Service_Object> * until the current scope is left, at
  //     which point the object's <fini> hook is called and the
  //     service_object_ gets deleted.
  //
  // = DESCRIPTION
  //     This class is similar to the Standard C++ Library class
  //     <auto_ptr>.  It is used in conjunction with statically linked
  //     <ACE_Service_Objects>, as shown in the
  //     ./netsvcs/server/main.cpp example.
public:
  // = Initialization and termination methods.
  ACE_Service_Object_Ptr (ACE_Service_Object *so);
  // Acquire ownership of the <so>.

  ~ACE_Service_Object_Ptr (void);
  // Release the held <ACE_Service_Object> by calling its <fini> hook.

  ACE_Service_Object *operator-> ();
  // Smart pointer to access the underlying <ACE_Service_Object>.

private:
  ACE_Service_Object *service_object_;
  // Holds the service object until we're done.
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Object.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SERVICE_OBJECT_H */
