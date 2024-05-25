#include "util.h"

int random_int(int min, int max) { return rand() % (max - min + 1) + min; };

// the return value must be freed
char* read_file(std::string filename) {
    FILE* file;
    errno_t err = fopen_s(&file, filename.c_str(), "rb");
    if (err != 0) {
        std::cout << "Failed to open file: " << filename << std::endl;
        std::cout << "Error code: " << err << std::endl;
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    char* buffer = (char*)malloc(sizeof(char) * file_size + 1);
    if (buffer == NULL) {
        std::cout << "Failed to allocate memory for file: " << filename
                  << std::endl;
        return NULL;
    }
    size_t result = fread(buffer, 1, file_size, file);
    if (result != file_size) {
        std::cout << "Failed to read file: " << filename << std::endl;
        return NULL;
    }

    buffer[file_size] = '\0';
    fclose(file);
    return buffer;
}
