/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Service_Types.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SERVICE_TYPE_H)
#define ACE_SERVICE_TYPE_H

#include "ace/Service_Object.h"
#include "ace/Synch.h"

class ACE_Export ACE_Service_Type_Impl
  // = TITLE
  //     The abstract base class of the hierarchy that defines the
  //     contents of the <ACE_Service_Repository>.  The subclasses of
  //     this class allow the configuration of <ACE_Service_Objects>,
  //     <ACE_Modules>, and <ACE_Streams>.
  //
  // = DESCRIPTION
  //     This class provides the root of the implementation hierarchy
  //     of the "Bridge" pattern.  It maintains a pointer to the
  //     appropriate type of service implementation, i.e.,
  //     <ACE_Service_Object>, <ACE_Module>, or <ACE_Stream>.
{
public:
  // = Initialization and termination methods.
  ACE_Service_Type_Impl (const void *object, 
			 const char *s_name, 
			 u_int flags = 0);
  virtual ~ACE_Service_Type_Impl (void);

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
  // Pointer to object that implements the service.  This actually
  // points to an <ACE_Service_Object>, <ACE_Module>, or <ACE_Stream>.

  u_int flags_;
  // Flags that control serivce behavior (particularly deletion).
};

class ACE_Export ACE_Service_Object_Type : public ACE_Service_Type_Impl
  // = TITLE
  //     Define the methods for handling the configuration of
  //     <ACE_Service_Objects>. 
{
public:
  // = Initialization method.
  ACE_Service_Object_Type (const void *so,
			   const char *name, 
			   u_int flags = 0);

  // = Implement the hooks for <ACE_Service_Objects>.
  virtual int suspend (void) const;
  virtual int resume (void) const;
  virtual int init (int argc, char *argv[]) const;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const;
};

class ACE_Export ACE_Module_Type : public ACE_Service_Type_Impl
  // = TITLE
  //     Define the methods for handling the configuration of
  //     <ACE_Modules>. 
{
public:
  // = Initialization method.
  ACE_Module_Type (const void *m, // Really an <ACE_Module> *.
		   const char *identifier, 
		   u_int flags = 0);

  // = Implement the hooks for <ACE_Modules>.
  virtual int suspend (void) const;
  virtual int resume (void) const;
  virtual int init (int argc, char *argv[]) const;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const;

  // Get/set the link pointer.
  ACE_Module_Type *link (void) const;
  void link (ACE_Module_Type *);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Module_Type *link_; 
  // Pointer to the next <ACE_Module_Type> in an <ACE_Stream_Type>.
};

class ACE_Export ACE_Stream_Type : public ACE_Service_Type_Impl
  // = TITLE
  //     Define the methods for handling the configuration of
  //     <ACE_Streams>. 
{
public:
  // = Initialization method.
  ACE_Stream_Type (const void *s, // Really an <ACE_Stream> *.
		   const char *identifier, 
		   u_int flags = 0);

  // = Implement the hooks for <ACE_Streams>.
  virtual int suspend (void) const;
  virtual int resume (void) const;
  virtual int init (int argc, char *argv[]) const;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const;

  int push (ACE_Module_Type *new_module);
  // Add a new <ACE_Module> to the top of the <ACE_Stream>.

  int remove (ACE_Module_Type *module);
  // Search for <module> and remove it from the <ACE_Stream>.

  ACE_Module_Type *find (const char *mod_name) const;
  // Locate the <ACE_Module> with <mod_name>.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Module_Type *head_;
  // Pointer to the head of the <ACE_Module> list.
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Types.i"
#endif /* __ACE_INLINE__ */

#endif /* _SERVICE_TYPE_H */
