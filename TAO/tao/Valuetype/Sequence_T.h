// -*- C++ -*-

//=============================================================================
/**
 *  @file    Sequence_T.h
 *
 *  $Id$
 *
 *  @authors Jeff Parsons
 */
//=============================================================================

#ifndef TAO_VALUETYPE_SEQUENCE_T_H
#define TAO_VALUETYPE_SEQUENCE_T_H

#include /**/ "ace/pre.h"

#include "tao/Sequence.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<typename T, typename T_var, typename T_life>
class TAO_Unbounded_Valuetype_Sequence;

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
template<typename T, typename T_var, typename T_life>
class TAO_Valuetype_Manager
{
  friend class TAO_Unbounded_Valuetype_Sequence<T,T_var,T_life>;
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
  TAO_Valuetype_Manager (const TAO_Valuetype_Manager<T,T_var,T_life> & rhs);

  /**
   * Constructor from address of an element, it should be private and
   * only TAO_*_Object_Sequence would use it, but we have some
   * problems with friendship and templates.
   */
  TAO_Valuetype_Manager (T **,
                         CORBA::Boolean release);

  /// Destructor, only releases the object if <release_> is true.
  ~TAO_Valuetype_Manager (void);

  /**
   * Assignment from another managed type, only release if
   * <this->release_> is true.
   * @@ TODO what happens if rhs.release_ is true an this->relase_ is
   * false?
   */
  TAO_Valuetype_Manager<T,T_var,T_life> &operator= (
      const TAO_Valuetype_Manager<T,T_var,T_life> & rhs
    );

  /// Assignment from T *.
  TAO_Valuetype_Manager<T,T_var,T_life> &operator= (T *);

  /// Assignment from T_var.
  TAO_Valuetype_Manager<T,T_var,T_life> &operator= (const T_var &);

  /// Return pointer.
  T * operator-> (void) const;

  /// Cast (read-only).
  operator const T * () const;

  /// Cast.
  operator T *& ();

  /// Cast (read-only) so that assignment from a structured
  /// type to a T_var will make a copy.
  operator const T_var() const;

  /// for in parameter.
  T * in (void) const;

  /// for inout parameter.
  T *& inout (void);

  /// for out parameter.
  T * &out (void);

  /// for return type
  T * _retn (void);

private:
  /// data member, notice that it is a pointer, to implement the
  /// reference behavior for assignment.
  T ** ptr_;

  /// release flag based on parent's flag
  CORBA::Boolean release_;
};

// *************************************************************

  /**
   * @class TAO_Abstract_Manager
   *
   * @brief Manager for abstract intefaces.
   *
   * An abstract interface can be either a valuetype or an object
   * references, so a manager class is needed.
   *
   * @see TAO_Object_Manager
   */
template<typename T, typename T_var, typename T_life>
class TAO_Abstract_Manager
{
//  friend class TAO_Unbounded_Abstract_Sequence<T,T_var>;
public:
  // = Initialization and termination methods.
  /**
   * Copy constructor, the semantics are non-trivial:
   *   + The referenced element is duplicated or not according to the
   *   release value on the <rhs>.
   *   + In any case a new reference to the same object is created.
   */
  TAO_Abstract_Manager (const TAO_Abstract_Manager<T,T_var,T_life> & rhs);

  /**
   * Constructor from address of an element, it should be private and
   * only TAO_*_Object_Sequence would use it, but we have some
   * problems with friendship and templates.
   */
  TAO_Abstract_Manager (T **,
                        CORBA::Boolean release);

  /// Destructor, only releases the object if <release_> is true.
  ~TAO_Abstract_Manager (void);

  /**
   * Assignment from another managed type, only release if
   * <this->release_> is true.
   * @@ TODO what happens if rhs.release_ is true an this->relase_ is
   * false?
   */
  TAO_Abstract_Manager<T,T_var,T_life> & operator= (
      const TAO_Abstract_Manager<T,T_var,T_life> & rhs
    );

  /// Assignment from T *.
  TAO_Abstract_Manager<T,T_var,T_life> & operator= (T *);

  /// Assignment from T_var.
  TAO_Abstract_Manager<T,T_var,T_life> & operator= (const T_var &);

  /// Return pointer.
  T * operator-> (void) const;

  /// Cast (read-only).
  operator const T * () const;

  /// Cast.
  operator T *& ();

  /// Cast (read-only) so that assignment from a structured
  /// type to a T_var will make a copy.
  operator const T_var () const;

  /// for in parameter.
  T * in (void) const;

  /// for inout parameter.
  T *& inout (void);

  /// for out parameter.
  T *& out (void);

  /// for return type
  T * _retn (void);

private:
  /// data member, notice that it is a pointer, to implement the
  /// reference behavior for assignment.
  T ** ptr_;

  /// release flag based on parent's flag
  CORBA::Boolean release_;
};

// *************************************************************

/**
 * @class TAO_Unbounded_Valuetype_Sequence
 *
 * @brief Parametric sequence for valuetypes
 *
 * @see TAO_Unbounded_Object_Sequence
 */
template<typename T, typename T_var, typename T_life>
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
                                    T ** data,
                                    CORBA::Boolean release = 0);

  /// Copy ctor, deep copies.
  TAO_Unbounded_Valuetype_Sequence (
      const TAO_Unbounded_Valuetype_Sequence<T,T_var,T_life> &
    );

  /// The destructor releases all object reference memebrs and frees
  /// all string members.
  ~TAO_Unbounded_Valuetype_Sequence (void);

  /**
   * The assignment operator first releases all object reference
   * members and frees all string members, and then performs a
   * deepcopy to create a new structure.
   */
  TAO_Unbounded_Valuetype_Sequence<T,T_var,T_life> & operator= (
      const TAO_Unbounded_Valuetype_Sequence <T,T_var,T_life> &
    );

  /// read-write accessor
  TAO_Valuetype_Manager<T,T_var,T_life> operator[] (CORBA::ULong slot) const;

  /// The allocbuf function allocates a vector of T elements that can
  /// be passed to the T *data constructor.
  static T ** allocbuf (CORBA::ULong);

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
template<typename T, typename T_var, typename T_life, size_t MAX>
class TAO_Bounded_Valuetype_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  // Default ctor.
  TAO_Bounded_Valuetype_Sequence (void);

  /// Constructor from data.
  TAO_Bounded_Valuetype_Sequence (CORBA::ULong length,
                                  T ** value,
                                  CORBA::Boolean release = 0);

  /// Copy constructor.
  TAO_Bounded_Valuetype_Sequence (
      const TAO_Bounded_Valuetype_Sequence<T,T_var,T_life,MAX> &
    );

  /// destructor
  ~TAO_Bounded_Valuetype_Sequence (void);

  /// Assignment from another Bounded sequence.
  TAO_Bounded_Valuetype_Sequence &operator= (
      const TAO_Bounded_Valuetype_Sequence<T,T_var,T_life,MAX> &
    );

  /// Read-write accessor.
  TAO_Valuetype_Manager<T,T_var,T_life> operator[] (CORBA::ULong slot) const;

  /// Allocate storage for a sequence..
  static T ** allocbuf (CORBA::ULong length);

  /// Free a buffer allocated by allocbuf() and release each element on
  /// it.
  static void freebuf (T ** buffer);

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
 * @class TAO_Unbounded_Abstract_Sequence
 *
 * @brief Parametric sequence for abstract interfaces
 *
 * @see TAO_Unbounded_Object_Sequence
 */
template<typename T, typename T_var, typename T_life>
class TAO_Unbounded_Abstract_Sequence : public TAO_Unbounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  // Default ctor.
  TAO_Unbounded_Abstract_Sequence (void);

  /// Constructor with a "hint" for the maximum capacity.
  TAO_Unbounded_Abstract_Sequence (CORBA::ULong max);

  /// Constructor with a given buffer.
  TAO_Unbounded_Abstract_Sequence (CORBA::ULong maximum,
                                   CORBA::ULong length,
                                   T ** data,
                                   CORBA::Boolean release = 0);

  /// Copy ctor, deep copies.
  TAO_Unbounded_Abstract_Sequence (
      const TAO_Unbounded_Abstract_Sequence<T,T_var,T_life> &
    );

  /// The destructor releases all object reference memebrs and frees
  /// all string members.
  ~TAO_Unbounded_Abstract_Sequence (void);

  /**
   * The assignment operator first releases all object reference
   * members and frees all string members, and then performs a
   * deepcopy to create a new structure.
   */
  TAO_Unbounded_Abstract_Sequence<T,T_var,T_life> &operator= (
      const TAO_Unbounded_Abstract_Sequence <T,T_var,T_life> &
    );

  /// read-write accessor
  TAO_Abstract_Manager<T,T_var,T_life> operator[] (CORBA::ULong slot) const;

  /// The allocbuf function allocates a vector of T elements that can
  /// be passed to the T *data constructor.
  static T ** allocbuf (CORBA::ULong);

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
 * @class TAO_Bounded_Abstract_Sequence
 *
 * @brief Parametric sequence for types that require managers.
 *
 * Please see the documentation for the unbounded case.
 */
template<typename T, typename T_var, typename T_life, size_t MAX>
class TAO_Bounded_Abstract_Sequence : public TAO_Bounded_Base_Sequence
{
public:
  // = Initialization and termination methods.

  // Default ctor.
  TAO_Bounded_Abstract_Sequence (void);

  /// Constructor from data.
  TAO_Bounded_Abstract_Sequence (CORBA::ULong length,
                                 T ** value,
                                 CORBA::Boolean release = 0);

  /// Copy constructor.
  TAO_Bounded_Abstract_Sequence (
      const TAO_Bounded_Abstract_Sequence<T,T_var,T_life,MAX> &
    );

  /// destructor
  ~TAO_Bounded_Abstract_Sequence (void);

  /// Assignment from another Bounded sequence.
  TAO_Bounded_Abstract_Sequence &operator= (
      const TAO_Bounded_Abstract_Sequence<T,T_var,T_life,MAX> &
    );

  /// Read-write accessor.
  TAO_Abstract_Manager<T,T_var,T_life> operator[] (CORBA::ULong slot) const;

  /// Allocate storage for a sequence..
  static T ** allocbuf (CORBA::ULong length);

  /// Free a buffer allocated by allocbuf() and release each element on
  /// it.
  static void freebuf (T ** buffer);

  // The Base_Sequence functions, please see "tao/sequence.h"
  /// No default to workaround egcs problem with templates and
  /// namespaces
  virtual void _allocate_buffer (CORBA::ULong length);
  virtual void _deallocate_buffer (void);
  virtual void _shrink_buffer (CORBA::ULong new_length,
                               CORBA::ULong old_length);
};

#if defined (__ACE_INLINE__)
#include "Sequence_T.inl"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Sequence_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Sequence_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_VALUETYPE_SEQUENCE_T_H */
