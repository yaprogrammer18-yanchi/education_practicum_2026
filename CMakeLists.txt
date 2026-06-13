cmake_minimum_required(VERSION 3.25)
project(Huffman_Archivator C)
add_compile_options(-Wall -Wextra -pedantic -fsanitize=address -g -O2)
add_link_options(-fsanitize=address)

# библиотеки

add_library(huffmanTree src/huffmanTree.c)
add_library(minHeap     src/minHeap.c)
add_library(frequencyTree src/frequencyTree.c)
add_library(fileReader src/fileReader.c)

# зависимости библиотек

target_link_libraries(frequencyTree PRIVATE huffmanTree minHeap)
target_link_libraries(fileReader PRIVATE frequencyTree)


# тесты
enable_testing()

add_executable(testHeap tests/testsForHeap.c)
target_link_libraries(testHeap PRIVATE huffmanTree minHeap)
add_test(NAME HeapTest COMMAND testHeap)

add_executable(testFrequencyTree tests/testsForFrequencyTree.c)
target_link_libraries(testFrequencyTree PRIVATE huffmanTree minHeap frequencyTree)
add_test(NAME FrequencyTreeTest COMMAND testFrequencyTree)

add_executable(testHuffmanTree tests/testsForHuffmanTree.c)
target_link_libraries(testHuffmanTree PRIVATE huffmanTree)
add_test(NAME HuffmanTreeTest COMMAND testHuffmanTree)

add_executable(integrationTest tests/testsIntergational.c)
target_link_libraries(integrationTest PRIVATE huffmanTree minHeap frequencyTree fileReader)
add_test(NAME IntegrationTest COMMAND integrationTest)

add_executable(systemTest tests/testsIntegrationalAndSystem.c)
target_link_libraries(systemTest PRIVATE fileReader)
add_test(NAME SystemTest COMMAND systemTest)

# исполняемый файл main
add_executable(main src/main.c)
target_link_libraries(main PRIVATE fileReader)