// $Id$
#include "Options.h"
#include "new.h"
#include "File_Manager.h"
#include "BS_Client.h"

BS_Client::BS_Client (void)
{
  this->count_ = File_Manager::open_file (Options::friend_file);

  if (this->count_ < 0)
    ACE_ERROR ((LM_ERROR, "%p\n"n, Options::program_name));
  else
    {
      this->protocol_record_ =
        new (PRIVATE_POOL) Protocol_Record[this->count_];
      this->sorted_record_ = 
        new (PRIVATE_POOL) Protocol_Record *[this->count_];
  
      for (int i = 0; i < this->count_; i++)
        {
          Protocol_Record *rec_ptr = &this->protocol_record_[i];
          this->sorted_record_[i] = rec_ptr;
          File_Manager::get_login_and_real_name (rec_ptr->key_name1_, 
                                                 rec_ptr->key_name2_);
        }
  
      ACE_OS::qsort (this->sorted_record_,
                     this->count_,
                     sizeof *this->sorted_record_,
                     (int (*)(const void *, const void *)) Binary_Search::name_compare);
    }
}

// This function is used to merge the KEY_NAME from server HOST_NAME
// into the sorted list of userids kept on the client's side.  Since
// we *know* we are going to find the name we use the traditional
// binary search.

Protocol_Record *
BS_Client::insert (char *key_name, int)
{
  int lo = 0;
  int hi = this->count_ - 1;
  Protocol_Record **sorted_buffer = this->sorted_record_;
  
  while (lo <= hi)
    {
      int mid 	= (lo + hi) / 2;
      Protocol_Record *frp = sorted_buffer[mid];
      int cmp = ACE_OS::strcmp (key_name,
                                frp->get_login ());
      
      if (cmp == 0)
	return frp;
      else if (cmp < 0)
	hi = mid - 1;
      else
	lo = mid + 1;
    }

  return 0;
}

Protocol_Record *
BS_Client::get_each_entry (void)
{
  Protocol_Record *frp;

  while ((frp = Binary_Search::get_each_entry ()) != 0)
    if (frp->get_drwho_list () != 0)
      return frp;

  return 0;
}
