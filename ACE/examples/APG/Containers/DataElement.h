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
  DataElement () : data_ (0) { count_++; }

  explicit DataElement (int data) : data_ (data) { count_++; }

  DataElement (DataElement const &e)
  {
    data_ = e.getData ();
    count_++;
  }

  DataElement &operator = (DataElement const &e)
  {
    data_ = e.getData ();
    return *this;
  }

  bool operator == (DataElement const &e) const
  { return this->data_ == e.data_; }

  ~DataElement () { count_--; }

  int getData () const { return data_; }

  void setData (int val) { data_ = val; }

  static int numOfActiveObjects () { return count_; }

 private:
  int data_;
  inline static int count_ {};
};
// Listing 1

#endif /*DATAELEMENT_H*/

