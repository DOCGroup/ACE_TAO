#include "Options.h"
#include "Binary_Search.h"
#include "ace/Log_Msg.h"
#include <cstring>

// This function is passed to qsort to perform the comparison between
// login names for two friends.

int
Binary_Search::name_compare (void const *s1, void const *s2)
{
  Protocol_Record * const *left = static_cast<Protocol_Record * const *> (s1);
  Protocol_Record * const *right = static_cast<Protocol_Record * const *> (s2);
  return std::strcmp ((*left)->key_name1_, (*right)->key_name1_);
}

// Returns the next friend in the sequence of sorted friends.  Note
// that this function would be simplified if we expanded the iterator
// interface to include an "initialize" and "next" function!

Protocol_Record *
Binary_Search::get_next_entry ()
{
  // Reset the iterator if we are starting from the beginning.

  if (this->current_ptr_ == nullptr)
    this->current_ptr_ = this->sorted_record_.get ();

  // Now check to see if we've hit the end, in which case we set
  // things up for the next round!

  if (this->current_ptr_ < this->sorted_record_.get () + this->count_)
    return *this->current_ptr_++;
  else
    {
      this->current_ptr_ = nullptr;
      return nullptr;
    }
}

// An iterator, similar to Binary_Search::get_next_friend, though in
// this case the friend records are returned in the order they
// appeared in the friend file, rather than in sorted order.  Also, we
// skip over entries that don't have any hosts associated with them.

Protocol_Record *
Binary_Search::get_each_entry ()
{
  // Reset the iterator if we are starting from the beginning.

  if (this->current_index_ == -1)
    this->current_index_ = 0;

  // Now check to see if we've hit the end, in which case we set
  // things up for the next round!

  for (;
       this->current_index_ < this->count_;
       ++this->current_index_)
    if (this->protocol_record_[this->current_index_].drwho_list_ != nullptr)
      return &this->protocol_record_[this->current_index_++];

  this->current_index_ = -1;
  return nullptr;
}

Binary_Search::~Binary_Search ()
{
  if (Options::get_opt (Options::DEBUGGING))
    ACE_DEBUG ((LM_DEBUG,
                "disposing Binary_Search\n"));
}

// Used to initialize the values for the iterators...

Binary_Search::Binary_Search () = default;
