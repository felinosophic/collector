# Collector
***Not even alpha yet!!! Earliest stages of development!***


A LLM based news aggregator that allows for deep similiraty matching of previous news items - YWIT you won't see it twice paradigm


## Requirements
1. Install CMake
2. Install Conan 2
3. Run ```conan profile detect --force```
   
## Debug Build 
```
conan install . --profile:host=default -s build_type=Debug --build=missing
cmake --preset conan-debug
cmake --build --preset conan-debug
```

## Release Build 
```
conan install . --profile:host=default -s build_type=Release --build=missing
cmake --preset conan-release
cmake --build --preset conan-release
```
