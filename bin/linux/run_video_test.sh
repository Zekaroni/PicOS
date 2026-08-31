# Run from the project root
g++ ./tests/graphicsTest.cpp -o ./tests/executables/graphicsTest \
    $(pkg-config --cflags --libs raylib)

./tests/executables/graphicsTest
