
// $Id$

#include "Barrier_i.h"
#include "ace/Task.h"

class Test : public ACE_Task<ACE_NULL_SYNCH>
{
public:
    Test(void);
    ~Test(void);

    int open(int _threads );
    int svc(void);

protected:
    Barrier barrier_;
};

Test::Test(void)
{
    ;
}

Test::~Test(void)
{
    ;
}

int Test::open( int _threads )
{
    barrier_.threads(_threads);
    return this->activate(THR_NEW_LWP, _threads);
}

int Test::svc(void)
{
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Entry\n"));
    
    ACE_RANDR_TYPE seed = ACE_OS::thr_self();
    ACE_OS::srand(seed);
    int delay;
    
    delay = ACE_OS::rand_r(seed)%5;
    ACE_OS::sleep(abs(delay)+1);
    
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Entering wait()\n"));

    barrier_.wait();
    
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Everybody together?\n"));

    delay = ACE_OS::rand_r(seed)%5;
    ACE_OS::sleep(abs(delay)+1);

    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Entering done()\n"));

    barrier_.done();
    
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Is everyone still here?\n"));
    
    delay = ACE_OS::rand_r(seed)%5;
    ACE_OS::sleep(abs(delay)+1);
    
    ACE_DEBUG ((LM_INFO, "(%P|%t|%T)\tTest::svc() Chaos and anarchy for all!\n"));

    return(0);
}

int main(int, char**)
{
    Test test;

    test.open(10);
    test.wait();

    test.open(5);
    test.wait();
    
    return(0);
}
