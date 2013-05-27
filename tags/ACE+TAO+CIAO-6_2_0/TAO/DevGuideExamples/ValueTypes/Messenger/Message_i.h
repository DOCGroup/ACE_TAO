// $Id$

#ifndef MESSAGE_I_H
#define MESSAGE_I_H

#include "MessengerC.h"

#include "tao/Valuetype/ValueFactory.h"

class MessageImpl : public virtual OBV_Message,
                    public virtual CORBA::DefaultValueRefCountBase
{
public:
  MessageImpl();

  MessageImpl(
    const char* address,
    const char* user,
    const char* subject,
    const char* txt
    );

  MessageImpl(
    ::Message::AddrList &address,
    const char* user,
    const char* subject,
    const char* txt
    );

  virtual ::CORBA::ValueBase *_copy_value (void);

  virtual ::Message::AddrList* getAddresses();
  virtual void addAddress(const char*);

  virtual char* user();
  virtual void user(const char*);

  virtual char* subject();
  virtual void subject(const char*);

  virtual char* text();
  virtual void text(const char*);

  virtual void print();

protected:
  virtual ~MessageImpl();
};

class MessageFactory
  : public virtual CORBA::ValueFactoryBase
{
public:
  static void register_new_factory(CORBA::ORB& orb);

private:
  virtual CORBA::ValueBase* create_for_unmarshal();
};

#endif
