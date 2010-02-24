/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = FILENAME
//   Identity_i.h
//
// = DESCRIPTION
//    Implements the interface in Identity.idl.
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================
#ifndef IDENTITY_I_H_
#define IDENTITY_I_H_

#include "IdentityS.h"

class Identity_i :
  public virtual POA_Identity
{
  // = TITLE
  //    This class implements Identity.idl interface.
public:

  Identity_i (const char *name);
  // Constructor - initializes the name of this object.

  ~Identity_i (void);
  // Destructor.

  virtual void get_name (CORBA::String_out name);
  // Returns the name of this object.

private:

  CORBA::String_var name_;
  // Stores the name of this object.
};

#endif /* IDENTITY_I_H_ */
