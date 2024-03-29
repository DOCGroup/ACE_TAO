// -*- C++ -*-
TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  ACE_INLINE
  ClientRequestDetails::ClientRequestDetails ()
    : processing_mode_(PortableInterceptor::LOCAL_AND_REMOTE)
  {
  }

  ACE_INLINE
  bool
  ClientRequestDetails::should_be_processed (bool is_remote_request) const
  {
    return ((this->processing_mode_ == PortableInterceptor::LOCAL_AND_REMOTE) ||
            ((this->processing_mode_ == PortableInterceptor::REMOTE_ONLY) &&
             (is_remote_request)) ||
            ((this->processing_mode_ == PortableInterceptor::LOCAL_ONLY) &&
             (!is_remote_request)));
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL
