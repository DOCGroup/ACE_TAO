//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    root_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Root in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_root,
           root_ch,
           "$Id$")

// ********************************
// Root visitor for client header
// ********************************

be_visitor_root_ch::be_visitor_root_ch (be_visitor_context *ctx)
  : be_visitor_root (ctx)
{
}

be_visitor_root_ch::~be_visitor_root_ch (void)
{
}

int
be_visitor_root_ch::init (void)
{
  // First open the client-side header file for writing
  if (tao_cg->start_client_header (be_global->be_get_client_hdr_fname ())
        == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_root_ch::init - "
                         "Error opening client header file\n"),
                        -1);
    }

  // Initialize the stream.
  this->ctx_->stream (tao_cg->client_header ());

  // If this IDL file contains an non local interface declaration, generated a
  // forward declaration of the proxy broker for a possible collocated call.
  if (idl_global->non_local_iface_seen_)
    {
      TAO_OutStream *os = this->ctx_->stream ();

      *os << be_nl << be_nl
          << "// TAO_IDL - Generated from " << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      *os << be_global->core_versioning_begin () << be_nl;

      *os << be_nl << be_nl
          << "namespace TAO" << be_nl
          << "{" << be_idt_nl;

     if (be_global->gen_direct_collocation () || be_global->gen_thru_poa_collocation ())
       {
         *os << "class Collocation_Proxy_Broker;" << be_nl;
       }

      if (idl_global->abstract_iface_seen_)
        {
          *os << "template<typename T> class AbstractBase_Narrow_Utils;" << be_nl;
        }

      *os << "template<typename T> class Narrow_Utils;" << be_uidt_nl
          << "}" ;

      *os << be_global->core_versioning_end () << be_nl;
    }

  return 0;
}
