// -*- C++ -*-

// ================================================================
/**
 * @file DLL_Test_Parent.h
 *
 * @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 * @author Ossama Othman  <ossama@dre.vanderbilt.edu>
 */
// ================================================================

#ifndef ACE_TESTS_DLL_TEST_PARENT_H
#define ACE_TESTS_DLL_TEST_PARENT_H

#include "DLL_Test_Parent_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <cstdint>

class DLL_Test_Parent_Export Parent
{
public:
  virtual ~Parent ();

  virtual void test ();
};

class DLL_Test_Parent_Export Data
{
public:
  Data () = default;
  Data (const Data&) = default;
  Data (Data&&) = default;
  Data& operator= (const Data&) = default;
  Data& operator= (Data&&) = default;
  virtual ~Data();
private:
  int8_t i {};
};

class DLL_Test_Parent_Export Base
{
public:
  virtual void _raise () const;
  Base ();
  virtual ~Base() = default;
};

class DLL_Test_Parent_Export Derived : public Base
{
public:
  Derived ();
  static Base* _alloc ();
  void _raise () const override;
  ~Derived () override = default;
};

#endif /* ACE_TESTS_DLL_TEST_PARENT_H */
