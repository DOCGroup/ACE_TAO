// -*- C++ -*-

//=============================================================================
/**
 *  @file    Indirected_Type_TypeCode.h
 *
 *  Header file for a indirected recursive type (@c struct, @c union or
 *  @c valuetype) CORBA::TypeCode.
 *
 *  @author Martin Corino
 */
//=============================================================================

#ifndef TAO_INDIRECTED_TYPE_TYPECODE_H
#define TAO_INDIRECTED_TYPE_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

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
     * @class Indirected_Type
     *
     * @brief Recursive type @c TypeCode indirection template.
     *
     * This class template delegates to the indirected @c TypeCode
     * implementation @a Recursive_Type to provide support for IDL
     * defined recursive types.  Only an IDL @c struct, @c union or
     * @c valuetype may be recursive.
     */
    class TAO_AnyTypeCode_Export Indirected_Type
      : public CORBA::TypeCode
    {
    public:
      /// Constructor.
      Indirected_Type (CORBA::TCKind kind,
                       char const * id);

      /// Constructor for TypeCode factory.
      Indirected_Type (char const * id);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      bool tao_marshal_kind (TAO_OutputCDR & cdr) const override;
      bool tao_marshal (TAO_OutputCDR & cdr, CORBA::ULong offset) const override;
      void tao_duplicate () override;
      void tao_release () override;
      //@}

      /// Setter for recursive reference.
      void set_recursive_tc (CORBA::TypeCode_ptr);

    protected:
      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * @c tk_struct, @c tk_union, @c tk_value and @c tk_event
       * @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const override;
      CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const override;
      CORBA::TypeCode_ptr get_compact_typecode_i () const override;
      char const * id_i () const override;
      char const * name_i () const override;
      CORBA::ULong member_count_i () const override;
      char const * member_name_i (CORBA::ULong index) const override;
      CORBA::TypeCode_ptr member_type_i (CORBA::ULong index) const override;
      //@}

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * Additional @c tk_union
       * @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      CORBA::Any * member_label_i (CORBA::ULong index) const override;
      CORBA::TypeCode_ptr discriminator_type_i () const override;
      CORBA::Long default_index_i () const override;
      //@}

      /**
       * @name @c TAO CORBA::TypeCode Template Methods
       *
       * Additional @c tk_value and @c tk_event
       * @c CORBA::TypeCode -specific template methods.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      CORBA::Visibility member_visibility_i (CORBA::ULong index) const override;
      CORBA::ValueModifier type_modifier_i () const override;
      CORBA::TypeCode_ptr concrete_base_type_i () const override;
      //@}

    private:
      //@{
      /// Typecode id
      CORBA::String_var id_;

      /// Reference of recursive typecode instance.
      CORBA::TypeCode_ptr recursive_tc_;

      /// Lock
      TAO_SYNCH_MUTEX mutex_;

      /// Reference counter
      unsigned long refcount_;

      //@}
    };
  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL


#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Indirected_Type_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_INDIRECTED_TYPE_TYPECODE_H */
