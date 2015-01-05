// -*- C++ -*-

//=============================================================================
/**
 *  @file    HTBP_Inside_Squid_Filter.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier
 */
//=============================================================================

#ifndef ACE_HTBP_INSIDE_SQUID_FILTER_H
#define ACE_HTBP_INSIDE_SQUID_FILTER_H
#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "HTBP_Export.h"
#include "HTBP_Filter.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTBP
  {

    /**
     * @class Inside_Squid_Filter
     *
     * @brief Defines the methods in the <Filter> abstraction.
     *
     * A filter is responsible for wrapping / unwrapping messages
     * depending on direction of flow on each stream. There will be 4
     * filters participating in a session, inside-send, inside-recv,
     * outside-send, ouside-recv. It is anticipated that specialized
     * filters will be implemented that tune the filter behavior based
     * on proxy particulars. That, or there will be a matrix
     * configuration parameters that may be tweeked to match the
     * configuration.  It remains to be seen which is easier to
     * manage.
     */
    class HTBP_Export Inside_Squid_Filter : public Filter
    {
    public:
      /// Destructor.
      virtual ~Inside_Squid_Filter (void);

      /// Inside and outside Send filters need to supply a data header
      /// that sets the stage before sending the data body, and then
      /// (maybe) send a data trailer that closes the message. The
      /// data header may be formatted differently on the first
      /// message, to send optional context information.
      virtual ssize_t send_data_header (ssize_t data_len, Channel *);
      virtual ssize_t send_data_trailer (Channel *);

      /// Upon receiving data, the Channel is obliged to send an
      /// ack. This is either an empty document if this is the
      /// in-to-out stream, or a new long-duration document request if
      /// this is the out-to-in stream.
      virtual int send_ack (Channel *);
      virtual int recv_ack (Channel *);

      /// receiving data must compliment sending. In the case of an
      /// in-to-out stream. It is possible that the stream identity is
      /// not known until the first request is received. In this case
      /// the filter will have to look lookup the received session ID
      /// and either create a new instance or reassign itself to an
      /// existing sesion. In that case, the associated stream will
      /// have to be reassigned as well.
      ///
      virtual ssize_t recv_data_header (Channel *);
      virtual ssize_t recv_data_trailer(Channel *);
    private:
      int make_request_header (Channel *, const char *, char *, size_t );

    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "HTBP_Inside_Squid_Filter.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTBP_FILTER_H */
