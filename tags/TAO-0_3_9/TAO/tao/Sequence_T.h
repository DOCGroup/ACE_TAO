/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    sequence_T.h
//
// = AUTHOR
//    Carlos O'Ryan and Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_SEQUENCE_T_H
#define TAO_SEQUENCE_T_H

template <class T>
class TAO_Unbounded_Sequence : public TAO_Unbounded_Base_Sequence
{
  // = TITLE
  //   Parametrized implementation of IDL unbounded sequences.
  //
  // = DESCRIPTION
  //   This class completes the implementation for TAO_Base_Sequence
  //   for the unbounded case.
public:
  // = Initialization and termination methods.

  TAO_Unbounded_Sequence (void);
  // Default constructor.

  TAO_Unbounded_Sequence (CORBA::ULong max);
  // Constructor using a maximum length value.

  TAO_Unbounded_Sequence (CORBA::ULong max,
                          CORBA::ULong length,
                          T *data,
                          CORBA::Boolean release = 0);
  // Constructor using the data and memory management flag.  Memory
  // *must* be allocated using the ::allocbuf static method, since in
  // the future this classes may use a global ACE_Allocator.

  TAO_Unbounded_Sequence (const TAO_Unbounded_Sequence<T> &);
  // Copy constructor.

  TAO_Unbounded_Sequence<T> &operator= (const TAO_Unbounded_Sequence<T> &);
  // Assignment operator.

  ~TAO_Unbounded_Sequence (void);
  // Dtor.

  // = Accessors.

  T &operator[] (CORBA::ULong);
  // operator []

  const T &operator[] (CORBA::ULong) const;
  // operator []

  // = Static operations.

  static T *allocbuf (CORBA::ULong);
  // Allocate storage for the sequence.

  static void freebuf (T *);
  // Free the sequence.

  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  // Implement the TAO_Base_Sequence methods (see Sequence.h)

  // = orbos/98-01-11 proposed extensions.
  T *get_buffer (CORBA::Boolean orphan = 0);
  // Allows read-write access to the underlying buffer.  If <orphan>
  // is FALSE the sequence returns a pointer to its buffer, allocating
  // one if it has not yet done so.  The number of elements in the
  // buffer can be determined from the sequence <length> accessor.
  //
  // If the <orphan> argument to <get_buffer> is FALSE, the sequence
  // maintains ownership of the underlying buffer.  Elements in the
  // returned buffer may be directly replaced by the caller.  For
  // sequences of strings, wide strings, and object references, the
  // caller must use the sequence <release> accessor to determine
  // whether elements should be freed (using <string_free>,
  // <wstring_free>, or <CORBA::release> for strings, wide straings,
  // and object references, respective) before being directly assigned
  // to.
  //
  // If the <orphan> argument to <get_buffer> is TRUE, the sequence
  // yields ownership of the buffer to the caller.  If <orphan> is
  // TRUE and the sequence does not own its buffer (i.e., its
  // <release> flag is FALSE), the return value is a null pointer.  If
  // the buffer is taken from the sequence using this form of
  // <get_buffer>, the sequence reverts to the same state it would
  // have if constructed using its default constructor.  The caller
  // becomes responsible for eventually freeing each element of the
  // returned buffer (for strings, wide string, and object
  // references), and then freeing the returned buffer itself using
  // <freebuf>.

  const T *get_buffer (void) const;
  // This function allows read-only access to the sequence buffer.
  // The sequence returns its buffer, allocating one of one has not
  // yet been allocated.  No direct modification of the returned
  // buffer by the caller is permitted.

  void replace (CORBA::ULong max,
                CORBA::ULong length,
                T *data,
                CORBA::Boolean release = 0);
  // Allows the buffer underlying a sequence to be replaced.  The
  // parameters to <replace> are identical in type, order, and purpose
  // to those for the <T *data> constructor for the sequence.
};

template <class T, CORBA::ULong MAX>
class TAO_Bounded_Sequence : public TAO_Bounded_Base_Sequence
{
  // = TITLE
  //   Parametric bounded sequence.
  //
  // = DESCRIPTION
  //   This class completes the implementation for TAO_Base_Sequence
  //   for the bounded case.
public:
  // = Initialization and termination methods.
  TAO_Bounded_Sequence (void);
  // Default constructor.

  TAO_Bounded_Sequence (CORBA::ULong length,
                        T *data,
                        CORBA::Boolean release=0);
  // Constructor using the data and memory management flag.

  TAO_Bounded_Sequence (const TAO_Bounded_Sequence<T,MAX> &);
  // Copy constructor.

  TAO_Bounded_Sequence<T,MAX> &operator= (const TAO_Bounded_Sequence<T,MAX> &);
  // Assignment operator.

  ~TAO_Bounded_Sequence (void);
  // Dtor.

  // = Accessors.
  T &operator[] (CORBA::ULong);
  // operator []

  const T &operator[] (CORBA::ULong) const;
  // operator []

  // = Static operations.

  static T *allocbuf (CORBA::ULong);
  // Allocate storage for the sequence.

  static void freebuf (T *);
  // Free the sequence.

  virtual void _allocate_buffer (CORBA::ULong length);
  // allocate a buffer of the requested length. The buffer is allocated for the
  // right type

  virtual void _deallocate_buffer (void);
  // deallocate the buffer

  // = orbos/98-01-11 proposed extensions.
  T *get_buffer (CORBA::Boolean orphan = 0);
  // Allows read-write access to the underlying buffer.  If <orphan>
  // is FALSE the sequence returns a pointer to its buffer, allocating
  // one if it has not yet done so.  The size of the returned buffer
  // is equal to the sequence bound, which can be determined with the
  // <maximum> accessor.  The number of elements in the buffer can be
  // determined from the sequence <length> accessor.
  //
  // If the <orphan> argument to <get_buffer> is FALSE, the sequence
  // maintains ownership of the underlying buffer.  Elements in the
  // returned buffer may be directly replaced by the caller.  For
  // sequences of strings, wide strings, and object references, the
  // caller must use the sequence <release> accessor to determine
  // whether elements should be freed (using <string_free>,
  // <wstring_free>, or <CORBA::release> for strings, wide straings,
  // and object references, respective) before being directly assigned
  // to.
  //
  // If the <orphan> argument to <get_buffer> is TRUE, the sequence
  // yields ownership of the buffer to the caller.  If <orphan> is
  // TRUE and the sequence does not own its buffer (i.e., its
  // <release> flag is FALSE), the return value is a null pointer.  If
  // the buffer is taken from the sequence using this form of
  // <get_buffer>, the sequence reverts to the same state it would
  // have if constructed using its default constructor.  The caller
  // becomes responsible for eventually freeing each element of the
  // returned buffer (for strings, wide string, and object
  // references), and then freeing the returned buffer itself using
  // <freebuf>.

  const T *get_buffer (void) const;
  // This function allows read-only access to the sequence buffer.
  // The sequence returns its buffer, allocating one of one has not
  // yet been allocated.  No direct modification of the returned
  // buffer by the caller is permitted.

  void replace (CORBA::ULong max,
                CORBA::ULong length,
                T *data,
                CORBA::Boolean release = 0);
  // Allows the buffer underlying a sequence to be replaced.  The
  // parameters to <replace> are identical in type, order, and purpose
  // to those for the <T *data> constructor for the sequence.
};

// *************************************************************

template<class T> class TAO_Unbounded_Object_Sequence;
template<class T, CORBA::ULong MAX> class TAO_Bounded_Object_Sequence;
template<CORBA::ULong MAX> class TAO_Bounded_String_Sequence;

// *************************************************************

template<class T>
class TAO_Object_Manager
{
  // = TITLE
  //   Manager for Objects.
  //
  // = DESCRIPTION
  //   The mapping for sequences of objects (and other pseudo objects)
  //   requires an auxiliar class or <Manager> to handle the reference
  //   count on the object, (a kind of smart pointer).
  //   The main difference with respect to T_var classes is that
  //   automatic release is not controlled on a per-item basis, but
  //   for the sequence as a whole.
  //   Further the class must implement "transparent" assignment into
  //   the sequence, for instance, operator[] returns a managed
  //   object, but:
  //
  //      object_sequence[i] = foo;
  //
  //   must change the state of the sequence.
  //
  //   This class implements the generic object manager and is used to
  //   instantiate the proper sequence types.
  //
  // = NOTES
  //   It has been proposed that the class should be parametric on
  //   both T and T_ptr, IMHO this is no necesary: though the IDL spec
  //   says that T_ptr *could* map to a type different to T* in the
  //   particular case of TAO it does map to <T*>.
  //
  friend class TAO_Unbounded_Object_Sequence<T>;
public:
  // @@ Use partial template specialization here to give access only
  // to the right kind of sequence.
  // friend template<CORBA::ULong MAX>
  //    class TAO_Bounded_Object_Sequence<T,MAX>;

  // = Initialization and termination methods.
  TAO_Object_Manager (const TAO_Object_Manager<T> &rhs);
  // Copy constructor, the semantics are non-trivial:
  //   + The referenced element is duplicated or not according to the
  //   release value on the <rhs>.
  //   + In any case a new reference to the same object is created.

  TAO_Object_Manager (T **, CORBA::Boolean release);
  // Constructor from address of an element, it should be private and
  // only TAO_*_Object_Sequence would use it, but we have some
  // problems with friendship and templates.

  ~TAO_Object_Manager (void);
  // Destructor, only releases the object if <release_> is true.

  TAO_Object_Manager<T> &operator= (const TAO_Object_Manager<T> &rhs);
  // Assignment from another managed type, only release if
  // <this->release_> is true.
  // @@ TODO what happens if rhs.release_ is true an this->relase_ is
  // false?

  TAO_Object_Manager<T> &operator= (T *);
  // Assignment from T *.

  operator const T *() const;
  // Cast (read-only).

  operator T *&();
  // Cast.

  const T *in (void) const;
  // for in parameter.

  T *&inout (void);
  // for inout parameter.

  T *&out (void);
  // for out parameter.

  T *_retn (void);
  // for return type

private:
  T **ptr_;
  // data member, notice that it is a pointer, to implement the
  // reference behavior for assignment.

  CORBA::Boolean release_;
  // release flag based on parent's flag
};

// *************************************************************

template<class T>
class TAO_Unbounded_Object_Sequence : public TAO_Unbounded_Base_Sequence
{
  // = TITLE
  //   Parametric sequence for types that require managers.
  //
  // = DESCRIPTION
  //   Some IDL types require that sequences on them have a "manager"
  //   class, in charge of handling the object lifetime, examples are
  //   pseudo objects, object references and strings.

  // = SPEC
  //   16.8 Mapping for Structured Types
  //   The mapping for struct, union, and sequence (but not array) is a
  //   C++ struct or class with a default constructor, a copy
  //   constructor, an assignment operator, and a destructor.
  //
public:
  // = Initialization and termination methods.

  TAO_Unbounded_Object_Sequence (void);
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

  TAO_Unbounded_Object_Sequence (CORBA::ULong max);
  // Unbounded sequences provide a constructor that allows only the
  // initial value of the maximum length to be set (the ``maximum
  // constructor'' shown in the example above). This allows
  // applications to control how much buffer space is initially
  // allocated by the sequence. This constructor also sets the length
  // to 0 and the release flag to TRUE.

  TAO_Unbounded_Object_Sequence (CORBA::ULong maximum,
                                 CORBA::ULong length,
                                 T* *data,
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

  TAO_Unbounded_Object_Sequence(const TAO_Unbounded_Object_Sequence<T> &);
  // The copy constructor performs a deep copy from the existing
  // structure to create a new structure, including calling _duplicate
  // on all object  reference members and performing the necessary
  // heap allocations for all string members.
  //
  // The copy constructor creates a new sequence with the same maximum
  // and length as the given sequence, copies each of its current
  // elements (items zero through length-1), and sets the release
  // flag to TRUE.

  ~TAO_Unbounded_Object_Sequence (void);
  // The destructor releases all object reference memebrs and frees
  // all string members.

  TAO_Unbounded_Object_Sequence<T> &operator= (const TAO_Unbounded_Object_Sequence <T> &);
  // The assignment operator first releases all object reference
  // members and frees all string members, and then performs a
  // deepcopy to create a new structure.
  //
  // The assignment operator deepcopies its parameter, releasing old
  // storage if necessary. It behaves as if the original sequence is
  // destroyed via its destructor and then the source sequence copied
  // using the copy constructor. If release=TRUE, the destructor
  // destroys each of the current elements (items zero through
  // length--1).  For an unbounded sequence, if a reallocation is
  // necessary due to a change in the length and the sequence was
  // created using the release=TRUE parameter in its constructor, the
  // sequence will deallocate the old storage. If release is FALSE
  // under these circumstances, old storage will not be freed before
  // the reallocation is performed. After reallocation, the release
  // flag is always set to TRUE.

  TAO_Object_Manager<T> operator[] (CORBA::ULong index) const;
  // read-write accessor

  static T **allocbuf (CORBA::ULong);
  // The allocbuf function allocates a vector of T elements that can
  // be passed to the T *data constructor. The length of the vector is
  // given by the nelems function argument.  The allocbuf function
  // initializes each element using its default constructor, except
  // for strings, which are initialized to null pointers, and object
  // references, which are initialized to suitably typed nil object
  // references. A null pointer is returned if allocbuf for some
  // reason cannot allocate the requested vector. Vectors allocated by
  // allocbuf should be freed using the freebuf function.

  static void freebuf (T **);
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // The Base_Sequence functions, please see "tao/Sequence.h"
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
  virtual void _downcast (void* target,
                          CORBA_Object* src,
                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  virtual CORBA_Object* _upcast (void* src) const;
};

// *************************************************************

template<class T, CORBA::ULong MAX>
class TAO_Bounded_Object_Sequence : public TAO_Bounded_Base_Sequence
{
  // = TITLE
  //   Parametric sequence for types that require managers.
  //
  // = DESCRIPTION
  //   Please see the documentation for the unbounded case.
  //
public:
  // = Initialization and termination methods.

  TAO_Bounded_Object_Sequence (void);
  // For bounded sequences, the maximum length is part of the type and
  // cannot be set or modified, while for unbounded sequences, the
  // default constructor also sets the maximum length to 0. The
  // default constructor for a bounded sequence always allocates a
  // contents vector, so it always sets the release flag to TRUE.

  TAO_Bounded_Object_Sequence (CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release=0);
  // Constructor from data.

  TAO_Bounded_Object_Sequence (const TAO_Bounded_Object_Sequence<T,MAX> &);
  // Copy constructor.

  ~TAO_Bounded_Object_Sequence (void);
  // destructor

  TAO_Bounded_Object_Sequence &operator= (const TAO_Bounded_Object_Sequence<T,MAX> &);
  // Assignment from another Bounded sequence.

  TAO_Object_Manager<T> operator[] (CORBA::ULong index) const;
  // Read-write accessor.

  static T **allocbuf (CORBA::ULong length);
  // Allocate storage for a sequence..

  static void freebuf (T **buffer);
  // Free a buffer allocated by allocbuf() and release each element on
  // it.

  // The Base_Sequence functions, please see "tao/sequence.h"
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
  virtual void _downcast (void* target,
                          CORBA_Object* src,
                          CORBA_Environment &TAO_IN_ENV = CORBA::default_environment ());
  virtual CORBA_Object* _upcast (void* src) const;
};

// *************************************************************

template<class T>
class TAO_Unbounded_Pseudo_Sequence : public TAO_Unbounded_Base_Sequence
{
  // = TITLE
  //   Parametric sequence for pseudo objects.
  //
  // = DESCRIPTION
  //   Some IDL types (including pseudo objects) require that
  //   sequences on them have a "manager" class, in charge of handling
  //   the object lifetime.
  //   This parametric class implements those sequences. In general
  //   the sequence is similar to Object_Sequente, except for some
  //   TAO internal details. The complete documentation of each method
  //   is provided in TAO_Unbounded_Object_Sequece
public:
  // = Initialization and termination methods.

  TAO_Unbounded_Pseudo_Sequence (void);
  // default ctor

  TAO_Unbounded_Pseudo_Sequence (CORBA::ULong max);
  // Constructor with a "hint" for the maximum capacity.

  TAO_Unbounded_Pseudo_Sequence (CORBA::ULong maximum,
                                 CORBA::ULong length,
                                 T* *data,
                                 CORBA::Boolean release=0);
  // Constructor with a given buffer.

  TAO_Unbounded_Pseudo_Sequence(const TAO_Unbounded_Pseudo_Sequence<T> &);
  // Copy ctor, deep copies.

  ~TAO_Unbounded_Pseudo_Sequence (void);
  // dtor releases all the contained elements.

  TAO_Unbounded_Pseudo_Sequence<T> &operator= (const TAO_Unbounded_Pseudo_Sequence <T> &);
  // The assignment operator first releases all object reference
  // members and frees all string members, and then performs a
  // deepcopy to create a new structure.

  TAO_Object_Manager<T> operator[] (CORBA::ULong index) const;
  // read-write accessor

  static T **allocbuf (CORBA::ULong);
  // The allocbuf function allocates a vector of T elements that can
  // be passed to the T *data constructor.

  static void freebuf (T **);
  // Release all the elements.

  // The Base_Sequence functions, please see "tao/Sequence.h"
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

template<class T, CORBA::ULong MAX>
class TAO_Bounded_Pseudo_Sequence : public TAO_Bounded_Base_Sequence
{
  // = TITLE
  //   Bounded version of TAO_Unbounded_Psuedo_Sequence.
  //
  // = DESCRIPTION
  //   Please see the documentation for the unbounded case.
  //
public:
  // = Initialization and termination methods.

  TAO_Bounded_Pseudo_Sequence (void);
  // default ctor.

  TAO_Bounded_Pseudo_Sequence (CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release=0);
  // Constructor from data.

  TAO_Bounded_Pseudo_Sequence (const TAO_Bounded_Pseudo_Sequence<T,MAX> &);
  // Copy constructor.

  ~TAO_Bounded_Pseudo_Sequence (void);
  // destructor

  TAO_Bounded_Pseudo_Sequence &operator= (const TAO_Bounded_Pseudo_Sequence<T,MAX> &);
  // Assignment from another Bounded sequence.

  TAO_Object_Manager<T> operator[] (CORBA::ULong index) const;
  // Read-write accessor.

  static T **allocbuf (CORBA::ULong length);
  // Allocate storage for a sequence..

  static void freebuf (T **buffer);
  // Free a buffer allocated by allocbuf() and release each element on
  // it.

  // The Base_Sequence functions, please see "tao/sequence.h"
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

template<CORBA::ULong MAX>
class TAO_Bounded_String_Sequence : public TAO_Bounded_Base_Sequence
{
  // = TITLE
  //   Bounded sequence of strings.
  //
  // = DESCRIPTION
  //   As its unbounded counterpart it duplicates and releases strings
  //   (via CORBA::string_dup and CORBA::string_free) under the
  //   control of a per sequence flag, but the capacity of the
  //   sequence is bound on the type.
public:
  TAO_Bounded_String_Sequence (void);
  // {SPEC}
  // For bounded sequences, the maximum length is part of the type and
  // cannot be set or modified, while for unbounded  sequences, the
  // default constructor also sets the maximum length to 0. The
  // default constructor for a bounded sequence always allocates a
  // contents vector, so it always sets the release flag to TRUE.

  TAO_Bounded_String_Sequence (CORBA::ULong length,
                               char* *value,
                               CORBA::Boolean release = 0);
  // {SPEC}
  // The ``T *data'' constructor (as shown in the example above)
  // allows the length and contents of a bounded or unbounded sequence
  // to be set. For unbounded sequences, it also allows the initial
  // value of the maximum length to be set. For this constructor,
  // ownership of the contents vector is determined by the release
  // parameter---FALSE means the caller owns the storage, while TRUE
  // means that the sequence assumes ownership of the storage.
  //
  // If release is TRUE, the contents vector must have been allocated
  // using the sequence  allocbuf function, and the sequence will pass
  // it to freebuf when finished with it.

  TAO_Bounded_String_Sequence (const TAO_Bounded_String_Sequence<MAX> &);
  // {SPEC}
  // Management Functions'' on page 16. The copy constructor creates a
  // new sequence with the same maximum and length as the given
  // sequence, copies each of its current elements (items zero through
  // length--1), and sets the release flag to TRUE.

  TAO_Bounded_String_Sequence &operator= (const TAO_Bounded_String_Sequence<MAX> &);
  // {SPEC}
  // The assignment operator deep copies its parameter, releasing
  // old storage if necessary. It behaves as if the original sequence
  // is destroyed via its destructor and then the source sequence
  // copied using the copy constructor.

  ~TAO_Bounded_String_Sequence (void);
  // {SPEC}
  // If release=TRUE, the destructor destroys each of the current
  // elements (items zero through length-1).

  TAO_SeqElem_String_Manager operator[] (CORBA::ULong index) const;
  // read-write accessor

  static char **allocbuf (CORBA::ULong length);
  // {SPEC}
  // The allocbuf function allocates a vector of T elements that can
  // be passed to the T *data constructor. The length of the vector is
  // given by the nelems function argument.  The allocbuf function
  // initializes each element using its default constructor, except
  // for strings, which are initialized to null pointers, and object
  // references, which are initialized to suitably typed nil object
  // references. A null pointer is returned if allocbuf for some
  // reason cannot allocate the requested vector. Vectors allocated by
  // allocbuf should be freed using the freebuf function.

  static void freebuf (char **buffer);
  // {SPEC}
  // The freebuf function ensures that the destructor for each element
  // is called before the buffer is destroyed, except for string
  // elements, which are freed using string_free(), and object
  // reference elements, which are freed using release(). The freebuf
  // function will ignore null pointers passed to it.

  // @@ Please document me.
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

#if defined (__ACE_INLINE__)
#include "tao/Sequence_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Sequence_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Sequence_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_SEQUENCE_T_H */
