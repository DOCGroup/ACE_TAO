// -*- C++ -*-
//=============================================================================
/**
 * @file Auto_Functor.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
//=============================================================================
#ifndef TAO_UTILS_AUTO_FUNCTOR_H
#define TAO_UTILS_AUTO_FUNCTOR_H
#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Environment.h"
#include "ace/Global_Macros.h"


namespace TAO
{
  namespace Utils
  {

    /**
     * @class Auto_Functor_Ref
     *
     * @brief Helper class to implement assignment and copy-construction
     *        as expected
     */
    template<typename X, typename Functor>
    struct Auto_Functor_Ref
    {
      X * p_;
      Functor f_;

      Auto_Functor_Ref(X * p, Functor f);
    };

    /**
     * @class Auto_Functor
     *
     * @brief Helper template to implement auto_ptr<>-like classes, but
     *    executing a functor in the destructor, instead of always
     *    deleting things.
     *
     * The functor is called in the destructor, and it must implement:
     *
     * Functor() throw();<BR>
     * Functor(Functor const &) throw();<BR>
     * Functor & operator=(Functor const &) throw();<BR>
     * void operator()(X * p) throw();<BR>
     *
     */
    template<typename X, typename Functor>
    class Auto_Functor
    {
    public:
      typedef X element_type;
      typedef Functor functor_type;

      /// Constructor
      explicit Auto_Functor (X * p = 0,
                             Functor functor = Functor())
        ACE_THROW_SPEC (());

      Auto_Functor (Auto_Functor & rhs)
        ACE_THROW_SPEC (());

      Auto_Functor<X,Functor>& operator= (Auto_Functor & rhs)
        ACE_THROW_SPEC (());

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
      template<typename Y>
      Auto_Functor(Auto_Functor<Y,Functor>& rhs)
        ACE_THROW_SPEC (());

      template<typename Y>
      Auto_Functor<X,Functor>& operator= (Auto_Functor<Y,Functor>& rhs)
        ACE_THROW_SPEC (());
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

      ~Auto_Functor()
        ACE_THROW_SPEC (());

      X & operator*() const
        ACE_THROW_SPEC (());

      X * operator->() const
        ACE_THROW_SPEC (());

      X * get()
        ACE_THROW_SPEC (());

      X * release()
        ACE_THROW_SPEC (());

      void reset (X * p = 0)
        ACE_THROW_SPEC (());

      void reset (X * p, Functor f)
        ACE_THROW_SPEC (());

      Functor const & functor() const
        ACE_THROW_SPEC (());

      Auto_Functor(Auto_Functor_Ref<X,Functor> rhs)
        ACE_THROW_SPEC (());

      Auto_Functor<X,Functor> & operator=(Auto_Functor_Ref<X,Functor> rhs)
        ACE_THROW_SPEC (());

#if !defined(ACE_LACKS_MEMBER_TEMPLATES)
      template<typename Y> operator Auto_Functor_Ref<Y,Functor>()
        ACE_THROW_SPEC (());

      template<typename Y> operator Auto_Functor<Y,Functor>()
        ACE_THROW_SPEC (());
#else
      operator Auto_Functor_Ref<X,Functor>()
        ACE_THROW_SPEC (());
#endif /* ACE_LACKS_MEMBER_TEMPLATES */

    private:
      X * p_;

      Functor f_;
    };

  } // namespace Utils
} // namespace TAO

#if defined(__ACE_INLINE__)
# include "Auto_Functor.inl"
#endif /* __ACE_INLINE__ */

#if defined(ACE_TEMPLATES_REQUIRE_SOURCE)
# include "Auto_Functor.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#include /**/ "ace/post.h"
#endif /* TAO_UTILS_AUTO_FUNCTOR_H*/
