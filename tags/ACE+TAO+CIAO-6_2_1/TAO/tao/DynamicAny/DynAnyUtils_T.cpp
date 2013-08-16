// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynAnyUtils_T.cpp
 *
 *  $Id$
 *
 *  Implementation of templatized common code used in Dynamic Any
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_DYNANYUTILS_T_CPP
#define TAO_DYNANYUTILS_T_CPP

#include "tao/DynamicAny/DynAnyUtils_T.h"

#include "tao/DynamicAny/DynAny_i.h"
#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynValue_i.h"
#include "tao/DynamicAny/DynValueBox_i.h"

#include "tao/DynamicAny/DynAnyFactory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  template<typename T>
  void
  DynAnyBasicTypeUtils<T>::insert_value (const T &val,
                                         TAO_DynCommon *the_dynany)
  {
    if (the_dynany->destroyed ())
      {
        throw ::CORBA::OBJECT_NOT_EXIST ();
      }

    if (the_dynany->has_components ())
      {
        DynamicAny::DynAny_var cc = the_dynany->check_component ();
        TAO_DynCommon *dc = dynamic_cast<TAO_DynCommon *> (cc.in ());
        DynAnyBasicTypeUtils<T>::insert_value (val, dc);
      }
    else
      {
        the_dynany->check_type (BasicTypeTraits<T>::tc_value);
        CORBA::Any &my_any = the_dynany->the_any ();
        typedef typename TAO::BasicTypeTraits<T>::insert_type i_type;
        i_type insert_arg (val);
        my_any <<= insert_arg;
      }
  }

  template<typename T>
  typename BasicTypeTraits<T>::return_type
  DynAnyBasicTypeUtils<T>::get_value (TAO_DynCommon *the_dynany)
  {
    if (the_dynany->destroyed ())
      {
        throw ::CORBA::OBJECT_NOT_EXIST ();
      }

    if (the_dynany->has_components ())
      {
        DynamicAny::DynAny_var cc = the_dynany->check_component ();
        TAO_DynCommon *dc = dynamic_cast<TAO_DynCommon *> (cc.in ());
        return DynAnyBasicTypeUtils<T>::get_value (dc);
      }
    else
      {
        typedef typename BasicTypeTraits<T>::return_type ret_type;
        typedef typename BasicTypeTraits<T>::extract_type ext_type;
        ret_type retval = ret_type ();
        ext_type extval (retval);
        const CORBA::Any &my_any = the_dynany->the_any ();

        if (!(my_any >>= extval))
          {
            throw DynamicAny::DynAny::TypeMismatch ();
          }

        return BasicTypeTraits<T>::convert (extval);
      }
  }

  template<typename T>
  void
  DynAnyFlagUtils<T>::set_flag_t (DynamicAny::DynAny_ptr component,
                                  CORBA::Boolean destroying)
  {
    T *tmp = T::_narrow (component);

    if (destroying)
      {
        tmp->container_is_destroying (true);
      }
    else
      {
        tmp->ref_to_component (true);
      }
  }

  template<typename DA_IMPL, typename ANY_TC>
  DynamicAny::DynAny_ptr
  CreateDynAnyUtils<DA_IMPL, ANY_TC>::create_dyn_any_t (
    ANY_TC any_tc,
    CORBA::Boolean allow_truncation)
  {
    DA_IMPL *p = 0;
    ACE_NEW_THROW_EX (p,
                      DA_IMPL (allow_truncation),
                      CORBA::NO_MEMORY ());

    ACE_Auto_Basic_Ptr<DA_IMPL> dp (p);
    try
      {
        p->init (any_tc);
      }
    catch (DA_IMPL *original)
      {
        // Currently only TAO_DynValue_i can throw the original (duplicate
        // of a previously found TAO_DynValue_i). The new BLANK one created
        // above on which we called init() will be deleted automatically by
        // the ACE_Auto_Basic_Ptr.

        return original;
      }

    return dp.release ();
  }

  template<typename DA_IMPL, typename ANY_TC>
  DynamicAny::DynAny_ptr
  CreateDynAnyUtils<DA_IMPL, ANY_TC>::create_dyn_any_t (
    CORBA::TypeCode_ptr tc,
    ANY_TC any_tc,
    CORBA::Boolean allow_truncation)
  {
    DA_IMPL *p = 0;
    ACE_NEW_THROW_EX (p,
                      DA_IMPL (allow_truncation),
                      CORBA::NO_MEMORY ());

    ACE_Auto_Basic_Ptr<DA_IMPL> dp (p);
    try
      {
        p->init (tc, any_tc);
      }
    catch (DA_IMPL *original)
      {
        // Currently only TAO_DynValue_i can throw the original (duplicate
        // of a previously found TAO_DynValue_i). The new BLANK one created
        // above on which we called init() will be deleted automatically by
        // the ACE_Auto_Basic_Ptr.

        return original;
      }

    return dp.release ();
  }

  namespace MakeDynAnyUtils
  {
    template<typename ANY_TC>
    DynamicAny::DynAny_ptr
    make_dyn_any_t (
      CORBA::TypeCode_ptr tc,
      ANY_TC any_tc,
      CORBA::Boolean allow_truncation)
    {
      switch (TAO_DynAnyFactory::unalias (tc))
        {
        case CORBA::tk_null:
        case CORBA::tk_void:
        case CORBA::tk_short:
        case CORBA::tk_long:
        case CORBA::tk_ushort:
        case CORBA::tk_ulong:
        case CORBA::tk_float:
        case CORBA::tk_double:
        case CORBA::tk_longlong:
        case CORBA::tk_ulonglong:
        case CORBA::tk_longdouble:
        case CORBA::tk_boolean:
        case CORBA::tk_char:
        case CORBA::tk_wchar:
        case CORBA::tk_octet:
        case CORBA::tk_any:
        case CORBA::tk_TypeCode:
        case CORBA::tk_objref:
        case CORBA::tk_string:
        case CORBA::tk_wstring:
          return
            CreateDynAnyUtils<
              TAO_DynAny_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_struct:
        case CORBA::tk_except:
          return
            CreateDynAnyUtils<
              TAO_DynStruct_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_sequence:
          if (TAO_DynCommon::is_basic_type_seq (tc))
            {
              return
                CreateDynAnyUtils<
                  TAO_DynAny_i,
                  ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);
            }
          else
            {
              return
                CreateDynAnyUtils<
                  TAO_DynSequence_i,
                  ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);
            }

        case CORBA::tk_union:
          return
            CreateDynAnyUtils<
              TAO_DynUnion_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_enum:
          return
            CreateDynAnyUtils<
              TAO_DynEnum_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_array:
          return
            CreateDynAnyUtils<
              TAO_DynArray_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_value:
          return
            CreateDynAnyUtils<
              TAO_DynValue_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_value_box:
          return
            CreateDynAnyUtils<
              TAO_DynValueBox_i,
              ANY_TC>::create_dyn_any_t (any_tc, allow_truncation);

        case CORBA::tk_fixed:
        case CORBA::tk_abstract_interface:
        case CORBA::tk_component:
        case CORBA::tk_home:
          throw ::CORBA::NO_IMPLEMENT ();

        case CORBA::tk_native:
          throw DynamicAny::DynAnyFactory::InconsistentTypeCode ();

        default:
          break;
        }

      return DynamicAny::DynAny::_nil ();
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_DYNANYUTILS_T_CPP */
