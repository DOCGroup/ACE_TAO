
// $Id$

#ifndef XMIT_H
#define XMIT_h

#include "Protocol_Task.h"

class ACE_SOCK_Stream;

class Xmit : public Protocol_Task
{
public:

    typedef Protocol_Task inherited;
    
    Xmit( ACE_SOCK_Stream & _peer );
    ~Xmit(void);

    int close(u_long flags);
    
protected:

    ACE_SOCK_Stream & peer(void)
        {
            return this->peer_;
        }

     int send(ACE_Message_Block *message,
              ACE_Time_Value *timeout);

private:
    ACE_SOCK_Stream & peer_;
};

#endif // XMIT_H
