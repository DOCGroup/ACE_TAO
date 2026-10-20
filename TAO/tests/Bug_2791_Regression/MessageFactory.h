#include "tao/Valuetype/ValueFactory.h"
#include "Message_i.h"


class MessageFactory : public virtual CORBA::ValueFactoryBase
{
  public:
    MessageFactory () = default;
    MessageFactory (MessageFactory const &) = delete;
    MessageFactory &operator= (MessageFactory const &) = delete;
    MessageFactory (MessageFactory &&) = delete;
    MessageFactory &operator= (MessageFactory &&) = delete;

    static int destructorInvoked_;

    ~MessageFactory () override;
    CORBA::ValueBase *create_for_unmarshal () override;
};
