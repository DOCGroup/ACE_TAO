// $Id$

//=============================================================================
/**
 *  @file    ECDriver.h
 *
 *  $Id$
 *
 *  @author Bryan Thrall <thrall@cse.wustl.edu>
 */
//=============================================================================


#ifndef ECDRIVER_H
#define ECDRIVER_H

class ECDriver
{
public:
  ECDriver (void);

  virtual ~ECDriver (void);

  virtual bool get_time_master (void) const = 0;
  virtual void set_time_master (bool tm) = 0;
};

class ECTestDriver : public ECDriver
{
public:
  ECTestDriver (void);

  virtual ~ECTestDriver (void);

  virtual bool get_time_master (void) const;
  virtual void set_time_master (bool tm);

protected:
  bool time_master;
};

#endif // ECDRIVER_H
