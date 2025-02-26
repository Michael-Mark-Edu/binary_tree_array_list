COMPILER = clang++
FLAGS = -std=c++23 -g3 -O0 -pedantic -Wall -Wextra -Werror
TEST = bin/run_tests
LIBS = -l:libgtest.a

$(TEST): bin/binary_tree_array_list_tests.o bin/main.o
	$(COMPILER) $(FLAGS) $^ -o $@ $(LIBS)

bin/binary_tree_array_list_tests.o: tests/binary_tree_array_list_tests.cpp src/binary_tree_array_list.h
	$(COMPILER) $(FLAGS) $< -c -o $@

bin/main.o: tests/main.cpp
	$(COMPILER) $(FLAGS) $< -c -o $@

.PHONY: build
build: $(TEST)

.PHONY: test
test: $(TEST)
	./$<

.PHONY: clean
clean:
	rm bin/*
