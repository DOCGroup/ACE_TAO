// -*- C++ -*-
//$Id$

//
// GIOP_Message_Base
//
TAO_Queued_Data *
TAO_GIOP_Message_Base::make_queued_data (size_t sz)
{
  TAO_Queued_Data *qd =
    TAO_Queued_Data::get_queued_data ();

  ACE_Data_Block *db =
    this->orb_core_->data_block_for_message_block (sz);

  ACE_Allocator *alloc =
    this->orb_core_->message_block_msgblock_allocator ();

  ACE_Message_Block mb (db,
                        0,
                        alloc);

  ACE_Message_Block *new_mb = mb.duplicate ();

  ACE_CDR::mb_align (new_mb);

  qd->msg_block_ = new_mb;


  return qd;
}
