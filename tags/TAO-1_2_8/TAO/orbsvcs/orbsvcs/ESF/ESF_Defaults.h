/* -*- C++ -*- */
/**
 *  @file   ESF_Defaults.h
 *
 *  $Id$
 *
 * In this file we set the compile time defaults for the framework.
 *
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_DEFAULTS_H
#define TAO_ESF_DEFAULTS_H

#ifndef TAO_ESF_ENABLE_DEBUG_MESSAGES
#define TAO_ESF_ENABLE_DEBUG_MESSAGES 0
#endif /* TAO_ESF_ENABLE_DEBUG_MESSAGES */

// Control the maximum degree of concurrency tolerated by the EC, some
// kind of limit is required to avoid starvation of delayed write
// operations.
#ifndef TAO_ESF_DEFAULT_BUSY_HWM
# define TAO_ESF_DEFAULT_BUSY_HWM 1024
#endif /* TAO_ESF_DEFAULT_BUSY_HWM */

#ifndef TAO_ESF_DEFAULT_MAX_WRITE_DELAY
# define TAO_ESF_DEFAULT_MAX_WRITE_DELAY 2048
#endif /* TAO_ESF_DEFAULT_MAX_WRITE_DELAY */

#ifndef TAO_ESF_DEFAULT_ORB_ID
# define TAO_ESF_DEFAULT_ORB_ID "" /* */
#endif /* TAO_ESF_DEFAULT_ORB_ID */

#endif /* TAO_ESF_DEFAULTS_H */
