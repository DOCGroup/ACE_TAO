// -*- C++ -*-

//=============================================================================
/**
 *  @file   HTIOP_Factory.h
 *
 *  @author Fred Kuhns <fredk@cs.wustl.edu>
 */
//=============================================================================


#ifndef HTIOP_FACTORY_H
#define HTIOP_FACTORY_H
#include /**/ "ace/pre.h"

#include "orbsvcs/HTIOP/HTIOP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Protocol_Factory.h"
#include "ace/Service_Config.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
namespace ACE
{
  namespace HTBP
  {
    class Environment;
  }
}
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace HTIOP
  {
    class HTIOP_Export Protocol_Factory : public TAO_Protocol_Factory
    {
    public:
      Protocol_Factory ();
      ~Protocol_Factory () override;

      // = Service Configurator hooks.
      /// Dynamic linking hook
      int init (int argc, ACE_TCHAR* argv[]) override;

      /// Verify prefix is a match
      int match_prefix (const ACE_CString &prefix) override;

      /// Returns the prefix used by the protocol.
      const char *prefix () const override;

      /// Return the character used to mark where an endpoint ends and
      /// where its options begin.
      char options_delimiter () const override;

      // = Check Protocol_Factory.h for a description of these methods.
      TAO_Acceptor  *make_acceptor () override;
      TAO_Connector *make_connector  () override;
      int requires_explicit_endpoint () const override;

    private:
      /// Configuration reference
      ACE::HTBP::Environment *ht_env_;

      /// Flag used to determine if an acceptor should use an "inside"
      /// local address, or try to compose an "outside" address.
      /// If the value is 1, then always use an HTID for local addr,
      /// otherwise use host:port if 0, or guess based on proxy config
      /// if -1. The default is -1.
      int inside_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

// Note that these declarations are placed outside of the scope of the
// namespace. The generated functions use a C style signature which does
// not accommodate namespaces, so assuming namespace concatenation is an
// error. The service class name should be composed of a flattened class
// name, with the namespaces preceding the class name with '_'.
ACE_STATIC_SVC_DECLARE_EXPORT (HTIOP, TAO_HTIOP_Protocol_Factory)
ACE_FACTORY_DECLARE (HTIOP, TAO_HTIOP_Protocol_Factory)

#include /**/ "ace/post.h"
#endif /* HTIOP_FACTORY_H */
