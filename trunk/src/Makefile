cc = g++

#target=seed.o
target=mysql_ops.o
target+=logic.o
target+=mock.o

a.out: $(target)
	$(cc) $(target) -lpthread -lmysqlclient

logic.o: logic.cc
	$(cc) -c $<
mysql_ops.o: mysql_ops.cc
	$(cc) -c $<
mock.o.o: mock.cc
	$(cc) -c $<

clean:  *.o a.out
