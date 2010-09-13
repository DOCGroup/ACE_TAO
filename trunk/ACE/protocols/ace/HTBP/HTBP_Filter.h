// -*- C++ -*-

//=============================================================================
/**
 *  @file    HTBP_Filter.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier
 */
//=============================================================================

#ifndef ACE_HTBP_FILTER_H
#define ACE_HTBP_FILTER_H
#include /**/ "ace/pre.h"

#include "HTBP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace HTBP
  {

    // Forward declarations.
    class Channel;

    /**
     * @class ACE_HTBP_Filter
     *
     * @brief Defines the methods in the <ACE_HTBP_Filter> abstraction.
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
    class HTBP_Export Filter
    {
    public:
      Filter (void);

      virtual ~Filter (void);

      /// Inside and outside Send filters need to supply a data header that
      /// sets the stage before sending the data body, and then (maybe) send a
      /// data trailer that closes the message. The data header may be formatted
      /// differently on the first message, to send optional context information.
      virtual ssize_t send_data_header (ssize_t data_len, Channel *ch);
      virtual ssize_t send_data_trailer (Channel *ch);

      /// Upon receiving data, the Channel is obliged to send an ack. This is
      /// either an empty document if this is the in-to-out stream, or a new
      /// long-duration document request if this is the out-to-in stream.
      virtual int send_ack (Channel *ch);
      virtual int recv_ack (Channel *ch);

      /// receiving data must compliment sending. In the case of an in-to-out
      /// stream. It is possible that the stream identity is not known until the
      /// first request is received. In this case the filter will have to look
      /// lookup the received session ID and either create a new instance or
      /// reassign itself to an existing sesion. In that case, the associated
      /// stream will have to be reassigned as well.
      ///
      virtual ssize_t recv_data_header (Channel *ch);
      virtual ssize_t recv_data_trailer(Channel *ch);

      /// reset the http_code_ in anticipation of a new message.
      void reset_http_code (void);

      /// get the value of the most recent http result code
      int http_code (void);

    protected:
      char *header_complete (Channel *ch);

    private:
      /// Store the most recent HTTP result code. Typically it will be 200,
      /// but it could be a 4xx or 5xx code if the proxy reports an error.
      int http_code_;

    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "HTBP_Filter.inl"
#endif

#include /**/ "ace/post.h"
#endif /* ACE_HTBP_FILTER_H */
