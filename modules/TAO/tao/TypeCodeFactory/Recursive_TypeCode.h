// -*- C++ -*-

//=============================================================================
/**
 *  @file    Recursive_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a intermediate recursive CORBA::TypeCode.
 *
 *  @author Ossama Othman
 */
//=============================================================================

#ifndef TAO_RECURSIVE_TYPECODE_H
#define TAO_RECURSIVE_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/True_RefCount_Policy.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace TypeCodeFactory
  {
    /**
     * @class Recursive_TypeCode
     *
     * @brief Recursive @c TypeCode placeholder.
     *
     * This class implements a placeholder for recursive TypeCodes.
     * It is meant solely for use as an intermediate TypeCode, and
     * merely forwards all operations to the actual recursive TypeCode
     * that is set by the TypeCodeFactory.
     *
     * @note This class serves a purpose different than the one served
     *       by the @c TAO::TypeCode::Recursive class.
     */
    class Recursive_TypeCode
      : public CORBA::TypeCode,
        private TAO::True_RefCount_Policy
    {
    public:

      /// Constructor.
      Recursive_TypeCode (char const * id);

      /// Set the actual recursive TypeCode to which all TypeCode
      /// operations will be forwarded.
      void the_typecode (CORBA::TypeCode_ptr tc);

      /**
       * @name TAO-specific @c CORBA::TypeCode Methods
       *
       * Methods required by TAO's implementation of the
       * @c CORBA::TypeCode class.
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual bool tao_marshal_kind (TAO_OutputCDR & cdr) const;
      virtual bool tao_marshal (TAO_OutputCDR &cdr, CORBA::ULong offset) const;
      virtual void tao_duplicate (void);
      virtual void tao_release (void);
      //@}

    protected:

      /**
       * @name TAO @c CORBA::TypeCode Template Methods
       *
       * Recursive type-capable (i.e. @c struct, @c union,
       * @c valuetype and @c eventtype) @c CORBA::TypeCode -specific
       * template methods
       *
       * @see @c CORBA::TypeCode
       */
      //@{
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (void) const;
      virtual char const * id_i (void) const;
      virtual char const * name_i (void) const;
      virtual CORBA::ULong member_count_i (void) const;
      virtual char const * member_name_i (CORBA::ULong index) const;
      virtual CORBA::TypeCode_ptr member_type_i (CORBA::ULong index) const;
      virtual CORBA::Any * member_label_i (CORBA::ULong index) const;
      virtual CORBA::TypeCode_ptr discriminator_type_i (void) const;
      virtual CORBA::Long default_index_i (void) const;
      virtual CORBA::Visibility member_visibility_i (CORBA::ULong index) const;
      virtual CORBA::ValueModifier type_modifier_i (void) const;
      virtual CORBA::TypeCode_ptr concrete_base_type_i (void) const;
      //@}

    private:

      /// Repository ID.
      CORBA::String_var id_;

      /// The actual recursive TypeCode.
      CORBA::TypeCode_var the_typecode_;
    };

  }  // End namespace TypeCodeFactory
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/TypeCodeFactory/Recursive_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_RECURSIVE_TYPECODE_H */
