// -*- C++ -*-
//$Id$

//
// GIOP_Message_Base
//
TAO_Queued_Data *
GIOP_Message_Base::make_queued_data (size_t sz)
{
  qd = TAO_Incoming_Message_Queue::get_queued_data ();

  ACE_Data_Block *db =
    this->orb_core_->data_block_for_message_block (sz);

  ACE_Allocator *alloc =
    this->orb_core_->message_block_msgblock_allocator ();

  ACE_Message_Block mb (db,
                        0,
                        alloc);

  ACE_Message_Block *new_mb = mb.duplicate ();

  qd.msg_block_ = new_mb;
  ACE_CDR::mb_align (new_mb);

  return qd;
}
