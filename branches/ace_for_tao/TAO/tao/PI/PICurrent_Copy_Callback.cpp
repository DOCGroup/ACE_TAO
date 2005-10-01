#include "PICurrent_Copy_Callback.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "PICurrent_Impl.h"

ACE_RCSID (tao,
           PICurrent_Copy_Callback,
           "$Id$")


TAO::PICurrent_Copy_Callback::PICurrent_Copy_Callback (void)
  : src_ (0),
    dst_ (0)
{
}

TAO::PICurrent_Copy_Callback::~PICurrent_Copy_Callback (void)
{
  // Disable copy callbacks through this callback object in the source
  // PICurrent_Impl object to prevent the source PICurrent_Impl from
  // potentially calling back on a destructed copy callback object.
  if (this->src_ != 0)
    this->src_->copy_callback (0);
}

int
TAO::PICurrent_Copy_Callback::execute (void)
{
  if (this->src_ != 0 && this->dst_ != 0
      && this->src_ != this->dst_)
    {
      const PICurrent_Impl::Table & s = this->src_->current_slot_table ();

      // Disable use of the logically copied slot table before
      // performing the deep copy.
      this->dst_->lc_slot_table (0);

      PICurrent_Impl::Table & d = this->dst_->slot_table ();

      d = s;

      // Prevent subsequent deep copies, effectively disabling this
      // callback.
      this->src_->copy_callback (0);
    }

  return 0;
}

void
TAO::PICurrent_Copy_Callback::src_and_dst (PICurrent_Impl * src,
                                           PICurrent_Impl * dst)
{
  this->src_ = src;
  this->dst_ = dst;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
