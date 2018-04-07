.SUFFIXES: .cpp .o

CC=g++
PROC=proc

ORACLE_HOME=/opt/oracle/product/11.2.0
ORAFLAGS1=/usr/include/linux
ORAFLAGS2=/usr/lib/gcc/i686-redhat-linux/4.4.4/include

PROCSRCS=myoracle.pc
DBSRCS=$(PROCSRCS:.pc=.cpp)

SRCS=qqserver.cpp\
	pub.cpp\
	work.cpp\
	$(DBSRCS)

OBJS=$(SRCS:.cpp=.o)
EXEC=qqserverd

ORCFLAGS1=-L${ORACLE_HOME}/lib
ORCFLAGS2=-lclntsh

all: $(OBJS)
	$(CC) -o $(EXEC) $(OBJS) $(ORCFLAGS1) $(ORCFLAGS2)
	@echo '-------------ok--------------'

.cpp.o: $(DBSRCS)
	$(CC) -Wall -g -o $@ -c $<
	
$(DBSRCS):
	${PROC} INAME=$(PROCSRCS) INCLUDE=$(ORAFLAGS1) INCLUDE=$(ORAFLAGS2) CPOOL=YES MODE=ANSI CODE=CPP PARSE=PARTIAL THREADS=YES ONAME=$(DBSRCS)

clean:
	rm -f $(OBJS)
	rm -f $(DBSRCS)
	rm -f core*

