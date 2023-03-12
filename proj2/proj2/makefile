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
text-client : text-client.o
	$(link) $@ $^

text-client.o : text-client.cc text-client.h UnixDomainSocket.h
	$(compile) $< -o $@

#text server compile and link
text-server : text-server.o
	$(link) $@ $^

text-server.o : text-server.cc text-server.h UnixDomainSocket.h
	$(compile) $< -o $@

#UnixDomainSocket class compile and link for client/server
# UnixDomainSocket : UnixDomainSocket.o
# 	$(link) $@ $^

# UnixDomainSocket.o : UnixDomainSocket.cc UnixDomainSocket.h
# 	$(compile) $< -o $@

#Cpplint style checker
style : text-server.cc text-server.h text-client.cc text-client.h
	cpplint --root=$(path) $^

clean:
	rm -f *.o text-server text-client
