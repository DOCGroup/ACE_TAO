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
public:
  friend class TAO_Marshal_Sequence;
  // We give access to TAO_Marshal_Sequence, this allows a safe yet
  // small footprint implementation of the marshal engine.

  virtual ~TAO_Base_Sequence (void);
  // destructor.

  CORBA::ULong maximum (void) const;
  // return the maximum length of the sequence

  virtual void _allocate_buffer (CORBA::ULong length) = 0;
  // Ensure that the buffer contains space for at least <length>
  // elements.  The constructor must be called for any new elements,
  // the old ones (if any) must be copied into the buffer using
  // operator= and then their destructors must be called.
  // Finally the old buffer must be released.

  virtual void _deallocate_buffer (void) = 0;
  // Must deallocate the buffer and then set it to zero.

  virtual void _shrink_buffer (CORBA::ULong new_length,
			       CORBA::ULong old_length);
  // Some sequences (of objects and strings) require some cleanup if
  // the sequence is shrunk. The spec requires the destructor to
  // release the objects only from position <0> to <length-1>; so
  // shrink and then delete could result in a memory leak.

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
  // = SPEC {16.11.2}
  // For an unbounded sequence, setting the length to a larger value
  // than the current length may reallocate the sequence
  // data. Reallocation is conceptually equivalent to creating a new
  // sequence of the desired new length, copying the old sequence
  // elements zero through length into the new sequence, and then
  // assigning the old sequence to be the same as the new sequence.

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
  //   T::_duplicate(), and released using CORBA::string_free()
  //   instead of CORBA::release()
  //
  //   This class implements the generic string manager and is used to
  //   instantiate the proper sequence types.
  //
public:
  friend class TAO_Unbounded_String_Sequence;

  // @@ Giving friendship to a template is not implemented on several
  // compilers:
  // friend template<CORBA::ULong MAX>
  //   class TAO_Bounded_String_Sequence<TAO_String_Manager,MAX>;

  
  TAO_String_Manager (const TAO_String_Manager &);
  // copy constructor

  TAO_String_Manager (char* *buffer, CORBA::Boolean release);
  // constructor from address of an element

  ~TAO_String_Manager (void);
  // destructor

  TAO_String_Manager &operator= (const TAO_String_Manager&);
  // assignment from another managed type

  TAO_String_Manager &operator= (const char *);
  // assignment from a constant char*

  operator const char*() const;
  // cast  (read-only)

private:
  char* *ptr_;
  // address of string element from the parent's buffer

  CORBA::Boolean release_;
  // control memory managment semantics.
};

class TAO_Unbounded_String_Sequence : public TAO_Unbounded_Base_Sequence
{
  // =TITLE
  //  Parametric sequence for types that require managers.
  //
  // =DESCRIPTION
  //  Some IDL types require that sequences on them have a "manager"
  //  class, in charge of handling the object lifetime, examples are
  //  pseudo objects, object references and strings.

  // = SPEC
  // 16.8 Mapping for Structured Types 
  // The mapping for struct, union, and sequence (but not array) is a
  // C++ struct or class with a default constructor, a copy
  // constructor, an assignment operator, and a destructor.
  //
public:
  typedef TAO_String_Manager Manager;

  // =operations for the Unbounded_ObjectSequence

  TAO_Unbounded_String_Sequence (void);
  // {orbos/97-05-15:16.8}
  // The default constructor initializes object reference members to
  // appropriately typed nil object references and string members to
  // NULL; all other members are initialized via their default
  // constructors.
  //
  // {orbos/97-05-15:16.11}
  // For both bounded and unbounded sequences, the default constructor
  // (as shown in the example above) sets the sequence length equal to
  // 0.

  TAO_Unbounded_String_Sequence (CORBA::ULong maximum);
  // Unbounded sequences provide a constructor that allows only the
  // initial value of the maximum length to be set (the ``maximum
  // constructor'' shown in the example above). This allows
  // applications to control how much buffer space is initially
  // allocated by the sequence. This constructor also sets the length
  // to 0 and the release flag to TRUE. 

  TAO_Unbounded_String_Sequence (CORBA::ULong maximum,
				 CORBA::ULong length,
				 char* *data,
				 CORBA::Boolean release=0);
  // The ``T *data'' constructor (as shown in the example above)
  // allows the length and contents of a bounded or unbounded sequence
  // to be set. For unbounded sequences, it also allows the initial
  // value of the maximum length to be set. For this constructor,
  // ownership of the contents vector is determined by the release
  // parameter---FALSE means the caller owns the storage, while TRUE
  // means that the sequence assumes ownership of the storage. 
  // If release is TRUE, the contents vector must have been allocated
  // using the sequence allocbuf function, and the sequence will pass
  // it to freebuf when finished with it. 

  TAO_Unbounded_String_Sequence(const TAO_Unbounded_String_Sequence&);
  // The copy constructor performs a deep copy from the existing
  // structure to create a new structure, including calling _duplicate
  // on all object  reference members and performing the necessary
  // heap allocations for all string members.
  //
  // The copy constructor creates a new sequence with the same maximum
  // and length as the given sequence, copies each of its current
  // elements (items zero through length-1), and sets the release
  // flag to TRUE. 

  ~TAO_Unbounded_String_Sequence (void);
  // The destructor releases all object reference memebrs and frees
  // all string members.

  TAO_Unbounded_String_Sequence &operator= (const TAO_Unbounded_String_Sequence &);
  // The assignment operator first releases all object reference
  // members and frees all string  members, and then performs a
  // deep­copy to create a new structure. 
  //
  // The assignment operator deep­copies its parameter, releasing
  // old storage if necessary. It behaves as if the original sequence
  // is destroyed via its destructor and then the source sequence
  // copied using the copy constructor. If release=TRUE, the
  // destructor destroys each of the current elements (items zero
  // through length--1). 
  // For an unbounded sequence, if a reallocation is necessary due to
  // a change in the length and the sequence was created using the
  // release=TRUE parameter in its constructor, the sequence will
  // deallocate the old storage. If release is FALSE under these
  // circumstances, old storage will not be freed before the
  // reallocation is performed. After reallocation, the release flag
  // is always set to TRUE.

  Manager operator[] (CORBA::ULong index) const;
  // read-write accessor

  static char* *allocbuf (CORBA::ULong);
  // The allocbuf function allocates a vector of T elements that can
  // be passed to the T *data constructor. The length of the vector is
  // given by the nelems function argument.  The allocbuf function
  // initializes each element using its default constructor, except
  // for strings, which are initialized to null pointers, and object
  // references, which are initialized to suitably typed nil object
  // references. A null pointer is returned if allocbuf for some
  // reason cannot allocate the requested vector. Vectors allocated by
  // allocbuf should be freed using the freebuf function. 

  static void freebuf (char* *);
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
			       CORBA::ULong old_length);
};


#if defined (__ACE_INLINE__)
#include "tao/sequence.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_SEQUENCE_H */
