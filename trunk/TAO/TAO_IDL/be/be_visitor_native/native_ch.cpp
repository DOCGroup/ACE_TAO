
//=============================================================================
/**
 *  @file    native_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Native in the client header
 *
 *
 *  @author Johnny Willemsen
 */
//=============================================================================


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

be_visitor_native_ch::~be_visitor_native_ch (void)
{
}

// Visit the native_ch node and its scope.
int
be_visitor_native_ch::visit_native (be_native *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__
      << be_nl << be_nl;

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
      *os << be_nl << be_nl
          << "typedef ::TAO::DCPS::ZeroCopyInfoSeq< "
          << "SampleInfo"
          << ", DCPS_ZERO_COPY_SEQ_DEFAULT_SIZE> "
          << "SampleInfo"
          << "Seq;" << be_nl;
    }
  else if (idl_global->dcps_support_zero_copy_read ()
      && ACE_OS::strlen (node_name) > 3
      && 0 == ACE_OS::strcmp (node_name + ACE_OS::strlen (node_name)-3, "Seq") )
    {
      // DDS/DCPS zero-copy read sequence type support.

      // strip  the "Seq" ending to get the sample's name
      const size_t max_name_length = 2000;

      if (ACE_OS::strlen (node_name) >= max_name_length)
        {
          return -1;
        }

      char sample_name[max_name_length];
      ACE_OS::strncpy (sample_name,
                       node_name,
                       ACE_OS::strlen (node_name) - 3);
      sample_name[ACE_OS::strlen (node_name) - 3] = '\0';

      *os << be_nl << be_nl
          << "typedef ::TAO::DCPS::ZeroCopyDataSeq< "
          << sample_name
          << ", DCPS_ZERO_COPY_SEQ_DEFAULT_SIZE> "
          << node->local_name ()
          << ";" << be_nl;
    }


  node->cli_hdr_gen (true);
  return 0;
}

