
// $Id$

#ifndef HANDLER_H
#define HANDLER_H

#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "Protocol_Stream.h"

class Handler : public ACE_Svc_Handler < ACE_SOCK_STREAM, ACE_NULL_SYNCH >
{

public:

    Handler(void);
    ~Handler(void);
    
    int open (void *);
    void destroy (void);
    int close (u_long);
    
protected:

    int handle_input (ACE_HANDLE);
    int handle_close(ACE_HANDLE, ACE_Reactor_Mask _mask);

private:
    Protocol_Stream stream_;

    Protocol_Stream & stream(void)
        {
            return this->stream_;
        }
};

#endif //  HANDLER_H
