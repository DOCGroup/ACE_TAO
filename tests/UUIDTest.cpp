$Id$

#include "test_config.h"
#include "ace/UUID.h"
#include <string>

class Tester
{
public:
  Tester();
 ~Tester();
 
 int init( int argc, char* argv[]);
 void fini();
 int test();
 
private:
 int test1( void);
 int test2( void);
 int test3( void);
 int test4( void);
 void test5( void*& d);
};
	
Tester::Tester()
{	
}

Tester::~Tester()
{
}

int Tester::init( int, char*[])
{
 return 0;
}

void Tester::fini( void)
{
}

int Tester::test()
{
  if (test1() == -1)
    return -1;
  if (test2() == -1)
    return -1;

  return 0;
}

int Tester::test1( void)
{
 
  ACE_UUID nilACE_UUID;
  ACE_UUID anotherNilACE_UUID;
    
  if (nilACE_UUID != anotherNilACE_UUID)
    return -1;
  if (!(nilACE_UUID == anotherNilACE_UUID))
    return -1;
  
  nilACE_UUID = anotherNilACE_UUID;
  if (nilACE_UUID != anotherNilACE_UUID)
    return -1;
  if (!(nilACE_UUID == anotherNilACE_UUID))
    return -1;
  
  ACE_UUID yetAnotherNilACE_UUID( anotherNilACE_UUID);
  if (yetAnotherNilACE_UUID != anotherNilACE_UUID)
    return -1;
  if (!(yetAnotherNilACE_UUID == anotherNilACE_UUID))
    return -1;
  
  ACE_SString asString = nilACE_UUID.toString();
  if (asString != "00000000-0000-0000-0000-000000000000")
    return -1;
  
  
  ACE_UUID uuid( "00000000-0000-0000-0000-000000000000");
  
  if (uuid != ACE_UUID())
    return -1;
  if (!(uuid == ACE_UUID()))
    return -1;
  if (uuid.toString() != "00000000-0000-0000-0000-000000000000")
    return -1;
  
  //  try {
  
  // Wrong size
      ACE_UUID uuid1( "00000000-0000-0000-0000");
      //return -1;
      
    //} catch( const IllegalArgument& ) {

      //;  Expected

     //try {

      // Wrong format
      ACE_UUID uuid2( "000-000-0000-0000-0000-000000000000");
      //return -1;

    //} catch( const IllegalArgument& ) {

      //;  Expected

    
    //try {

      // Unsupported varient - this one is reserved for Microsoft backward compatibility
      ACE_UUID uuid3( "00000000-0000-0000-C000-000000000000");
      //return -1;

    //} catch( const IllegalArgument& ) {

      //;  Expected

    
    //try {

      // Unsupported version - this one is reserved for DCE Security
      ACE_UUID uuid4( "00000000-0000-2000-8000-000000000000");
      //return -1;

    //} catch( const IllegalArgument& ) {

      //; // Expected

    
    //try {

      ACE_UUID uuid5( "00000000-0000-1000-8000-000000000000");
      ACE_UUID uuid6( "00000000-0000-3000-8000-000000000000");
      ACE_UUID uuid7( "00000000-0000-4000-8000-000000000000");

    //} catch( const IllegalArgument& ) {

      //return -1;

    
    //try {

      ACE_SString asString1( "01234567-89ab-1cde-8f01-23456789abcd");
      ACE_UUID uuid8( asString1);
      if (uuid.toString() != asString1)
        return -1;

    //} catch( const IllegalArgument& ) {

      //return -1;

     return 0;
}

int Tester::test2( void)
{

   ACE_UUID uuid( );//ACE_UUID::newACE_UUID);

  /*
    if (uuid == ACE_UUID())
      return -1;
  */

    ACE_UUID* uuid1 = new ACE_UUID( );//ACE_UUID::newACE_UUID);
    ACE_UUID* uuid2 = new ACE_UUID( );//ACE_UUID::newACE_UUID);

    if (uuid1 == uuid2)
      return -1;
    if (!(uuid1 != uuid2))
      return -1;

    if (!(uuid1 < uuid2))
      return -1;
    if (!(uuid1 <= uuid2))
      return -1;
    if ((uuid1 >= uuid2))
      return -1;
    if ((uuid1 > uuid2))
      return -1;
  
  return 0;
}



// main ------------------------------------------------------------------------


int main(int argc, char* argv[])
{

  ACE::init();
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Starting %s test at %D\n"), "ACE_UUIDTest"));


  ACE_UUID_Impl::UUID_t uuid_t;
  ACE_UUID uuid (uuid_t);
  
  
  ACE_DEBUG ((LM_DEBUG,
	      "%s \n",
	      uuid.toString ().c_str()));
  

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) Ending %s test at %D\n"), "ACE_UUIDTest"));

  ACE_UUID uuid1 ("5c32ea90-1dd2-11b2-acda-000795b08ec6");
  ACE_DEBUG ((LM_DEBUG,
	      "%s \n",
	      uuid1.toString ().c_str()));


  ACE::fini();

  return 0;
}
