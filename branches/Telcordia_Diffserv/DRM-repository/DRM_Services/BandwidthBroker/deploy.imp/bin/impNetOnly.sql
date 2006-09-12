use impNet;
--------------------------------------------------
-- host
--------------------------------------------------
INSERT INTO host (hostname,type,mib2_host,snmp_read_community,snmp_timeout,snmp_retries,telnet_username,telnet_password,enable_password,gateway_device,gateway_address,is_linux,has_rtc)
VALUES ('rondomon08','imp','Y','public','2','2','','','','eth1','','Y','N');
INSERT INTO host (hostname,type,mib2_host,snmp_read_community,snmp_timeout,snmp_retries,telnet_username,telnet_password,enable_password,gateway_device,gateway_address,is_linux,has_rtc)
VALUES ('rondomon09','imp','Y','public','2','2','','','','eth1','','Y','N');
INSERT INTO host (hostname,type,mib2_host,snmp_read_community,snmp_timeout,snmp_retries,telnet_username,telnet_password,enable_password,gateway_device,gateway_address,is_linux,has_rtc)
VALUES ('access1','router','Y','public','2','2','','','','','','N','N');
INSERT INTO host (hostname,type,mib2_host,snmp_read_community,snmp_timeout,snmp_retries,telnet_username,telnet_password,enable_password,gateway_device,gateway_address,is_linux,has_rtc)
VALUES ('access2','router','Y','public','2','2','','','','','','N','N');
INSERT INTO host (hostname,type,mib2_host,snmp_read_community,snmp_timeout,snmp_retries,telnet_username,telnet_password,enable_password,gateway_device,gateway_address,is_linux,has_rtc)
VALUES ('access3','router','Y','public','2','2','','','','','','N','N');
INSERT INTO host (hostname,type,mib2_host,snmp_read_community,snmp_timeout,snmp_retries,telnet_username,telnet_password,enable_password,gateway_device,gateway_address,is_linux,has_rtc)
VALUES ('access4','router','Y','public','2','2','','','','','','N','N');
--------------------------------------------------
-- imp_assign
--------------------------------------------------
INSERT INTO imp_assign (hostname,imp)
VALUES ('rondomon08','rondomon08');
INSERT INTO imp_assign (hostname,imp)
VALUES ('rondomon09','rondomon09');
INSERT INTO imp_assign (hostname,imp)
VALUES ('pool1bb','rondomon08');
INSERT INTO imp_assign (hostname,imp)
VALUES ('alpha','rondomon08');
INSERT INTO imp_assign (hostname,imp)
VALUES ('pool1db','rondomon08');
INSERT INTO imp_assign (hostname,imp)
VALUES ('pool2bb','rondomon09');
INSERT INTO imp_assign (hostname,imp)
VALUES ('pool2db','rondomon09');
INSERT INTO imp_assign (hostname,imp)
VALUES ('beta','rondomon09');
--------------------------------------------------
-- interface
--------------------------------------------------
