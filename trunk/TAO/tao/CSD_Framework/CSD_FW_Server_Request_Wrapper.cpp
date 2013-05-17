// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_FW_Server_Request_Wrapper.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/ORB_Core.h"
#include "tao/Transport.h"
#include "tao/CDR.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_FW_Server_Request_Wrapper.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::FW_Server_Request_Wrapper::~FW_Server_Request_Wrapper()
{
  // Only delete the request if we cloned it.
  if (this->is_clone_)
    {
      //  The TAO_Tagged_Profile type_id_ may have been duplicated.
      if (this->request_->profile_.type_id_ != 0)
        CORBA::string_free (
                 const_cast<char*> (this->request_->profile_.type_id_));

      //  Since this TAO_ServerRequest object is a clone, it
      //              "owns" the input and output CDR objects held by the
      //              incoming_ and outgoing_ data members, respectfully.
      //              Thus, for the clone case, the TAO_ServerRequest dtor
      //              needs to release (aka, delete) the CDR objects.
      delete this->request_->incoming_;

      //  Get the start message block that reference to the data allocated
      //  on the heap.
      if (this->request_->outgoing_ != 0)
        {
          char* buffer = this->request_->outgoing_->begin ()->base ();
          delete [] buffer;
          delete this->request_->outgoing_;
        }
      if (this->request_->operation_details_ != 0)
        {
          char* opname =
            const_cast<char*> (this->request_->operation_details_->opname_);
          delete [] opname;

          if (this->request_->operation_details_->num_args_ > 0)
            {
              for (CORBA::ULong i = 0;
                   i < this->request_->operation_details_->num_args_; i++)
                {
                  delete this->request_->operation_details_->args_[i];
                }

              delete [] this->request_->operation_details_->args_;
            }

          delete this->request_->operation_details_;
        }

      if (this->request_->transport_ != 0)
        this->request_->transport_->remove_reference ();

      delete this->request_;
    }
}


// Assumes that the servant argument is not a NULL pointer.
void
TAO::CSD::FW_Server_Request_Wrapper::dispatch (
  PortableServer::Servant servant,
  TAO::Portable_Server::Servant_Upcall *servant_upcall)
{
  try
    {
      servant->_dispatch(*this->request_, servant_upcall);
    }
  // Only CORBA exceptions are caught here.
  catch (const ::CORBA::Exception& ex)
    {
      if (this->request_->collocated())
        {
          // For collocated requests, we re-throw the exception.
          throw;
        }
      else if (!this->request_->sync_with_server() &&
              this->request_->response_expected() &&
              !this->request_->deferred_reply())
        {
          // The request is a remote request that is expecting a reply.
          this->request_->tao_send_reply_exception(ex);
        }
      else if (TAO_debug_level > 0)
        {
          // It is unfortunate that an exception (probably a system
          // exception) was thrown by the dispatch code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.

          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) exception thrown ")
                      ACE_TEXT ("but client is not waiting a response\n")));

          ex._tao_print_exception ("FW_Server_Request_Wrapper::dispatch ()");
        }
     }
  catch (...)
    {
      // @@ TODO some c++ exception or another, but what do we do with
      //    it?
      // We are supposed to map it into a CORBA::UNKNOWN exception.
      // BTW, this cannot be detected if using the <env> mapping.  If
      // we have native exceptions but no support for them in the ORB
      // we should still be able to catch it.  If we don't have native
      // exceptions it couldn't have been raised in the first place!
      CORBA::UNKNOWN exception (CORBA::SystemException::_tao_minor_code
                                 (TAO_UNHANDLED_SERVER_CXX_EXCEPTION, 0),
                                  CORBA::COMPLETED_MAYBE);

      if (this->request_->collocated())
        {
          // For collocated requests, we re-throw the exception.
          throw;
        }
      else if (!this->request_->sync_with_server() &&
              this->request_->response_expected() &&
              !this->request_->deferred_reply())
        {
          // The request is a remote request that is expecting a reply.
          this->request_->tao_send_reply_exception(exception);
        }
      else if (TAO_debug_level > 0)
        {
          // It is unfortunate that an exception (probably a system
          // exception) was thrown by the dispatch code (even by the
          // user) when the client was not expecting a response.
          // However, in this case, we cannot close the connection
          // down, since it really isn't the client's fault.

          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) exception thrown ")
                      ACE_TEXT ("but client is not waiting a response\n")));

          exception._tao_print_exception (
            "FW_Server_Request_Wrapper::dispatch ()");
        }
     }

}


TAO_ServerRequest*
TAO::CSD::FW_Server_Request_Wrapper::clone (TAO_ServerRequest*& request)
{
  // TBD-CSD: Ultimately add an argument for an allocator.
  TAO_ServerRequest* clone_obj = 0;
  ACE_NEW_RETURN (clone_obj,
                  TAO_ServerRequest (),
                  0);

  // TYPE: TAO_GIOP_Message_Base*
  // ACTION: Assuming that a shallow-copy is ok here.
  clone_obj->mesg_base_ = request->mesg_base_;

  // TYPE: const char*
  // ACTION: Method performs deep-copy of string contents.
  clone_obj->operation (CORBA::string_dup (request->operation ()),
                        request->operation_length (),
                        1);

  // TYPE: CORBA::Object_var
  // ACTION: Assignment performs reference-counted copy of object ref.
  clone_obj->forward_location_ = request->forward_location_;

  // TYPE: bool
  // ACTION: Primitive data type assignment
  clone_obj->is_forwarded_ = request->is_forwarded_;

  // TYPE: TAO_InputCDR*
  // ACTION: This *must* be "cloned".
  if (request->incoming_ != 0)
    {
      clone_obj->incoming_ = this->clone (request->incoming_);
    }

  // TYPE: TAO_OutputCDR*
  // ACTION: This *must* be "cloned".
  if (request->outgoing_ != 0)
    {
      clone_obj->outgoing_ = this->create_new_output_cdr ();
    }

  // TYPE: TAO_Transport*
  // ACTION: Assuming that a shallow-copy is ok here.
  clone_obj->transport_ = request->transport_;
  if (clone_obj->transport_ != 0)
    clone_obj->transport_->add_reference ();

  // TYPE: CORBA::Boolean
  // ACTION: Primitive data type assignment.
  clone_obj->response_expected_ = request->response_expected_;

  // TYPE: CORBA::Boolean
  // ACTION: Primitive data type assignment.
  clone_obj->deferred_reply_ = request->deferred_reply_;

  // TYPE: CORBA::Boolean
  // ACTION: Primitive data type assignment.
  clone_obj->sync_with_server_ = request->sync_with_server_;

  // TYPE: CORBA::Boolean
  // ACTION: Primitive data type assignment.
  clone_obj->is_dsi_ = request->is_dsi_;

  // TYPE: CORBA::ULong
  // ACTION: Primitive data type assignment.
  clone_obj->reply_status_ = request->reply_status_;

  // TYPE: TAO_ORB_Core*
  // ACTION: Assuming that a shallow-copy is ok here.
  clone_obj->orb_core_ = request->orb_core_;

  // TYPE: TAO_Service_Context
  // ACTION: No copy/assignment operator, so adding/using a clone operation.
  this->clone (request->request_service_context_, clone_obj->request_service_context_);

  // TYPE: TAO_Service_Context
  // ACTION: No copy/assignment operator, so adding/using a clone operation.
  this->clone (request->reply_service_context_, clone_obj->reply_service_context_);

  // TYPE: CORBA::ULong
  // ACTION: Primitive data type assignment.
  clone_obj->request_id_ = request->request_id_;

  // TYPE: TAO_Tagged_Profile
  // ACTION: No copy/assignment operator, so adding/using a clone operation.
  this->clone (request->profile_, clone_obj->profile_);

  // TYPE: CORBA::OctetSeq_var
  // ACTION: Assignment performs reference-counted copy of sequence.
  clone_obj->requesting_principal_ = request->requesting_principal_;

  // TYPE: ptrdiff_t
  // ACTION: Primitive data type assignment (unsigned integral type).
  clone_obj->dsi_nvlist_align_ = request->dsi_nvlist_align_;

  // TYPE: TAO_Operation_Details const * const
  // ACTION: Need to clone this.
  if (request->operation_details_ != 0)
    {
      ACE_ASSERT (request->incoming_ == 0);
      if (this->clone (request->operation_details_,
                       clone_obj->operation_details_,
                       clone_obj->incoming_) == false)
        {
          return 0;
        }
    }

  // TYPE: CORBA::Boolean
  // ACTION: Primitive data type assignment.
  clone_obj->argument_flag_ = request->argument_flag_;

  //#if TAO_HAS_INTERCEPTORS == 1
  // TYPE: size_t
  // ACTION: Primitive data type assignment.
  clone_obj->interceptor_count_ = request->interceptor_count_;

  // TYPE: TAO::PICurrent_Impl
  // ACTION: Copy/assignment operator disabled on purpose.
  //         Just leave this alone for a clone.
  //
  // clone_obj->rs_pi_current_

  // TYPE: CORBA::OctetSeq_var
  // ACTION: Assignment performs reference-counted copy of sequence.
  //         Assuming that this is ok.
  //         Just leave this alone for a clone.
  //
  //clone_obj->result_seq_ = request->result_seq_;
  //#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  if (clone_obj->transport_ != 0)
    {
      clone_obj->transport_->assign_translators(clone_obj->incoming_,
                                                clone_obj->outgoing_);
    }
  return clone_obj;
}


TAO_InputCDR*
TAO::CSD::FW_Server_Request_Wrapper::clone (TAO_InputCDR*& from)
{
  TAO_InputCDR* clone_ptr = 0;
  ACE_NEW_RETURN (clone_ptr,
                  TAO_InputCDR(*from),
                  0);
  return clone_ptr;
}


bool
TAO::CSD::FW_Server_Request_Wrapper::clone (TAO_Operation_Details const *& from,
                                            TAO_Operation_Details const *& to,
                                            TAO_InputCDR*& cdr)
{
  TAO_Operation_Details *& from_non_const
    = const_cast <TAO_Operation_Details *&>(from);

  char* cloned_op_name = 0;
  ACE_NEW_RETURN (cloned_op_name,
                  char[from_non_const->opname_len_ + 1],
                  false);
  ACE_OS::strncpy(cloned_op_name, from_non_const->opname_, from_non_const->opname_len_);
  cloned_op_name[from_non_const->opname_len_] = '\0';

  // See if we can clone arguments.  If we can, the user compiled the
  // idl with the clonable arguments option, great.  If not, then
  // use the marshaling technique to copy the arguments
  TAO::Argument** cloned_args = 0;
  CORBA::ULong num_cloned_args = 0;
  if (from->num_args_ > 0)
    {
      TAO::Argument* retval = from->args_[0]->clone();
      if (retval != 0)
        {
          ACE_NEW_RETURN (cloned_args,
                          TAO::Argument*[from->num_args_],
                          false);
          cloned_args[0] = retval;
          for (CORBA::ULong i = 1; i < from->num_args_; i++)
            {
              cloned_args[i] = from->args_[i]->clone();
            }
          num_cloned_args = from->num_args_;
        }
      else
        {
          static const size_t mb_size = 2048;
          ACE_NEW_RETURN (cdr,
                          TAO_InputCDR (mb_size),
                          false);

          // To avoid duplicating and copying the data block, allow the
          // TAO_OutputCDR to share the data block of TAO_InputCDR's message block.
          ACE_Message_Block* mb = const_cast<ACE_Message_Block*> (cdr->start ());
          TAO_OutputCDR outcdr (mb);

          if (! from_non_const->marshal_args (outcdr))
            {
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT("(%P|%T) TAO::CSD::FW_Server_Request_Wrapper::")
                          ACE_TEXT("clone TAO_Operation_Details failed\n")));
              return false;
            }

          // The TAO_OutputCDR made a new message block around the data block
          // held by the message block owned by the TAO_InputCDR.  We need to
          // make sure that the results of marshaling are propagated back to the
          // message block in the TAO_InputCDR.
          const ACE_Message_Block* begin = outcdr.begin ();
          if (begin == outcdr.current ())
            {
              // A chain was not made, so we can just adjust the read and write
              // pointers
              mb->rd_ptr (begin->rd_ptr ());
              mb->wr_ptr (begin->wr_ptr ());
            }
          else
            {
              // A costly, but necessary, copying of data blocks.  This shouldn't
              // happen that often assuming that the size of the message block
              // allocated during the allocation of TAO_InputCDR is "big enough"
              // for most operation parameters.
              cdr->reset (begin, outcdr.byte_order ());
            }
        }
    }

  // CSD-TBD: Eventually need to use allocators.

  // CSD-TBD: Assert that this->ex_data_ and this->ex_count_ are both == 0
  TAO_Operation_Details* to_non_const = 0;
  ACE_NEW_RETURN (to_non_const,
                  TAO_Operation_Details(cloned_op_name,
                                        from_non_const->opname_len_,
                                        cloned_args,
                                        num_cloned_args,
                                        0,
                                        0),
                  false);


  // DATA MEMBER: const char *opname_;
  // DATA MEMBER: CORBA::ULong opname_len_;
  // DATA MEMBER: CORBA::Boolean argument_flag_;
  // DATA MEMBER: TAO::Argument **args_;
  // DATA MEMBER: CORBA::ULong num_args_;
  // DATA MEMBER: TAO::Exception_Data *ex_data_;
  // DATA MEMBER: CORBA::ULong ex_count_;
  //
  //    ACTION: None - handled in ctor
  //

  // DATA MEMBER: CORBA::ULong request_id_;
  // DATA MEMBER: CORBA::Octet response_flags_;
  // DATA MEMBER: TAO_Target_Specification::TAO_Target_Address addressing_mode_;
  // DATA MEMBER: TAO_Service_Context request_service_info_;
  // DATA MEMBER: TAO_Service_Context reply_service_info_;
  //
  //    ACTION: Use assignment op to copy from "this" object to the clone.
  //
  to_non_const->request_id_      = from->request_id_;
  to_non_const->response_flags_  = from->response_flags_;
  to_non_const->addressing_mode_ = from->addressing_mode_;

  // DATA MEMBER: TAO_Service_Context request_service_info_;
  // DATA MEMBER: TAO_Service_Context reply_service_info_;
  //
  //    ACTION: Use the TAO_Service_Context clone() method.
  //
  this->clone (from_non_const->request_service_info_, to_non_const->request_service_info_);
  this->clone (from_non_const->reply_service_info_, to_non_const->reply_service_info_);

  to = to_non_const;

  return true;
}


void
TAO::CSD::FW_Server_Request_Wrapper::clone (TAO_Tagged_Profile& from,
                                            TAO_Tagged_Profile& to)
{
  to.orb_core_             = from.orb_core_;
  to.discriminator_        = from.discriminator_;
  to.object_key_extracted_ = from.object_key_extracted_;
  to.object_key_           = from.object_key_;
  to.profile_              = from.profile_;
  to.profile_index_        = from.profile_index_;
  to.type_id_              = from.type_id_ == 0 ? 0 :
                               CORBA::string_dup (from.type_id_);
}


void
TAO::CSD::FW_Server_Request_Wrapper::clone (TAO_Service_Context& from,
                                            TAO_Service_Context& to)
{
  to.service_context_ = from.service_context_;
}

TAO_OutputCDR*
TAO::CSD::FW_Server_Request_Wrapper::create_new_output_cdr ()
{
  TAO_OutputCDR* cdr = 0;

  // A buffer that we will use to initialise the CDR stream
  char* repbuf = 0;
  ACE_NEW_RETURN (repbuf,
                 char[ACE_CDR::DEFAULT_BUFSIZE],
                 0);

  TAO_GIOP_Message_Version giop_version;
  this->request_->outgoing_->get_version (giop_version);

  // Initialze an output CDR on the stack
  // NOTE: Don't jump to a conclusion as to why we are using the
  // input_cdr and hence the  global pool here. These pools will move
  // to the lanes anyway at some point of time. Further, it would have
  // been awesome to have this in TSS. But for some reason the cloning
  // that happens when the ORB gets flow controlled while writing a
  // reply is messing things up. We crash horribly. Doing this adds a
  // lock, we need to set things like this -- put stuff in TSS here
  // and transfer to global memory when we get flow controlled. We
  // need to work on the message block to get it right!
  ACE_NEW_RETURN (cdr,
                  TAO_OutputCDR (repbuf,
                                 ACE_CDR::DEFAULT_BUFSIZE,
                                 TAO_ENCAP_BYTE_ORDER,
                                 this->request_->orb_core_->input_cdr_buffer_allocator (),
                                 this->request_->orb_core_->input_cdr_dblock_allocator (),
                                 this->request_->orb_core_->input_cdr_msgblock_allocator (),
                                 this->request_->orb_core_->orb_params ()->cdr_memcpy_tradeoff (),
                                 giop_version.major,
                                 giop_version.minor),
                  0);

  return cdr;
}

TAO_END_VERSIONED_NAMESPACE_DECL
