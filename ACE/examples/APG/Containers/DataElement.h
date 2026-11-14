/* -*- C++ -*- */
#if !defined(DATAELEMENT_H)
#define DATAELEMENT_H

class DataElementEx;

// Listing 1 code/ch05
// A simple data element class.
class DataElement
{
  friend class DataElementEx;

public:
  DataElement () { ++count_; }

  explicit DataElement (int const data) : data_ (data) { ++count_; }

  DataElement (DataElement const &element)
    : data_ (element.data_)
  {
    ++count_;
  }

  DataElement &operator= (DataElement const &element)
  {
    this->data_ = element.data_;
    return *this;
  }

  bool operator== (DataElement const &element) const
  { return this->data_ == element.data_; }

  ~DataElement () { --count_; }

  int getData () const { return data_; }

  void setData (int const val) { data_ = val; }

  static int numOfActiveObjects () { return count_; }

private:
  int data_ {};
  inline static int count_ {};
};
// Listing 1

#endif /*DATAELEMENT_H*/

