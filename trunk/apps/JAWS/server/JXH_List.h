/* -*- C++ -*- */
// JXH_List.h

#ifndef JXH_LIST_H
#define JXH_LIST_H

#include <iostream.h>

template <class ItemType>
class JXH_List
  // = TITLE
  //    JXH_List Template Class
  //
  // = DESCRIPTION
  //    Yet another implementation of a list.
  //    Completely unordered.  Elements inserted at tail, but may
  //    be moved about by other operations.  Must use linear
  //    search to find.
  //
  //    What's different:
  //    (1) If you are creating a list of pointers, it will return
  //        the same pointers you gave it.
  //    (2) If you are creating a list of objects, your objects must
  //        have a copy constructor.  The List will create its own
  //        local copy of the object.
  //
{
public:
  // = Public Interfaces
  //

  JXH_List() : count_(0), current_(0), size_(256)
  { theList_ = new JXH_ListItem *[size_]; }

  //  JXH_List(long size) : current_(0), count_(0), size_(size)
  //  { theList_ = new JXH_ListItem *[size_]; }

  JXH_List(unsigned long size) : count_(0), current_(0), size_(size)
  { theList_ = new JXH_ListItem *[size_]; }

  ~JXH_List()
  { for (unsigned long i = 0; i < count_; i++) delete theList_[i];
    delete [] theList_; }

  int Insert(const ItemType & item)
  { if (IsFull()) {
      JXH_ListItem ** alist = new JXH_ListItem *[2*size_];
      if (alist == 0) return 0;
      for (unsigned long i = 0; i < count_; i++) alist[i] = theList_[i];
      delete [] theList_;
      theList_ = alist;
      size_ = 2*size_;
    }
    current_ = count_++;
    theList_[current_] = new JXH_ListItem(item);
    return 1; }

  int Delete()
  { if (IsEmpty()) return 0;
    delete theList_[current_];
    theList_[current_] = theList_[--count_];
    theList_[count_] = 0;
    return 1; }

  int Size()    const { return count_; }
  int IsFull()  const { return count_ == size_; }
  int IsEmpty() const { return count_ == 0; }

  const ItemType & Head()    { if (IsEmpty()) return garbage;
                               return theList_[current_ = 0]->item_; }

  const ItemType & Tail()    { if (IsEmpty()) return garbage;
                               return theList_[current_ = count_-1]->item_; }

  const ItemType & Current() { if (IsEmpty()) return garbage;
                               return theList_[current_]->item_; }

  const ItemType & Next()    { if (IsEmpty()) return garbage;
                               if (++current_ >= count_) current_ = count_-1;
                               return theList_[current_]->item_; }

  const ItemType & Prev()    { if (IsEmpty()) return garbage;
                               if (current_ != 0) --current_;
                               return theList_[current_]->item_; }

  const ItemType & operator [] (unsigned long i)
  { if (IsEmpty()) return garbage;
    if (i >= count_) i = count_-1;
    current_ = i;
    return theList_[current_]->item_; }

  /*
  void Isort()
  { unsigned long i = 0, j;
    while ((j = ++i) < count_)
    { JXH_ListItem * x = theList_[j];
      while (j-- && (*x < *theList_[j]))
        theList_[j+1] = theList_[j];
      theList_[j+1] = x; } }
  */

protected:
  // = Protected Interfaces
  //

  void Dump()
  { for (unsigned long i = 0; i < count_; i++)
      cerr << "[" << i << "] "
           << theList_[i]->item_ << endl; }

public:
  // = Private Data
  //

  class JXH_ListItem
  {
  public:
    JXH_ListItem(const ItemType & item) : item_(item) {};
    const ItemType item_;
  };

private:
  unsigned long count_;
  unsigned long current_;

  unsigned long size_;
  JXH_ListItem **theList_;

  ItemType garbage;
};

#endif // JXH_List_H
