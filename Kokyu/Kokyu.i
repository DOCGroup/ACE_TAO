// $Id$


namespace Kokyu
{

ACE_INLINE
Dispatch_Command::Dispatch_Command (int dont_delete)
        :dont_delete_ (dont_delete)
{
}

ACE_INLINE
int Dispatch_Command::can_be_deleted (void) const
{
  return !dont_delete_;
}

ACE_INLINE
void Dispatch_Command::destroy (void)
{
   delete this;
}

ACE_INLINE
Dispatch_Command::~Dispatch_Command (void)
{
}


}
