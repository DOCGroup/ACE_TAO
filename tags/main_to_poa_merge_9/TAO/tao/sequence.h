/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    sequence.h
//
// = AUTHOR
//
//    Carlos O'Ryan and Aniruddha Gokhale
//
// ============================================================================

#if !defined (TAO_SEQUENCE_H)
#  define TAO_SEQUENCE_H

class TAO_Export TAO_Base_Sequence
{
  // = TITLE
  //   Base class for TAO sequences.
  //
  // = DESCRIPTION
  //   This class provides a common interface for all IDL sequences,
  //   hence the interpreted marshal engine can manipulate them in a
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

  // void length (CORBA::ULong length);
  // set the length

  virtual void _allocate_buffer (CORBA::ULong length) = 0;
  // Ensure that the buffer contains space for at least <length>
  // elements.  The constructor must be called for any new elements,
  // the old ones (if any) must be copied into the buffer using
  // operator= and then their destructors must be called.
  // Finally the old buffer must be released.

  virtual void _deallocate_buffer (void) = 0;
  // Must deallocate the buffer and then set it to zero.

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

class TAO_Export TAO_Unbounded_Base_Sequence : public TAO_Base_Sequence
{
  // = TITLE
  //   Base class for all bounded sequences.
  //
  // = DESCRIPTION
  //   This class implements part of the funcionality common to all
  //   bounded sequences, using this intermediate class instead of
  //   virtual methods on TAO_Base_Sequence give us a slight
  //   improvement of performance, but also reduces the amount of
  //   generated code in the templates.
public:
  void length (CORBA::ULong length);
  // set the length, for this sequences this call increases the
  // capacity on demand.

  CORBA::ULong length (void) const;
  // return the current length, it cannot go into the base class due
  // to the C++ name lookup rules (if you don't know what I'm talking
  // about, then try moving it there).

protected:
  TAO_Unbounded_Base_Sequence (void);
  // Default constructor.

  TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
			       CORBA::ULong length,
			       void* buffer,
			       CORBA::Boolean release = 0);
  // Constructor with control of ownership.

  TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
			       void* buffer);
  // Assume ownership and set length to 0.
};

class TAO_Export TAO_Bounded_Base_Sequence : public TAO_Base_Sequence
{
  // = TITLE
  //   Base class for all bounded sequences.
  //
  // = DESCRIPTION
  //   This class implements part of the funcionality common to all
  //   bounded sequences, using this intermediate class instead of
  //   virtual methods on TAO_Base_Sequence give us a slight
  //   improvement of performance, but also reduces the amount of
  //   generated code in the templates.
public:
  void length (CORBA::ULong length);
  // set the length, for this sequences this call is ignored if the
  // new length is greater that the maximum.

  CORBA::ULong length (void) const;
  // return the current length, it cannot go into the base class due
  // to the C++ name lookup rules (if you don't know what I'm talking
  // about, then try moving it there).

protected:
  TAO_Bounded_Base_Sequence (void);
  // Default constructor.

  TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
		     CORBA::ULong length,
		     void* buffer,
		     CORBA::Boolean release = 0);
  // Constructor with control of ownership.

  TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
		     void* buffer);
  // Assume ownership and set length to 0.
};

/****************************************************************/

template<class T, class Manager> class TAO_Unbounded_Managed_Sequence;
template<class T, class Manager, CORBA::ULong MAX> class TAO_Bounded_Managed_Sequence;

class TAO_Export TAO_String_Manager
{
  // = TITLE
  //   Manager for strings.
  //
  // = DESCRIPTION
  //   Similar to the mapping for sequences of objects (and other
  //   pseudo objects) the mapping for sequences of strings 
  //   requires an auxiliar class or <Manager> to handle the
  //   allocation and deallocation of the string.
  //   The main difference with respect to String_var classes is that
  //   automatic release is not controlled on a per-item basis, but
  //   for the sequence as a whole.
  //   The difference wrt Object_Manager is that strings are
  //   duplicated using CORBA::string_copy() as opposed to
  //   T::_duplicate().
  //
  //   This class implements the generic string manager and is used to
  //   instantiate the proper sequence types.
  //
  // = NOTES
  //   It has been proposed that the class should be parametric on
  //   both T and T_ptr, IMHO this is no necesary: though the IDL spec
  //   says that T_ptr *could* map to a type different to T* in the
  //   particular case of TAO it does map to <T*>.
  //
public:
  friend class TAO_Unbounded_Managed_Sequence<char*,TAO_String_Manager>;

  // @@ Use partial template specialization here to give access only
  // to the right kind of sequence.
  // friend template<CORBA::ULong MAX>
  // class TAO_Bounded_Managed_Sequence<char*,TAO_String_Manager,MAX>;

  
  TAO_String_Manager (const TAO_String_Manager &);
  // copy constructor

  TAO_String_Manager (char **buffer, CORBA::Boolean release);
  // constructor from address of an element

  ~TAO_String_Manager (void);
  // destructor

  TAO_String_Manager &operator= (const TAO_String_Manager&);
  // assignment from another managed type

  TAO_String_Manager &operator= (char *);
  // assignment from a char*

  TAO_String_Manager &operator= (const char *);
  // assignment from a constant char*

  operator const char*() const;
  // cast  (read-only)

  operator char *();
  // cast

  char &operator[] (CORBA::ULong index);
  // accessor

  const char* &operator[] (CORBA::ULong index) const;
  // read-only accessor

  // in, inout, out, _retn
  const char *in (void) const;
  // returns the underlying string

  char *&inout (void);
  // returns a reference to the underlying string for in arguments

  char *&out (void);
  // used for out arguments

  char *_retn (void);
  // gives up ownership, used for return types.

private:
  char **ptr_;
  // address of string element from the parent's buffer

  CORBA::Boolean release_;
  // based on parent's release flag
};

#endif /* TAO_SEQUENCE_H */
