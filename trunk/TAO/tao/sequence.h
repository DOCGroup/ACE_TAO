// This may look like C, but it's really -*- C++ -*-

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
// ============================================================================

#if !defined (TAO_SEQUENCE_H)
#  define TAO_SEQUENCE_H

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

#if 0
typedef CORBA_SEQUENCE <CORBA_Octet> CORBA_OctetSeq;
#endif

#endif /* TAO_SEQUENCE_H */
