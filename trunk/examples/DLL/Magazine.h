// $Id$

#ifndef MAGAZINE_H
#define MAGAZINE_H

class Magazine 
{
  //= TITLE 
  //   This is an abstract class used in the DLL example.
  //
  //= DESCRIPTION 
  //   This class simply is an inetrface which the derived classes will
  //   exploit.
 public:

  // No-op virtual destructor.
  virtual ~Magazine (void) {};
  
  // This method gives the title of the magazine.
  virtual void title (void) = 0;
};

#endif /* MAGAZINE_H */
