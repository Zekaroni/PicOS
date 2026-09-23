# Run from the project root
g++ main.cpp -o ./bin/main \
    $(pkg-config --cflags --libs raylib)

./bin/main
