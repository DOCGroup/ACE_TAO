/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/config/features.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_FEATURES_H
#define PACE_CONFIG_FEATURES_H

/* Features: Can be customized by the developer. */

#if defined (WIN32)

/* If using win32 .. define PACE_HAS_MFC to 1, if you want ACE to use
 * CWinThread. This should be defined, if your application uses MFC.
 * Setting applies to  : building PACE
 * Runtime restrictions: MFC DLLs must be installed
 * Additonal notes     : If both PACE_HAS_MFC and PACE_MT_SAFE are
 *                       defined, the MFC DLL (not the static lib)
 *                       will be used from ACE. */
# if !defined (PACE_HAS_MFC)
#  define PACE_HAS_MFC 0
# endif /* PACE_HAS_MFC */

/* Turn off the following define if you want to disable threading.
 * Compile using multi-thread libraries.
 * Setting applies to  : building PACE, linking with PACE
 * Runtime restrictions: multithreaded runtime DLL must be installed. */
# if !defined (PACE_MT_SAFE)
#  define PACE_MT_SAFE 1
# endif /* PACE_MT_SAFE */

/* Turn on multithreading. */
# if !defined (_MT)
# define _MT
# endif /* _MT */

#endif /* WIN32 */

#endif /* PACE_CONFIG_FEATURES_H */
