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

#if !defined (ACE_SERVICE_OBJECT_H)
#define ACE_SERVICE_OBJECT_H

#include "ace/Shared_Object.h"
#include "ace/Event_Handler.h"

class ACE_Export ACE_Service_Object : public ACE_Event_Handler, public ACE_Shared_Object
  // = TITLE
  //     Provide the abstract base class common to all services 
{
public:
  // = Initialization and termination methods.
  ACE_Service_Object (void);
  virtual ~ACE_Service_Object (void);

  virtual int suspend (void);
    // Temporarily disable a service without removing it completely 
  virtual int resume (void);
    // Re-enable a previously suspended service 
};

class ACE_Export ACE_Service_Type
  // = TITLE
  //     Provide the class hierarchy that defines the contents of
  //     the Service Repository search structure.
{
public:
  enum
  {
    DELETE_OBJ = 1, // Delete the payload object.
    DELETE_THIS = 2 // Delete the enclosing object.
  };

  // = Initialization method.
  ACE_Service_Type (const void *object, 
		    const char *s_name, 
		    u_int flags = 0);

  // = Pure virtual interface (must be defined by the subclass).
  virtual int suspend (void) const = 0;
  virtual int resume (void) const = 0;
  virtual int init (int argc, char *argv[]) const = 0;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const = 0;

  const void *object (void) const;
  // The pointer to the service.

  const char *name (void) const;
  // Get the name of the service.

  void name (const char *);
  // Set the name of the service.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  const char *name_;
  // Name of the service.

  const void *obj_;
  // Pointer to object that implements the service.

  u_int flags_;
  // Flags that control serivce behavior (particularly deletion).
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Object.i"
#endif /* __ACE_INLINE__ */

#endif /* ACE_SERVICE_OBJECT_H */
