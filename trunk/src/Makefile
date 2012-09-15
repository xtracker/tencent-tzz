
target = cgi-server
objs = logic.o mysql_ops.o cgi-server.o

$(target) : $(objs)
	$(CXX) -o $(target) $(objs) -lfcgi -lpthread -lmysqlclient


all : $(target)

clean:
	rm $(objs) $(target)
