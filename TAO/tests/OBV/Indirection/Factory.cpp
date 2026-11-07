#include "Factory.h"
#include "tao/AnyTypeCode/TypeCode.h"

void
NodeFactory::register_new_factory (CORBA::ORB& orb)
{
  CORBA::ValueFactoryBase_var const factory (new NodeFactory);
  CORBA::String_var const id = ::demo::value::idl::_tc_Node->id ();
  orb.register_value_factory (id.in (), factory.in ());
}

CORBA::ValueBase *
NodeFactory::create_for_unmarshal ()
{
  ::CORBA::ValueBase *ret_val {};
  ACE_NEW_THROW_EX (
      ret_val,
      OBV_demo::value::idl::Node,
      ::CORBA::NO_MEMORY ());
  return ret_val;
}

void
BoxedValueFactory::register_new_factory (CORBA::ORB& orb)
{
  CORBA::ValueFactoryBase_var const factory (new BoxedValueFactory);
  CORBA::String_var const id = demo::value::idl::_tc_boxedValue->id ();
  orb.register_value_factory (id.in (), factory.in ());
}

CORBA::ValueBase *
BoxedValueFactory::create_for_unmarshal ()
{
  ::CORBA::ValueBase *ret_val {};
  ACE_NEW_THROW_EX (
      ret_val,
      OBV_demo::value::idl::boxedValue,
      ::CORBA::NO_MEMORY ());
  return ret_val;
}

void
BaseValueFactory::register_new_factory (CORBA::ORB& orb)
{
  CORBA::ValueFactoryBase_var const factory (new BaseValueFactory);
  CORBA::String_var const id = demo::value::idl::_tc_BaseValue->id ();
  orb.register_value_factory (id.in (), factory.in ());
}

CORBA::ValueBase *
BaseValueFactory::create_for_unmarshal ()
{
  ::CORBA::ValueBase *ret_val {};
  ACE_NEW_THROW_EX (
      ret_val,
      OBV_demo::value::idl::BaseValue,
      ::CORBA::NO_MEMORY ());
  return ret_val;
}

void
TValueFactory::register_new_factory (CORBA::ORB& orb)
{
  CORBA::ValueFactoryBase_var const factory (new TValueFactory);
  CORBA::String_var const id = demo::value::idl::_tc_TValue->id ();
  orb.register_value_factory (id.in (), factory.in ());
}

CORBA::ValueBase *
TValueFactory::create_for_unmarshal ()
{
  ::CORBA::ValueBase *ret_val {};
  ACE_NEW_THROW_EX (
      ret_val,
      OBV_demo::value::idl::TValue,
      ::CORBA::NO_MEMORY ());
  return ret_val;
}

void
ConfigValueFactory::register_new_factory (CORBA::ORB& orb)
{
  CORBA::ValueFactoryBase_var const factory (new ConfigValueFactory);
  CORBA::String_var const id = demo::value::idl::_tc_ConfigValue->id ();
  orb.register_value_factory (id.in (), factory.in ());
}

CORBA::ValueBase *
ConfigValueFactory::create_for_unmarshal ()
{
  ::CORBA::ValueBase *ret_val {};
  ACE_NEW_THROW_EX (
      ret_val,
      OBV_demo::value::idl::ConfigValue,
      ::CORBA::NO_MEMORY ());
  return ret_val;
}

ConfigValueImpl::ConfigValueImpl (char const *name, char const *value)
{
  this->name (name);
  this->value () <<= value;
}
