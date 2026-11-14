/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Identity_i.h
 *
 *  Implements the interface in Identity.idl.
 *
 *  @author Marina Spivak <marina@cs.wustl.edu> with modifications by Bala Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef IDENTITY_I_H_
#define IDENTITY_I_H_

#include "IdentityS.h"

/// Implements the Identity.idl interface.
class Identity_i :
  public virtual POA_Identity
{
public:
  /// Constructor - initializes the name of this object.
  Identity_i (char const *name,
              PortableServer::POA_ptr poa);

  ~Identity_i () override = default;

  Identity_i (Identity_i const &) = delete;
  Identity_i (Identity_i &&) = delete;
  Identity_i &operator= (Identity_i const &) = delete;
  Identity_i &operator= (Identity_i &&) = delete;

  /// Returns the name of this object.
  void get_name (CORBA::String_out name) override;

  /// Returns the persistent POA reference stored in this servant.
  PortableServer::POA_ptr _default_POA () override;

private:
  /// Stores the name of this object.
  CORBA::String_var name_;

  PortableServer::POA_var poa_;
};

#endif /* IDENTITY_I_H_ */
