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

class TAO_Base_Sequence
{
  // = TITLE
  //   Base class for TAO sequences.
  //
  // = DESCRIPTION
  //   This class provides a common interface for all IDL sequences,
  //   hence the interpreted marshall engine can manipulate them in a
  //   type safe manner.
  //
public:
  friend class TAO_Marshal_Sequence;
  // We give access to TAO_Marshal_Sequence, this allows a safe yet
  // small footprint implementation of the marshal engine.

  virtual ~TAO_Base_Sequence (void);
  // destructor.

  CORBA::ULong maximum (void) const;
  // return the maximum length of the sequence

  void length (CORBA::ULong length);
  // set the length

  CORBA::ULong length (void) const;
  // return the current length

  virtual void _allocate_buffer (CORBA::ULong length) = 0;
  // Ensure that the buffer contains space for at leat <length>
  // elements.  The constructor must be called for any new elements,
  // the old ones (if any) must be copied into the buffer using
  // operator= and then their destructors must be called.
  // Finally the old buffer must be released.

  virtual void _deallocate_buffer (void) = 0;
  // Releases the buffer and call the destructor for all the elements
  // in it; remember that there are <maximum> elements in the buffer.

  virtual int _bounded (void) const = 0;
  // Retuns 1 if the sequence is bounded (hence it cannot be resized)
  // and 0 otherwise.

protected:
  TAO_Base_Sequence (void);
  // Default constructor.

  TAO_Base_Sequence (CORBA::ULong maximum,
		     CORBA::ULong length,
		     void* buffer,
		     CORBA::Boolean release = 0);
  // Constructor with control of ownership.

  TAO_Base_Sequence (CORBA::ULong maximum,
		     void* buffer);
  // Assume ownership and set length to 0.

  TAO_Base_Sequence (const TAO_Base_Sequence& rhs);
  TAO_Base_Sequence& operator=(const TAO_Base_Sequence& rhs);
  // Copy constructor and assignment operator are protected, the
  // derived classes must provided the right semantics for the buffer
  // copy, only the static fields are actually copy.

protected:
  CORBA::ULong maximum_;
  // The maximum number of elements the buffer can contain.

  CORBA::ULong length_;
  // The current number of elements in the buffer.

  void* buffer_;
  // The buffer with all the elements, casting must be done in derived
  // classes.

  CORBA::Boolean release_;
  // If true then the sequence should release the buffer when it is
  // destroyed.
};

#endif /* TAO_SEQUENCE_H */
