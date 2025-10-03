conan install . --build=missing -s="build_type=Debug"
cmake --preset=conan-debug
cp build/Debug/compile_commands.json .
