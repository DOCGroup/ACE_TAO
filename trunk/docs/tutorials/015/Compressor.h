
// $Id$

#ifndef COMPRESSOR_H
#define COMPRESSOR_h

#include "Protocol_Task.h"

class ACE_SOCK_Stream;

class Compressor : public Protocol_Task
{
public:
    
    typedef Protocol_Task inherited;
    
    Compressor( int _thr_count = 0 );
    ~Compressor(void);

protected:

     int send(ACE_Message_Block *message,
              ACE_Time_Value *timeout);

     int recv(ACE_Message_Block *message,
              ACE_Time_Value *timeout);

private:
    mode_t mode_;
};

#endif // COMPRESSOR_H
