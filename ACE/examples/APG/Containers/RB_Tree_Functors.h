/* -*- C++ -*- */
#ifndef __RB_TREE_FUNCTORS_H_
#define __RB_TREE_FUNCTORS_H_

#include "ace/Functor.h"

// Listing 1 code/ch05
// Same key type.
class KeyType
{
public:
  KeyType () = default;
  explicit KeyType (int const i) : val_ (i) {}
  KeyType (KeyType const &) = default;
  operator int () const { return this->val_; }

private:
  int val_ {};
};

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

template<>
class ACE_Less_Than<KeyType>
{
public:
  int operator() (KeyType const &k1, KeyType const &k2) const
  { return k1 < k2; }
};

ACE_END_VERSIONED_NAMESPACE_DECL

// Listing 1

#endif /* __RB_TREE_FUNCTORS_H_ */
