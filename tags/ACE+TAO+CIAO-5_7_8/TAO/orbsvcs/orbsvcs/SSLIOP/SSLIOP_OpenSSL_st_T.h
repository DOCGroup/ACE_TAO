// -*- C++ -*-

//=============================================================================
/**
 *  @file   SSLIOP_OpenSSL_st_T.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SSLIOP_OPENSSL_ST_T_H
#define TAO_SSLIOP_OPENSSL_ST_T_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace SSLIOP
  {
    /**
     * @struct OpenSSL_traits
     *
     * @brief Template traits structure for OpenSSL data structures.
     *
     * Template traits structure containing constants and functions
     * specific to a given OpenSSL data structure.
     */
    template <typename T>
    struct OpenSSL_traits;

    /**
     * @name CORBA-style Reference Count Manipulation Methods
     *
     * These reference count manipulation methods are generally
     * specific to OpenSSL structures.
     */
    /// Increase the reference count on the given OpenSSL structure.
    template <typename T>
    T * _duplicate (T * st);

    /// Deep copy the given OpenSSL structure.
    template <typename T>
    T * copy (T const & st);

    /// Decrease the reference count on the given OpenSSL structure.
    template <typename T>
    void release (T * st);

    /**
     * @class OpenSSL_st_var
     *
     * @brief "_var" class for the OpenSSL @param T structure.
     *
     * This class is simply used to make operations on instances of
     * the OpenSSL @c T structure exception safe.  It is only used
     * internally by the SSLIOP pluggable transport.
     */
    template <typename T>
    class OpenSSL_st_var
    {
    public:

      /**
       * @name Constructors
       *
       * Constructors.
       */
      //@{
      OpenSSL_st_var (void);
      OpenSSL_st_var (T * st);
      OpenSSL_st_var (OpenSSL_st_var<T> const & v);
      OpenSSL_st_var (T const & st);
      //@}

      /// Destructor
      ~OpenSSL_st_var (void);

      /**
       * @name Assignment operators.
       *
       * Assignment operators.
       */
      //@{
      OpenSSL_st_var<T> & operator= (T* st);
      OpenSSL_st_var<T> & operator= (OpenSSL_st_var<T> const & v);
      OpenSSL_st_var<T> & operator= (T const & st);
      //@}

      T const * operator-> (void) const;
      T* operator-> (void);

      operator const T& () const;
      operator T& ();

      T*  in (void) const;
      T*& inout (void);
      T*& out (void);
      T*  _retn (void);
      T*  ptr (void) const;

    private:

      /// The OpenSSL structure whose reference count is managed.
      T * st_;

    };
  }  // End SSLIOP namespace.
}  // End TAO namespace.

TAO_END_VERSIONED_NAMESPACE_DECL


#if defined (__ACE_INLINE__)
#include "orbsvcs/SSLIOP/SSLIOP_OpenSSL_st_T.inl"
#endif  /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
# include "orbsvcs/SSLIOP/SSLIOP_OpenSSL_st_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
# pragma implementation ("SSLIOP_OpenSSL_st_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif  /* TAO_SSLIOP_OPENSSL_ST_T_H */
