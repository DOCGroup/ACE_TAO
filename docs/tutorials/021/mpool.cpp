
// $Id$ 

#include "mpool.h"

const int Constants::SEM_KEY_1 = ACE_DEFAULT_SEM_KEY + 1;
const int Constants::SEM_KEY_2 = ACE_DEFAULT_SEM_KEY + 2;

const int Constants::SHMSZ = 27;
const char * Constants::SHMDATA = "abcdefghijklmnopqrstuvwxyz";

const char * Constants::PoolName = "SharedMemoryPool";
const char * Constants::RegionName = "Alphabet";

Allocator::Allocator( const char * _name )
        : name_(ACE_OS::strdup(_name)),
          pool_(0)
{
    if( ! name_ )
    {
        ACE_ERROR ((LM_ERROR, "(%P) %p",
                    "Allocator::Allocator cannot strdup pool name" ));
    }
}

Allocator::~Allocator(void)
{
    if( pool_ )
    {
        free(pool_);
    }
}

Allocator::pool_t & Allocator::pool(void)
{
    if( ! pool_ )
    {
        pool_ = new pool_t( name_ );
    }

        // Obviously, we're assuming that the new above was
        // successful.

    return *pool_;
}
