// -*- C++ -*-

//=============================================================================
/**
 *  @file    Alias_TypeCode_Static.h
 *
 *  $Id$
 *
 *  Header file for a static @c tk_alias and @c tk_value_box
 *  @c CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ALIAS_TYPECODE_STATIC_H
#define TAO_ALIAS_TYPECODE_STATIC_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/AnyTypeCode/TypeCode_Base_Attributes.h"
#include "tao/AnyTypeCode/Null_RefCount_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCode
  {
    template <typename StringType,
              typename TypeCodeType,
              class RefCountPolicy> class Alias;

    /**
     * @class Alias
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c typedef.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c typedef.
     */
    template<>
    class TAO_AnyTypeCode_Export Alias<char const *,
                           CORBA::TypeCode_ptr const *,
                           TAO::Null_RefCount_Policy>
      : public CORBA::TypeCode,
        private TAO::Null_RefCount_Policy
    {
    public:

      /// Constructor.
      Alias (CORBA::TCKind kind,
             char const * id,
             char const * name,
             CORBA::TypeCode_ptr const * tc);

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
       * @c tk_alias and @c tk_value_box @c CORBA::TypeCode -specific
       * template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;
      virtual char const * id_i (void) const;
      virtual char const * name_i (void) const;
      virtual CORBA::TypeCode_ptr content_type_i (void) const;

    private:

      /// Base attributes for this @c TypeCode containing the
      /// repository ID and name of the @c typedef.
      Base_Attributes<char const *> attributes_;

      /// The @c TypeCode corresponding to the original type upon
      /// which the IDL @c typedef was made.
      /**
       * A pointer to the @c CORBA::TypeCode_ptr rather than the
       * @c CORBA::TypeCode_ptr itself is stored since that address is
       * well-defined.  We may not know the value of the @c
       * CORBA::TypeCode_ptr when creating this @c Field statically at
       * compile-time, hence the indirection.
       *
       * @note This @c TypeCode is released upon destruction of this
       *       @c TypeCode::Alias.
       */
      CORBA::TypeCode_ptr const * content_type_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Alias_TypeCode_Static.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ALIAS_TYPECODE_STATIC_H */
