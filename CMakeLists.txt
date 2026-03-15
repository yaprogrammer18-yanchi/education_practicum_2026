cmake_minimum_required(VERSION 3.25)
project(Huffman_Archivator C)
add_compile_options(-Wall -Wextra -pedantic -g)

add_library(HuffmanTree src/HuffmanTree.c)
add_library(minHeap     src/minHeap.c)
add_library(frequencyTree src/frequencyTree.c)

# зависимости frequencyTree от других либ
target_link_libraries(frequencyTree PRIVATE HuffmanTree minHeap)

add_executable(testsHeap tests/testsForHeap.c)
target_link_libraries(testsHeap PRIVATE HuffmanTree minHeap)

add_executable(testsFT tests/testsForFrequencyTree.c)
target_link_libraries(testsFT PRIVATE HuffmanTree minHeap frequencyTree)