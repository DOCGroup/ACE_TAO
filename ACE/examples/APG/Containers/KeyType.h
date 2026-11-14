/* -*- C++ -*- */
#ifndef __KEYTYPE_H_
#define __KEYTYPE_H_

// Listing 1 code/ch05
class KeyType
{
public:
  friend bool operator== (KeyType const &, KeyType const &);

  KeyType () = default;
  explicit KeyType (int const i) : val_ (i) {}
  operator int () const { return this->val_; }

private:
  int val_ {};
};

inline bool operator== (KeyType const &a, KeyType const &b)
{
  return (a.val_ == b.val_);
}
// Listing 1

#endif /* __KEYTYPE_H_ */
