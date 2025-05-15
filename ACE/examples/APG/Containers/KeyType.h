/* -*- C++ -*- */
#ifndef __KEYTYPE_H_
#define __KEYTYPE_H_

// Listing 1 code/ch05
class KeyType
{
public:
  friend bool operator == (const KeyType&, const KeyType&);

  KeyType () = default;
  KeyType (int i) : val_(i) {}
  KeyType (const KeyType&) = default;
  KeyType (KeyType&&) = default;
  KeyType& operator= (const KeyType&) = default;
  KeyType& operator= (KeyType&&) = default;
  operator int() const { return val_; };

private:
  int val_ {};
};

bool operator == (const KeyType& a, const KeyType& b)
{
  return (a.val_ == b.val_);
}
// Listing 1

#endif /* __KEYTYPE_H_ */
