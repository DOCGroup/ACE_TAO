/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    TAO
// 
// = FILENAME
//    sequence.h
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems Inc.
//
//    Aniruddha Gokhale
// 
// ============================================================================

#if !defined (TAO_SEQUENCE_H)
#  define TAO_SEQUENCE_H

#if 0
template <class T>
struct CORBA_SEQUENCE 
{
  // = TITLE
  //    Utility template class.
  //
  // = DESCRIPTION
#if SIZEOF_LONG == 4
  u_long maximum;
  u_long length;
#else
  // = Just assume "int" is four bytes long ...
  u_int maximum;
  u_int length;
#endif /* SIZEOF_LONG */
  T *buffer;

  CORBA_SEQUENCE (void)
    : maximum (0), length (0), buffer (0) { }

  // XXX destructor should free buffer, elements!!
  ~CORBA_SEQUENCE (void) { }
};

typedef CORBA_SEQUENCE <CORBA_Octet> CORBA_OctetSeq;
#endif

// unbounded sequences
template <class T>
class TAO_UnboundedSeq
{
  // =TITLE
  //  TAO_UnboundedSeq
  // =DESCRIPTION
  //  parametrized type for unbounded sequences
public:

  typedef T ElemType;

  // =operations

  TAO_UnboundedSeq (void);
  // default constructor

  TAO_UnboundedSeq (CORBA::ULong max);
  // constructor using a maximum length value

  TAO_UnboundedSeq (CORBA::ULong max, CORBA::ULong length, T *data,
                    CORBA::Boolean release=0);
  // constructor using the data and memory management flag

  TAO_UnboundedSeq (const TAO_UnboundedSeq<T> &);
  // copy constructor

  ~TAO_UnboundedSeq (void);
  // destructor

  TAO_UnboundedSeq<T> &operator= (const TAO_UnboundedSeq<T> &);
  // assignment operator

  CORBA::ULong maximum (void) const;
  // return the max length of the sequence

  void length (CORBA::ULong);
  // set the length

  CORBA::ULong length (void) const;
  // return the current length

  T &operator[] (CORBA::ULong);
  // operator []

  const T &operator[] (CORBA::ULong) const;
  // operator []

  // =static operations

  static T *allocbuf (CORBA::ULong);
  // allocate storage for the sequence

  static void freebuf (T *);
  // free the sequence

private:
  CORBA::ULong maximum_;
  // maximum length

  CORBA::ULong length_;
  // length

  CORBA::Boolean release_;
  // memory management

  T *buffer_;
  // buffer
};

// bounded sequences
template <class T, CORBA::ULong size>
class TAO_BoundedSeq
{
  // =TITLE
  //  TAO_BoundedSeq
  // =DESCRIPTION
  //  parametrized type for Bounded sequences
public:

  typedef T ElemType;

  // =operations

  TAO_BoundedSeq (void);
  // default constructor

  TAO_BoundedSeq (CORBA::ULong length, T *data,
                    CORBA::Boolean release=0);
  // constructor using the data and memory management flag

  TAO_BoundedSeq (const TAO_BoundedSeq<T,size> &);
  // copy constructor

  ~TAO_BoundedSeq (void);
  // destructor

  TAO_BoundedSeq<T,size> &operator= (const TAO_BoundedSeq<T,size> &);
  // assignment operator

  CORBA::ULong maximum (void) const;
  // return the max length of the sequence

  void length (CORBA::ULong);
  // set the length

  CORBA::ULong length (void) const;
  // return the current length

  T &operator[] (CORBA::ULong);
  // operator []

  const T &operator[] (CORBA::ULong) const;
  // operator []

  // =static operations

  static T *allocbuf (CORBA::ULong);
  // allocate storage for the sequence

  static void freebuf (T *);
  // free the sequence

private:
  CORBA::ULong length_;
  // length

  CORBA::Boolean release_;
  // memory management

  T *buffer_;
  // buffer
};

#endif /* TAO_SEQUENCE_H */
