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

#ifndef TAO_SEQUENCE_H
#define TAO_SEQUENCE_H

#include "tao/corbafwd.h"
#include "tao/Managed_Types.h"

class CORBA_Object;
class CORBA_Environment;

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
  // operator= and then their destructors must be called.  Finally the
  // old buffer must be released.

  virtual void _deallocate_buffer (void) = 0;
  // Must deallocate the buffer and then set it to zero.

  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
  // Some sequences (of objects and strings) require some cleanup if
  // the sequence is shrunk. The spec requires the destructor to
  // release the objects only from position <0> to <length-1>; so
  // shrink and then delete could result in a memory leak.

  virtual void _downcast (void *target,
                          CORBA_Object *src,
                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  // Used for sequences of objects to downcast a recently demarshalled
  // object reference into the right type.

  virtual CORBA_Object *_upcast (void *src) const;
  // Used for sequences of object to convert from the derived type
  // into the Object class.

protected:
  TAO_Base_Sequence (void);
  // Default constructor.

  TAO_Base_Sequence (CORBA::ULong maximum,
                     CORBA::ULong length,
                     void *buffer,
                     CORBA::Boolean release = 0);
  // Constructor with control of ownership.

  TAO_Base_Sequence (CORBA::ULong maximum,
                     void *buffer);
  // Assume ownership and set length to 0.

  // = orbos/98-01-11 proposed extensions.
  CORBA::Boolean release (void) const;
  // Returns the state of the sequence release flag.

public:

  // = The following two functions should be protected but we made it
  // public because it breaks some compilers.

  TAO_Base_Sequence (const TAO_Base_Sequence &rhs);
  TAO_Base_Sequence &operator= (const TAO_Base_Sequence &rhs);
  // Copy constructor and assignment operator are protected, the
  // derived classes must provided the right semantics for the buffer
  // copy, only the static fields are actually copy.

protected:

  CORBA::ULong maximum_;
  // The maximum number of elements the buffer can contain.

  CORBA::ULong length_;
  // The current number of elements in the buffer.

  void *buffer_;
  // The buffer with all the elements, casting must be done in derived
  // classes.

  CORBA::Boolean release_;
  // If true then the sequence should release the buffer when it is
  // destroyed.
};

// ****************************************************************

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

  virtual ~TAO_Unbounded_Base_Sequence (void);
  // destructor.

protected:
  TAO_Unbounded_Base_Sequence (void);
  // Default constructor.

  TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
                               CORBA::ULong length,
                               void *buffer,
                               CORBA::Boolean release = 0);
  // Constructor with control of ownership.

  TAO_Unbounded_Base_Sequence (CORBA::ULong maximum,
                               void *buffer);
  // Assume ownership and set length to 0.
};

// ****************************************************************

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

  virtual ~TAO_Bounded_Base_Sequence (void);
  // destructor.

protected:
  TAO_Bounded_Base_Sequence (void);
  // Default constructor.

  TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
                             CORBA::ULong length,
                             void *buffer,
                             CORBA::Boolean release = 0);
  // Constructor with control of ownership.

  TAO_Bounded_Base_Sequence (CORBA::ULong maximum,
                             void *buffer);
  // Assume ownership and set length to 0.
};

// ****************************************************************

class TAO_Export TAO_Unbounded_String_Sequence : public TAO_Unbounded_Base_Sequence
{
  // =TITLE
  //  Unbounded sequence of strings.
  //
  // =DESCRIPTION
  //  IDL sequences of strings must automatically duplicate and
  //  release their members based on some global <release> flag.

  // = SPEC
  // 16.8 Mapping for Structured Types
  // The mapping for struct, union, and sequence (but not array) is a
  // C++ struct or class with a default constructor, a copy
  // constructor, an assignment operator, and a destructor.
  //
public:
  // = Operations for the Unbounded_ObjectSequence

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
                                 char **data,
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
  // The destructor releases all object reference members and frees
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

  TAO_SeqElem_String_Manager operator[] (CORBA::ULong index) const;
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

  static void freebuf (char **);
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // = Fast buffer accessors.
  char **get_buffer (CORBA::Boolean orphan = 0);
  const char **get_buffer (void) const;

  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// ****************************************************************

#if defined (TAO_NO_COPY_OCTET_SEQUENCES)
class ACE_Message_Block;
template<class T> class TAO_Unbounded_Sequence;
// forward declaration, we are going to specialize that template
// here.
// The template itself requires this file so every user of the
// template should also see the specialization.

class TAO_Export TAO_Unbounded_Sequence<CORBA::Octet> : public TAO_Unbounded_Base_Sequence
{
  // = TITLE
  //   An unbounded sequence of Octets
  //
  // = DESCRIPTION
  //   Marshalling and demarshalling octet sequences can be highly
  //   optimize, for instance at demarshalling we don't require a copy
  //   from the CDR buffer to the octet sequence buffer, we can simply
  //   hold a duplicate of the underlying ACE_Message_Block.
  //   Specializing the TAO_Unbounded_Sequence<T> parametric
  //   class, is an excellent way to achieve this optimizations.
  //
public:
  friend class TAO_Marshal_Sequence;
  // For efficient marshalling and demarshalling.

  TAO_Unbounded_Sequence (void);
  TAO_Unbounded_Sequence (CORBA::ULong max);
  TAO_Unbounded_Sequence (CORBA::ULong max,
                          CORBA::ULong length,
                          CORBA::Octet *data,
                          CORBA::Boolean release = 0);
  ~TAO_Unbounded_Sequence (void);
  // see TAO_Unbounded_Sequence in "Sequence_T.h"

  TAO_Unbounded_Sequence (const TAO_Unbounded_Sequence<CORBA::Octet> &);
  TAO_Unbounded_Sequence<CORBA::Octet>& operator= (const TAO_Unbounded_Sequence<CORBA::Octet> &);
  // The copy constructor and assignment operators *do* copy the data,
  // though we could simply duplicate the ref count in the
  // ACE_Message_Block this will change the semantics for this
  // operations.

  CORBA::Octet &operator[] (CORBA::ULong);
  const CORBA::Octet &operator[] (CORBA::ULong) const;
  // See the general description in "Sequence_T.h"
  // NOTE: This last two methods can be rendered useless in certain
  // cases, see below.

  // = Static operations.

  static CORBA::Octet *allocbuf (CORBA::ULong);
  // Allocate storage for the sequence, please note that the storage
  // is always held in a ACE_Message_Block.

  static void freebuf (CORBA::Octet *);
  // Free the storage.

  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  // Implement the methods for all the sequence, please seee
  // TAO_Base_Sequence.

  // = Fast buffer accessors.
  CORBA::Octet *get_buffer (CORBA::Boolean orphan = 0);
  const CORBA::Octet *get_buffer (void) const;

  // NOTE: This last two methods can be rendered useless in certain
  // cases, see below.
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                CORBA::Octet *data,
                CORBA::Boolean release = 0);
  // See the general description of this methods in "Sequence_T.h".

  // = TAO extensions

  ACE_Message_Block* mb (void) const;
  // Returns the underlying message block, the caller must *not*
  // release the copy.

  TAO_Unbounded_Sequence (CORBA::ULong length,
                          const ACE_Message_Block* mb);
  // Create a sequence of octets from a single message block (i.e. it
  // ignores any chaining in the meesage block).

  void replace (CORBA::ULong length, const ACE_Message_Block* mb);
  // Replaces the current buffer with <mb>, using only <length> bytes.
  // It takes a duplicate of <mb> so the user still owns it.

  //
  // NOTE:
  // In the last two methods if the <mb> is the head of a chain then
  // the following methods are not warranteed to work properly:
  //    operator[]
  //    get_buffer ()
  // the main purpose of this method is to support custom marshaling;
  // so the complete chain is marshaled when the octet sequence is.
  //

private:
  ACE_Message_Block* mb_;
};

#endif /* defined (TAO_NO_COPY_OCTET_SEQUENCES) */

// ****************************************************************

// Comparison of octet sequence.
extern TAO_Export int operator== (const TAO_Unbounded_Sequence<CORBA::Octet> &l,
                                  const TAO_Unbounded_Sequence<CORBA::Octet> &r);

extern TAO_Export int operator!= (const TAO_Unbounded_Sequence<CORBA::Octet> &l,
                                  const TAO_Unbounded_Sequence<CORBA::Octet> &r);

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "tao/Sequence.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_SEQUENCE_H */
