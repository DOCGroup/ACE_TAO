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

#include "tao/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

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
    template <class RefCountPolicy>
    class Sequence : public CORBA::TypeCode
      : private RefCountPolicy
    {
    public:

      /// Constructor.
      Sequence (CORBA::TCKind kind,
             CORBA::ULong length,
             CORBA::TypeCode_ptr * tc);

      /// Destructor.
      ~Sequence (void);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
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
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::ULong length_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr content_type (ACE_ENV_SINGLE_ARG_DECL) const;

    private:

      /// The kind of this @c TypeCode.
      /**
       * @c kind_ is either @c CORBA::tk_sequence or
       * @c CORBA::tk_array.
       */
      CORBA::TCKind const kind_;

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
      CORBA::TypeCode_ptr * content_type_;

      /// Length of the @c sequence or array.  A length of zero
      /// indicates an unbounded @c sequence.
      CORBA::ULong const length_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Sequence_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_SEQUENCE_TYPECODE_H */
