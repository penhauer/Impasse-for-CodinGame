
optimized: 
	g++ --std=c++11 -O2 mine.cpp board.cpp common.cpp game.cpp simple_terminal_player.cpp random_player.cpp minimax_player.cpp state.cpp
	# ai.cpp


debug: 
	g++ --std=c++11 -g mine.cpp board.cpp common.cpp game.cpp simple_terminal_player.cpp random_player.cpp minimax_player.cpp state.cpp

clean:
	rm *.h.gch

