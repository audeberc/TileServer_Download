xyz_tilescrapper: src/main.cpp
	g++ -o xyz_tilescrapper src/main.cpp src/string_utils.cpp src/map_utils.cpp src/download_utils.cpp -I./include -lcurl -lboost_program_options -lboost_filesystem -lboost_system

clean:
	rm -f *.o xyz_tilescrapper
