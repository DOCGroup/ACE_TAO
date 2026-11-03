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
  explicit Identity_i (char const *name);

  /// Destructor.
  ~Identity_i () override = default;

  Identity_i (Identity_i const &) = delete;
  Identity_i (Identity_i &&) = delete;
  Identity_i &operator= (Identity_i const &) = delete;
  Identity_i &operator= (Identity_i &&) = delete;

  /// Returns the name of this object.
  void get_name (CORBA::String_out name) override;

private:
  /// Stores the name of this object.
  CORBA::String_var name_;
};

#endif /* IDENTITY_I_H_ */
