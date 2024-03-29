/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Identity_i.h
 *
 *  Implements the interface in Identity.idl.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================

#ifndef IDENTITY_I_H_
#define IDENTITY_I_H_

#include "IdentityS.h"

class Identity_i :
  public virtual POA_Identity
{
  // = TITLE
  //    This class implements Identity.idl interface.
public:
  /// Constructor - initializes the name of this object.
  Identity_i (const char *name);

  /// Destructor.
  ~Identity_i ();

  /// Returns the name of this object.
  virtual void get_name (CORBA::String_out name);

private:
  /// Stores the name of this object.
  CORBA::String_var name_;
};

#endif /* IDENTITY_I_H_ */
