CC=g++

make: #format
	$(CC) *.cpp *.c -std=c++17 -o fcp

format:
	clang-format -i -style=Webkit *.c || TRUE