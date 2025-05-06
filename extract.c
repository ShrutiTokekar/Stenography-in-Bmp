#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BMP_HEADER_SIZE 54
#define BMP_INPUT "santorini-stega.bmp"
#define TEXT_OUTPUT "stegno.txt"
#define PERMS 0644

// Helper to print error and exit
void fail(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int main() {
    int bmp = open(BMP_INPUT, O_RDONLY);
    if (bmp < 0) fail("Opening stego BMP");

    int out = open(TEXT_OUTPUT, O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    if (out < 0) fail("Creating output text file");

    // Skip header
    if (lseek(bmp, BMP_HEADER_SIZE, SEEK_SET) != BMP_HEADER_SIZE)
        fail("Seeking BMP content");

    unsigned char bmp_byte;
    unsigned char current_char = 0;
    int bit_count = 0;

    while (read(bmp, &bmp_byte, 1) == 1) {
        int lsb = bmp_byte & 1;
        current_char = (current_char << 1) | lsb;
        bit_count++;

        if (bit_count == 8) {
            if (current_char == '\0') break;
            write(out, &current_char, 1);
            current_char = 0;
            bit_count = 0;
        }
    }

    close(bmp);
    close(out);
    return 0;
}


