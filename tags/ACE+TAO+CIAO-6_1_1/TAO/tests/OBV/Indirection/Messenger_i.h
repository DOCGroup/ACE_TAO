// $Id$

#ifndef MESSENGER_I_H
#define MESSENGER_I_H

#include "MessengerS.h"

#include "tao/Valuetype/ValueFactory.h"

class Messenger_i : public virtual POA_demo::value::idl::ValueServer
{
public:
  Messenger_i();

  virtual char * receive_boxedvalue (
    ::demo::value::idl::boxedValue * b);

  virtual char * receive_long (
    ::demo::value::idl::boxedLong * p1,
    ::demo::value::idl::boxedLong * p2);

  virtual char * receive_string (
    ::demo::value::idl::boxedString * s1,
    ::demo::value::idl::boxedString * s2);

  virtual char * receive_list (
    ::demo::value::idl::Node * node);

  virtual char * receive_truncatable (
    ::demo::value::idl::TValue *& v);

  virtual char * receive_sequence (
    const ::demo::value::idl::ConfigValues & v);

protected:
  virtual ~Messenger_i();
};




//class NodeImpl : public virtual POA_demo::value::idl::Node
//class NodeFactory
//  : public virtual CORBA::ValueFactoryBase
//{
//public:
//  static void register_new_factory(CORBA::ORB& orb);
//private:
//  virtual CORBA::ValueBase* create_for_unmarshal();
//};

#endif
