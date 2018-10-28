
/*
    В целом по кодстайлу - хорошо бы разбивать код на кусочки.
    У тебя весь код сплошным текстом идет, а так не должно быть,
    читается не очень. Разделяй на логичские блоки. То есть, скажем,
    "вызов функции + проверка аргументов", "цикл + переменные к нему"
    и тд.

    И определись с кодстайлом. У тебя функции ни с того ни с сего 
    начинаются то с больших, то с маленьких букв. Надо более
    внимательно относиться к этому...
 */

#include <iostream>
#include <stdio.h>
#include <string>
#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <cmath>
#include <ctype.h>

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

/*!
    Function to read text from file and
    prepare that to analysis
    @param input - file with text

    @param Quantity_enters - pointer to save
        quantity of string
    @param size_buffer - pointer to save
        quantity of string

    @return Pointer to array, which was read
*/
char* Read_text(FILE* input, int* Quantity_enters,
                 int* size_buffer);

/*!
    Function to parsing buffer by strings

    @param input - file with text
    @param quantity_enters - quantity of strings
    @param size_buffer - size of buffer

    @return Pointer to array of strings
*/
char** Create_list_strings(char* buffer,
                        int quantity_enters, int size_buffer);

/*
    Не вижу разницы названиях этих двух функций. Не отражают они сути
    польностью.
 */
/*!
    Function for qsorting
*/
int String_comparision(const void* first_str,
                          const void* second_str);
/*!
    Function for qsorting
*/
int str_comparision(const void* first_str,
                          const void* second_str);

/*
    Есть смысл сделать так, чтобы она возвращала число строк, 
    потому что буфер ты и так меняешь, можно явно не переприсваивать.
    Тогда не надо будет передавать Quantity_enters, вызов будет проще
 */
char* reassembly_buffer(char* buffer,
                         int* Quantity_enters, int long_file);


int main()
{
/*
    Не надо так писать. Лучше

    FILE* input = fopen(INPUT_FILE, "r");
 */
    FILE* input;
    input = fopen(INPUT_FILE, "r");
/*
    Проверка ассертами пропадет в Release сборке. Надо ручками 
    проверять
 */
    assert(input);
    int quantity_enters = 0;
    int size_buffer     = 0;
    char* buffer = Read_text(input, &quantity_enters, &size_buffer);
    buffer = reassembly_buffer(buffer, &quantity_enters, size_buffer);
    if(buffer == nullptr)
    {
/*
    Во время отладки можно печатать инфу, что выходишь здесь.
    И один стейтмент обычно в скобки не заключают.
 */
        return 1;
    }
    if(quantity_enters < 0)
    {
        printf("ERROR in QUANTITIES of enters");
        return 1;
    }
    if(!std::isfinite(quantity_enters))
        std::cout << "File is very big!";
    FILE* output = fopen(OUTPUT_FILE, "a");
/*
    Уже говорил
 */
    assert(output);
    
    char** list_strings = Create_list_strings(buffer, quantity_enters,
                                       size_buffer);
    if(list_strings == nullptr)
    {
        return 1;
    }
/*
    Страшное дело)
    Это вообще стремная вещь - в release тупо пробежаться 
    по всем строчкам, ничего не делая... Странно, странно)

    Если это проверка на ненулевость, то лучше условную компиляцию
    сделать и закинуть в функцию VerifyLines()
 */
    for(int i = 0; i < quantity_enters; i++)
    {
        assert(list_strings[i]);
    }

    qsort(list_strings, quantity_enters, sizeof(char*),
           str_comparision);
/*
    Про такое уже говорил, да?)
 */
    for(int i = 0; i < quantity_enters; i++)
    {
        assert(list_strings[i]);
    }

/*
    fputs - не страшно, но зачем два вызова, если можно просто 
    printf()?
 */
    for(int i = 0; i < quantity_enters; i++)
    {
        fputs(list_strings[i], output);
        fputc('\n', output);

    }
    fputc('\n', output);

    qsort(list_strings, quantity_enters, sizeof(char*),
           String_comparision);
/*
    Ну понятно, да?))
 */
    for(int i = 0; i < quantity_enters; i++)
    {
        assert(list_strings[i]);
    }

    for(int i = 0; i < quantity_enters; i++)
    {

        fputs(list_strings[i], output);
        fputc('\n', output);
    }
    return 0;
}

char* Read_text(FILE* input, int* Quantity_enters, int* Size_buffer)
{
    assert(Quantity_enters);
    assert(Size_buffer);

/*
    Непонятно, зачем переменная тут? Объявить ее прямо 
    перед использованием лучше
 */
    int end_file = 0;

    int count_fseek = fseek(input, 0, SEEK_END);
    if(count_fseek)
    {
        printf("ERROR with fseek in the end");
        return nullptr;
    }
    end_file = ftell(input);


    count_fseek = fseek(input, 0, SEEK_SET);
    if(count_fseek)
    {
        printf("ERROR with fseek in the start");
        return nullptr;
    }

    int start_file = ftell(input);
    if(end_file <= start_file ||
        end_file == -1 || start_file == -1)
    {
        printf("ERROR with start and end of file");
        return nullptr;
    }

    int long_file = end_file - start_file;
    if(!std::isfinite(end_file) || !std::isfinite(long_file))
    {
        printf("ERROR: file is big");
        return nullptr;
    }

    *Size_buffer = long_file;
/*
    Снова странный ассерт в середине кода... Пропадет же проверка
 */
    assert(end_file);

    char* buffer = (char*)calloc(long_file+1, sizeof(char));
/*
    calloc может на законных основаниях вернуть ошибку и прога упадет
    молча. Обидно же :(
 */
    fread(buffer, long_file, 1, input);

    return buffer;
}

char* reassembly_buffer(char* buffer, int* Quantity_enters, int long_file)
{
    for(int i = 0; i < long_file; i++)
    {
        //std::cout << (int)buffer[i]<< *(Quantity_enters) << "\n";
        if(buffer[i] == '\n')
        {
            (*Quantity_enters)++;
            buffer[i] = '\0';
        }
        if(buffer[i] == '\0' && buffer[i-1] == '\0'
            && buffer[i+1] != '\0')
            (*Quantity_enters)--;
    }
    buffer[long_file] = '\0';
    return buffer;
}

char** Create_list_strings(char* buffer, int quantity_enters,
                        int size_buffer)
{
    if(quantity_enters < 0 || size_buffer <= 0)
    {
        printf("ERROR with parameters in Create_list_strings");
        return nullptr;
    }
    char** list_strings_res = (char**)calloc(quantity_enters,
                                  sizeof(char*));
/*
    Снооова...)
 */
    assert(list_strings_res);
    list_strings_res[0] = buffer;
/*
    Это можно внести в for:

    for(int j = 1, unsigned int i = 1; i < size_buffer; i++)
 */
    int j = 1;
    for(unsigned int i = 1; i < size_buffer; i++)
    {
        char cur_char = buffer[i];
        if(cur_char == '\0' &&
            buffer[i+1] != '\0' && (i+1) < size_buffer)
        {
            list_strings_res[j] = buffer + i + 1;
            j++;
        }
    }
    if(j != quantity_enters)
        std::cout << "ERROR with enters\n"
         << "Waiting: " << quantity_enters
         << "\nComes: " << j << std::endl;


    return list_strings_res;
}


int str_comparision(const void* first_str_void,
                          const void* second_str_void)
{
    assert(first_str_void);
    assert(second_str_void);
    return strcmp(*(char**)first_str_void, *(char**)second_str_void);
}

int String_comparision(const void* first_str_void,
                        const void* second_str_void)
{
    char* first_str = *(char**) first_str_void;
    char* second_str = *(char**) second_str_void;

/*
    Лучше когда все собираешь, заюзать структуру, где будешь хранить 
    длину или указатели на начало и конец строки. strlen - дорого
 */
    int first_length = strlen(first_str);
    int second_length = strlen(second_str);

    char end1 = first_str[first_length-1],
        end2 = second_str[second_length-1];
/*
    Для этого есть isalfa :)
 */
    if(end1 == '.' || end1 == '!' || end1 == ',' ||
       end1 == '?' || end1 == ':' || end1 == ';')
        first_length-=1;
    if(end2 == '.' || end2 == '!' || end2 == ',' ||
       end2 == '?' || end2 == ':' || end2 == ';')
        second_length-=1;
    int i = 0;
    for(i = 0; i < first_length && i < second_length; i++)
    {

        if (first_str[first_length - i-1] >
            second_str[second_length-i-1])
        {
            std::cout << first_str[first_length - i -1] << " ";
            std::cout << second_str[second_length - i -1] << "\n";
            return 1;
        }
        else if(first_str[first_length-i-1] <
                second_str[second_length-i-1])
            return -1;
    }
    if(i == first_length-1)         return 1;
    else if(i == second_length-1)   return -1;

    printf("ERROR with func in qsort");
    return 0;
}






