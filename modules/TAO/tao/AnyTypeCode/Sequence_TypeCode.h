// -*- C++ -*-

//=============================================================================
/**
 *  @file    Sequence_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for @c tk_sequence and @c tk_array @c CORBA::TypeCodes.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_SEQUENCE_TYPECODE_H
#define TAO_SEQUENCE_TYPECODE_H

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
     * @class Sequence
     *
     * @brief @c CORBA::TypeCode implementation for OMG IDL
     *        @c sequence and @c array types.
     *
     * This class implements a @c CORBA::TypeCode for OMG IDL
     * @c sequence and array types.
     */
    template <typename TypeCodeType, class RefCountPolicy>
    class Sequence
      : public CORBA::TypeCode
      , private RefCountPolicy
    {
    public:

      /// Constructor.
      Sequence (CORBA::TCKind kind,
#if defined (__BORLANDC__) && (__BORLANDC__ < 0x572)
                // Borland C++ currently can't handle a reference to
                // const pointer to const CORBA::TypeCode_ptr
                TypeCodeType content_type,
#else
                TypeCodeType const & content_type,
#endif
                CORBA::ULong length);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr, CORBA::ULong offset) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c CORBA::TypeCode template methods specific to @c tk_sequence
       * @c TypeCodes.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;
      virtual CORBA::ULong length_i (void) const;
      virtual CORBA::TypeCode_ptr content_type_i (void) const;

    private:

      /// Element type of the sequence.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the @c
       * CORBA::TypeCode_ptr when creating this @c Field statically at
       * compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c TypeCode::Sequence.
       */
      TypeCodeType const content_type_;

      /// Length of the @c sequence or array.  A length of zero
      /// indicates an unbounded @c sequence.
      CORBA::ULong const length_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Sequence_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/AnyTypeCode/Sequence_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Sequence_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_SEQUENCE_TYPECODE_H */
