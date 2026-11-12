#include "Options.h"
#include "File_Manager.h"
#include "BS_Client.h"
#include "ace/Log_Msg.h"
#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"
#include <cstring>

BS_Client::BS_Client ()
{
  this->count_ = FILE_MANAGER::instance ()->open_file (Options::friend_file);

  if (this->count_ < 0)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                Options::program_name));
  else
    {
      Protocol_Record *protocol_records {};
      ACE_NEW (protocol_records, Protocol_Record[this->count_]);
      this->protocol_record_.reset (protocol_records);

      Protocol_Record **sorted_records {};
      ACE_NEW (sorted_records, Protocol_Record *[this->count_]);
      this->sorted_record_.reset (sorted_records);

      for (int i = 0; i < this->count_; ++i)
        {
          Protocol_Record *prp = &this->protocol_record_[i];

          this->sorted_record_[i] = prp;

          FILE_MANAGER::instance ()->get_login_and_real_name
            (prp->key_name1_, prp->key_name2_);
        }

      ACE_OS::qsort (this->sorted_record_.get (),
                     this->count_,
                     sizeof (Protocol_Record *),
                     Binary_Search::name_compare);
    }
}

// This function is used to merge the KEY_NAME from server HOST_NAME
// into the sorted list of userids kept on the client's side.  Since
// we *know* we are going to find the name we use the traditional
// binary search.

Protocol_Record *
BS_Client::insert (char const *key_name, int)
{
  int lo = 0;
  int hi = this->count_ - 1;
  Protocol_Record **sorted_buffer = this->sorted_record_.get ();

  while (lo <= hi)
    {
      int mid   = (lo + hi) / 2;
      Protocol_Record *prp = sorted_buffer[mid];
      int cmp = std::strcmp (key_name,
                             prp->get_login ());
      if (cmp == 0)
        return prp;
      else if (cmp < 0)
        hi = mid - 1;
      else
        lo = mid + 1;
    }

  return nullptr;
}

Protocol_Record *
BS_Client::get_each_entry ()
{
  for (Protocol_Record *prp = Binary_Search::get_each_entry ();
       prp != nullptr;
       prp = Binary_Search::get_each_entry ())
    if (prp->get_drwho_list () != nullptr)
      return prp;

  return nullptr;
}
