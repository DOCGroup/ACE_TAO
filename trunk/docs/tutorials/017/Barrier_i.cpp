
// $Id$

#include "Barrier_i.h"

Barrier::Barrier(void)
   : threads_(0)
    ,barrier_(0)
{
    owner_ = ACE_OS::thr_self();
}

Barrier::~Barrier(void)
{
    this->done();
}

u_int Barrier::threads(void)
{
    return threads_.value();
}

int Barrier::threads( u_int _threads )
{
    if( ACE_OS::thr_self() != owner_ )
    {
        return -1;
    }

    threads_ = _threads;
    
    return make_barrier();
}

int Barrier::wait(void)
{
    if( ! barrier_ )
    {
        return -1;
    }

    return barrier_->wait();
}

int Barrier::done(void)
{
    if( this->wait() == -1 )
    {
        return -1;
    }

    --threads_;

    if( ! threads_.value() )
    {
        delete barrier_;
        barrier_ = 0;
    }

    return 0;
}

int Barrier::make_barrier(void)
{
    if( barrier_ )
    {
        barrier_->wait();
        delete barrier_;
    }

    if( ! threads_.value() )
    {
        return -1;
    }
        
    ACE_NEW_RETURN(barrier_,ACE_Barrier(threads_.value()),-1);

    return 0;
}
