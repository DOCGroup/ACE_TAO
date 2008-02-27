/* $Id$ */
#ifndef MCPP_LIB_H
#define MCPP_LIB_H

#ifdef OUT
#undef OUT
#endif

/* Choices for output destination */
typedef enum {
    OUT,                        /* ~= fp_out    */
    ERR,                        /* ~= fp_err    */
    DBG,                        /* ~= fp_debug  */
    NUM_OUTDEST
} OUTDEST;

#include "mcpp_lib_export.h"

extern MCPP_LIB_Export int     mcpp_lib_main( int argc, char ** argv);
extern MCPP_LIB_Export void    mcpp_reset_def_out_func( void);
extern MCPP_LIB_Export void    mcpp_set_out_func(
                    int (* func_fputc)  ( int c, OUTDEST od),
                    int (* func_fputs)  ( const char * s, OUTDEST od),
                    int (* func_fprintf)( OUTDEST od, const char * format, ...)
                    );
extern MCPP_LIB_Export void    mcpp_use_mem_buffers( int tf);
extern MCPP_LIB_Export char *  mcpp_get_mem_buffer( OUTDEST od);
//#endif  /* MCPP_LIB */
#endif  /* MCPP_LIB_H   */
