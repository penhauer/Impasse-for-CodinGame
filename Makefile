
optimized: 
	g++ --std=c++11 -O2 mine.cpp ai.cpp board.cpp common.cpp game.cpp simple_terminal_player.cpp random_player.cpp
	# g++ mine.cpp common.h
	# g++ -c *.cpp -o impasse


debug: 
	g++ --std=c++11 -g mine.cpp ai.cpp board.cpp common.cpp game.cpp simple_terminal_player.cpp random_player.cpp
	# g++ mine.cpp common.h
	# g++ -c *.cpp -o impasse


clean:
	rm *.h.gch

