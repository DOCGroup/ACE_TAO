/* -*- C++ -*- */
// $Id$

#ifndef __HASH_MAP_HASH_H_
#define __HASH_MAP_HASH_H_

// Listing 1 code/ch05
// Key type that we are going to use.
class KeyType
{
public:
  KeyType () : val_(0) {}

  KeyType (int i) : val_(i) {}

  KeyType (const KeyType& kt) { this->val_ = kt.val_; }

  operator int (void) const { return val_; }

private:
  int val_;
};

// Specialize the hash functor.
template<>
class ACE_Hash<KeyType>
{
public:
  u_long operator() (const KeyType kt) const
  {
    int val = kt;
    return (u_long)val;
  }
};


// Specialize the equality functor.
template<>
class ACE_Equal_To<KeyType>
{
public:
  int operator() (const KeyType& kt1,
                  const KeyType& kt2) const
  {
    int val1 = kt1;
    int val2 = kt2;
    return (val1 == val2);
  }
};
// Listing 1

#endif /* __HASH_MAP_HASH_H_ */
