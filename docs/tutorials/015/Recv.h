
// $Id$

#ifndef RECV_H
#define RECV_h

#include "Protocol_Task.h"

class ACE_SOCK_Stream;

class Recv : public Protocol_Task
{
public:

    typedef Protocol_Task inherited;
    
    Recv( ACE_SOCK_Stream & _peer );
    ~Recv(void);

    int get(void);
    
    int error(void)
        {
            return this->error_;
        }
    
protected:

    ACE_SOCK_Stream & peer(void)
        {
            return this->peer_;
        }

     int recv(ACE_Message_Block * message,
              ACE_Time_Value *timeout = 0);

private:
    ACE_SOCK_Stream & peer_;
    ACE_Message_Block * tickler_;
    int error_;
};

#endif // RECV_H
