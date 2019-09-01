CC=g++
CFLAGS=-std=c++14 -Wall -Wextra -pedantic -O3

test_exh: exhaustive.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) exhaustive.cpp -o test_exh

test_cc: copy_ctor.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) copy_ctor.cpp -o test_cc

test_2: Test_2.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) Test_2.cpp -o test_2

test_1: Test_1.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) Test_1.cpp -o test_1

performance: Performance_test.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) Performance_test.cpp -o test_performance

minimal: Minimal.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) Minimal.cpp -o test_minimal

morse: Morse_code.cpp Map.hpp SkipList.hpp Node.hpp
	$(CC) $(CFLAGS) Morse_code.cpp -o test_morse

clean:
	rm test_*