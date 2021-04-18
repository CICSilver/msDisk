TARGET=msdisk_server
OBJS=msdisk_server.o xfile_server_task.o
CPPFLAGS=-I ../Com
LIBS=-L ../Com -lxcom
$(TARGET):$(OBJS)
		g++ $+ -o $@ $(LIBS)

clean:
		rm -rf $(OBJS) $(TARGET)
