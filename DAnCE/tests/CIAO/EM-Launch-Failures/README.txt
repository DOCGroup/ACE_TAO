 
 This script tests redeployment of plans in after initial launch failures.
 Each test uses 2 descriptors; teh first with a configured launch failure,
 the second for the same plan except the configured failure.
 
 As of this moment (2010-03-25) all these tests fail because of shortcomings
 in the exception handling and teardown logic of the node entities.
 
