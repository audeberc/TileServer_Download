xyz_tilescrapper: src/main.cpp
	g++ -o xyz_tilescrapper src/main.cpp -lcurl -lboost_program_options -lboost_filesystem -lboost_system

clean:
	rm -f *.o xyz_tilescrapper
