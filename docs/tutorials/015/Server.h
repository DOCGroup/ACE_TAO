
// $Id$

#ifndef SERVER_H
#define SERVER_H

#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "Handler.h"

typedef ACE_Acceptor < Handler, ACE_SOCK_ACCEPTOR > Acceptor;


class Server
{
public:
    Server(void);
    ~Server(void);

    int open(void);
    static int close(void);

    int run(void);

private:
    Acceptor acceptor_;
    static sig_atomic_t finished_;
};

#endif //  SERVER_H
