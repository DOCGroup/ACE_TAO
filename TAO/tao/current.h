// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
// 
// = FILENAME
//     current.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#if !defined (TAO_POA_CURRENT_H)
#  define TAO_POA_CURRENT_H

#include "tao/corba.h"

class TAO_POA_Current : public POA_PortableServer::Current
{
  // = TITLE
  // Implementation of the PortableServer::Current object.
  //
  // = DESCRIPTION
  //
  // Objects of this class hold state information regarding the
  // current POA invocation.  Savvy readers will notice that this
  // contains substantially more methods than the POA spec shows; they
  // exist because the ORB either (a) needs them or (b) finds them
  // useful for implementing a more efficient ORB.
  //
  // The intent is that instances of this class are held in
  // Thread-Specific Storage so that upcalls can get context
  // information regarding their invocation.  The POA itself must
  // insure that all <set_*> operations are performed in the execution
  // thread so that the proper <TAO_POA_Current> pointer is
  // obtained from TSS.

public:
  // = Specification-mandated methods
  
  PortableServer::POA_ptr get_POA (CORBA_Environment &env);
  // Return pointer to the invoking POA.  This has _out
  // semantics. (Irfan is this correct?)  Raises the
  // <CORBA::NoContext> exception.
  //
  // 

  PortableServer::ObjectId *get_object_id (CORBA_Environment &env);
  // Return pointer to the object id through which this was invoked.
  // This may be necessary in cases where a <Servant> is serving under
  // the guise of multiple object ids.  This has _out semantics
  // (Irfan, is this correct?)  Raises the <CORBA::NoContext>
  // exception.

  // = TAO Extensions

  void clear (void);
  // Clear any prior settings made.  This will make things which can
  // throw the <CORBA::NoContext> exception raise it if invoked
  // without a corresponding <set_*> operation.

  int context_is_valid (void);
  // Returns non-zero if the context is valid, i.e., if it would be
  // impossible for a <CORBA::NoContext> exception to be raised.
  
  void POA_impl (TAO_POA *impl);
  // Set the POA implementation.

  TAO_POA *POA_impl (void) const;
  // Get the POA imeplemantation

  void object_id (const PortableServer::ObjectId &id);
  // Set the object ID.

  const PortableServer::ObjectId &object_id (void) const;
  // Get the object ID.

  void object_key (const TAO::ObjectKey &key);
  // Set the object key.

  const TAO::ObjectKey &object_key (void) const;
  // Get the object key.

  void in_upcall (int yesno);
  // Set whether we're in an upcall (non-zero is yes).

  int in_upcall (void) const;
  // Get whether we're in an upcall (non-zero is yes).

  TAO_POA_Current (void);
  // Constructor

  virtual ~TAO_POA_Current (void);
  // Destructor
  
private:
  TAO_POA *poa_impl_;
  // The POA implementation invoking an upcall
  
  const PortableServer::ObjectId *object_id_;
  // The object ID of the current context.
  
  int in_upcall_;
  // Flag which indicates whether we're in an upcall.
  
  const TAO::ObjectKey *object_key_;
  // The object key of the current context.

  // = Hidden because we don't allow these
  TAO_POA_Current (const TAO_POA_Current &);
  void operator= (const TAO_POA_Current &);
};

#endif /* TAO_POA_CURRENT_H */
