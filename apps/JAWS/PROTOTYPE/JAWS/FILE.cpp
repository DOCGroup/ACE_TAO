// $Id$

#include "JAWS/FILE.h"


JAWS_FILE::JAWS_FILE (void)
  : map_handle_ (ACE_INVALID_HANDLE),
    map_ (0)
{
}

JAWS_FILE::JAWS_FILE (ACE_HANDLE map_handle)
  : map_handle_ (map_handle),
    map_ (0)
{
}

ACE_Mem_Map *
JAWS_FILE::mem_map (void) const
{
  JAWS_FILE *mutable_this = (JAWS_FILE *) this;
  return mutable_this->mem_map ();
}

ACE_Mem_Map *
JAWS_FILE::mem_map (void)
{
  if (this->map_ == 0)
    {
      ACE_Guard<ACE_SYNCH_MUTEX> g (this->lock_);

      if (this->map_ == 0)
        {
          this->map_ = new ACE_Mem_Map;
          if (this->map_ != 0)
            {
              int r = this->map_->map (this->map_handle_);
              if (r < 0)
                {
                  delete this->map_;
                  this->map_ = 0;
                }
            }
        }
    }

  return this->map_;

}


ACE_Mem_Map *
JAWS_FILE::map (void) const
{
  return this->map_;
}
