all: server stat sigint
stat: stat.cpp
	g++ -o stat stat.cpp
server: server.cpp
	g++ -o server server.cpp
sigint: sigint.cpp
	g++ -o sigint sigint.cpp
