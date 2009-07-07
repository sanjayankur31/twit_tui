CFLAGS = -g -Wall -Wextra -W -pedantic -ansi -c 
CC = gcc
OBJS =  test.o greet.o authenticate.o update.o xml_parser.o display_time_line.o sqlite_ops.o make_dir.o send_update.o send_reply.o errorapp.o auto.o update_dm.o xml_parser_dm.o display_dm.o send_dm.o destroy.o
LFLAGS = -lncurses `curl-config --libs` `xml2-config --libs`  -lsqlite3 
XMLCFLAGS = `xml2-config --cflags`

twit_tui : $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o twit_tui

test.o : src/test.c src/test.h 
	$(CC) $(CFLAGS) src/test.c

greet.o : src/greet.c src/greet.h
	$(CC) $(CFLAGS) src/greet.c
	
authenticate.o : src/authenticate.c src/authenticate.h
	$(CC) $(CFLAGS) src/authenticate.c

send_update.o : src/send_update.c src/send_update.h
	$(CC) $(CFLAGS) src/send_update.c

send_dm.o : src/send_dm.c src/send_dm.h
	$(CC) $(CFLAGS) src/send_dm.c

send_reply.o : src/send_reply.c src/send_reply.h
	$(CC) $(CFLAGS) src/send_reply.c

update_dm.o : src/update_dm.c src/update_dm.h
	$(CC) $(CFLAGS) src/update_dm.c

update.o : src/update.c src/update.h
	$(CC) $(CFLAGS) src/update.c

destroy.o : src/destroy.c src/destroy.h
	$(CC) $(CFLAGS) src/destroy.c

xml_parser.o : src/xml_parser.c src/xml_parser.h
	$(CC) $(CFLAGS) $(XMLCFLAGS) src/xml_parser.c

xml_parser_dm.o : src/xml_parser_dm.c src/xml_parser_dm.h
	$(CC) $(CFLAGS) $(XMLCFLAGS) src/xml_parser_dm.c

display_time_line.o : src/display_time_line.c src/display_time_line.h
	$(CC) $(CFLAGS) src/display_time_line.c

display_dm.o : src/display_dm.c src/display_dm.h
	$(CC) $(CFLAGS) src/display_dm.c

make_dir.o : src/make_dir.c src/make_dir.h
	$(CC) $(CFLAGS) src/make_dir.c

sqlite_ops.o : src/sqlite_ops.c src/sqlite_ops.h
	$(CC) $(CFLAGS) src/sqlite_ops.c -Wl,-R/usr/local/lib 

errorapp.o : src/errorapp.c src/errorapp.h
	$(CC) $(CFLAGS) src/errorapp.c 

auto.o : src/auto.c src/auto.h
	$(CC) $(CFLAGS) src/auto.c

fav.o : src/fav.c src/fav.h
	$(CC) $(CFLAGS) src/fav.c
clean :
	rm -f *.o twit_tui 

test :
	make -B
	rm -f src/*.c

install :
	make -B
	cp twit_tui /bin/
	make clean

uninstall :
	make clean
	rm -f /bin/twit_tui
	
