TARGETS=Crosses_zeros 
LIBS=`pkg-config --libs gtk+-2.0`
LIBS1=`pkg-config --cflags gtk+-2.0`
SOURCES=*.c *.o
CC=gcc
all: $(TARGETS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< $(LIBS1) 

$(TARGETS):Crosses_zeros.o Other_Cheking.o
	$(CC) -lm -lreadline *.o -o $@ $(LIBS)


install: all
	/usr/bin/install -m 755 $(TARGETS) /usr/bin

uninstall:
	cd /usr/bin/; /bin/rm $(TARGETS)

clean:
	/bin/rm $(TARGETS) *.o
