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

#ifndef _DRV_LINK_DRV_LINK_HH
#define _DRV_LINK_DRV_LINK_HH

// drv_link.hh - Define data and functions which are used to 
//		 link in the back end functions

/*
 * Front end interface
 */

class File;
typedef void (* DRV_PF_FE_init_stage1)();
typedef void (* DRV_PF_FE_init_stage2)();
typedef int  (* DRV_PF_FE_yyparse)();
typedef void (* DRV_PF_FE_set_yyin)(File *);

class UTL_Error;
class UTL_Indenter;
class UTL_String;
typedef UTL_Error * (* DRV_PF_FE_new_UTL_Error)();
typedef UTL_Indenter * (* DRV_PF_FE_new_UTL_Indenter)();
typedef UTL_String * (* DRV_PF_FE_new_UTL_String)(char *);

extern DRV_PF_FE_init_stage1	DRV_FE_init_stage1;
extern DRV_PF_FE_init_stage2	DRV_FE_init_stage2;
extern DRV_PF_FE_yyparse	DRV_FE_yyparse;
extern DRV_PF_FE_set_yyin	DRV_FE_set_yyin;

extern DRV_PF_FE_new_UTL_Error	DRV_FE_new_UTL_Error;
extern DRV_PF_FE_new_UTL_Indenter DRV_FE_new_UTL_Indenter;
extern DRV_PF_FE_new_UTL_String	DRV_FE_new_UTL_String;

/* 
 * Back end interface
 */

class AST_Generator;
typedef AST_Generator * (* DRV_PF_BE_init)();
typedef void (* DRV_PF_BE_produce)();
typedef void (* DRV_PF_BE_abort)();
typedef void (* DRV_PF_BE_prep_arg)(char *, idl_bool);
typedef void (* DRV_PF_BE_version)();

extern DRV_PF_BE_init		DRV_BE_init;
extern DRV_PF_BE_produce 	DRV_BE_produce;
extern DRV_PF_BE_abort   	DRV_BE_abort;
extern DRV_PF_BE_prep_arg	DRV_BE_prep_arg;
extern DRV_PF_BE_version	DRV_BE_version;

// Functions

extern void DRV_FE_open();
extern void DRV_FE_close();

extern void DRV_BE_open();
extern void DRV_BE_close();

#endif	// _DRV_LINK_DRV_LINK_HH
