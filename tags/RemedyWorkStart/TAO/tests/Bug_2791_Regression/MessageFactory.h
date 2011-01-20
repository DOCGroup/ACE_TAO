// $Id$

#include "tao/Valuetype/ValueFactory.h"
#include "Message_i.h"


class MessageFactory : public virtual CORBA::ValueFactoryBase
{
  public:

    static int destructorInvoked_;

    ~MessageFactory ();
    virtual CORBA::ValueBase *create_for_unmarshal ();
};
