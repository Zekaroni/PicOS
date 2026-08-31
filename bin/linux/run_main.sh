# Run from the project root
g++ main.cpp -o main \
    $(pkg-config --cflags --libs raylib)

./main
