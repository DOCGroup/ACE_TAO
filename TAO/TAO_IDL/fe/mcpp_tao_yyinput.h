/** $Id$
 * @file mcpp_yyinput.h
 * @author William R. Otte <wotte@dre.vanderbilt.edu>
 * 
 * Provides a YY_INPUT macro which enables lexing from an in-memory buffer
 * instead of a file.
 */

#ifndef MCPP_YYINPUT_H
#define MCPP_YYINPUT_H

extern char *tao_preproc_buffer;
extern int tao_preproc_buffer_length;

int tao_preproc_buffer_pos = 0;

#define TAO_YY_INPUT(buf,result,max_size)                           \
  {                                                                 \
    result = 0      ;                                               \
    while ((tao_preproc_buffer_pos < tao_preproc_buffer_length) &&  \
          (result < max_size))                                      \
      {                                                             \
        buf[result] = tao_preproc_buffer[tao_preproc_buffer_pos];   \
        ++tao_preproc_buffer_pos;                                   \
        ++result;                                                   \
      }                                                             \
    if (result == 0) result = TAO_YY_NULL;}                         \

#endif /* MCPP_YYINPUT_H */
