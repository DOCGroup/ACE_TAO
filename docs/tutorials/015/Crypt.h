
// $Id$

#ifndef CRYPT_H
#define CRYPT_h

#include "Protocol_Task.h"

class ACE_SOCK_Stream;

class Crypt : public Protocol_Task
{
public:

    typedef Protocol_Task inherited;
    
    Crypt( int _thr_count = 0 );
    ~Crypt(void);

protected:

     int send(ACE_Message_Block *message,
              ACE_Time_Value *timeout);

     int recv(ACE_Message_Block *message,
              ACE_Time_Value *timeout);
private:
};

#endif // CRYPT_H
