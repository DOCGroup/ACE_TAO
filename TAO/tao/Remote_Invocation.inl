// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE
  void
  Remote_Invocation::_tao_byte_order (int byte_order)
  {
    this->byte_order_ = byte_order;
  }

  ACE_INLINE
  int
  Remote_Invocation::_tao_byte_order ()
  {
    return this->byte_order_;
  }

  ACE_INLINE
  void
  Remote_Invocation::marshal_data (TAO_OutputCDR &out_stream)
  {
    // Marshal application data
    if (this->details_.marshal_args (out_stream) == false)
      {
        throw ::CORBA::MARSHAL ();
      }
  }

  ACE_INLINE
  Remote_Invocation::CDR_Byte_Order_Guard::CDR_Byte_Order_Guard (
      TAO_OutputCDR& cdr, int byte_order)
      : cdr_(cdr), byte_order_(byte_order), present_byte_order_(
          cdr.byte_order())
  {
    if (byte_order_ != present_byte_order_)
    {
      cdr_.reset_byte_order(byte_order_);
    }
  }

  ACE_INLINE
  Remote_Invocation::CDR_Byte_Order_Guard::~CDR_Byte_Order_Guard ()
  {
    this->reset ();
  }

  ACE_INLINE
  void Remote_Invocation::CDR_Byte_Order_Guard::reset ()
  {
    if (byte_order_ != present_byte_order_)
    {
      cdr_.reset_byte_order(byte_order_);
      present_byte_order_ = byte_order_;
    }
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
