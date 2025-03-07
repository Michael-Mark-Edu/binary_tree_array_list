# binary_tree_array_list

An AVL tree internally represented as an array.

Because of the need to copy potentially large amounts of data during AVL
rebalancing and deletion, insertion and deletion can drop down to O(n) in the
worst case when a standard linked list implementation of an AVL tree is always
O(log n) in these cases. However, thanks to cache locality, this data structure
should *theoretically* perform faster than a linked tree in operations like
searching (no benchmarking has been done thus far).

The memory layout of the tree is very similar to a std::vector, where more
memory than is typically needed is allocated to allow the data structure to
grow. As of now, the tree will allocate a new "layer" every time a node is
inserted and would go into the new layer (even if that node is moved back up to
the above layer after AVL balancing). Ultimately, this means that although
memory complexity is O(n), some amount of memory is "wasted".

Although there are theoretical advantages, there is a reason why every AVL tree
is a linked list. That's why I consider this an Impractical Data Structure
(ImDaSt).

## Time Complexity (compared to linked implementation)

| Operation | Array Best Case | Array Worst Case | Linked Best Case | Linked Worst Case |
|-----------|-----------------|------------------|------------------|-------------------|
| Search    | O(log n)        | O(log n)         | O(log n)         | O(log n)          |
| Insert    | O(log n)        | O(n)             | O(log n)         | O(log n)          |
| Remove    | O(log n)        | O(n)             | O(log n)         | O(log n)          |

## Installation

This is a header-only library, so just place `src/binary_tree_array_list.h`
somewhere your compiler can find. The library works on C++17-23.

### Linux (maybe Mac too?)

To install the header file into `/usr/local/include` (gcc and clang
automatically pull header files from here, so it should just work), simply run:

```
sudo make install
```

To uninstall, run:

```
sudo make uninstall
```

### Windows

Copy `src/binary_tree_array_list.h` to somewhere Visual Studio recognizes as a
place to look for header files at.

## Testing

This library uses gtest for testing. It assumes that gtest is installed
globally.

### Linux

Run tests using the following:

```
mkdir build
make test
```

Or alternatively:

```
make
./build/run_tests
```

Additionally, to use valgrind:

```
make vg
```

Or to enable code coverage via gcov and gcovr:

```
make gcov
```

Finally, you may add optimization to any of these if the tests are too slow
(make sure to run `make clean` first to clear out the unoptimized build):

```
make clean
make vg OPTIMIZE=true
```

### Windows

Uh... good luck. This codebase isn't structured into a VS solution, but you may
be able to copy all of the source and testing files into a VS project and get it
to run that way.

## Usage

If installed globally, use the following include:

```
#include <imdast/binary_tree_array_list.h>
```

If installed locally to your project, use a relative path instead.

The class is in the `imdast` namespace, so watch out for that.

## License

This library uses the MIT license. See `LICENSE` or the license header of
`src/binary_tree_array_list.h` for the fine print.
