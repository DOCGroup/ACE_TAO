#include "ace/Guard_T.h"

#include "JAWS/FILE.h"


JAWS_FILE::JAWS_FILE ()
  : map_ ()
{
}

JAWS_FILE::~JAWS_FILE () = default;

ACE_Mem_Map *
JAWS_FILE::mem_map (int length,
                    int prot,
                    int share,
                    void *addr,
                    ACE_OFF_T offset,
                    LPSECURITY_ATTRIBUTES sa) const
{
  JAWS_FILE *mutable_this = const_cast<JAWS_FILE *> (this);
  return mutable_this->mem_map (length, prot, share, addr, offset, sa);
}

ACE_Mem_Map *
JAWS_FILE::mem_map (int length,
                    int prot,
                    int share,
                    void *addr,
                    ACE_OFF_T offset,
                    LPSECURITY_ATTRIBUTES sa)
{
  if (this->map_ == nullptr)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, g ,this->lock_, nullptr);

      if (this->map_ == nullptr)
        {
          this->map_ = std::make_unique<ACE_Mem_Map> ();
          if (this->map_ != nullptr)
            {
              int r = this->map_->map (this->get_handle (),
                                       static_cast<size_t> (length),
                                       prot,
                                       share,
                                       addr,
                                       offset,
                                       sa);
              if (r < 0)
                {
                  this->map_.reset ();
                }
            }
        }
    }

  return this->map_.get ();
}


ACE_Mem_Map *
JAWS_FILE::map () const
{
  return this->map_.get ();
}
