#include "tao/Valuetype/ValueFactory.h"
#include "tao/ORB.h"
#include "MessengerC.h"

class NodeFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal();
};


class BoxedValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal();
};


class BaseValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal();
};


class TValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal();
};

class ConfigValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal();
};

class ConfigValueImpl : public ::OBV_demo::value::idl::ConfigValue
{
public:
  ConfigValueImpl (const char* name, const char* value);
};

