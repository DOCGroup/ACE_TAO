
// $Id$

#ifndef BARRIER_H
#define BARRIER_H

#include "ace/Synch.h"

class Barrier
{
public:
    Barrier(void);
    ~Barrier(void);

    int threads( u_int _threads);
    u_int threads(void);
    
    int wait(void);
    int done(void);
    
protected:
    ACE_Atomic_Op<ACE_Mutex,u_int> threads_;
    ACE_Barrier * barrier_;
    ACE_thread_t  owner_;

    int make_barrier(void);
};
    
#endif // BARRIER_H
