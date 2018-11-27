TARGET=a.out
OBJS=walk.o

all: $(TARGET)

clean:
	@rm -f $(OBJS) $(TARGET) *~ core

a.out: $(OBJS)
	$(CC) $(LDFLAGS) -o $(TARGET) $(OBJS)
.c.o:
	$(CC) $(CFLAGS) -o $@ -c $<

