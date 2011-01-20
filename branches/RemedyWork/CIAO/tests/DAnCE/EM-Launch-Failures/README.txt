
 This script tests redeployment of plans after initial launch failures.
 Each test uses 2 descriptors; the first with a configured launch failure
 (at startup/configuration complete or at activate), the second for the
 same plan without the configured failure.
 After detecting the expected lauch failure of the first plan the test
 relaunches using the second plan and finishes by tearing it down.
 