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

# calculate
#
calculate: main.o calculate.o
	$(link) $@ $^
main.o : main.cc
	$(compile) $< -o $@
calculate.o : calculate.cc calculate.h
	$(compile) $< -o $@

#Cpplint style checker
style: calculate.cc calculate.h main.cc
	cpplint --root=$(path) $^

#Clean up
clean:
	rm -f *.o calculate