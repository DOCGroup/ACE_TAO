// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//
// = FILENAME
//
// = DESCRIPTION
//
// = AUTHOR
//
// ============================================================================
#include <tao/corba.h>

TAO_MProfile::TAO_MProfile (CORBA::ULong sz)
  :  fwded_mprofile_(0),
     pfiles_ (0),
     current_ (0),
     size_ (sz),
     last_ (0)
{

  this->set (sz);

}

TAO_MProfile::TAO_MProfile (TAO_MProfile *mprofiles)
  :  fwded_mprofile_(0),
     pfiles_ (0),
     current_ (0),
     size_ (0),
     last_ (0)
{

  this->set (mprofiles);

}

int
TAO_MProfile::set (CORBA::ULong sz)
{
  // first release all of our profiles.
  if (this->pfiles_)
    {
      for (PHandle h = 0 ; h < this->size_ ; h++ )
      {
        if (this->pfiles_[h]) {
          this->pfiles_[h]->_decr_refcnt ();
          this->pfiles_[h] = 0;
        }
      }
  
      // next see if we can reuse our profile list memory
      // Since this->pfiles_ != 0, size_ > 0!  So if 
      // sz == 0 then memory is deallocated.
      if (this->size_ != sz)
      {
        // we cant!
        delete [] this->pfiles_;
  
        if (this->size_ == sz)
        {
          this->pfiles_ = new TAO_Profile_ptr [this->size_];
          ACE_OS::memset (this->pfiles_,0, sizeof(TAO_Profile_ptr)*this->size_);
        }
        else // do not allocate any memory!
          this->pfiles_ = 0;
      }
    }
  else
    {
      if (this->size_ == sz)
        {
          pfiles_ = new TAO_Profile_ptr [this->size_];
          ACE_OS::memset (this->pfiles_,0, sizeof(TAO_Profile_ptr)*this->size_);
        }
      else
        pfiles_ = 0;
    } // this->pfiles_

  this->last_ = 0;
  this->current_ = 0;
  this->fwded_mprofile_ = 0;

  return 1;
}
  
int
TAO_MProfile::set (TAO_MProfile *mprofile)
{
  // NOTE:  We use mprofile->last_ instead of mprofile->size_
  // to set this->size_.  This is so we can use set () to trim 
  // a profile list!!

  if (! mprofile)
    return this->set ((CORBA::ULong)0);

  this->set (mprofile->last_);

	  // set indexes ...
  this->last_ = mprofile->last_;
  
  // these ate set in set (ULong);
  // this->current_ = 0;
  // this->fwded_mprofile_ = 0;

  // now reference all profiles.
  for (PHandle h = 0 ; h < this->size_ ; h++ )
    {
      if (mprofile->pfiles_[h]) 
        {
          this->pfiles_[h] = mprofile->pfiles_[h];
          this->pfiles_[h]->_incr_refcnt ();
        }
    } // for (PHandle ...)

  if (mprofile->fwded_mprofile_)
    {
      this->fwded_mprofile_ = mprofile->fwded_mprofile_;
    }

  return 1;
}

TAO_MProfile::~TAO_MProfile (void)
{
  if (this->pfiles_)
    {
      for ( PHandle h = 0 ; h < size_ ; h++ )
        {
          if (this->pfiles_[h])
            this->pfiles_[h]->_decr_refcnt ();
        }
    }

  delete [] pfiles_;
  pfiles_ = 0;

}

// cyclic get next.  It will simply cycle through the complete list.
TAO_Profile *
TAO_MProfile::get_cnext (void)
{
  return (last_ == 0) ? 0 : ((current_ < last_) ? pfiles_ [current_++] : 
                                                  pfiles_ [(current_=1,0)]);
}

// this will return the next element until either null is found or the
// end of list.  It then continues to return NULL until rewind.
TAO_Profile *
TAO_MProfile::get_next (void)
{
  return (last_ == 0) ? 0 : ((current_ == last_) ? 0 : pfiles_ [current_++]);
}

TAO_Profile *
TAO_MProfile::get_cprev (void)
{
  return (last_ == 0) ? 0 : 
              ((last_ == 1) ? pfiles_ [current_=0] :
                    ((current_ > 1) ? pfiles_ [(--current_ - 1)] :
                                      pfiles_ [(current_=last_, last_-1)]));
}

TAO_Profile *
TAO_MProfile::get_prev (void)
{
  return (last_ == 0) ? 0 :
         ((current_ > 1) ? pfiles_ [--current_ - 1] : 
          (TAO_Profile *)(current_=0,0));
}

// does not affect the current_ setting!
TAO_Profile *
TAO_MProfile::get_profile (PHandle handle)
{
 return (handle >= 0 && handle < last_) ?  pfiles_ [handle] : 0;
}
  
TAO_Profile *
TAO_MProfile::get_current_profile (void)
{
  if (last_ == 0) 
    return 0;
  return (current_ == 0) ? pfiles_ [current_] : pfiles_ [current_-1];
}

PHandle 
TAO_MProfile::get_current_handle (void)
{
  return (current_ > 0) ? (current_ - 1) : 0;
  // the last profile returned!
}

void 
TAO_MProfile::rewind (void)
{
  current_ = 0;
}

int 
TAO_MProfile::add_profile (TAO_Profile *pfile)
{
  // skip by the used slots
  if (last_ == size_) // full!
    return -1;
   
  pfiles_ [last_++] = pfile;

  if (pfile && pfile->_incr_refcnt () == 0) 
    {
      ACE_ERROR_RETURN ((LM_ERROR, 
             "(%P|%t) Unable to increment reference count in add_profile!\n"), 
             -1);
    }

  return last_ - 1;
}

void
TAO_MProfile::fwded_mprofile (TAO_MProfile *fwded)
{
  this->fwded_mprofile_ = fwded;
}

TAO_MProfile *
TAO_MProfile::fwded_mprofile (void)
{
  return this->fwded_mprofile_;
}

CORBA::ULong
TAO_MProfile::profile_count (void)
{
  return this->last_;
}

TAO_Profile_ptr *
TAO_MProfile::pfiles (void) const
{
  return this->pfiles_;
}

CORBA::Boolean
TAO_MProfile::is_equivalent (TAO_MProfile *first, 
                             TAO_MProfile *second,
                             CORBA::Environment &env)
{
  // Two profile lists are equivalent iff at least one of the 
  // profiles form the first list is_equivalent to at least one
  // of the profiles from the second list!!
  TAO_Profile_ptr *pfiles1 = first->pfiles ();
  TAO_Profile_ptr *pfiles2 = second->pfiles ();
  int first_cnt = first->profile_count ();
  int second_cnt = second->profile_count ();
  
  for (PHandle h1 = 0; h1 < first_cnt;h1++)
    {
      for (PHandle h2 = 0; h2 < second_cnt; h2++ )
        {
          if (pfiles1[h1]->is_equivalent (pfiles2[h2], env))
            return 1;
        }
    }
  
  return 0;
}

CORBA::ULong
TAO_MProfile::hash (CORBA::ULong max, CORBA::Environment &env)
{

  CORBA::ULong hashval=0;

  if (last_ == 0) return 0;

  for (PHandle h=0; h < last_ ; h++) 
    {
      hashval += pfiles_[h]->hash (max, env);
    }

  // the above hash function return an ULong between 0 and max
  // here we simply take the average value and round
  return hashval / last_; 

}
