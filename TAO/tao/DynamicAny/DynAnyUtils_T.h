// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynAnyUtils_T.h
 *
 *  $Id$
 *
 *  Collection of templatized common code used in Dynamic Any
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#include "tao/DynamicAny/DynAny_i.h"
#include "tao/DynamicAny/DynArray_i.h"
#include "tao/DynamicAny/DynEnum_i.h"
#include "tao/DynamicAny/DynSequence_i.h"
#include "tao/DynamicAny/DynStruct_i.h"
#include "tao/DynamicAny/DynUnion_i.h"
#include "tao/DynamicAny/DynAnyFactory.h"

#include "tao/AnyTypeCode/BasicTypeTraits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  // Encapsulates common code for inserting and extracting basic
  // types, parameterized on the basic type.
  template<typename T>
  struct DynAnyBasicTypeUtils
  {
    static void
    insert_value (const T &val,
                  TAO_DynCommon *the_dynany)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          DynamicAny::DynAny::TypeMismatch,
          DynamicAny::DynAny::InvalidValue
        ))
    {
      if (the_dynany->destroyed ())
        {
          ACE_THROW (CORBA::OBJECT_NOT_EXIST ());
        }

      if (the_dynany->has_components ())
        {
          DynamicAny::DynAny_var cc = the_dynany->check_component ();
          TAO_DynCommon *dc = dynamic_cast<TAO_DynCommon *> (cc.in ());
          TAO::DynAnyBasicTypeUtils<T>::insert_value (val, dc);
        }
      else
        {
          the_dynany->check_type (TAO::BasicTypeTraits<T>::tc_value);
          CORBA::Any &my_any = the_dynany->the_any ();
          typedef typename TAO::BasicTypeTraits<T>::insert_type i_type;
          my_any <<= i_type (val);
        }
    }

    static typename TAO::BasicTypeTraits<T>::return_type
    get_value (TAO_DynCommon *the_dynany)
      ACE_THROW_SPEC ((
          CORBA::SystemException,
          DynamicAny::DynAny::TypeMismatch,
          DynamicAny::DynAny::InvalidValue
        ))
    {
      if (the_dynany->destroyed ())
        {
          ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                            TAO::BasicTypeTraits<T>::return_type ());
        }

      if (the_dynany->has_components ())
        {
          DynamicAny::DynAny_var cc = the_dynany->check_component ();
          TAO_DynCommon *dc = dynamic_cast<TAO_DynCommon *> (cc.in ());
          return TAO::DynAnyBasicTypeUtils<T>::get_value (dc);
        }
      else
        {
          typedef typename TAO::BasicTypeTraits<T>::return_type ret_type;
          typedef typename TAO::BasicTypeTraits<T>::extract_type ext_type;
          ret_type retval = ret_type ();
          CORBA::Any &my_any = the_dynany->the_any ();

          if (!(my_any >>= ext_type (retval)))
            {
              ACE_THROW_RETURN (DynamicAny::DynAny::TypeMismatch (),
                                TAO::BasicTypeTraits<T>::return_type ());
            }

          return retval;
        }
    }
  };

  // Encapsulates code that would otherwise be repeated in
  // TAO_DynCommon::set_flag(). Parameterized on the type
  // of dynany impl class that underlies the DynAny arg.
  template<typename T>
  struct DynAnyFlagUtils
  {
    static void
    set_flag_t (DynamicAny::DynAny_ptr component,
                CORBA::Boolean destroying)
      ACE_THROW_SPEC ((
          CORBA::SystemException
        ))
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
  };

  // Used by MakeDynAnyUtils below, parameterized on the type of
  // impl class and on {Any | TypeCode}.
  template<typename DA_IMPL, typename ANY_TC>
  struct CreateDynAnyUtils
  {
    static DynamicAny::DynAny_ptr
    create_dyn_any_t (ANY_TC any_tc)
    {
      DA_IMPL *p = 0;
      ACE_NEW_THROW_EX (p,
                        DA_IMPL,
                        CORBA::NO_MEMORY ());

      ACE_Auto_Basic_Ptr<DA_IMPL> dp (p);
      p->init (any_tc);

      return dp.release ();
    }
  };

  // Code common to DynAnyFactory create_* calls, parameterized on
  // {Any | TypeCode}.
  template<typename ANY_TC>
  struct MakeDynAnyUtils
  {
    static DynamicAny::DynAny_ptr
    make_dyn_any_t (CORBA::TypeCode_ptr tc, ANY_TC any_tc)
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
              TAO::CreateDynAnyUtils<
                TAO_DynAny_i,
                ANY_TC>::create_dyn_any_t (any_tc);
          case CORBA::tk_struct:
          case CORBA::tk_except:
            return
              TAO::CreateDynAnyUtils<
                TAO_DynStruct_i,
                ANY_TC>::create_dyn_any_t (any_tc);
          case CORBA::tk_sequence:
            if (TAO_DynCommon::is_basic_type_seq (tc))
              {
                return
                  TAO::CreateDynAnyUtils<
                    TAO_DynAny_i,
                    ANY_TC>::create_dyn_any_t (any_tc);
              }
            else
              {
                return
                  TAO::CreateDynAnyUtils<
                    TAO_DynSequence_i,
                    ANY_TC>::create_dyn_any_t (any_tc);
              }
          case CORBA::tk_union:
            return
              TAO::CreateDynAnyUtils<
                TAO_DynUnion_i,
                ANY_TC>::create_dyn_any_t (any_tc);
          case CORBA::tk_enum:
            return
              TAO::CreateDynAnyUtils<
                TAO_DynEnum_i,
                ANY_TC>::create_dyn_any_t (any_tc);
          case CORBA::tk_array:
            return
              TAO::CreateDynAnyUtils<
                TAO_DynArray_i,
                ANY_TC>::create_dyn_any_t (any_tc);
        case CORBA::tk_fixed:
        case CORBA::tk_value:
        case CORBA::tk_value_box:
        case CORBA::tk_abstract_interface:
        case CORBA::tk_component:
        case CORBA::tk_home:
          ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                            DynamicAny::DynAny::_nil ());
        case CORBA::tk_native:
          ACE_THROW_RETURN (DynamicAny::DynAnyFactory::InconsistentTypeCode (),
                            DynamicAny::DynAny::_nil ());
        default:
          break;
        }

      return DynamicAny::DynAny::_nil ();
    }
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL
