TEST = bin/run_tests
LIBS = -l:libgtest.a

FLAGS = -std=c++23 -pedantic -Wall -Wextra -Werror
ifeq ($(OPTIMIZE), true)
	FLAGS += -O3
else
	FLAGS += -g3 -O0 -fprofile-arcs -ftest-coverage --coverage
endif

SOURCES = $(wildcard tests/*.cpp)
OBJECTS = $(SOURCES:tests/%.cpp=bin/%.o)

$(TEST): $(OBJECTS)
	g++ $(FLAGS) $^ -o $@ $(LIBS)

bin/%.o: tests/%.cpp src/binary_tree_array_list.h
	g++ $(FLAGS) $< -c -o $@

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
