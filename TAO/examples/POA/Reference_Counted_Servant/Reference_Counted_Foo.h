// $Id$

//=============================================================================
//
// = FILENAME
//     Reference_Counted_Foo.h
//
// = DESCRIPTION
//     Defines Reference_Counted_Foo class for the Foo interface
//
// = AUTHOR
//     Irfan Pyarali
//
//=============================================================================

#ifndef REFERENCE_COUNTED_FOO_H
#define REFERENCE_COUNTED_FOO_H

#include "FooS.h"

// This is to remove "inherits via dominance" warnings from MSVC.
// MSVC is being a little too paranoid.
#if defined (_MSC_VER)
# pragma warning (disable : 4250)
#endif /* _MSC_VER */

class Reference_Counted_Foo : public virtual PortableServer::RefCountServantBase,
                              public virtual POA_Foo
{
public:
  Reference_Counted_Foo (CORBA::ORB_ptr orb,
                PortableServer::POA_ptr poa,
                CORBA::Long value);
  // Constructor - takes a POA and a value parameter

  virtual ~Reference_Counted_Foo (void);
  // Destructor

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
  // Returns the Default POA of this Servant object

  virtual CORBA::Long doit (CORBA::Environment &env);
  // Simple doit method

  virtual void simply_doit (CORBA::Environment &env);
  // Even simpler doit method

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown the ORB

protected:
  CORBA::ORB_var orb_;
  // Keep a pointer to the ORB so we can shut it down.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()

  CORBA::Long value_;
  // The current value.
};

#endif /* REFERENCE_COUNTED_FOO_H */
