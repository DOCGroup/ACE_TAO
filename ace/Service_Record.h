/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Service_Record.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SERVICE_RECORD_H)
#define ACE_SERVICE_RECORD_H

#include "ace/Service_Object.h"
#include "ace/Synch.h"
#include "ace/Stream.h"

typedef ACE_Stream<ACE_SYNCH> MT_Stream;
typedef ACE_Module<ACE_SYNCH> MT_Module;
typedef ACE_Task<ACE_SYNCH> MT_Task;

class ACE_Export ACE_Service_Object_Type : public ACE_Service_Type
  // = TITLE
  //     Define the methods for handling <ACE_Service_Objects>.
{
public:
  ACE_Service_Object_Type (ACE_Service_Object *so, 
			   const char *name, 
			   u_int flags = 0);
  virtual int suspend (void) const;
  virtual int resume (void) const;
  virtual int init (int argc, char *argv[]) const;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const;
};

class ACE_Export ACE_Module_Type : public ACE_Service_Type
  // = TITLE
  //     Define the methods for handling <ACE_Modules>.
{
public:
  ACE_Module_Type (MT_Module *m, 
		   const char *identifier, 
		   u_int flags = 0);

  virtual int suspend (void) const;
  virtual int resume (void) const;
  virtual int init (int argc, char *argv[]) const;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const;

  ACE_Module_Type *link (void) const;
  void link (ACE_Module_Type *);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Module_Type *next_;
};

class ACE_Export ACE_Stream_Type : public ACE_Service_Type
  // = TITLE
  //     Define the methods for handling <ACE_Streams>.
{
public:
  ACE_Stream_Type (MT_Stream *s, 
		   const char *identifier, 
		   u_int flags = 0);

  virtual int suspend (void) const;
  virtual int resume (void) const;
  virtual int init (int argc, char *argv[]) const;
  virtual int fini (void) const;
  virtual int info (char **str, size_t len) const;

  int push (ACE_Module_Type *new_module);
  int remove (ACE_Module_Type *module);
  ACE_Module_Type *find (const char *mod_name) const;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Module_Type *head_;
};

class ACE_Export ACE_Service_Record
  // = TITLE
  //      Packaging interface for the various types of
  //      <ACE_Service_Types>.
{
public:
  // = Initialization and termination methods.
  ACE_Service_Record (const char *n, 
		      ACE_Service_Type *o, 
		      const void *handle, 
		      int active);
  ~ACE_Service_Record (void);
			 
  const char *name (void) const;
  void name (const char *);

  const ACE_Service_Type *type (void) const;
  void type (const ACE_Service_Type *, 
	     int active = 1);

  const void *handle (void) const;
  void handle (const void *);

  void suspend (void) const;
  void resume (void) const;
  int  active (void) const;
  void active (int);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  const char *name_;   
  // Humanly readible name of svc.

  const ACE_Service_Type *type_;
  // Pointer to C++ object that implements the svc.

  const void *handle_; 
  // Handle to shared object file (non-zero if dynamically linked).

  int active_;  
  // 1 if svc is currently active, otherwise 0.
};

#if defined (__ACE_INLINE__)
#include "ace/Service_Record.i"
#endif /* __ACE_INLINE__ */

#endif /* _SERVICE_RECORD_H */
