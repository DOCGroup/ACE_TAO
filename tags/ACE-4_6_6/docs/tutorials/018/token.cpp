
// $Id$

// Get our two Test derivatives...
#include "Token_i.h"
#include "Mutex_i.h"

int main(int,char**)
{
        // See what an ACE_Token does for us.
    Token token;
    token.run();

        // And now and ACE_Mutex.
    Mutex mutex;
    mutex.run();
    
    return(0);
}
