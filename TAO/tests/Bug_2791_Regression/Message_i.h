#include "MessageC.h"
#include "tao/corba.h"

class MessageImpl : public virtual OBV_Message,
                    public virtual CORBA::DefaultValueRefCountBase
{
  public:
    MessageImpl () = default;
    MessageImpl (char const *user, char const *subject, char const *text);
    MessageImpl (MessageImpl const &) = delete;
    MessageImpl &operator= (MessageImpl const &) = delete;
    MessageImpl (MessageImpl &&) = delete;
    MessageImpl &operator= (MessageImpl &&) = delete;

    ::CORBA::ValueBase *_copy_value () override;

    char *user () override;
    void user (char const *) override;

    char *subject () override;
    void subject (char const *) override;

    char *text () override;
    void text (char const *) override;

    void print () override;
};
