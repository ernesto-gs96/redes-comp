run: compile	
compile: compile-libs
	g++ cliente.cpp -o cliente PaqueteDatagrama.o SocketDatagrama.o -lpthread
	g++ servidor.cpp -o servidor PaqueteDatagrama.o SocketDatagrama.o -lpthread
compile-libs: 
	g++ -c PaqueteDatagrama.cpp
	g++ -c SocketDatagrama.cpp
clean:
	rm PaqueteDatagrama.o
	rm SocketDatagrama.o
	rm cliente.out
	rm servidor.out

