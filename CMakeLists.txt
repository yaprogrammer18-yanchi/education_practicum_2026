cmake_minimum_required(VERSION 3.25)
project(Huffman_Archivator C)
add_compile_options(-Wall -Wextra -pedantic -fsanitize=address -g)
add_link_options(-fsanitize=address)


add_library(HuffmanTree src/HuffmanTree.c)
add_library(minHeap     src/minHeap.c)
add_library(frequencyTree src/frequencyTree.c)
add_library(fileReader src/fileReader.c)

# зависимости frequencyTree от других либ
target_link_libraries(frequencyTree PRIVATE HuffmanTree minHeap)

add_executable(testsHeap tests/testsForHeap.c)
target_link_libraries(testsHeap PRIVATE HuffmanTree minHeap)

add_executable(testsFT tests/testsForFrequencyTree.c)
target_link_libraries(testsFT PRIVATE HuffmanTree minHeap frequencyTree)

target_link_libraries(fileReader PRIVATE frequencyTree)



# тест анализ-куча-дерево-ячейки

add_executable(testEv tests/testsCodeTable.c)
target_link_libraries(testEv PRIVATE HuffmanTree minHeap frequencyTree fileReader)