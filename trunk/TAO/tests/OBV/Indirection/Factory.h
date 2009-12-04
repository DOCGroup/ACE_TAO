#include "tao/Valuetype/ValueFactory.h"
#include "tao/ORB.h"
#include "MessengerC.h"

class NodeFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal(void);
};


class BoxedValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal(void);
};


class BaseValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal(void);
};


class TValueFactory : public CORBA::ValueFactoryBase
{
 public:
  static void register_new_factory(CORBA::ORB& orb);
  virtual CORBA::ValueBase* create_for_unmarshal(void);
};




