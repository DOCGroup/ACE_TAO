# $Id$
use ARMSBB;

## insert switches
INSERT INTO Switch (name, equipmentID, vendor, password, enablePW, managedIPAddr, readCommunity, osVersion)
VALUES
  ("daisy", "5000", "Cisco", "ngnlab", "mtv", "daisy", "public", "CatOS"),
  ("daffodil", "5000", "Cisco", "ngnlab", "mtv", "daffodil", "public", "CatOS");

## Insert PTPs
INSERT INTO PTP (name, adminState, operationalState, rate, phyAddress, switchID, routerID)
VALUES
  ('3/1', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/4', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 1, NULL),
  ('3/1', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/4', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 2, NULL);

## Insert Links
INSERT INTO Link (aPtpID, zPtpID)
VALUES
  (1, 5),
  (4, 8);
