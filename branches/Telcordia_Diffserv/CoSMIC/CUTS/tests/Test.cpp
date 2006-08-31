// $Id$

#include "Test.h"
#include "Unit_Test.h"
#include "ace/Log_Msg.h"

//
// CUTS_Test
//
CUTS_Test::CUTS_Test (const char * name)
: name_ (name),
  passed_ (0),
  failed_ (0)
{

}

//
// CUTS_Test
//
CUTS_Test::~CUTS_Test (void)
{
  for (CUTS_Unit_Test_List::iterator iter = this->unit_test_.begin ();
       iter != this->unit_test_.end ();
       iter ++)
  {
    delete (*iter);
  }
}

//
// run
//
void CUTS_Test::run (int argc, char * argv [])
{
  for (CUTS_Unit_Test_List::iterator iter = this->unit_test_.begin ();
       iter != this->unit_test_.end ();
       iter ++)
  {
    if ((*iter)->run (argc, argv) == 0)
    {
      ++ this->passed_;
    }
    else
    {
      ACE_ERROR ((LM_ERROR,
                  "%M - %s::%s failed (%s)\n",
                  this->name_.c_str (),
                  (*iter)->name (),
                  (*iter)->message ()));

      ++ this->failed_;
    }
  }
}

//
// add_unit_test
//
void CUTS_Test::add_unit_test (CUTS_Unit_Test * unit_test)
{
  this->unit_test_.push_back (unit_test);
}

//
// close
//
void CUTS_Test::close (void)
{
  delete this;
}

//
// name
//
const char * CUTS_Test::name (void) const
{
  return this->name_.c_str ();
}

//
// passed
//
size_t CUTS_Test::passed (void) const
{
  return this->passed_;
}

//
// failed
//
size_t CUTS_Test::failed (void) const
{
  return this->failed_;
}
