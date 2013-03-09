// $Id$

#include "ValueTypes_impl.h"
#include "tao/AnyTypeCode/TypeCode.h"

//============= "BaseValue" Value Type implimentation =======

BaseValue_impl::~BaseValue_impl ()
{
}

CORBA::ValueBase *
BaseValue_impl::_copy_value ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (
    vb,
    BaseValue_impl (this->Base_us1 (), this->Base_us2 ()),
    CORBA::NO_MEMORY ());
  return vb;
}

void
BaseValue_impl::print ()
{
  ACE_DEBUG ((LM_DEBUG, "BaseValue (Base_us1 %u, Base_us2 %u)\n",
              this->Base_us1 (),
              this->Base_us2 () ));
}

const char * BaseValue_Factory::id () const
{
  return DynValue_Test::_tc_BaseValue->id ();
}

CORBA::ValueBase * BaseValue_Factory::create_for_unmarshal ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (vb, BaseValue_impl (), CORBA::NO_MEMORY ());
  return vb;
}

//============= "Nested" Value Type implimentation =======

NestedValue_impl::~NestedValue_impl ()
{
}

CORBA::ValueBase * NestedValue_impl::_copy_value ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (
    vb,
    NestedValue_impl (this->Nested_s1 (), this->Nested_s2 ()),
    CORBA::NO_MEMORY ());
  return vb;
}

const char * NestedValue_Factory::id () const
{
  return DynValue_Test::_tc_NestedValue->id ();
}

CORBA::ValueBase * NestedValue_Factory::create_for_unmarshal ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (vb, NestedValue_impl (), CORBA::NO_MEMORY ());
  return vb;
}

//============= "Null" Value Type implimentation =======

NullValue_impl::~NullValue_impl ()
{
}

CORBA::ValueBase * NullValue_impl::_copy_value ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (vb, NullValue_impl (), CORBA::NO_MEMORY ());
  return vb;
}

const char * NullValue_Factory::id () const
{
  return DynValue_Test::_tc_NullValue->id ();
}

CORBA::ValueBase * NullValue_Factory::create_for_unmarshal ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (vb, NullValue_impl (), CORBA::NO_MEMORY ());
  return vb;
}

//============= "Trunc" Value Type implimentation =======

Trunc_impl::~Trunc_impl ()
{
}

CORBA::ValueBase * Trunc_impl::_copy_value ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (
    vb,
    Trunc_impl (
      this->Base_us1(),
      this->Base_us2(),
      dynamic_cast<DynValue_Test::BoxedLong *>
        (this->Trunc_bl ()->_copy_value()),
      dynamic_cast<NestedValue_impl *>
        (this->Trunc_nested ()->_copy_value()),
      dynamic_cast<NullValue_impl   *>
        (this->Trunc_null1 ()->_copy_value()),
      dynamic_cast<NullValue_impl   *>
        (this->Trunc_null2 ()->_copy_value()),
      dynamic_cast<NullValue_impl   *>
        (this->Trunc_null3 ()->_copy_value())   ),
    CORBA::NO_MEMORY ());
  return vb;
}

void
Trunc_impl::print ()
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("BaseValue (Base_us1 %u, Base_us2 %u) ")
              ACE_TEXT ("Trunc (Trunc_bl (long %d), ")
              ACE_TEXT ("Trunc_nested (Nested_s1 %d, Nested_s2 %d), ")
              ACE_TEXT ("3xTrunc_null (%x %x %x))\n"),
              this->Base_us1 (),
              this->Base_us2 (),
              this->Trunc_bl()->_value (),
              this->Trunc_nested()->Nested_s1 (),
              this->Trunc_nested()->Nested_s2 (),
              (void *) this->Trunc_null1(),
              (void *) this->Trunc_null2(),
              (void *) this->Trunc_null3()  ));
}

const char * Trunc_Factory::id () const
{
  return DynValue_Test::_tc_Trunc->id ();
}

CORBA::ValueBase * Trunc_Factory::create_for_unmarshal ()
{
  CORBA::ValueBase *vb= 0;
  ACE_NEW_THROW_EX (vb, Trunc_impl (), CORBA::NO_MEMORY ());
  return vb;
}
