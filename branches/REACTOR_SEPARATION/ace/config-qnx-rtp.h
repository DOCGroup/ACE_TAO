// -*- C++ -*-
// $Id$
// The following configuration file is designed to work for QNX RTP
// GNU C++ and the POSIX (pthread) threads package. You can get QNX
// RTP at http://get.qnx.com. 
// This header is intended to switch between configuration for 
// various NTO versions.
#ifndef ACE_CONFIG_QNX_RTP_H
#define ACE_CONFIG_QNX_RTP_H
#include /**/ "ace/pre.h"

#if !defined(ACE_NTO_VERS)
#    error "Correct ACE_NTO_VERS in include/makeinclude/platform_qnx_rtp_gcc.GNU"
#elif ACE_NTO_VERS < 620
#    include /**/ "ace/config-qnx-rtp-pre62x.h"
#else
#    include /**/ "ace/config-qnx-rtp-62x.h"
#endif
#include /**/ "ace/post.h"
#endif /* ACE_CONFIG_QNX_RTP_H */
