#ifndef INCLUDE_VALUETYPE_IMPL_H
#define INCLUDE_VALUETYPE_IMPL_H

#include "DynValue_TestC.h"

//============= Factory_id helper mix-in class ===========

class factory_id : public virtual CORBA::ValueFactoryBase
{
public:
  virtual const char * id () const = 0;
};

//============= "BaseValue" Value Type implimentation =======

class BaseValue_impl : public virtual OBV_DynValue_Test::BaseValue,
                       public virtual CORBA::DefaultValueRefCountBase
{
public:
  BaseValue_impl () = default;
  BaseValue_impl (CORBA::UShort us1, CORBA::UShort us2)
  : OBV_DynValue_Test::BaseValue (us1,us2) {}
  ~BaseValue_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
  void print () override;
};

class BaseValue_Factory : public factory_id
{
public:
  const char * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

//============= "Nested" Value Type implimentation =======

class NestedValue_impl : public virtual OBV_DynValue_Test::NestedValue,
                         public virtual CORBA::DefaultValueRefCountBase
{
public:
  NestedValue_impl () = default;
  NestedValue_impl (CORBA::Short s1, CORBA::Short s2)
  : OBV_DynValue_Test::NestedValue (s1,s2) {}
  ~NestedValue_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
};

class NestedValue_Factory : public factory_id
{
public:
  const char * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

//============= "Null" Value Type implimentation =======

class NullValue_impl : public virtual OBV_DynValue_Test::NullValue,
                       public virtual CORBA::DefaultValueRefCountBase
{
public:
  NullValue_impl () = default;
  ~NullValue_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
};

class NullValue_Factory : public factory_id
{
public:
  const char * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

//============= "Trunc" Value Type implimentation =======

class Trunc_impl : public virtual OBV_DynValue_Test::Trunc,
                   public virtual CORBA::DefaultValueRefCountBase
{
public:
  Trunc_impl () = default;
  Trunc_impl (
    CORBA::UShort us1,
    CORBA::UShort us2,
    DynValue_Test::BoxedLong *bl,
    NestedValue_impl *nv1,
    NullValue_impl *nv2,
    NullValue_impl *nv3,
    NullValue_impl *nv4)
  {
    this->Base_us1     (us1);
    this->Base_us2     (us2);
    this->Trunc_bl     (bl);
    this->Trunc_nested (nv1);
    this->Trunc_null1  (nv2);
    this->Trunc_null2  (nv3);
    this->Trunc_null3  (nv4);
  }
  ~Trunc_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
  void print () override;
};

class Trunc_Factory : public factory_id
{
public:
  const char * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

#endif // INCLUDE_VALUETYPE_IMPL_H
