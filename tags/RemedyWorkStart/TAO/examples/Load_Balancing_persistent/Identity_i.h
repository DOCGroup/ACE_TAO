/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Identity_i.h
 *
 *  $Id$
 *
 *  Implements the interface in Identity.idl.
 *
 *
 *  @author Marina Spivak <marina@cs.wustl.edu> with modifications by Bala Natarajan <bala@cs.wustl.edu>
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
  Identity_i (const char *name,
              PortableServer::POA_ptr poa);

  /// Destructor.
  ~Identity_i (void);

  /// Returns the name of this object.
  virtual void get_name (CORBA::String_out name);

  /// Method for the POA that will return the persistent POA_ptr stored
  /// in here..
  PortableServer::POA_ptr _default_POA (void);

private:

  /// Stores the name of this object.
  CORBA::String_var name_;

  PortableServer::POA_var poa_;
};

#endif /* IDENTITY_I_H_ */
