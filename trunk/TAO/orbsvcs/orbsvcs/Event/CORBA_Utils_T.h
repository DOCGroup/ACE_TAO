/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    ace_orb
// 
// = FILENAME
//    CORBA_Utilities.h
//
// = AUTHORS
//    Tim Harrison.
//
// = NOTE
//   This class is a bit ackward when using a real ORB, but we cannot
//   easily remove it since some of the types are used in the
//   implementation of the EC. TODO
// 
// ============================================================================

#if !defined (ACE_CORBA_UTILS_H)
#define ACE_CORBA_UTILS_H

#include "ace/Time_Value.h"
#include "tao/corba.h"

template <class TYPE>
class ACE_CORBA_Sequence// : public CORBA::Object
// = TITLE
//
// = DESCRIPTION
{
public:

  ACE_CORBA_Sequence (const ACE_CORBA_Sequence<TYPE> &source);
  // Copy construction.

  ACE_CORBA_Sequence (CORBA::ULong max);
  ACE_CORBA_Sequence (CORBA::ULong max, 
		      CORBA::ULong length, 
		      TYPE* data, 
		      CORBA::Boolean release = 0);
  ACE_CORBA_Sequence (void);

  virtual ~ACE_CORBA_Sequence (void);

  ACE_CORBA_Sequence<TYPE> &operator= (const ACE_CORBA_Sequence<TYPE> &);

  // static TYPE* allocbuf(CORBA::ULong nelems);
  // static void freebuf(TYPE* data);

  CORBA::ULong maximum (void) const;
  CORBA::ULong length (void) const;
  void length (CORBA::ULong len);

  TYPE& operator [] (CORBA::ULong IT_i);

  const TYPE& operator [] (CORBA::ULong IT_i) const;

  virtual TYPE *new_array (size_t len);
  // Allocates TYPE[len].  This facilitates template methods through
  // template specialization to allow the use of different memory
  // pools.

  virtual void delete_array (TYPE *buf, size_t len);
  // delete [] <buf>.

protected:
  CORBA::ULong maximum_;
  CORBA::ULong length_;
  TYPE* buffer_;
  unsigned char release_;
};

// Utility for debugging sequences.
//template <class TYPE>
//void dump (const ACE_CORBA_Sequence<TYPE> &seq);

// Utility for appending single items.  Increases the length of <set>
// and adds <event> to the end of <set>.
template <class TYPE> void
operator += (ACE_CORBA_Sequence<TYPE> &seq,
	     const TYPE &item)
{
  CORBA::ULong length = seq.length ();
  seq.length (length + 1);
  seq[length] = item;
}

// Utility for appending sequences.
template <class TYPE> void
operator += (ACE_CORBA_Sequence<TYPE> &dest, 
	     const ACE_CORBA_Sequence<TYPE> &source);

template <class TYPE> void
remove_item (TYPE &seq, CORBA::ULong index)
{
  int new_length = seq.length () - 1;

  // Shift the set back one, starting at <index>.
  for (int x = index; x < new_length; x++)
    seq[x] = seq[x+1];

  // Set the new length.
  seq.length (new_length);
}
// Removes seq[index] by moving everything after <index> back and
// decrementing the length.

// ************************************************************

template <class TYPE>
class ACE_CORBA_var
// = TITLE
//
// = DESCRIPTION
{
public:
  ACE_CORBA_var (const ACE_CORBA_var<TYPE> &source);
  ACE_CORBA_var (void);
  ACE_CORBA_var (TYPE *source);
  ~ACE_CORBA_var (void);
  ACE_CORBA_var<TYPE> &operator= (TYPE *source);
  ACE_CORBA_var<TYPE> &operator= (const ACE_CORBA_var<TYPE> &source);
  TYPE *operator->(void);
  operator TYPE *(void) const;
  //  operator TYPE *&(void);
  operator TYPE &(void) const;
private:
  TYPE *me_;
};

// ************************************************************

/*
template <class IMPL>
class ACE_CORBA_Object_Ref : public CORBA::Object
// = TITLE
//    ACE CORBA Object Reference
//
// = DESCRIPTION
//    A wrapper for making CORBA Object References.  For a single
//    address space ORB, this points directly to the IMPL
//    class.  This is supposed to look like a pointer to
//    IMPL with an additional static _duplicate method.
{
public:
  // ACE_CORBA_Object_Ref (ACE_CORBA_Object_Ref<IMPL> &obj);
  ACE_CORBA_Object_Ref (void);
  ACE_CORBA_Object_Ref (IMPL *impl);
  ~ACE_CORBA_Object_Ref (void);
  //  ACE_CORBA_Object_Ref<IMPL> &operator= (IMPL *source);
  ACE_CORBA_Object_Ref<IMPL> &operator= 
    (const ACE_CORBA_Object_Ref<IMPL> &source);
  
  IMPL *operator->(void);
  // A distributed ORB would require that the CORBA Object Reference
  // implement all IMPL interfaces, marshall the parameters,
  // and transmit over IIOP.  For this single address space ORB, we'll
  // just delegate method calls to the IMPL directly.

  operator IMPL *(void) const;
  // operator IMPL *&(void);
  // operator IMPL &(void) const;

  static ACE_CORBA_Object_Ref<IMPL> *_duplicate 
    (ACE_CORBA_Object_Ref<IMPL> *source);

private:
  IMPL *impl_;
};
*/

// ************************************************************

template <class ITEM, size_t SIZE>
class ACE_ES_Simple_Array
// = TITLE
//    Simple Array
//
// = DESCRIPTION
//    Wraps ITEM[SIZE] with insert and an iterator.  There is no
//    remove.  It allows duplicates.  It is truly very simple.
{
 public:
  ACE_ES_Simple_Array (void);
  int insert (const ITEM &copy_me);
  size_t size (void);
  ITEM *data (void);
 protected:
  ITEM data_[SIZE];
  size_t size_;
};

template <class ITEM>
class ACE_ES_Array_Iterator
// = TITLE
//    Simple Iterator
//
// = DESCRIPTION
//    Iterates through an array of ITEMs.
{
public:
  ACE_ES_Array_Iterator (ITEM *data, size_t size);

  int next (ITEM *&next_item);
  // Returns 0 when all items have been seen, else 1.  Sets
  // <next_item> to point at the next ITEM.

  int advance (void);
  // Move forward by one element in the Stack.  Returns 0 when all the
  // items in the Stack have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

private:
  ITEM *data_;
  size_t size_;
  size_t index_;
};

#if defined (__ACE_INLINE__)
#include "CORBA_Utils_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "CORBA_Utils_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("CORBA_Utils_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_CORBA_UTILS_H */
