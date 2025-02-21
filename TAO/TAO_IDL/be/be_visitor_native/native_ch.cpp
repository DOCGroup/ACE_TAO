
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
#include <cstring>

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

  const char *node_name = node->full_name ();

  if (ACE_OS::strcmp (node_name, "PortableServer::ServantLocator::Cookie") == 0)
    {
      *os << "typedef void *Cookie;" << be_nl;
    }
  else if (ACE_OS::strcmp (node_name, "CORBA::VoidData") == 0)
    {
      *os << "typedef void *VoidData;" << be_nl;
    }
  else if (idl_global->dcps_support_zero_copy_read ()
           && 0 == ACE_OS::strcmp (node->full_name (),"DDS::SampleInfoSeq"))
    {
      // DDS/DCPS zero-copy read sequence type support.
      *os << be_nl_2
          << "typedef ::TAO::DCPS::ZeroCopyInfoSeq< "
          << "SampleInfo"
          << ", DCPS_ZERO_COPY_SEQ_DEFAULT_SIZE> "
          << "SampleInfo"
          << "Seq;" << be_nl;
    }
  else if (idl_global->dcps_support_zero_copy_read ())
    {
      // DDS/DCPS zero-copy read sequence type support.

      // strip  the "Seq" ending to get the sample's name
      const char * node_name = node->full_name ();
      const size_t max_name_length = 2000;
      const size_t node_name_length = std::strlen (node_name);
      if (node_name_length >= max_name_length ||
          node_name_length <= 3)
        {
          return -1;
        }
      char sample_name[max_name_length];
      // Copy node_name into sample_name and shorten to remove Seq suffix
      std::strcpy (sample_name, node_name);
      sample_name[node_name_length - 3] = '\0';

      *os << be_nl_2
          << "typedef ::TAO::DCPS::ZeroCopyDataSeq< "
          << sample_name
          << ", DCPS_ZERO_COPY_SEQ_DEFAULT_SIZE> "
          << node->local_name ()
          << ";" << be_nl;
    }

  node->cli_hdr_gen (true);
  return 0;
}

