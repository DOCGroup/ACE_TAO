// -*- C++ -*-
//=============================================================================
/**
 * @file Servant_Var.h
 *
 * $Id$
 *
 * @author Jody Hagins <jody@atdesk.com>
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_SERVANT_VAR_H
#define TAO_UTILS_SERVANT_VAR_H
#include /**/ "ace/pre.h"
#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "tao/Environment.h"

namespace TAO
{
  namespace Utils
  {

    /**
     * @class Servant_Var
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
    class Servant_Var
    {
    public:
      typedef T Servant_Type;

      /// Constructor.  Assumes ownership of @c p.
      Servant_Var(T * p = 0);

      /// Copy constructor.  Adds reference to @c rhs.
      Servant_Var(Servant_Var<T> const & rhs);

      /// Assignment operator.  Adds reference to @c rhs.
      Servant_Var<T> & operator=(Servant_Var<T> const & rhs);

      /// Destructor.  Removes a reference from the underlying object,
      /// possibly destroying it.
      ~Servant_Var()
        ACE_THROW_SPEC (());

      /// Assignment operator.  Assumes ownership of @c p.
      Servant_Var<T> & operator=(T * p);

# if !defined(ACE_LACKS_MEMBER_TEMPLATES)
      /// Template member constructor from a pointer that will implicitly
      /// cast to type T.  Assumes ownership of @c p.
      /// This constructor allows constructs such as:
      /// Servant_Base<Base> p(new Derived);
      template <class Y>
      Servant_Var(Y * p);

      /// Template member copy constructor from a Servant_Var<Y>, where
      /// Y can be implicitly cast to type T.
      template <class Y>
      Servant_Var(Servant_Var<Y> const & rhs);

      /// Template member assignment operator from a Servant_Var<Y>, where
      /// Y can be implicitly cast to type T.
      template <class Y>
      Servant_Var<T> & operator=(Servant_Var<Y> const & rhs);

      /// Template member assignment operator from a pointer to Y, where Y
      /// can be implicitly cast to type T.
      template <class Y>
      Servant_Var<T> & operator=(Y * p);
# endif /* ACE_LACKS_MEMBER_TEMPLATES */

      /// Smart pointer operator-> provides access to the underlying object.
      T const * operator->() const;

      /// Smart pointer operator-> provides access to the underlying object.
      T * operator->();

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

      // Return a pointer to the underlying object, and this counted
      // reference will no longer own the object.
      T * _retn();

      /// Increment the reference count and return the servant.
      /**
       * It is safe to pass in a null pointer, the pointer is simply
       * returned in that case.
       *
       * @todo We might want to add a throw spec and catch all (potential)
       *       exceptions in _add_ref()
       *
       * @todo It might be useful to add an _release() method that handles
       *       any potential exceptions...
       */
      static T * _duplicate (T *);

      /// Swap the contents of a Servant_Var<T> with another
      /// Servant_Var<T>
      /**
       * Often used to implement strong exception safety.
       */
      void swap(Servant_Var<T> & rhs)
        ACE_THROW_SPEC(());

    private:
      T * ptr_;
    };

    /// Compare two Servant_Vars for equivalence.
    template <class X, class Y>
    bool operator==(Servant_Var<X> const & x,
                    Servant_Var<Y> const & y);

    /// Compare two Servant_Vars for non-equivalence.
    template <class X, class Y>
    bool operator!=(Servant_Var<X> const & x,
                    Servant_Var<Y> const & y);

  } // namespace Utils
} // namespace TAO

#if defined (__ACE_INLINE__)
# include "Servant_Var.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Servant_Var.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Servant_Var.cpp")
#endif

#include /**/ "ace/post.h"
#endif /*TAO_UTILS_SERVANT_VAR_H*/
