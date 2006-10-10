/* -*- C++ -*- */
// $Id$

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

  DataElement (int data) : data_(data) { count_++; }

  DataElement (const DataElement& e)
  {
    data_ = e.getData ();
    count_++;
  }

  DataElement & operator= (const DataElement& e)
  {
    data_ = e.getData ();
    return *this;
  }

  bool operator== (const DataElement& e)
  { return this->data_ == e.data_; }

  ~DataElement () { count_--; }

  int getData (void) const { return data_; }

  void setData (int val) { data_ = val; }

  static int numOfActiveObjects (void) { return count_; }

 private:
  int data_;
  static int count_;
};
// Listing 1

int DataElement::count_ = 0;

#endif /*DATAELEMENT_H*/

