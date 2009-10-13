/* -*- C++ -*- */
// $Id$

#ifndef __KEYTYPE_H_
#define __KEYTYPE_H_

// Listing 1 code/ch05
class KeyType
{
public:
  friend bool operator == (const KeyType&, const KeyType&);

  KeyType () : val_(0) {}
  KeyType (int i) : val_(i) {}
  KeyType (const KeyType& kt) { this->val_ = kt.val_; };
  operator int() const { return val_; };

private:
  int val_;
};

bool operator == (const KeyType& a, const KeyType& b)
{
  return (a.val_ == b.val_);
}
// Listing 1

#endif /* __KEYTYPE_H_ */
