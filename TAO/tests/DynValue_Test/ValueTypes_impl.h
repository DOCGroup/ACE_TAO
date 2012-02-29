// $Id$

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
  BaseValue_impl () {}
  BaseValue_impl (CORBA::UShort us1, CORBA::UShort us2)
  :BaseValue (us1,us2) {}
  virtual ~BaseValue_impl ();
  virtual CORBA::ValueBase * _copy_value (void);
  virtual void print ();
};

class BaseValue_Factory : public factory_id
{
public:
  const char * id () const;
  virtual CORBA::ValueBase * create_for_unmarshal ();
};

//============= "Nested" Value Type implimentation =======

class NestedValue_impl : public virtual OBV_DynValue_Test::NestedValue,
                         public virtual CORBA::DefaultValueRefCountBase
{
public:
  NestedValue_impl () {}
  NestedValue_impl (CORBA::Short s1, CORBA::Short s2)
  :NestedValue (s1,s2) {}
  virtual ~NestedValue_impl ();
  virtual CORBA::ValueBase * _copy_value (void);
};

class NestedValue_Factory : public factory_id
{
public:
  const char * id () const;
  virtual CORBA::ValueBase * create_for_unmarshal ();
};

//============= "Null" Value Type implimentation =======

class NullValue_impl : public virtual OBV_DynValue_Test::NullValue,
                       public virtual CORBA::DefaultValueRefCountBase
{
public:
  NullValue_impl () {}
  virtual ~NullValue_impl ();
  virtual CORBA::ValueBase * _copy_value (void);
};

class NullValue_Factory : public factory_id
{
public:
  const char * id () const;
  virtual CORBA::ValueBase * create_for_unmarshal ();
};

//============= "Trunc" Value Type implimentation =======

class Trunc_impl : public virtual OBV_DynValue_Test::Trunc,
                   public virtual CORBA::DefaultValueRefCountBase
{
public:
  Trunc_impl () {}
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
  virtual ~Trunc_impl ();
  virtual CORBA::ValueBase * _copy_value ();
  virtual void print ();
};

class Trunc_Factory : public factory_id
{
public:
  const char * id () const;
  virtual CORBA::ValueBase * create_for_unmarshal ();
};

#endif // INCLUDE_VALUETYPE_IMPL_H
