all: client server

client: ../classes/NexusClientApplet.class

../classes/NexusClientApplet.class:
	javac -d ../classes -depend -g NexusClientApplet.java 

server: ../classes/NexusIIserver.class

../classes/NexusIIserver.class: 
	javac -d ../classes -depend -g NexusIIserver.java

clean:
	rm -f ../classes/*.class *~

