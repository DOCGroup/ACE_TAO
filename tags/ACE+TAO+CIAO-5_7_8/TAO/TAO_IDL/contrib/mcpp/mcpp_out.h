/* $Id$ mcpp_out.h: declarations of OUTDEST data types for MCPP  */
#ifndef _MCPP_OUT_H
#define _MCPP_OUT_H

#ifdef OUT
#undef OUT
#endif

/* Choices for output destination */
enum OUTDEST {
    OUT,                        /* ~= fp_out    */
    ERR,                        /* ~= fp_err    */
    DBG,                        /* ~= fp_debug  */
    NUM_OUTDEST
};

#endif  /* _MCPP_OUT_H  */
