all:
	g++ -o ./rpgen *.cpp core/*.cpp utils/*.cpp -ILEDA/incl -LLEDA -lG -lL -lz -g -O3
	cp ./rpgen ./cada039
clean:
	rm -f ./rpgen
	rm -f ./cada039
	rm -f tmp*
