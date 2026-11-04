#ifndef INCLUDE_VALUETYPE_IMPL_H
#define INCLUDE_VALUETYPE_IMPL_H

#include "DynValue_TestC.h"

//============= Factory_id helper mix-in class ===========

class factory_id : public virtual CORBA::ValueFactoryBase
{
public:
  factory_id () = default;
  factory_id (factory_id const &) = delete;
  factory_id (factory_id &&) = delete;
  factory_id &operator= (factory_id const &) = delete;
  factory_id &operator= (factory_id &&) = delete;

  virtual char const * id () const = 0;
};

//============= "BaseValue" Value Type implimentation =======

class BaseValue_impl : public virtual OBV_DynValue_Test::BaseValue,
                       public virtual CORBA::DefaultValueRefCountBase
{
public:
  BaseValue_impl () = default;
  BaseValue_impl (CORBA::UShort us1, CORBA::UShort us2)
    : OBV_DynValue_Test::BaseValue (us1, us2) {}
  BaseValue_impl (BaseValue_impl const &) = delete;
  BaseValue_impl (BaseValue_impl &&) = delete;
  BaseValue_impl &operator= (BaseValue_impl const &) = delete;
  BaseValue_impl &operator= (BaseValue_impl &&) = delete;
  ~BaseValue_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
  void print () override;
};

class BaseValue_Factory : public factory_id
{
public:
  BaseValue_Factory () = default;
  BaseValue_Factory (BaseValue_Factory const &) = delete;
  BaseValue_Factory (BaseValue_Factory &&) = delete;
  BaseValue_Factory &operator= (BaseValue_Factory const &) = delete;
  BaseValue_Factory &operator= (BaseValue_Factory &&) = delete;
  char const * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

//============= "Nested" Value Type implimentation =======

class NestedValue_impl : public virtual OBV_DynValue_Test::NestedValue,
                         public virtual CORBA::DefaultValueRefCountBase
{
public:
  NestedValue_impl () = default;
  NestedValue_impl (CORBA::Short s1, CORBA::Short s2)
    : OBV_DynValue_Test::NestedValue (s1, s2) {}
  NestedValue_impl (NestedValue_impl const &) = delete;
  NestedValue_impl (NestedValue_impl &&) = delete;
  NestedValue_impl &operator= (NestedValue_impl const &) = delete;
  NestedValue_impl &operator= (NestedValue_impl &&) = delete;
  ~NestedValue_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
};

class NestedValue_Factory : public factory_id
{
public:
  NestedValue_Factory () = default;
  NestedValue_Factory (NestedValue_Factory const &) = delete;
  NestedValue_Factory (NestedValue_Factory &&) = delete;
  NestedValue_Factory &operator= (NestedValue_Factory const &) = delete;
  NestedValue_Factory &operator= (NestedValue_Factory &&) = delete;
  char const * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

//============= "Null" Value Type implimentation =======

class NullValue_impl : public virtual OBV_DynValue_Test::NullValue,
                       public virtual CORBA::DefaultValueRefCountBase
{
public:
  NullValue_impl () = default;
  NullValue_impl (NullValue_impl const &) = delete;
  NullValue_impl (NullValue_impl &&) = delete;
  NullValue_impl &operator= (NullValue_impl const &) = delete;
  NullValue_impl &operator= (NullValue_impl &&) = delete;
  ~NullValue_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
};

class NullValue_Factory : public factory_id
{
public:
  NullValue_Factory () = default;
  NullValue_Factory (NullValue_Factory const &) = delete;
  NullValue_Factory (NullValue_Factory &&) = delete;
  NullValue_Factory &operator= (NullValue_Factory const &) = delete;
  NullValue_Factory &operator= (NullValue_Factory &&) = delete;
  char const * id () const override;
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
  Trunc_impl (Trunc_impl const &) = delete;
  Trunc_impl (Trunc_impl &&) = delete;
  Trunc_impl &operator= (Trunc_impl const &) = delete;
  Trunc_impl &operator= (Trunc_impl &&) = delete;
  ~Trunc_impl () override = default;
  CORBA::ValueBase * _copy_value () override;
  void print () override;
};

class Trunc_Factory : public factory_id
{
public:
  Trunc_Factory () = default;
  Trunc_Factory (Trunc_Factory const &) = delete;
  Trunc_Factory (Trunc_Factory &&) = delete;
  Trunc_Factory &operator= (Trunc_Factory const &) = delete;
  Trunc_Factory &operator= (Trunc_Factory &&) = delete;
  char const * id () const override;
  CORBA::ValueBase * create_for_unmarshal () override;
};

#endif // INCLUDE_VALUETYPE_IMPL_H
