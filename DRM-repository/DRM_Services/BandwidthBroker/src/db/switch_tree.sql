# $Id$
use ARMSBB;

## insert switches
INSERT INTO Switch (name, equipmentID, vendor, osVersion, password, enablePW, managedIPAddr, readCommunity)
VALUES
  ("daisy", "5000", "Cisco", "CatOS", "ngnlab", "mtv", "daisy", "public"),
  ("daffodil", "5000", "Cisco", "CatOS", "ngnlab", "mtv", "daffodil", "public"),
  ("dummy", "6500", "Cisco", "CatOS", "ngnlab", "mtv", "dummy", "public"),
  ("dummy2", "5000", "Cisco", "CatOS", "ngnlab", "mtv", "dummy2", "public"),
  ("dummy3", "5000", "Cisco", "CatOS", "ngnlab", "mtv", "dummy3", "public"),
  ("dandelion", "6500", "Cisco", "IOS", "ngnlab", "mtv", "192.4.20.146", "public");

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
  ('3/12', 'up', 'up', 100000, 'phy', 2, NULL),
  ('3/1', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/4', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 3, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 4, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 4, NULL),
  ('3/5', 'up', 'up', 100000, 'phy', 5, NULL),
  ('3/12', 'up', 'up', 100000, 'phy', 5, NULL),
  ('FastEthernet4/1', 'up', 'up', 100000, 'phy', 6, NULL),
  ('FastEthernet4/2', 'up', 'up', 100000, 'phy', 6, NULL);



## Insert Links
INSERT INTO Link (aPtpID, zPtpID)
VALUES
  (1, 5),
  (6, 9),
  (10, 13),
  (15, 8);


## Insert appropriate state into the activeVLAN table
INSERT INTO activeVLAN (vlanID, ptpID, switchID)
VALUES
  (100, 1, 1),
  (100, 5, 2),
  (100, 6, 2),
  (100, 9, 3),
  (100, 10, 3),
  (100, 13, 4),
#  (100, 15, 5),
  (100, 8, 2);


## Insert L2Interface
INSERT INTO L2Interface (name, adminState, operationalState, ptpID, switchID)
VALUES
  ('3/1', 'up', 'up', 1, 1),
  ('3/4', 'up', 'up', 2, 1),
  ('3/5', 'up', 'up', 3, 1),
  ('3/12', 'up', 'up', 4, 1),
  ('3/1', 'up', 'up', 5, 2),
  ('3/4', 'up', 'up', 6, 2),
  ('3/5', 'up', 'up', 7, 2),
  ('3/12', 'up', 'up', 8, 2),
  ('3/1', 'up', 'up', 9, 3),
  ('3/4', 'up', 'up', 10, 3),
  ('3/5', 'up', 'up', 11, 3),
  ('3/12', 'up', 'up', 12, 3),
  ('3/5', 'up', 'up', 13, 4),
  ('3/12', 'up', 'up', 14, 4),
  ('3/5', 'up', 'up', 15, 5),
  ('3/12', 'up', 'up', 16, 5),
  ('Fa4/1', 'up', 'up', 17, 6),
  ('Fa4/2', 'up', 'up', 18, 6);
