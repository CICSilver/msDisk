TARGET=libcom.so
OBJS=msserver_task.o xthread.o xthread_pool.o
CPPFLAGS=-fPIC
LIBS=-levent -lpthread
$(TARGET):$(OBJS)
	g++ -shared $+ -o $@ $(TARGET)

clean:
	rm -rf $(OBJS) $(TARGET)