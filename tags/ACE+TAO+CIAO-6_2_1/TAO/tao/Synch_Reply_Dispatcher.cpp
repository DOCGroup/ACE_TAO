// $Id$

#include "tao/Synch_Reply_Dispatcher.h"
#include "tao/ORB_Core.h"
#include "tao/Pluggable_Messaging_Utils.h"
#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Constructor.
TAO_Synch_Reply_Dispatcher::TAO_Synch_Reply_Dispatcher (
    TAO_ORB_Core *orb_core,
    IOP::ServiceContextList &sc
  )
  : reply_service_info_ (sc),
    orb_core_ (orb_core),
    db_ (sizeof buf_,
         ACE_Message_Block::MB_DATA,
         this->buf_,
         this->orb_core_->input_cdr_buffer_allocator (),
         this->orb_core_->locking_strategy (),
         ACE_Message_Block::DONT_DELETE,
         this->orb_core_->input_cdr_dblock_allocator ()),
    reply_cdr_ (&db_,
                ACE_Message_Block::DONT_DELETE,
                TAO_ENCAP_BYTE_ORDER,
                TAO_DEF_GIOP_MAJOR,
                TAO_DEF_GIOP_MINOR,
                orb_core)
{
  // As a TAO_LF_Event we start in the active state....
  this->state_changed_i (TAO_LF_Event::LFS_ACTIVE);
}

// Destructor.
TAO_Synch_Reply_Dispatcher::~TAO_Synch_Reply_Dispatcher (void)
{
}

TAO_InputCDR &
TAO_Synch_Reply_Dispatcher::reply_cdr (void)
{
  return this->reply_cdr_;
}

void
TAO_Synch_Reply_Dispatcher::reply_timed_out (void)
{
  // noop
}

int
TAO_Synch_Reply_Dispatcher::dispatch_reply (
    TAO_Pluggable_Reply_Params &params)
{
  if (params.input_cdr_ == 0)
    return -1;

  this->reply_status_ = params.reply_status ();
  this->locate_reply_status_ = params.locate_reply_status ();

  // Steal the buffer, that way we don't do any unnecesary copies of
  // this data.
  CORBA::ULong const max = params.svc_ctx_.maximum ();
  CORBA::ULong const len = params.svc_ctx_.length ();
  IOP::ServiceContext* context_list = params.svc_ctx_.get_buffer (true);
  this->reply_service_info_.replace (max, len, context_list, true);

  if (this->reply_service_info_.length() > 0)
    {
      orb_core_->service_context_registry ().
        process_service_contexts (this->reply_service_info_, *(params.transport_), 0);
    }

  // Must reset the message state, it is possible that the same reply
  // dispatcher is used because the request must be re-sent.
  // this->message_state_.reset (0);

  // Transfer the <params.input_cdr_>'s content to this->reply_cdr_
  if (ACE_BIT_DISABLED ((*params.input_cdr_).start()->data_block()->flags(),
                        ACE_Message_Block::DONT_DELETE))
  {
    // Data block is on the heap, so just duplicate it.
    this->reply_cdr_ = *params.input_cdr_;
    this->reply_cdr_.clr_mb_flags (ACE_Message_Block::DONT_DELETE);
  }
  else
  {
    ACE_Data_Block *db = this->reply_cdr_.clone_from (*params.input_cdr_);

    if (db == 0)
      {
        if (TAO_debug_level > 2)
          {
            TAOLIB_ERROR ((LM_ERROR,
                        "TAO (%P|%t) - Synch_Reply_Dispatcher::dispatch_reply "
                        "clone_from failed\n"));
          }
        return -1;
      }

    // See whether we need to delete the data block by checking the
    // flags. We cannot be happy that we initally allocated the
    // datablocks of the stack. If this method is called twice, as is in
    // some cases where the same invocation object is used to make two
    // invocations like forwarding, the release becomes essential.
    if (ACE_BIT_DISABLED (db->flags (),
                          ACE_Message_Block::DONT_DELETE))
      {
        db->release ();
      }
  }

  this->state_changed (TAO_LF_Event::LFS_SUCCESS,
                       this->orb_core_->leader_follower ());

  return 1;
}

void
TAO_Synch_Reply_Dispatcher::connection_closed (void)
{
  this->state_changed (TAO_LF_Event::LFS_CONNECTION_CLOSED,
                       this->orb_core_->leader_follower ());
}

TAO_END_VERSIONED_NAMESPACE_DECL
