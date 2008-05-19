// -*- C++ -*-

//=============================================================================
/**
 *  @file    Empty_Param_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for @c CORBA::TypeCodes with empty parameter lists.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_EMPTY_PARAM_TYPECODE_H
#define TAO_EMPTY_PARAM_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {

    /**
     * @class Empty_Param
     *
     * @brief @c CORBA::TypeCode implementation for OMG IDL types with
     *        empty parameter lists.
     *
     * This class implements a @c CORBA::TypeCode for OMG IDL types
     * with empty parameter lists.
     *
     * @note @c Empty_Param @c TypeCodes are not reference counted in
     *       TAO since they are static, exist as constants for the
     *       length of a given OS process, and cannot be created
     *       through the @c CORBA::ORB or @c CORBA::TypeCodeFactory
     *       interfaces.
     */
    class Empty_Param : public CORBA::TypeCode
    {
    public:

      /// Constructor.
      Empty_Param (CORBA::TCKind k);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr,
                                CORBA::ULong offset) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c CORBA::TypeCode template methods specific to @c TypeCodes
       * with empty parameter lists.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Empty_Param_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_EMPTY_PARAM_TYPECODE_H */
