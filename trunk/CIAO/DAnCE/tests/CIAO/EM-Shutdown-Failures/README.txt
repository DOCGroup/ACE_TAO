 
 This script tests redeployment of plans after shutdown failures.
 Each test uses 2 descriptors; the first with a configured shutdown failure 
 (at passivate or at remove), the second for the same plan without the 
 configured failure.
 After successfully launching the first plan and detecting the expected 
 shutdown failure of the first plan at tear down the test repeats the cycle
 using the second plan.
 When tearing down the first plan the test uses the '-f' argument for the
 plan launcher to force it to complete the tear down (tear down application 
 manager) even after catching errors from the first step (application tear 
 down).
 