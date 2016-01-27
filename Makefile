all: server stat sigint jpg
stat: stat.cpp
	g++ -o stat stat.cpp
server: server.cpp
	g++ -o server server.cpp
sigint: sigint.cpp
	g++ -o sigint sigint.cpp
jpg: jpg.cpp
    g++ -o jpg jpg.cpp
