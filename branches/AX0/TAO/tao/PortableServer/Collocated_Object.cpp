// $Id$
//

#include "Collocated_Object.h"
#include "Object_Adapter.h"

#include "tao/ORB_Core.h"
#include "tao/Stub.h"

#if !defined (__ACE_INLINE__)
# include "Collocated_Object.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Object, "$Id")

TAO_Collocated_Object::~TAO_Collocated_Object (void)
{
}
/*
TAO_ServantBase *
TAO_Collocated_Object::_servant (void) const
{
  return this->servant_;
}
*/
TAO_Collocated_Object *
TAO_Collocated_Object::_narrow (CORBA::Object_ptr object
                                ACE_ENV_ARG_DECL_NOT_USED)
{
  if (object == 0)
    return 0;

  ptr_arith_t type =
    ACE_reinterpret_cast(ptr_arith_t,TAO_Collocated_Object::_narrow);
  void *downcast =
    object->_tao_QueryInterface (type);

  return ACE_static_cast(TAO_Collocated_Object*,downcast);
}

CORBA::Boolean
TAO_Collocated_Object::_is_a (const CORBA::Char *logical_type_id
                              ACE_ENV_ARG_DECL)
{
  // If the object is collocated then try locally....
  if (!this->_is_collocated ())
    return this->CORBA_Object::_is_a (logical_type_id ACE_ENV_ARG_PARAMETER);

  if (this->_stubobj ()->type_id.in () != 0
      && ACE_OS::strcmp (logical_type_id,
                         this->_stubobj ()->type_id.in ()) == 0)
    return 1;

  TAO_Stub *stub = this->_stubobj ();
  // Which collocation strategy should we use?
  if (stub != 0 &&
      stub->servant_orb_var ()->orb_core ()
      ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
    {
      TAO_Object_Adapter::Servant_Upcall servant_upcall (
          stub->servant_orb_var ()->orb_core ()
        );

      CORBA::Object_var forward_to;
      servant_upcall.prepare_for_upcall (this->_object_key (),
                                         "_is_a",
                                         forward_to.out ()
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      return servant_upcall.servant ()->_is_a (logical_type_id ACE_ENV_ARG_PARAMETER);
    }

  // Direct collocation strategy is used.
  if (this->servant_ != 0)
    return this->servant_->_is_a (logical_type_id ACE_ENV_ARG_PARAMETER);

  // @@ Maybe we want to change this exception...
  ACE_THROW_RETURN (CORBA::INV_OBJREF (), 0);
}

CORBA::Boolean
TAO_Collocated_Object::_is_equivalent (CORBA_Object_ptr other_obj
                                       ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  CORBA::Boolean equivalent =
    this->CORBA_Object::_is_equivalent (other_obj ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (equivalent)
    return 1;

  ptr_arith_t type =
    ACE_reinterpret_cast(ptr_arith_t,TAO_Collocated_Object::_narrow);
  void *down =
    other_obj->_tao_QueryInterface (type);

  if (down == 0)
    return 0;

  TAO_Collocated_Object *rhs =
    ACE_static_cast(TAO_Collocated_Object*,down);

  return this->servant_ == rhs->servant_;
}

#if (TAO_HAS_MINIMUM_CORBA == 0)
CORBA::Boolean
TAO_Collocated_Object::_non_existent (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Boolean _tao_retval = 0;

  ACE_TRY
    {
      // If the object is collocated then try locally....
      if (!this->_is_collocated ())
        return this->CORBA_Object::_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);

      TAO_Stub *stub = this->_stubobj ();

      // Which collocation strategy should we use?
      if (stub != 0 &&
          stub->servant_orb_var ()->orb_core ()
          ->get_collocation_strategy () == TAO_ORB_Core::THRU_POA)
        {
          TAO_Object_Adapter::Servant_Upcall servant_upcall (
              stub->servant_orb_var ()->orb_core ()
            );

          CORBA::Object_var forward_to;
          servant_upcall.prepare_for_upcall (this->_object_key (),
                                             "_non_existent",
                                             forward_to.out ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
          return servant_upcall.servant ()->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
        }

      // Direct collocation strategy is used.
      if (this->servant_ != 0)
        return this->servant_->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);

      // @@ Maybe we want to change this exception...
      ACE_THROW_RETURN (CORBA::INV_OBJREF (), 0);
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, ex)
    {
      _tao_retval = 1;
    }
  ACE_CATCHANY
    {
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  return _tao_retval;
}
#endif /* TAO_HAS_MINIMUM_CORBA */

void*
TAO_Collocated_Object::_tao_QueryInterface (ptr_arith_t query_type)
{
  ptr_arith_t type =
    ACE_reinterpret_cast(ptr_arith_t,TAO_Collocated_Object::_narrow);

  if (query_type == type)
    {
      this->_add_ref ();
      return this;
    }

  return this->CORBA_Object::_tao_QueryInterface (query_type);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
