echo "Running CMake..."
cmake -S . -B ./build && cmake --build ./build && cmake --install ./build && echo "Done." || echo "Error!"