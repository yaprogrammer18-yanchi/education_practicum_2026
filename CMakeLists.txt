cmake_minimum_required(VERSION 3.25)
project(Huffman_Archivator C)
add_compile_options(-Wall -Wextra -pedantic -fsanitize=address -g)
add_link_options(-fsanitize=address)

# библиотеки

add_library(HuffmanTree src/HuffmanTree.c)
add_library(minHeap     src/minHeap.c)
add_library(frequencyTree src/frequencyTree.c)
add_library(fileReader src/fileReader.c)

# зависимости библиотек

target_link_libraries(frequencyTree PRIVATE HuffmanTree minHeap)
target_link_libraries(fileReader PRIVATE frequencyTree)


# тесты

add_executable(testsHeap tests/testsForHeap.c)
add_executable(testsFT tests/testsForFrequencyTree.c)
add_executable(testsHuffmanTree tests/testsForHuffmanTree.c)
add_executable(integrationTests tests/testsIntergational.c)
add_executable(systemTests tests/testsSystem.c)

target_link_libraries(testsHeap PRIVATE HuffmanTree minHeap)
target_link_libraries(testsFT PRIVATE HuffmanTree minHeap frequencyTree)
target_link_libraries(testsHuffmanTree PRIVATE HuffmanTree)
target_link_libraries(integrationTests PRIVATE HuffmanTree minHeap frequencyTree fileReader)
target_link_libraries(systemTests PRIVATE fileReader)


# исполняемый файл main
add_executable(main src/main.c)
target_link_libraries(main PRIVATE fileReader)
