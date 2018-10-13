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
char** Create_pointers(char* buffer,
                        int quantity_enters, int size_buffer);

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


int main()
{

    FILE* input;
    input = fopen(INPUT_FILE, "r");
    assert(input);
    int quantity_enters = 0;
    int size_buffer = 0;
    char* buffer = Read_text(input, &quantity_enters, &size_buffer);
    assert(buffer);
    if(quantity_enters < 0)
    {
        printf("ERROR in QUANTITIES of enters");
        return 1;
    }
    if(!std::isfinite(quantity_enters))
        std::cout << "File is very big!";
    FILE* output = fopen(OUTPUT_FILE, "a");
    assert(output);
    /*for(int i = 0; i < size_buffer; i++)
    {
        std::cout << (int)buffer[i] << " ";
    }*/

    char** pointers = Create_pointers(buffer, quantity_enters,
                                       size_buffer);
    assert(pointers);
    for(int i = 0; i < quantity_enters; i++)
    {
        assert(pointers[i]);
    }
    qsort(pointers, quantity_enters, sizeof(char*),
           str_comparision);
    for(int i = 0; i < quantity_enters; i++)
    {
        assert(pointers[i]);
    }

    for(int i = 0; i < quantity_enters; i++)
    {
        fputs(pointers[i], output);
        fputc('\n', output);
        /*char cur_char = *(pointers[i]);

        int j = 0;
        while(cur_char != '\n' && (fputc(cur_char, output) != EOF))
        {
            //std::cout << cur_char << "\n";
            j++;
            cur_char = *(pointers[i] + j);
        }
        j = fputc('\n', output);

        assert(j != EOF);*/
    }
    fputc('\n', output);
    qsort(pointers, quantity_enters, sizeof(char*),
           String_comparision);
    for(int i = 0; i < quantity_enters; i++)
    {
        assert(pointers[i]);
    }

    for(int i = 0; i < quantity_enters; i++)
    {
        fputs(pointers[i], output);
        fputc('\n', output);
    }
    return 0;
}

char* Read_text(FILE* input, int* Quantity_enters, int* Size_buffer)
{
    assert(Quantity_enters);
    assert(Size_buffer);
    int end_file = 0;
    fseek(input, 0, SEEK_END);
    end_file = ftell(input);

    fseek(input, 0, SEEK_SET);
    int start_file = ftell(input);
    if(end_file <= start_file)
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
    assert(end_file);


    char* buffer = (char*)calloc(long_file+1, sizeof(char));
    fread(buffer, long_file, 1, input);

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

char** Create_pointers(char* buffer, int quantity_enters,
                        int size_buffer)
{
    if(quantity_enters < 0 || size_buffer <= 0)
    {
        printf("ERROR with parameters in Create_pointers");
        return nullptr;
    }
    char** pointers_res = (char**)calloc(quantity_enters,
                                  sizeof(char*));
    assert(pointers_res);
    pointers_res[0] = buffer;
    int j = 1;
    for(unsigned int i = 1; i < size_buffer; i++)
    {
        char cur_char = buffer[i];
        if(cur_char == '\0' &&
            buffer[i+1] != '\0' && (i+1) < size_buffer)
        {
            pointers_res[j] = buffer + i + 1;
            j++;
        }
    }
    if(j != quantity_enters)
        std::cout << "ERROR with enters\n"
         << "Waiting: " << quantity_enters
         << "\nComes: " << j << std::endl;


    return pointers_res;
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
    int first_length = strlen(first_str);
    int second_length = strlen(second_str);
    char end1 = first_str[first_length-1],
        end2 = second_str[second_length-1];
    if(end1 == '.' || end1 == '!' || end1 == ',' ||
       end1 == '?' || end1 == ':' || end1 == ';')
        first_length-=1;
    if(end2 == '.' || end2 == '!' || end2 == ',' ||
       end2 == '?' || end2 == ':' || end2 == ';')
        second_length-=1;
    int i = 0;
    for(i = 0; i < first_length &&
    i < second_length; i++)
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
    if(i == first_length-1) return 1;
    else if(i == second_length-1) return -1;
    printf("ERROR with func in qsort");
    return 0;
}





