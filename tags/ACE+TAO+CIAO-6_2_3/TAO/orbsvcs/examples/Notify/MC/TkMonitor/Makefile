EXTERNAL = CosNotification NotifyMonitoringExt

all: $(EXTERNAL)

$(EXTERNAL):
	perl external_idl.pl

clean:
	perl external_idl.pl -r $(EXTERNAL)

