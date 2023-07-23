# vas_string.h library (<string.h> alternative)

Here is an alternative to the original C string library and the sscanf and sprintf functions that was created from scratch by my team and me.

My work on this project involved managing the team and dividing responsibilities, programming parts of the functions, and defining the overall structure of sprintf and sscanf (a method was devised through a loop: part of the string, specifier, parameter). I also wrote the original version of the gcvt function.

Also, in this project I came up with interesting unit tests with combinations of parameters that can be supplied and take test cases from a file for convenience. My teammate wrote tests according to this scheme. To run the tests, you need to enter "make test" (you may need the "check" library for C). The tests cover all functions and have a very high coverage.

The library is built into a .a file. To use it, you need to place vas_string.a and vas_string.h in the project folder, and you can use it just like the original library, but the functions have the prefix "vas_".

To build the library, simply type "make" in the terminal in the src folder of the project.

The library passed automated tests at Sber's School of Programming 21 (School 42 analogue), auto-check (as well as my own tests) also included valgrind test, and no leaks were discovered.

-----------------------------------------

Здесь написанная моей командой с нуля альтернатива оригинальной библиотеке string в C, а так же функций sscanf и sprintf.

Моя работа в этом проекте заключалась в управлении командой и разделении обязанностей, а так же в программировании части функций и определении общей структуры sprintf, sscanf (придуман метод через цикл: часть строки, спецификатор, параметр). Так же мной была написана оригинальная версия функции gcvt.

Так же я придумал в этом проекте интересные юнит-тесты с комбинациями параметров которые могут подаваться, и берущие тест-кейсы из файла, для удобства. Тесты по этой схеме были написаны моим сокомандником. Для запуска тестов необходимо ввести make test (возможно понадобится библиотека check для С). Тесты покрывают все функции и имеют очень высокое покрытие.

Библиотека билдится в .a файл. Для работы нужно поместить в папку проекта vas_string.а и vas_string.h, и можно пользоваться так же как оригинальной библиотекой, только функции иметь подпись "vas_"

Для билда библиотеки достаточно написать make в терминале в папке src проекта.

Библиотека прошла авто-тесты в Школе программирования 21 от Сбера (аналог Школы 42), авто-проверка (как и мои личные тесты) включала в себя тест на valgrind и ликов не было обнаружено.
