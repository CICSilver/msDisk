TARGET=libxcom.so
OBJS=msserver_task.o xthread.o xthread_pool.o
CPPFLAGS=-fPIC
LIBS=-levent -lpthread
$(TARGET):$(OBJS)
	g++ -shared $+ -o $@ $(LIBS)

clean:
	rm -rf $(OBJS) $(TARGET)