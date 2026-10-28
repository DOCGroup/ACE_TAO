
//=============================================================================
/**
 *  @file    native_ch.cpp
 *
 *  Visitor generating code for Native in the client header
 *
 *  @author Johnny Willemsen
 */
//=============================================================================

#include "native.h"
#include "be_visitor_typecode/typecode_decl.h"
#include "global_extern.h"

// ********************************************************************
// Visitor implementation for the Native type
// This one for the client header file
// ********************************************************************

be_visitor_native_ch::be_visitor_native_ch (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

/// Visit the native_ch node and its scope.
int
be_visitor_native_ch::visit_native (be_native *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  std::string_view const node_name = node->full_name ();

  if (node_name == "PortableServer::ServantLocator::Cookie")
    {
      *os << "using Cookie = void *;" << be_nl;
    }
  else if (node_name == "CORBA::VoidData")
    {
      *os << "using VoidData = void *;" << be_nl;
    }
  else if (idl_global->dcps_support_zero_copy_read ()
           && node_name == "DDS::SampleInfoSeq")
    {
      // DDS/DCPS zero-copy read sequence type support.
      *os << be_nl_2
          << "using SampleInfoSeq = ::TAO::DCPS::ZeroCopyInfoSeq< "
          << "SampleInfo"
          << ", DCPS_ZERO_COPY_SEQ_DEFAULT_SIZE>;" << be_nl;
    }
  else if (idl_global->dcps_support_zero_copy_read ())
    {
      // DDS/DCPS zero-copy read sequence type support.

      // strip  the "Seq" ending to get the sample's name
      constexpr size_t max_name_length = 2000;
      size_t const node_name_length = node_name.size ();
      if (node_name_length >= max_name_length ||
          node_name_length <= 3)
        {
          return -1;
        }
      std::string_view const sample_name =
        node_name.substr (0, node_name_length - 3);

      *os << be_nl_2
          << "using " << node->local_name () << " = ::TAO::DCPS::ZeroCopyDataSeq< "
          << sample_name
          << ", DCPS_ZERO_COPY_SEQ_DEFAULT_SIZE>;" << be_nl;
    }

  node->cli_hdr_gen (true);
  return 0;
}

