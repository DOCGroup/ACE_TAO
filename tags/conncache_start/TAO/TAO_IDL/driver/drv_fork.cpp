// $Id$

/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.
2550 Garcia Avenue
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

/*
 * DRV_fork.cc - Fork a process for each file to be processed, wait for
 *               status from the child process
 */

#include        "idl.h"
#include        "idl_extern.h"

#include        "drv_private.h"

#include "ace/Process_Manager.h"

ACE_RCSID(driver, drv_fork, "$Id$")

/*
 * Fork off a process, wait for it to die
 */
void
DRV_fork()
{
  // This will not work on NT, but I can hardly think of some way to
  // make it work.
  // The idea is to make it compile, and always use the compiler with
  // just one file, that works because then there is no fork
  // involved.
  for (DRV_file_index = 0;
       DRV_file_index < DRV_nfiles;
       ++DRV_file_index)
    {
      ACE_Process_Options options;
      options.creation_flags (ACE_Process_Options::NO_EXEC);

      ACE_Process_Manager manager;
      pid_t child_pid = manager.spawn (options);
      if (child_pid == 0)
        {
          /*
           * OK, do it to this file (in the child)
           */
          DRV_drive(DRV_files[DRV_file_index]);
          ACE_OS::exit(0);
        }

      if (child_pid == -1)
        {
          cerr << GTDEVEL("IDL: spawn failed\n");
          ACE_OS::exit (99);
          /*NOTREACHED*/
        }

      // child_pid is the process id of something at this point.
      if (manager.wait () == -1)
        {
          cerr << GTDEVEL("IDL: wait failed\n");
          ACE_OS::exit (99);
          /*NOTREACHED*/
        }
    }
  /*
   * Now the parent process can exit
   */
  ACE_OS::exit (0);
}
