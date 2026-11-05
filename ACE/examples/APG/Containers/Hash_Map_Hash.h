/* -*- C++ -*- */
#ifndef __HASH_MAP_HASH_H_
#define __HASH_MAP_HASH_H_

// Listing 1 code/ch05
// Key type that we are going to use.
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

// Specialize the hash functor.
template<>
class ACE_Hash<KeyType>
{
public:
  u_long operator() (KeyType const &key) const
  {
    return static_cast<u_long> (static_cast<int> (key));
  }
};


// Specialize the equality functor.
template<>
class ACE_Equal_To<KeyType>
{
public:
  int operator() (KeyType const &key1,
                  KeyType const &key2) const
  {
    return static_cast<int> (key1) == static_cast<int> (key2);
  }
};

ACE_END_VERSIONED_NAMESPACE_DECL

// Listing 1

#endif /* __HASH_MAP_HASH_H_ */
