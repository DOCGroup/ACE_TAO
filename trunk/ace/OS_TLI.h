// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_TLI.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_OS_TLI_H
#define ACE_OS_TLI_H
#include "ace/pre.h"

#include "ace/config-all.h"
#include "ace/OS_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_HAS_TLI)
// Dummies to help compilation.
struct t_call { };
struct t_bind { };
struct t_info { };
struct t_optmgmt { };
struct t_discon { };
struct t_unitdata { };
struct t_uderr { };
struct netbuf { };

# else /* !ACE_WIN32 */

#   if !defined (ACE_HAS_TLI_PROTOTYPES)

// Define ACE_TLI headers for systems that don't prototype them....
extern "C"
{
  int t_accept(int fildes, int resfd, struct t_call *call);
  char *t_alloc(int fildes, int struct_type, int fields);
  int t_bind(int fildes, struct t_bind *req, struct t_bind *ret);
  int t_close(int fildes);
  int t_connect(int fildes, struct t_call *sndcall,
                struct t_call *rcvcall);
  void t_error(const char *errmsg);
  int t_free(char *ptr, int struct_type);
  int t_getinfo(int fildes, struct t_info *info);
  int t_getname (int fildes, struct netbuf *namep, int type);
  int t_getstate(int fildes);
  int t_listen(int fildes, struct t_call *call);
  int t_look(int fildes);
  int t_open(char *path, int oflag, struct t_info *info);
  int t_optmgmt(int fildes, struct t_optmgmt *req,
                struct t_optmgmt *ret);
  int t_rcv(int fildes, char *buf, unsigned int nbytes, int *flags);
  int t_rcvconnect(int fildes, struct t_call *call);
  int t_rcvdis(int fildes, struct t_discon *discon);
  int t_rcvrel(int fildes);
  int t_rcvudata(int fildes, struct t_unitdata *unitdata, int *flags);
  int t_rcvuderr(int fildes, struct t_uderr *uderr);
  int t_snd(int fildes, const char *buf, unsigned int nbytes, int flags);
  int t_snddis(int fildes, struct t_call *call);
  int t_sndrel(int fildes);
  int t_sndudata(int fildes, struct t_unitdata *unitdata);
  int t_sync(int fildes);
  int t_unbind(int fildes);
}
#   endif /* !ACE_HAS_TLI_PROTOTYPES */

#   if defined (ACE_HAS_TIUSER_H) || defined (ACE_HAS_XTI) || defined (ACE_HAS_FORE_ATM_XTI)
#     if defined (ACE_HAS_BROKEN_XTI_MACROS)
#       undef TCP_NODELAY
#       undef TCP_MAXSEG
#     endif /* ACE_HAS_BROKEN_XTI_MACROS */
#     if defined (ACE_HAS_TIUSER_H_BROKEN_EXTERN_C)
extern "C" {
#     endif /* ACE_HAS_TIUSER_H_BROKEN_EXTERN_C */
#     if defined (ACE_HAS_FORE_ATM_XTI)
#       include /**/ <fore_xti/xti_user_types.h>
#       include /**/ <fore_xti/xti.h>
#       include /**/ <fore_xti/xti_atm.h>
#       include /**/ <fore_xti/netatm/atm.h>
#       include /**/ <fore_xti/ans.h>
#     elif defined (ACE_HAS_TIUSER_H)
#       include /**/ <tiuser.h>
#     elif defined (ACE_HAS_SYS_XTI_H)
#         define class ace_xti_class
#         include /**/ <sys/xti.h>
#         undef class
#     else
#         include /**/ <xti.h>
#     endif /* ACE_HAS_FORE_ATM_XTI */
#     if defined (ACE_HAS_TIUSER_H_BROKEN_EXTERN_C)
}
#     endif /* ACE_HAS_TIUSER_H_BROKEN_EXTERN_C */
#   endif /* ACE_HAS_TIUSER_H || ACE_HAS_XTI */

// Apparently this particular prototype is missing in so many
// platforms that is just better to declare it ourselves.
extern "C" int t_getname (int, struct netbuf *, int);

# endif /* ACE_WIN32 */

// If the xti.h file redefines the function names, do it now, else
// when the function definitions are encountered, they won't match the
// declaration here.

# if defined (ACE_REDEFINES_XTI_FUNCTIONS)
#   include /**/ <xti.h>
#   if defined (UNIXWARE_2_0)         /* They apparently forgot one... */
extern "C" int _xti_error(char *);
#   endif /* UNIXWARE */
# endif /* ACE_REDEFINES_XTI_FUNCTIONS */

/**
 * @class ACE_OS_TLI
 *
 * @brief This class is a wrapper for the TLI operations
 *
 */
class ACE_OS_Export ACE_OS_TLI
{
public:
  // = A set of wrappers for TLI.
  static int t_accept (ACE_HANDLE fildes,
                       ACE_HANDLE resfd,
                       struct t_call
                       *call);
  static char *t_alloc (ACE_HANDLE fildes,
                        int struct_type,
                        int
                        fields);
  static int t_bind (ACE_HANDLE fildes,
                     struct t_bind *req,
                     struct
                     t_bind *ret);
  static int t_close (ACE_HANDLE fildes);
  static int t_connect (ACE_HANDLE fildes,
                        struct t_call *sndcall,
                        struct t_call *rcvcall);
  static void t_error (const char *errmsg);
  static int t_free (char *ptr,
                     int struct_type);
  static int t_getinfo (ACE_HANDLE fildes,
                        struct t_info *info);
  static int t_getname (ACE_HANDLE fildes,
                        struct netbuf *namep,
                        int type);
  static int t_getstate (ACE_HANDLE fildes);
  static int t_listen (ACE_HANDLE fildes,
                       struct t_call *call);
  static int t_look (ACE_HANDLE fildes);
  static ACE_HANDLE t_open (char *path,
                            int oflag,
                            struct t_info *info);
  static int t_optmgmt (ACE_HANDLE fildes,
                        struct t_optmgmt *req,
                        struct t_optmgmt *ret);
  static int t_rcv (ACE_HANDLE fildes,
                    char *buf,
                    unsigned int nbytes,
                    int *flags);
  static int t_rcvdis (ACE_HANDLE fildes,
                       struct t_discon *discon);
  static int t_rcvrel (ACE_HANDLE fildes);
  static int t_rcvudata (ACE_HANDLE fildes,
                         struct t_unitdata *unitdata,
                         int *flags);
  static int t_rcvuderr (ACE_HANDLE fildes,
                         struct t_uderr *uderr);
  static int t_snd (ACE_HANDLE fildes,
                    const char *buf,
                    unsigned int nbytes,
                    int flags);
  static int t_snddis (ACE_HANDLE fildes,
                       struct t_call *call);
  static int t_sndrel (ACE_HANDLE fildes);
  static int t_sync (ACE_HANDLE fildes);
  static int t_unbind (ACE_HANDLE fildes);
};

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_TLI.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include "ace/post.h"
#endif /* ACE_OS_TLI_H */
