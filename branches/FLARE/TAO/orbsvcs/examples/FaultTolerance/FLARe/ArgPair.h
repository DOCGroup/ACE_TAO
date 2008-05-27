#ifndef _ARGPAIR_H
#define _ARGPAIR_H

struct  ArgPair
{
  ArgPair (int argc, char **argv);
  ~ArgPair (void);
  ArgPair (const ArgPair &);
  ArgPair & operator = (const ArgPair &);
  void swap (ArgPair &);

  int argc;
  char **argv;
};

#endif /// _ARGPAIR_H

