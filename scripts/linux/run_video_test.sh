# Run from the project root
g++ ./tests/graphicsTest.cpp -o ./bin/graphicsTest \
    $(pkg-config --cflags --libs raylib)

./bin/graphicsTest
