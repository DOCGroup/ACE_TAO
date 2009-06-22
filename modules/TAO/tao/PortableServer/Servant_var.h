// -*- C++ -*-

//=============================================================================
/**
 * @file Servant_var.h
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_SERVANT_VAR_H
#define TAO_PORTABLESERVER_SERVANT_VAR_H
#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  /**
   * @class Servant_var
   *
   * @brief Provides a type safe counted reference to servants.
   *
   * @author Jody Hagins
   *
   * @todo Life would be much easier if _add_ref() and _remove_ref() had
   *       throw specs of "throw ()", that can be hidden in static
   *       methods though.
   */
  template<class T>
  class Servant_var
  {
  public:
    typedef T servant_type;

    /// Constructor.  Assumes ownership of @c p.
    Servant_var (T * p = 0);

    /// Copy constructor.  Adds reference to @c rhs.
    Servant_var (Servant_var<T> const & rhs);

    /// Assignment operator.  Adds reference to @c rhs.
    Servant_var<T> & operator= (Servant_var<T> const & rhs);

    /// Destructor.  Removes a reference from the underlying object,
    /// possibly destroying it.
    /**
     * This destructor doesn't throw exceptions.
     */
    ~Servant_var (void) /* throw () */;

    /// Assignment operator.  Assumes ownership of @c p.
    Servant_var<T> & operator= (T * p);

    /**
       @@ Ciju:
       The below member templates was commented out because:
       1. gcc 2.95 doesn't support them.
       2. Sun cc 5.8 member templates support is buggy (Bug Id: 6463114).

       Once these have been fixed a decision can be made regards
       uncommenting them. My own openion is that they shouldn't be used
       since making implicit constructors, member templates, you could
       potentially get an unwanted conversion where one was unwarranted.
     */
    /*
      # if !defined(ACE_LACKS_MEMBER_TEMPLATES)
      /// Template member constructor from a pointer that will implicitly
      /// cast to type T.  Assumes ownership of @c p.
      /// This constructor allows constructs such as:
      /// Servant_Base<Base> p(new Derived);
      template <class Y>
      Servant_var (Y * p);

      /// Template member copy constructor from a Servant_var<Y>, where
      /// Y can be implicitly cast to type T.
      template <class Y>
      Servant_var (Servant_var<Y> const & rhs);

      /// Template member assignment operator from a Servant_var<Y>, where
      /// Y can be implicitly cast to type T.
      template <class Y>
      Servant_var<T> & operator= (Servant_var<Y> const & rhs);

      /// Template member assignment operator from a pointer to Y, where Y
      /// can be implicitly cast to type T.
      template <class Y>
      Servant_var<T> & operator= (Y * p);
      # endif / * ACE_LACKS_MEMBER_TEMPLATES * /
    */

    /// Smart pointer operator-> provides access to the underlying object.
    T * operator->() const;

    /// Dereference the underlying object.
    T const & operator*() const;

    /// Dereference the underlying object.
    T & operator*();

    /// Return a void pointer to the underlying object.  This allows
    /// it to be used in conditional code and tested against 0.
    operator void const * () const;

    /// As an IN parameter.
    T * in() const;

    /// As an INOUT parameter.
    T *& inout();

    /// As an OUT parameter.
    T *& out();

    /// Return a pointer to the underlying object, and this counted
    /// reference will no longer own the object.
    T * _retn();

    /// Increment the reference count and return the servant.
    /**
     * It is safe to pass in a null pointer, the pointer is simply
     * returned in that case.
     *
     * @todo We might want to catch all (potential) exceptions in
     *       _add_ref().
     *
     * @todo It might be useful to add a _release() method that handles
     *       any potential exceptions...
     */
    static T * _duplicate (T *);

    /// Swap the contents of a Servant_var<T> with another
    /// Servant_var<T>
    /**
     * Non-throwing swap operation.
     * Often used to implement strong exception safety.
     */
    void swap (Servant_var<T> & rhs) /* throw () */;

  private:
    T * ptr_;
  };

  /// Compare two Servant_vars for equivalence.
  template <class X, class Y>
  bool operator==(Servant_var<X> const & x,
                  Servant_var<Y> const & y);

  /// Compare two Servant_vars for non-equivalence.
  template <class X, class Y>
  bool operator!=(Servant_var<X> const & x,
                  Servant_var<Y> const & y);

} // namespace PortableServer

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/PortableServer/Servant_var.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/PortableServer/Servant_var.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("tao/PortableServer/Servant_var.cpp")
#endif

#include /**/ "ace/post.h"
#endif  /* TAO_PORTABLESERVER_SERVANT_VAR_H */
