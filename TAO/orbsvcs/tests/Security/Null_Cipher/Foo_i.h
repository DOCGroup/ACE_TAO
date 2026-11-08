// -*- C++ -*-

//=============================================================================
/**
 * @file Foo_i.h
 *
 * Implementation header for the Null_Cipher test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef NULL_CIPHER_FOO_I_H
#define NULL_CIPHER_FOO_I_H

#include "FooS.h"

class Foo_i : public virtual POA_Foo
{
public:
  /// Constructor.
  explicit Foo_i (CORBA::ORB_ptr);

  void shutdown () override;

  Foo_i (Foo_i const &) = delete;
  Foo_i &operator= (Foo_i const &) = delete;
  Foo_i (Foo_i &&) = delete;
  Foo_i &operator= (Foo_i &&) = delete;

private:
  /// Reference to the ORB.
  CORBA::ORB_var orb_;
};

#endif  /* NULL_CIPHER_FOO_I_H */
