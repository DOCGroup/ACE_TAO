// -*- C++ -*-

//=============================================================================
/**
 *  @file    Enum_TypeCode_Static.h
 *
 *  $Id$
 *
 *  Header file for a static @c tk_enum CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ENUM_TYPECODE_STATIC_H
#define TAO_ENUM_TYPECODE_STATIC_H

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
              class EnumeratorArrayType,
              class RefCountPolicy> class Enum;

    /**
     * @class Enum
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c enum.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c enum.
     */
    template<>
    class TAO_AnyTypeCode_Export Enum<char const *,
                          char const * const *,
                          TAO::Null_RefCount_Policy>
      : public CORBA::TypeCode,
        private TAO::Null_RefCount_Policy
    {
    public:

      /// Constructor.
      Enum (char const * id,
            char const * name,
            char const * const * enumerators,
            CORBA::ULong nenumerators);

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
       * @c tk_enum @c CORBA::TypeCode -specific template methods.
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
      //@}

    private:

      /**
       * @c Enum Attributes
       *
       * Attributes representing the structure of an OMG IDL
       * @c enum.
       *
       * @note These attributes are declared in the order in which
       *       they are marshaled into a CDR stream in order to
       *       increase cache hits by improving spatial locality.
       */
      //@{

      /// Base attributes containing repository ID and name of
      /// structure type.
      Base_Attributes<char const *> base_attributes_;

      /// The number of enumerators in the OMG IDL enumeration.
      CORBA::ULong const nenumerators_;

      /// Array of @c TAO::TypeCode enumerators representing
      /// enumerators in the OMG IDL defined @c enum.
      char const * const * const enumerators_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/Enum_TypeCode_Static.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_ENUM_TYPECODE_STATIC_H */
