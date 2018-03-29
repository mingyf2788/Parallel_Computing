prime : main.o ProgressPool.o
	g++ main.o ProgressPool.o -o prime

main.o :src/main.cpp
	g++ -c src/main.cpp -o  main.o -std=c++11

ProgressPool.o :src/ProgressPool.cpp
	g++ -c src/ProgressPool.cpp -o  ProgressPool.o -std=c++11

clean:
	rm *.o
