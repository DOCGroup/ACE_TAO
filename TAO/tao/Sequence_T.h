/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Sequence_T.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan and Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_SEQUENCE_T_H
#define TAO_SEQUENCE_T_H
#include "ace/pre.h"

#include "tao/Sequence.h"

/**
 * @class TAO_Unbounded_Sequence
 *
 * @brief Parametrized implementation of IDL unbounded sequences.
 *
 * This class completes the implementation for TAO_Base_Sequence
 * for the unbounded case.
 */
template <class T>
class TAO_Unbounded_Sequence : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  /// Default constructor.
  TAO_Unbounded_Sequence (void);

  /// Constructor using a maximum length value.
  TAO_Unbounded_Sequence (CORBA::ULong max);

  /**
   * Constructor using the data and memory management flag.  Memory
   * *must* be allocated using the ::allocbuf static method, since in
   * the future this classes may use a global ACE_Allocator.
   */
  TAO_Unbounded_Sequence (CORBA::ULong max,
                          CORBA::ULong length,
                          T *data,
                          CORBA::Boolean release = 0);

  /// Copy constructor.
  TAO_Unbounded_Sequence (const TAO_Unbounded_Sequence<T> &);

  /// Assignment operator.
  TAO_Unbounded_Sequence<T> &operator= (const TAO_Unbounded_Sequence<T> &);

  /// Dtor.
  ~TAO_Unbounded_Sequence (void);

  // = Accessors.

  /// operator []
  T &operator[] (CORBA::ULong);

  /// operator []
  const T &operator[] (CORBA::ULong) const;

  // = Static operations.

  /// Allocate storage for the sequence.
  static T *allocbuf (CORBA::ULong);

  /// Free the sequence.
  static void freebuf (T *);

  /// Implement the TAO_Base_Sequence methods (see Sequence.h)
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);

  // = orbos/98-01-11 proposed extensions.
  /**
   * Allows read-write access to the underlying buffer.  If <orphan>
   * is FALSE the sequence returns a pointer to its buffer, allocating
   * one if it has not yet done so.  The number of elements in the
   * buffer can be determined from the sequence <length> accessor.
   *
   * If the <orphan> argument to <get_buffer> is FALSE, the sequence
   * maintains ownership of the underlying buffer.  Elements in the
   * returned buffer may be directly replaced by the caller.  For
   * sequences of strings, wide strings, and object references, the
   * caller must use the sequence <release> accessor to determine
   * whether elements should be freed (using <string_free>,
   * <wstring_free>, or <CORBA::release> for strings, wide straings,
   * and object references, respective) before being directly assigned
   * to.
   *
   * If the <orphan> argument to <get_buffer> is TRUE, the sequence
   * yields ownership of the buffer to the caller.  If <orphan> is
   * TRUE and the sequence does not own its buffer (i.e., its
   * <release> flag is FALSE), the return value is a null pointer.  If
   * the buffer is taken from the sequence using this form of
   * <get_buffer>, the sequence reverts to the same state it would
   * have if constructed using its default constructor.  The caller
   * becomes responsible for eventually freeing each element of the
   * returned buffer (for strings, wide string, and object
   * references), and then freeing the returned buffer itself using
   * <freebuf>.
   */
  T *get_buffer (CORBA::Boolean orphan = 0);

  /**
   * This function allows read-only access to the sequence buffer.
   * The sequence returns its buffer, allocating one of one has not
   * yet been allocated.  No direct modification of the returned
   * buffer by the caller is permitted.
   */
  const T *get_buffer (void) const;

  /**
   * Allows the buffer underlying a sequence to be replaced.  The
   * parameters to <replace> are identical in type, order, and purpose
   * to those for the <T *data> constructor for the sequence.
   */
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                T *data,
                CORBA::Boolean release = 0);
};

// *************************************************************

/**
 * @class TAO_Bounded_Sequence
 *
 * @brief Parametric bounded sequence.
 *
 * This class completes the implementation for TAO_Base_Sequence
 * for the bounded case.
 */
template <class T, size_t MAX>
class TAO_Bounded_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.
  /// Default constructor.
  TAO_Bounded_Sequence (void);

  /// Constructor using the data and memory management flag.
  TAO_Bounded_Sequence (CORBA::ULong length,
                        T *data,
                        CORBA::Boolean release=0);

  /// Copy constructor.
  TAO_Bounded_Sequence (const TAO_Bounded_Sequence<T,MAX> &);

  /// Assignment operator.
  TAO_Bounded_Sequence<T,MAX> &operator= (const TAO_Bounded_Sequence<T,MAX> &);

  /// Dtor.
  ~TAO_Bounded_Sequence (void);

  // = Accessors.
  /// operator []
  T &operator[] (CORBA::ULong);

  /// operator []
  const T &operator[] (CORBA::ULong) const;

  // = Static operations.

  /// Allocate storage for the sequence.
  static T *allocbuf (CORBA::ULong);

  /// Free the sequence.
  static void freebuf (T *);

  /// allocate a buffer of the requested length. The buffer is allocated for the
  /// right type
  virtual void _allocate_buffer (CORBA::ULong length);

  /// deallocate the buffer
  virtual void _deallocate_buffer (void);

  // = orbos/98-01-11 proposed extensions.
  /**
   * Allows read-write access to the underlying buffer.  If <orphan>
   * is FALSE the sequence returns a pointer to its buffer, allocating
   * one if it has not yet done so.  The size of the returned buffer
   * is equal to the sequence bound, which can be determined with the
   * <maximum> accessor.  The number of elements in the buffer can be
   * determined from the sequence <length> accessor.
   *
   * If the <orphan> argument to <get_buffer> is FALSE, the sequence
   * maintains ownership of the underlying buffer.  Elements in the
   * returned buffer may be directly replaced by the caller.  For
   * sequences of strings, wide strings, and object references, the
   * caller must use the sequence <release> accessor to determine
   * whether elements should be freed (using <string_free>,
   * <wstring_free>, or <CORBA::release> for strings, wide straings,
   * and object references, respective) before being directly assigned
   * to.
   *
   * If the <orphan> argument to <get_buffer> is TRUE, the sequence
   * yields ownership of the buffer to the caller.  If <orphan> is
   * TRUE and the sequence does not own its buffer (i.e., its
   * <release> flag is FALSE), the return value is a null pointer.  If
   * the buffer is taken from the sequence using this form of
   * <get_buffer>, the sequence reverts to the same state it would
   * have if constructed using its default constructor.  The caller
   * becomes responsible for eventually freeing each element of the
   * returned buffer (for strings, wide string, and object
   * references), and then freeing the returned buffer itself using
   * <freebuf>.
   */
  T *get_buffer (CORBA::Boolean orphan = 0);

  /**
   * This function allows read-only access to the sequence buffer.
   * The sequence returns its buffer, allocating one of one has not
   * yet been allocated.  No direct modification of the returned
   * buffer by the caller is permitted.
   */
  const T *get_buffer (void) const;

  /**
   * Allows the buffer underlying a sequence to be replaced.  The
   * parameters to <replace> are identical in type, order, and purpose
   * to those for the <T *data> constructor for the sequence.
   */
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                T *data,
                CORBA::Boolean release = 0);
};

// *************************************************************

template<class T,class T_var> class TAO_Unbounded_Object_Sequence;
template<class T,class T_var> class TAO_Unbounded_Pseudo_Sequence;
template<class T,class T_var> class TAO_Unbounded_Valuetype_Sequence;
template<class T, class T_var, size_t MAX> class TAO_Bounded_Object_Sequence;
template<class T, class T_var, size_t MAX> class TAO_Bounded_Valuetype_Sequence;
template<size_t MAX> class TAO_Bounded_String_Sequence;

// *************************************************************

  /**
   * @class TAO_Object_Manager
   *
   * @brief Manager for Objects.
   *
   * The mapping for sequences of objects (and other pseudo objects)
   * requires an auxiliar class or <Manager> to handle the reference
   * count on the object, (a kind of smart pointer).
   * The main difference with respect to T_var classes is that
   * automatic release is not controlled on a per-item basis, but
   * for the sequence as a whole.
   * Further the class must implement "transparent" assignment into
   * the sequence, for instance, operator[] returns a managed
   * object, but:
   * object_sequence[i] = foo;
   * must change the state of the sequence.
   * This class implements the generic object manager and is used to
   * instantiate the proper sequence types.
   * = NOTES
   * It has been proposed that the class should be parametric on
   * both T and T_ptr, IMHO this is no necesary: though the IDL spec
   * says that T_ptr *could* map to a type different to T* in the
   * particular case of TAO it does map to <T*>.
   */
template<class T, class T_var>
class TAO_Object_Manager
{
  friend class TAO_Unbounded_Object_Sequence<T,T_var>;
public:
  // @@ Use partial template specialization here to give access only
  // to the right kind of sequence.
  // friend template<CORBA::ULong MAX>
  //    class TAO_Bounded_Object_Sequence<T,MAX>;

  // = Initialization and termination methods.
  /**
   * Copy constructor, the semantics are non-trivial:
   *   + The referenced element is duplicated or not according to the
   *   release value on the <rhs>.
   *   + In any case a new reference to the same object is created.
   */
  TAO_Object_Manager (const TAO_Object_Manager<T,T_var> &rhs);

  /**
   * Constructor from address of an element, it should be private and
   * only TAO_*_Object_Sequence would use it, but we have some
   * problems with friendship and templates.
   */
  TAO_Object_Manager (T **, CORBA::Boolean release);

  /// Destructor, only releases the object if <release_> is true.
  ~TAO_Object_Manager (void);

  /**
   * Assignment from another managed type, only release if
   * <this->release_> is true.
   * @@ TODO what happens if rhs.release_ is true an this->relase_ is
   * false?
   */
  TAO_Object_Manager<T,T_var> &operator= (const TAO_Object_Manager<T,T_var> &rhs);

  /// Assignment from T *.
  TAO_Object_Manager<T,T_var> &operator= (T *);

  /// Assignment from T_var.
  TAO_Object_Manager<T,T_var> &operator= (const T_var &);

  /// Return pointer.
  T * operator-> (void) const;

  /// Cast (read-only).
  operator const T *() const;

  /// Cast.
  operator T *&();

  /// Cast (read-only) so that assignment from a structured
  /// type to a T_var will make a copy.
  operator const T_var() const;

  /// for in parameter.
  T *in (void) const;

  /// for inout parameter.
  T *&inout (void);

  /// for out parameter.
  T *&out (void);

  /// for return type
  T *_retn (void);

private:
  /// data member, notice that it is a pointer, to implement the
  /// reference behavior for assignment.
  T **ptr_;

  /// release flag based on parent's flag
  CORBA::Boolean release_;
};

// *************************************************************

  /**
   * @class TAO_Pseudo_Object_Manager
   *
   * @brief Manager for Pseudo Objects.
   *
   */
template<class T, class T_var>
class TAO_Pseudo_Object_Manager
{
  friend class TAO_Unbounded_Pseudo_Sequence<T,T_var>;
public:
  // @@ Use partial template specialization here to give access only
  // to the right kind of sequence.
  // friend template<CORBA::ULong MAX>
  //    class TAO_Bounded_Object_Sequence<T,MAX>;

  // = Initialization and termination methods.
  /**
   * Copy constructor, the semantics are non-trivial:
   *   + The referenced element is duplicated or not according to the
   *   release value on the <rhs>.
   *   + In any case a new reference to the same object is created.
   */
  TAO_Pseudo_Object_Manager (const TAO_Pseudo_Object_Manager<T,T_var> &rhs);

  /**
   * Constructor from address of an element, it should be private and
   * only TAO_*_Object_Sequence would use it, but we have some
   * problems with friendship and templates.
   */
  TAO_Pseudo_Object_Manager (T **, CORBA::Boolean release);

  /// Destructor, only releases the object if <release_> is true.
  ~TAO_Pseudo_Object_Manager (void);

  /**
   * Assignment from another managed type, only release if
   * <this->release_> is true.
   * @@ TODO what happens if rhs.release_ is true an this->relase_ is
   * false?
   */
  TAO_Pseudo_Object_Manager<T,T_var> &operator= (const TAO_Pseudo_Object_Manager<T,T_var> &rhs);

  /// Assignment from T *.
  TAO_Pseudo_Object_Manager<T,T_var> &operator= (T *);

  /// Assignment from T_var.
  TAO_Pseudo_Object_Manager<T,T_var> &operator= (const T_var &);

  /// Return pointer.
  T * operator-> (void) const;

  /// Cast (read-only).
  operator const T *() const;

  /// Cast.
  operator T *&();

  /// for in parameter.
  T *in (void) const;

  /// for inout parameter.
  T *&inout (void);

  /// for out parameter.
  T *&out (void);

  /// for return type
  T *_retn (void);

private:
  /// data member, notice that it is a pointer, to implement the
  /// reference behavior for assignment.
  T **ptr_;

  /// release flag based on parent's flag
  CORBA::Boolean release_;
};

// *************************************************************

  /**
   * @class TAO_Valuetype_Manager
   *
   * @brief Manager for valuetypes.
   *
   * According to the spec., valuetypes are handled similarly to object
   * references, so a manager class is needed.
   *
   * @see TAO_Object_Manager
   */
template<class T, class T_var>
class TAO_Valuetype_Manager
{
  friend class TAO_Unbounded_Valuetype_Sequence<T,T_var>;
public:
  // @@ Use partial template specialization here to give access only
  // to the right kind of sequence.
  // friend template<CORBA::ULong MAX>
  //    class TAO_Bounded_Valuetype_Sequence<T,MAX>;

  // = Initialization and termination methods.
  /**
   * Copy constructor, the semantics are non-trivial:
   *   + The referenced element is duplicated or not according to the
   *   release value on the <rhs>.
   *   + In any case a new reference to the same object is created.
   */
  TAO_Valuetype_Manager (const TAO_Valuetype_Manager<T,T_var> &rhs);

  /**
   * Constructor from address of an element, it should be private and
   * only TAO_*_Object_Sequence would use it, but we have some
   * problems with friendship and templates.
   */
  TAO_Valuetype_Manager (T **, CORBA::Boolean release);

  /// Destructor, only releases the object if <release_> is true.
  ~TAO_Valuetype_Manager (void);

  /**
   * Assignment from another managed type, only release if
   * <this->release_> is true.
   * @@ TODO what happens if rhs.release_ is true an this->relase_ is
   * false?
   */
  TAO_Valuetype_Manager<T,T_var> &operator= (const TAO_Valuetype_Manager<T,T_var> &rhs);

  /// Assignment from T *.
  TAO_Valuetype_Manager<T,T_var> &operator= (T *);

  /// Assignment from T_var.
  TAO_Valuetype_Manager<T,T_var> &operator= (const T_var &);

  /// Return pointer.
  T * operator-> (void) const;

  /// Cast (read-only).
  operator const T *() const;

  /// Cast.
  operator T *&();

  /// Cast (read-only) so that assignment from a structured
  /// type to a T_var will make a copy.
  operator const T_var() const;

  /// for in parameter.
  T *in (void) const;

  /// for inout parameter.
  T *&inout (void);

  /// for out parameter.
  T *&out (void);

  /// for return type
  T *_retn (void);

private:
  /// data member, notice that it is a pointer, to implement the
  /// reference behavior for assignment.
  T **ptr_;

  /// release flag based on parent's flag
  CORBA::Boolean release_;
};

// *************************************************************

/**
 * @class TAO_Unbounded_Object_Sequence
 *
 * @brief Parametric sequence for types that require managers.
 *
 * Some IDL types require that sequences on them have a "manager"
 * class, in charge of handling the object lifetime, examples are
 * pseudo objects, object references, valuetypes, and strings.
 */
template<class T,class T_var>
class TAO_Unbounded_Object_Sequence : public TAO_Unbounded_Base_Sequence
{

  // = SPEC
  //   16.8 Mapping for Structured Types
  //   The mapping for struct, union, and sequence (but not array) is a
  //   C++ struct or class with a default constructor, a copy
  //   constructor, an assignment operator, and a destructor.
  //
public:
  // = Initialization and termination methods.

  /**
   * {orbos/97-05-15:16.8}
   * The default constructor initializes object reference members to
   * appropriately typed nil object references and string members to
   * NULL; all other members are initialized via their default
   * constructors.
   *
   * {orbos/97-05-15:16.11}
   * For both bounded and unbounded sequences, the default constructor
   * (as shown in the example above) sets the sequence length equal to
   * 0.
   */
  TAO_Unbounded_Object_Sequence (void);

  /**
   * Unbounded sequences provide a constructor that allows only the
   * initial value of the maximum length to be set (the ``maximum
   * constructor'' shown in the example above). This allows
   * applications to control how much buffer space is initially
   * allocated by the sequence. This constructor also sets the length
   * to 0 and the release flag to TRUE.
   */
  TAO_Unbounded_Object_Sequence (CORBA::ULong max);

  /**
   * The ``T *data'' constructor (as shown in the example above)
   * allows the length and contents of a bounded or unbounded sequence
   * to be set. For unbounded sequences, it also allows the initial
   * value of the maximum length to be set. For this constructor,
   * ownership of the contents vector is determined by the release
   * parameter---FALSE means the caller owns the storage, while TRUE
   * means that the sequence assumes ownership of the storage.
   * If release is TRUE, the contents vector must have been allocated
   * using the sequence allocbuf function, and the sequence will pass
   * it to freebuf when finished with it.
   */
  TAO_Unbounded_Object_Sequence (CORBA::ULong maximum,
                                 CORBA::ULong length,
                                 T* *data,
                                 CORBA::Boolean release=0);

  /**
   * The copy constructor performs a deep copy from the existing
   * structure to create a new structure, including calling _duplicate
   * on all object  reference members and performing the necessary
   * heap allocations for all string members.
   *
   * The copy constructor creates a new sequence with the same maximum
   * and length as the given sequence, copies each of its current
   * elements (items zero through length-1), and sets the release
   * flag to TRUE.
   */
  TAO_Unbounded_Object_Sequence(const TAO_Unbounded_Object_Sequence<T,T_var> &);

  /// The destructor releases all object reference memebrs and frees
  /// all string members.
  ~TAO_Unbounded_Object_Sequence (void);

  /**
   * The assignment operator first releases all object reference
   * members and frees all string members, and then performs a
   * deepcopy to create a new structure.
   *
   * The assignment operator deepcopies its parameter, releasing old
   * storage if necessary. It behaves as if the original sequence is
   * destroyed via its destructor and then the source sequence copied
   * using the copy constructor. If release=TRUE, the destructor
   * destroys each of the current elements (items zero through
   * length--1).  For an unbounded sequence, if a reallocation is
   * necessary due to a change in the length and the sequence was
   * created using the release=TRUE parameter in its constructor, the
   * sequence will deallocate the old storage. If release is FALSE
   * under these circumstances, old storage will not be freed before
   * the reallocation is performed. After reallocation, the release
   * flag is always set to TRUE.
   */
  TAO_Unbounded_Object_Sequence<T,T_var> &operator= (const TAO_Unbounded_Object_Sequence <T,T_var> &);

  /// read-write accessor
  TAO_Object_Manager<T,T_var> operator[] (CORBA::ULong slot) const;

  /**
   * The allocbuf function allocates a vector of T elements that can
   * be passed to the T *data constructor. The length of the vector is
   * given by the nelems function argument.  The allocbuf function
   * initializes each element using its default constructor, except
   * for strings, which are initialized to null pointers, and object
   * references, which are initialized to suitably typed nil object
   * references. A null pointer is returned if allocbuf for some
   * reason cannot allocate the requested vector. Vectors allocated by
   * allocbuf should be freed using the freebuf function.
   */
  static T **allocbuf (CORBA::ULong);

  /**
   * The freebuf function ensures that the destructor for each element
   * is called before the buffer is destroyed, except for string
   * elements, which are freed using string_free(), and object
   * reference elements, which are freed using release(). The freebuf
   * function will ignore null pointers passed to it.
   */
  static void freebuf (T **);

  // The Base_Sequence functions, please see "tao/Sequence.h"
  /// No default to workaround egcs problem with templates and
  /// namespaces
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
  virtual void _downcast (void* target,
                          CORBA_Object* src
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual CORBA_Object* _upcast (void* src) const;
};

// *************************************************************

/**
 * @class TAO_Bounded_Object_Sequence
 *
 * @brief Parametric sequence for types that require managers.
 *
 * Please see the documentation for the unbounded case.
 */
template<class T, class T_var, size_t MAX>
class TAO_Bounded_Object_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  /**
   * For bounded sequences, the maximum length is part of the type and
   * cannot be set or modified, while for unbounded sequences, the
   * default constructor also sets the maximum length to 0. The
   * default constructor for a bounded sequence always allocates a
   * contents vector, so it always sets the release flag to TRUE.
   */
  TAO_Bounded_Object_Sequence (void);

  /// Constructor from data.
  TAO_Bounded_Object_Sequence (CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release=0);

  /// Copy constructor.
  TAO_Bounded_Object_Sequence (const TAO_Bounded_Object_Sequence<T,T_var,MAX> &);

  /// destructor
  ~TAO_Bounded_Object_Sequence (void);

  /// Assignment from another Bounded sequence.
  TAO_Bounded_Object_Sequence &operator= (const TAO_Bounded_Object_Sequence<T,T_var,MAX> &);

  /// Read-write accessor.
  TAO_Object_Manager<T,T_var> operator[] (CORBA::ULong slot) const;

  /// Allocate storage for a sequence..
  static T **allocbuf (CORBA::ULong length);

  /// Free a buffer allocated by allocbuf() and release each element on
  /// it.
  static void freebuf (T **buffer);

  // The Base_Sequence functions, please see "tao/sequence.h"
  /// No default to workaround egcs problem with templates and
  /// namespaces
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
  virtual void _downcast (void* target,
                          CORBA_Object* src
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS);
  virtual CORBA_Object* _upcast (void* src) const;
};

// *************************************************************

/**
 * @class TAO_Unbounded_Pseudo_Sequence
 *
 * @brief Parametric sequence for pseudo objects.
 *
 * Some IDL types (including pseudo objects) require that
 * sequences on them have a "manager" class, in charge of handling
 * the object lifetime.
 * This parametric class implements those sequences. In general
 * the sequence is similar to Object_Sequente, except for some
 * TAO internal details. The complete documentation of each method
 * is provided in TAO_Unbounded_Object_Sequece
 */
template<class T,class T_var>
class TAO_Unbounded_Pseudo_Sequence : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  /// default ctor
  TAO_Unbounded_Pseudo_Sequence (void);

  /// Constructor with a "hint" for the maximum capacity.
  TAO_Unbounded_Pseudo_Sequence (CORBA::ULong max);

  /// Constructor with a given buffer.
  TAO_Unbounded_Pseudo_Sequence (CORBA::ULong maximum,
                                 CORBA::ULong length,
                                 T* *data,
                                 CORBA::Boolean release=0);

  /// Copy ctor, deep copies.
  TAO_Unbounded_Pseudo_Sequence(const TAO_Unbounded_Pseudo_Sequence<T,T_var> &);

  /// dtor releases all the contained elements.
  ~TAO_Unbounded_Pseudo_Sequence (void);

  /**
   * The assignment operator first releases all object reference
   * members and frees all string members, and then performs a
   * deepcopy to create a new structure.
   */
  TAO_Unbounded_Pseudo_Sequence<T,T_var> &operator= (const TAO_Unbounded_Pseudo_Sequence <T,T_var> &);

  /// read-write accessor
  TAO_Pseudo_Object_Manager<T,T_var> operator[] (CORBA::ULong slot) const;

  /// The allocbuf function allocates a vector of T elements that can
  /// be passed to the T *data constructor.
  static T **allocbuf (CORBA::ULong);

  /// Release all the elements.
  static void freebuf (T **);

  // The Base_Sequence functions, please see "tao/Sequence.h"
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

/**
 * @class TAO_Bounded_Pseudo_Sequence
 *
 * @brief Bounded version of TAO_Unbounded_Pseudo_Sequence.
 *
 * Please see the documentation for the unbounded case.
 */
template<class T, class T_var, size_t MAX>
class TAO_Bounded_Pseudo_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  /// default ctor.
  TAO_Bounded_Pseudo_Sequence (void);

  /// Constructor from data.
  TAO_Bounded_Pseudo_Sequence (CORBA::ULong length,
                               T* *value,
                               CORBA::Boolean release=0);

  /// Copy constructor.
  TAO_Bounded_Pseudo_Sequence (const TAO_Bounded_Pseudo_Sequence<T,T_var,MAX> &);

  /// destructor
  ~TAO_Bounded_Pseudo_Sequence (void);

  /// Assignment from another Bounded sequence.
  TAO_Bounded_Pseudo_Sequence &operator= (const TAO_Bounded_Pseudo_Sequence<T,T_var,MAX> &);

  /// Read-write accessor.
  TAO_Pseudo_Object_Manager<T,T_var> operator[] (CORBA::ULong slot) const;

  /// Allocate storage for a sequence..
  static T **allocbuf (CORBA::ULong length);

  /// Free a buffer allocated by allocbuf() and release each element on
  /// it.
  static void freebuf (T **buffer);

  // The Base_Sequence functions, please see "tao/sequence.h"
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

/**
 * @class TAO_Unbounded_Valuetype_Sequence
 *
 * @brief Parametric sequence for valuetypes
 *
 * @see TAO_Unbounded_Object_Sequence
 */
template<class T,class T_var>
class TAO_Unbounded_Valuetype_Sequence : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  // Default ctor.
  TAO_Unbounded_Valuetype_Sequence (void);

  /// Constructor with a "hint" for the maximum capacity.
  TAO_Unbounded_Valuetype_Sequence (CORBA::ULong max);

  /// Constructor with a given buffer.
  TAO_Unbounded_Valuetype_Sequence (CORBA::ULong maximum,
                                    CORBA::ULong length,
                                    T* *data,
                                    CORBA::Boolean release=0);

  /// Copy ctor, deep copies.
  TAO_Unbounded_Valuetype_Sequence(const TAO_Unbounded_Valuetype_Sequence<T,T_var> &);

  /// The destructor releases all object reference memebrs and frees
  /// all string members.
  ~TAO_Unbounded_Valuetype_Sequence (void);

  /**
   * The assignment operator first releases all object reference
   * members and frees all string members, and then performs a
   * deepcopy to create a new structure.
   */
  TAO_Unbounded_Valuetype_Sequence<T,T_var> &operator= (const TAO_Unbounded_Valuetype_Sequence <T,T_var> &);

  /// read-write accessor
  TAO_Valuetype_Manager<T,T_var> operator[] (CORBA::ULong slot) const;

  /// The allocbuf function allocates a vector of T elements that can
  /// be passed to the T *data constructor.
  static T **allocbuf (CORBA::ULong);

  /// Release all the elements.
  static void freebuf (T **);

  // The Base_Sequence functions, please see "tao/Sequence.h"
  /// No default to workaround egcs problem with templates and
  /// namespaces
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

/**
 * @class TAO_Bounded_Valuetype_Sequence
 *
 * @brief Parametric sequence for types that require managers.
 *
 * Please see the documentation for the unbounded case.
 */
template<class T, class T_var, size_t MAX>
class TAO_Bounded_Valuetype_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  // Default ctor.
  TAO_Bounded_Valuetype_Sequence (void);

  /// Constructor from data.
  TAO_Bounded_Valuetype_Sequence (CORBA::ULong length,
                                  T* *value,
                                  CORBA::Boolean release=0);

  /// Copy constructor.
  TAO_Bounded_Valuetype_Sequence (const TAO_Bounded_Valuetype_Sequence<T,T_var,MAX> &);

  /// destructor
  ~TAO_Bounded_Valuetype_Sequence (void);

  /// Assignment from another Bounded sequence.
  TAO_Bounded_Valuetype_Sequence &operator= (const TAO_Bounded_Valuetype_Sequence<T,T_var,MAX> &);

  /// Read-write accessor.
  TAO_Valuetype_Manager<T,T_var> operator[] (CORBA::ULong slot) const;

  /// Allocate storage for a sequence..
  static T **allocbuf (CORBA::ULong length);

  /// Free a buffer allocated by allocbuf() and release each element on
  /// it.
  static void freebuf (T **buffer);

  // The Base_Sequence functions, please see "tao/sequence.h"
  /// No default to workaround egcs problem with templates and
  /// namespaces
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

/**
 * @class TAO_Unbounded_Array_Sequence
 *
 * @brief Parametric sequence for arrays.
 *
 * The IDL mapping for arrays includes some unique allocation,
 * deallocation, and copying functions, and precludes
 * direct assignment of one array to another. Also, the
 * Any and CDR operators use a special class derived from
 * the array. For these reasons, we use a special class for
 * sequences of arrays, parametrized on the array element type.
 */
template<class T, class T_var>
class TAO_Unbounded_Array_Sequence : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  /// default ctor
  TAO_Unbounded_Array_Sequence (void);

  /// Constructor with a "hint" for the maximum capacity.
  TAO_Unbounded_Array_Sequence (CORBA::ULong max);

  /// Constructor with a given buffer.
  TAO_Unbounded_Array_Sequence (CORBA::ULong maximum,
                                CORBA::ULong length,
                                T *data,
                                CORBA::Boolean release=0);

  /// Copy ctor, deep copies.
  TAO_Unbounded_Array_Sequence(const TAO_Unbounded_Array_Sequence<T, T_var> &);

  /// dtor releases all the contained elements.
  ~TAO_Unbounded_Array_Sequence (void);

  /**
   * The assignment operator first releases all object reference
   * members and frees all string members, and then performs a
   * deepcopy to create a new structure.
   */
  TAO_Unbounded_Array_Sequence<T, T_var> &operator= (
      const TAO_Unbounded_Array_Sequence <T, T_var> &
    );

  // = Accessors.
  /// operator []
  T &operator[] (CORBA::ULong);

  /// operator []
  const T &operator[] (CORBA::ULong) const;

  /// The allocbuf function allocates a vector of T elements that can
  /// be passed to the T *data constructor.
  static T *allocbuf (CORBA::ULong);

  /// Release all the elements.
  static void freebuf (T *);

  /// allocate a buffer of the requested length. The buffer is allocated for the
  /// right type
  virtual void _allocate_buffer (CORBA::ULong length);

  /// deallocate the buffer
  virtual void _deallocate_buffer (void);

  // = orbos/98-01-11 proposed extensions.
  /**
   * Allows read-write access to the underlying buffer.  If <orphan>
   * is FALSE the sequence returns a pointer to its buffer, allocating
   * one if it has not yet done so.  The number of elements in the
   * buffer can be determined from the sequence <length> accessor.
   *
   * If the <orphan> argument to <get_buffer> is FALSE, the sequence
   * maintains ownership of the underlying buffer.  Elements in the
   * returned buffer may be directly replaced by the caller.
   *
   * If the <orphan> argument to <get_buffer> is TRUE, the sequence
   * yields ownership of the buffer to the caller.  If <orphan> is
   * TRUE and the sequence does not own its buffer (i.e., its
   * <release> flag is FALSE), the return value is a null pointer.  If
   * the buffer is taken from the sequence using this form of
   * <get_buffer>, the sequence reverts to the same state it would
   * have if constructed using its default constructor.  The caller
   * becomes responsible for eventually freeing each element of the
   * returned buffer (for strings, wide string, and object
   * references), and then freeing the returned buffer itself using
   * <freebuf>.
   */
  T *get_buffer (CORBA::Boolean orphan = 0);

  /**
   * This function allows read-only access to the sequence buffer.
   * The sequence returns its buffer, allocating one of one has not
   * yet been allocated.  No direct modification of the returned
   * buffer by the caller is permitted.
   */
  const T *get_buffer (void) const;

  /**
   * Allows the buffer underlying a sequence to be replaced.  The
   * parameters to <replace> are identical in type, order, and purpose
   * to those for the <T *data> constructor for the sequence.
   */
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                T *data,
                CORBA::Boolean release = 0);
};

// *************************************************************

/**
 * @class TAO_Bounded_Array_Sequence
 *
 * @brief Bounded version of TAO_Unbounded_Array_Sequence.
 *
 * Please see the documentation for the unbounded case.
 */
template<class T, class T_var, size_t MAX>
class TAO_Bounded_Array_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  /// default ctor.
  TAO_Bounded_Array_Sequence (void);

  /// Constructor from data.
  TAO_Bounded_Array_Sequence (CORBA::ULong length,
                              T *value,
                              CORBA::Boolean release=0);

  /// Copy constructor.
  TAO_Bounded_Array_Sequence (const TAO_Bounded_Array_Sequence<T, T_var, MAX> &);

  /// destructor
  ~TAO_Bounded_Array_Sequence (void);

  /// Assignment from another Bounded sequence.
  TAO_Bounded_Array_Sequence &operator= (const TAO_Bounded_Array_Sequence<T, T_var, MAX> &);

  // = Accessors.
  /// operator []
  T &operator[] (CORBA::ULong);

  /// operator []
  const T &operator[] (CORBA::ULong) const;

  /// Allocate storage for a sequence..
  static T *allocbuf (CORBA::ULong length);

  /// Free a buffer allocated by allocbuf() and release each element on
  /// it.
  static void freebuf (T *buffer);

  /// allocate a buffer of the requested length. The buffer is allocated for the
  /// right type
  virtual void _allocate_buffer (CORBA::ULong length);

  /// deallocate the buffer
  virtual void _deallocate_buffer (void);

  // = orbos/98-01-11 proposed extensions.
  /**
   * Allows read-write access to the underlying buffer.  If <orphan>
   * is FALSE the sequence returns a pointer to its buffer, allocating
   * one if it has not yet done so.  The number of elements in the
   * buffer can be determined from the sequence <length> accessor.
   *
   * If the <orphan> argument to <get_buffer> is FALSE, the sequence
   * maintains ownership of the underlying buffer.  Elements in the
   * returned buffer may be directly replaced by the caller.
   *
   * If the <orphan> argument to <get_buffer> is TRUE, the sequence
   * yields ownership of the buffer to the caller.  If <orphan> is
   * TRUE and the sequence does not own its buffer (i.e., its
   * <release> flag is FALSE), the return value is a null pointer.  If
   * the buffer is taken from the sequence using this form of
   * <get_buffer>, the sequence reverts to the same state it would
   * have if constructed using its default constructor.  The caller
   * becomes responsible for eventually freeing each element of the
   * returned buffer (for strings, wide string, and object
   * references), and then freeing the returned buffer itself using
   * <freebuf>.
   */
  T *get_buffer (CORBA::Boolean orphan = 0);

  /**
   * This function allows read-only access to the sequence buffer.
   * The sequence returns its buffer, allocating one of one has not
   * yet been allocated.  No direct modification of the returned
   * buffer by the caller is permitted.
   */
  const T *get_buffer (void) const;

  /**
   * Allows the buffer underlying a sequence to be replaced.  The
   * parameters to <replace> are identical in type, order, and purpose
   * to those for the <T *data> constructor for the sequence.
   */
  void replace (CORBA::ULong max,
                CORBA::ULong length,
                T *data,
                CORBA::Boolean release = 0);
};

// *************************************************************

/**
 * @class TAO_Bounded_String_Sequence
 *
 * @brief Bounded sequence of strings.
 *
 * As its unbounded counterpart it duplicates and releases strings
 * (via CORBA::string_dup and CORBA::string_free) under the
 * control of a per sequence flag, but the capacity of the
 * sequence is bound on the type.
 */
template<size_t MAX>
class TAO_Bounded_String_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  /**
   * {SPEC}
   * For bounded sequences, the maximum length is part of the type and
   * cannot be set or modified, while for unbounded  sequences, the
   * default constructor also sets the maximum length to 0. The
   * default constructor for a bounded sequence always allocates a
   * contents vector, so it always sets the release flag to TRUE.
   */
  TAO_Bounded_String_Sequence (void);

  /**
   * {SPEC}
   * The ``T *data'' constructor (as shown in the example above)
   * allows the length and contents of a bounded or unbounded sequence
   * to be set. For unbounded sequences, it also allows the initial
   * value of the maximum length to be set. For this constructor,
   * ownership of the contents vector is determined by the release
   * parameter---FALSE means the caller owns the storage, while TRUE
   * means that the sequence assumes ownership of the storage.
   *
   * If release is TRUE, the contents vector must have been allocated
   * using the sequence  allocbuf function, and the sequence will pass
   * it to freebuf when finished with it.
   */
  TAO_Bounded_String_Sequence (CORBA::ULong length,
                               char* *value,
                               CORBA::Boolean release = 0);

  /**
   * {SPEC}
   * Management Functions'' on page 16. The copy constructor creates a
   * new sequence with the same maximum and length as the given
   * sequence, copies each of its current elements (items zero through
   * length--1), and sets the release flag to TRUE.
   */
  TAO_Bounded_String_Sequence (const TAO_Bounded_String_Sequence<MAX> &);

  /**
   * {SPEC}
   * The assignment operator deep copies its parameter, releasing
   * old storage if necessary. It behaves as if the original sequence
   * is destroyed via its destructor and then the source sequence
   * copied using the copy constructor.
   */
  TAO_Bounded_String_Sequence &operator= (const TAO_Bounded_String_Sequence<MAX> &);

  /**
   * {SPEC}
   * If release=TRUE, the destructor destroys each of the current
   * elements (items zero through length-1).
   */
  ~TAO_Bounded_String_Sequence (void);

  /// read-write accessor
  TAO_SeqElem_String_Manager operator[] (CORBA::ULong slot) const;

  /**
   * {SPEC}
   * The allocbuf function allocates a vector of T elements that can
   * be passed to the T *data constructor. The length of the vector is
   * given by the nelems function argument.  The allocbuf function
   * initializes each element using its default constructor, except
   * for strings, which are initialized to null pointers, and object
   * references, which are initialized to suitably typed nil object
   * references. A null pointer is returned if allocbuf for some
   * reason cannot allocate the requested vector. Vectors allocated by
   * allocbuf should be freed using the freebuf function.
   */
  static char **allocbuf (CORBA::ULong length);

  /**
   * {SPEC}
   * The freebuf function ensures that the destructor for each element
   * is called before the buffer is destroyed, except for string
   * elements, which are freed using string_free(), and object
   * reference elements, which are freed using release(). The freebuf
   * function will ignore null pointers passed to it.
   */
  static void freebuf (char **buffer);

  // Functions to create, destroy, and adjust the underlying buffer.
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

/**
 * @class TAO_Bounded_WString_Sequence
 *
 * @brief Bounded sequence of wstrings.
 *
 * As its unbounded counterpart it duplicates and releases strings
 * (via CORBA::wstring_dup and CORBA::wstring_free) under the
 * control of a per sequence flag, but the capacity of the
 * sequence is bound on the type.
 */
template<size_t MAX>
class TAO_Bounded_WString_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  /**
   * {SPEC}
   * For bounded sequences, the maximum length is part of the type and
   * cannot be set or modified, while for unbounded  sequences, the
   * default constructor also sets the maximum length to 0. The
   * default constructor for a bounded sequence always allocates a
   * contents vector, so it always sets the release flag to TRUE.
   */
  TAO_Bounded_WString_Sequence (void);

  /**
   * {SPEC}
   * The ``T *data'' constructor (as shown in the example above)
   * allows the length and contents of a bounded or unbounded sequence
   * to be set. For unbounded sequences, it also allows the initial
   * value of the maximum length to be set. For this constructor,
   * ownership of the contents vector is determined by the release
   * parameter---FALSE means the caller owns the storage, while TRUE
   * means that the sequence assumes ownership of the storage.
   *
   * If release is TRUE, the contents vector must have been allocated
   * using the sequence  allocbuf function, and the sequence will pass
   * it to freebuf when finished with it.
   */
  TAO_Bounded_WString_Sequence (CORBA::ULong length,
                                CORBA::WChar* *value,
                                CORBA::Boolean release = 0);

  /**
   * {SPEC}
   * Management Functions'' on page 16. The copy constructor creates a
   * new sequence with the same maximum and length as the given
   * sequence, copies each of its current elements (items zero through
   * length--1), and sets the release flag to TRUE.
   */
  TAO_Bounded_WString_Sequence (const TAO_Bounded_WString_Sequence<MAX> &);

  /**
   * {SPEC}
   * The assignment operator deep copies its parameter, releasing
   * old storage if necessary. It behaves as if the original sequence
   * is destroyed via its destructor and then the source sequence
   * copied using the copy constructor.
   */
  TAO_Bounded_WString_Sequence &operator= (const TAO_Bounded_WString_Sequence<MAX> &);

  /**
   * {SPEC}
   * If release=TRUE, the destructor destroys each of the current
   * elements (items zero through length-1).
   */
  ~TAO_Bounded_WString_Sequence (void);

  /// read-write accessor
  TAO_SeqElem_WString_Manager operator[] (CORBA::ULong slot) const;

  /**
   * {SPEC}
   * The allocbuf function allocates a vector of T elements that can
   * be passed to the T *data constructor. The length of the vector is
   * given by the nelems function argument.  The allocbuf function
   * initializes each element using its default constructor, except
   * for (w)strings, which are initialized to null pointers, and object
   * references, which are initialized to suitably typed nil object
   * references. A null pointer is returned if allocbuf for some
   * reason cannot allocate the requested vector. Vectors allocated by
   * allocbuf should be freed using the freebuf function.
   */
  static CORBA::WChar **allocbuf (CORBA::ULong length);

  /**
   * {SPEC}
   * The freebuf function ensures that the destructor for each element
   * is called before the buffer is destroyed, except for string
   * elements, which are freed using wstring_free(), and object
   * reference elements, which are freed using release(). The freebuf
   * function will ignore null pointers passed to it.
   */
  static void freebuf (CORBA::WChar **buffer);

  // Functions to create, destroy, and adjust the underlying buffer.
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

// *************************************************************

#if defined (__ACE_INLINE__)
#include "tao/Sequence_T.i"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Sequence_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Sequence_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* TAO_SEQUENCE_T_H */
