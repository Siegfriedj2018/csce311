# Copyright 2023 Siegfrij(Justin Siegfried)

cc = g++
path =..
standard = c++17

flags = -std=$(standard)
flags += -Wall
flags += -pedantic
flags += -g
flags += -I $(path)

link = $(cc) $(flags) -o
compile = $(cc) $(flags) -c


#text client compile and link
text-client : domainsocketclient.o text-client.o
	$(link) $@ $^

text-client.o : TextClient.cc UnixDomainSocket.h DomainSocketServer.h
	$(compile) $< -o $@

domainsocketclient.o : DomainSocketClient.cc DomainSocketClient.h UnixDomainSocket.h
	$(compile) $< -o $@

#text server compile and link
text-server : domainsocketserver.o text-server.o
	$(link) $@ $^

text-server.o : TextServer.cc UnixDomainSocket.h DomainSocketServer.h
	$(compile) $< -o $@

domainsocketserver.o : DomainSocketServer.cc DomainSocketServer.h UnixDomainSocket.h
	$(compile) $< -o $@

#UnixDomainSocket class compile and link for client/server
# UnixDomainSocket : UnixDomainSocket.o
# 	$(link) $@ $^

# UnixDomainSocket.o : UnixDomainSocket.cc UnixDomainSocket.h
# 	$(compile) $< -o $@

#Cpplint style checker
style : TextServer.cc \
		DomainSocketServer.cc \
		DomainSocketServer.h \
		TextClient.cc \
		DomainSocketClient.cc \
		DomainSocketClient.h \
		UnixDomainSocket.h
	cpplint --root=$(path) $^

clean:
	rm -f *.o text-server text-client
