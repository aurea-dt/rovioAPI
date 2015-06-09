#-------------------------------------------------------------------------------
#	AUTOR
#	Aurea Desarrollo Tecnológico.
#	FECHA
#	Notas:
#-------------------------------------------------------------------------------
APPNAME = GTKapp
#Compiler:
	CC=g++

#Compiler flags
	CFLAGS=-c -g -Wall
	INCLUIDES=$(shell pkg-config --cflags gtk+-2.0 libgnomeui-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
	LIBS=$(shell pkg-config --libs gtk+-2.0 libgnomeui-2.0 gstreamer-0.10 gstreamer-plugins-base-0.10)
#Directories
	DIRlib= /usr/local/lib	
	incDIR= /usr/local/include
#	libADT= $(CURDIR)/ADT_lib
	rovioDir = $(CURDIR)
#main function
	mainP= main
	callbacks = My_callbacks
	guiClass = My_guiClass
#-------------------------------------------------------------------------------
all: Project

Project: mainP.o guiClass.o callbacks.o ADT_joystick.o ADT_GTK.o
	$(CC) -export-dynamic -o $(APPNAME) \
	$(LIBS) -L$(rovioDir) \
	$(mainP).o \
	$(guiClass).o \
	$(callbacks).o \
	ADT_joystick.o \
	ADT_GTK.o \
	-lADT_Rovio -lgstinterfaces-0.10 -lespeak -lcurl
	
mainP.o: $(mainP).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(mainP).cpp
	
guiClass.o: $(guiClass).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(guiClass).cpp 

callbacks.o: $(callbacks).cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	$(callbacks).cpp

ADT_joystick.o: ADT_joystick.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	ADT_joystick.cpp 
	
ADT_GTK.o: ADT_GTK.cpp
	$(CC) $(INCLUIDES) $(CFLAGS) \
	-I $(CURDIR) \
	ADT_GTK.cpp 
