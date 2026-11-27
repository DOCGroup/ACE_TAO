#ifndef MESSAGE_I_H
#define MESSAGE_I_H

#include "MessengerC.h"

#include "tao/Valuetype/ValueFactory.h"

class MessageImpl : public virtual OBV_Message,
                    public virtual CORBA::DefaultValueRefCountBase
{
public:
  MessageImpl () = default;

  MessageImpl (
    char const *address,
    char const *user,
    char const *subject,
    char const *txt);

  MessageImpl (
    ::Message::AddrList &address,
    char const *user,
    char const *subject,
    char const *txt);

  ::CORBA::ValueBase *_copy_value () override;

  ::Message::AddrList *getAddresses () override;
  void addAddress (char const *address) override;

  char *user () override;
  void user (char const *value) override;

  char *subject () override;
  void subject (char const *value) override;

  char *text () override;
  void text (char const *value) override;

  void print () override;

  MessageImpl (MessageImpl const &) = delete;
  MessageImpl (MessageImpl &&) = delete;
  MessageImpl &operator= (MessageImpl const &) = delete;
  MessageImpl &operator= (MessageImpl &&) = delete;

protected:
  ~MessageImpl () override = default;
};

class MessageFactory
  : public virtual CORBA::ValueFactoryBase
{
public:
  MessageFactory () = default;

  static void register_new_factory (CORBA::ORB &orb);

  MessageFactory (MessageFactory const &) = delete;
  MessageFactory (MessageFactory &&) = delete;
  MessageFactory &operator= (MessageFactory const &) = delete;
  MessageFactory &operator= (MessageFactory &&) = delete;

private:
  CORBA::ValueBase *create_for_unmarshal () override;
};

#endif
