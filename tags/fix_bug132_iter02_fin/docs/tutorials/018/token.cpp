
// $Id$

// Get our two Test derivatives...
#include "Token_i.h"
#include "Mutex_i.h"

int main(int,char**)
{
        // See what an ACE_Token does for us.
    Token token;
    token.run();

        // And now the ACE_Mutex.
    Mutex mutex;
    mutex.run();

    return(0);
}
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class Test_T <ACE_Mutex>;
template class Test_T <ACE_Token>;
template class ACE_Atomic_Op <ACE_Mutex, int>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate Test_T <ACE_Mutex>
#pragma instantiate Test_T <ACE_Token>
#pragma instantiate ACE_Atomic_Op <ACE_Mutex, int>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */




