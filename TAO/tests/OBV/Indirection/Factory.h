#include "tao/Valuetype/ValueFactory.h"
#include "tao/ORB.h"
#include "MessengerC.h"

class NodeFactory : public CORBA::ValueFactoryBase
{
 public:
  NodeFactory () = default;
  NodeFactory (NodeFactory const&) = delete;
  NodeFactory (NodeFactory&&) = delete;
  NodeFactory& operator= (NodeFactory const&) = delete;
  NodeFactory& operator= (NodeFactory&&) = delete;

  static void register_new_factory (CORBA::ORB& orb);
  CORBA::ValueBase *create_for_unmarshal () override;
};


class BoxedValueFactory : public CORBA::ValueFactoryBase
{
 public:
  BoxedValueFactory () = default;
  BoxedValueFactory (BoxedValueFactory const&) = delete;
  BoxedValueFactory (BoxedValueFactory&&) = delete;
  BoxedValueFactory& operator= (BoxedValueFactory const&) = delete;
  BoxedValueFactory& operator= (BoxedValueFactory&&) = delete;

  static void register_new_factory (CORBA::ORB& orb);
  CORBA::ValueBase *create_for_unmarshal () override;
};


class BaseValueFactory : public CORBA::ValueFactoryBase
{
 public:
  BaseValueFactory () = default;
  BaseValueFactory (BaseValueFactory const&) = delete;
  BaseValueFactory (BaseValueFactory&&) = delete;
  BaseValueFactory& operator= (BaseValueFactory const&) = delete;
  BaseValueFactory& operator= (BaseValueFactory&&) = delete;

  static void register_new_factory (CORBA::ORB& orb);
  CORBA::ValueBase *create_for_unmarshal () override;
};


class TValueFactory : public CORBA::ValueFactoryBase
{
 public:
  TValueFactory () = default;
  TValueFactory (TValueFactory const&) = delete;
  TValueFactory (TValueFactory&&) = delete;
  TValueFactory& operator= (TValueFactory const&) = delete;
  TValueFactory& operator= (TValueFactory&&) = delete;

  static void register_new_factory (CORBA::ORB& orb);
  CORBA::ValueBase *create_for_unmarshal () override;
};

class ConfigValueFactory : public CORBA::ValueFactoryBase
{
 public:
  ConfigValueFactory () = default;
  ConfigValueFactory (ConfigValueFactory const&) = delete;
  ConfigValueFactory (ConfigValueFactory&&) = delete;
  ConfigValueFactory& operator= (ConfigValueFactory const&) = delete;
  ConfigValueFactory& operator= (ConfigValueFactory&&) = delete;

  static void register_new_factory (CORBA::ORB& orb);
  CORBA::ValueBase *create_for_unmarshal () override;
};

class ConfigValueImpl : public ::OBV_demo::value::idl::ConfigValue
{
public:
  ConfigValueImpl (char const *name, char const *value);
};

