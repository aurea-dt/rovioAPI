#-------------------------------------------------------------------------------
#
#
#	Notes:
#-------------------------------------------------------------------------------
APPNAME = appname
#Compiler:
	CC=g++

#Compiler flags
	CFLAGS=-c -g -Wall

#Directories
	DIRlib= /usr/local/lib	
	incDIR= /usr/local/include

#main function
	mainP= main
#-------------------------------------------------------------------------------
all: Project

Project: mainP.o
	$(CC) -o $(APPNAME) \
	-L $(DIRlib) \
	-I $(incDIR) \
	$(mainP).o -lespeak -lcurl

mainP.o: $(mainP).cpp
	$(CC) $(CFLAGS) \
	$(mainP).cpp

