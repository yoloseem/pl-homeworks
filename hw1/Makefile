# Makefile
#

all: fsa #regex

clean:
	${RM} calc *.o fsa regex

fsa: fsa_main.o fsa.o
	${CXX} -o fsa fsa_main.o fsa.o

#regex: regex_main.o fsa.o
#	${CXX} -o regex regex_main.o fsa.o

fsa.o: fsa.cc fsa.h
	${CXX} -c fsa.cc

fsa_main.o: fsa_main.cc fsa.h
	${CXX} -c fsa_main.cc

#regex_main.o: regex_main.cc fsa.h
#	${CXX} -c regex_main.cc

