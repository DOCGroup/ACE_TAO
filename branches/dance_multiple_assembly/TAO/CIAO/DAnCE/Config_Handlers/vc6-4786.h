/**
 * @file vc6-4786.h
 * @author Will Otte <wotte@dre.vanderbilt.edu
 * $Id$
 * Silences that nefarious 4786 warning in a bit cleaner way
 * than sprinkling the same preprocessor directives all over the code.
 */

#ifndef VC6_4786_H_
# define VC6_4786_H_
# if defined (_MSC_VER) && (_MSC_VER < 1300)
#  pragma warning (disable: 4786)
# endif
#endif /* VC6_4786_H_ */
