// -*- C++ -*-

//=============================================================================
/**
 *  @file    Enum_TypeCode.h
 *
 *  $Id$
 *
 *  Header file for a @c tk_enum CORBA::TypeCode.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_ENUM_TYPECODE_H
#define TAO_ENUM_TYPECODE_H

#include /**/ "ace/pre.h"

#include "tao/TypeCode.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TypeCode_Base_Attributes.h"


namespace TAO
{
  namespace TypeCode
  {
    template<typename StringType> struct Enumerator;

    /**
     * @class Enum
     *
     * @brief @c CORBA::TypeCode implementation for an OMG IDL
     *        @c enum.
     *
     * This class implements a @c CORBA::TypeCode for an OMG IDL
     * @c enum.
     */
    template <typename StringType,
              class EnumeratorArrayType,
              class RefCountPolicy>
    class Enum
      : public CORBA::TypeCode,
        private RefCountPolicy
    {
    public:

      /// Constructor.
      Enum (char const * id,
            char const * name,
            Enumerator<StringType> const * enumerators,
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
      virtual bool tao_marshal (TAO_OutputCDR & cdr) const;
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
      virtual CORBA::Boolean equal_i (CORBA::TypeCode_ptr tc
                                      ACE_ENV_ARG_DECL) const;
      virtual CORBA::Boolean equivalent_i (CORBA::TypeCode_ptr tc
                                           ACE_ENV_ARG_DECL) const;
      virtual CORBA::TCKind kind_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::TypeCode_ptr get_compact_typecode_i (
        ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * id_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * name_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual CORBA::ULong member_count_i (ACE_ENV_SINGLE_ARG_DECL) const;
      virtual char const * member_name_i (CORBA::ULong index
                                          ACE_ENV_ARG_DECL) const;
      //@}

    private:

      /// Get pointer to the underlying @c Enumerator array.
      Enumerator<StringType> const * enumerators (void) const;

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
      Base_Attributes<StringType> base_attributes_;

      /// The number of enumerators in the OMG IDL enumeration.
      CORBA::ULong const nenumerators_;

      /// Array of @c TAO::TypeCode enumerators representing
      /// enumerators in the OMG IDL defined @c enum.
      EnumeratorArrayType const enumerators_;

    };

  }  // End namespace TypeCode
}  // End namespace TAO


#ifdef __ACE_INLINE__
# include "tao/Enum_TypeCode.inl"
#endif  /* __ACE_INLINE__ */

#ifdef ACE_TEMPLATES_REQUIRE_SOURCE
# include "tao/Enum_TypeCode.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#ifdef ACE_TEMPLATES_REQUIRE_PRAGMA
# pragma implementation ("Enum_TypeCode.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */


#include /**/ "ace/post.h"

#endif /* TAO_ENUM_TYPECODE_H */
