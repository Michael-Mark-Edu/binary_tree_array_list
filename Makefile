TEST = bin/run_tests
LIBS = -l:libgtest.a

FLAGS = -std=c++23 -pedantic -Wall -Wextra -Werror
ifeq ($(RELEASE), true)
	FLAGS += -O3
else
	FLAGS += -g3 -O0 -fprofile-arcs -ftest-coverage --coverage
endif

$(TEST): bin/binary_tree_array_list_tests.o bin/main.o
	g++ $(FLAGS) $^ -o $@ $(LIBS)

bin/binary_tree_array_list_tests.o: tests/binary_tree_array_list_tests.cpp src/binary_tree_array_list.h
	g++ $(FLAGS) $< -c -o $@

bin/main.o: tests/main.cpp
	g++ $(FLAGS) $< -c -o $@

.PHONY: build
build: $(TEST)

.PHONY: test
test: $(TEST)
	./$<

.PHONY: gcov
gcov: $(TEST)
	./$<
	gcov -o bin tests/*.cpp
	mv *.gcov bin/
	gcovr --html-details bin/report.html

.PHONY: vg
vg: $(TEST)
	valgrind --error-exitcode=1 --leak-check=full --track-origins=yes -s -v ./$<

.PHONY: clean
clean:
	rm bin/* vgcore.*

.PHONY: install
install: src/binary_tree_array_list.h
	mkdir -p /usr/local/include/imdast
	cp $^ /usr/local/include/imdast

.PHONY: uninstall
uninstall:
	rm /usr/local/include/imdast/binary_tree_array_list.h
