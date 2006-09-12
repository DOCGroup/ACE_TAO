use ARMSBB;
update PTP set operationalState = 'down' where id =4;
update PTP set operationalState = 'down' where id =16;
DELETE FROM activeVLAN where ptpID=4;
DELETE FROM activeVLAN where ptpID=16;

INSERT INTO activeVLAN (vlanID, ptpID, switchID)
VALUES
  (100, 11, 3),
  (100, 17, 4);

commit;
