#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BMP_HEADER_SIZE 54
#define BMP_INPUT "Santorini.bmp"
#define TEXT_INPUT "nVidia-openACC.txt"
#define BMP_OUTPUT "santorini-stega.bmp"
#define PERMS 0644

// Helper to print error and exit
void fail(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

// Write one bit of `bitval` into LSB of `byte`
unsigned char insert_bit(unsigned char byte, int bitval) {
    return (byte & 0xFE) | (bitval & 0x01);
}

int main() {
    // Read entire text file into buffer
    FILE *txt = fopen(TEXT_INPUT, "rb");
    if (!txt) fail("Opening text file");

    fseek(txt, 0, SEEK_END);
    long txt_size = ftell(txt);
    fseek(txt, 0, SEEK_SET);

    char *text = malloc(txt_size + 1);
    if (!text) fail("Memory allocation");

    fread(text, 1, txt_size, txt);
    fclose(txt);
    text[txt_size] = '\0';  // Ensure null termination

    // Open BMP files
    int in_bmp = open(BMP_INPUT, O_RDONLY);
    if (in_bmp < 0) fail("Opening input BMP");

    int out_bmp = open(BMP_OUTPUT, O_WRONLY | O_CREAT | O_TRUNC, PERMS);
    if (out_bmp < 0) fail("Creating output BMP");

    // Copy BMP header
    char header[BMP_HEADER_SIZE];
    if (read(in_bmp, header, BMP_HEADER_SIZE) != BMP_HEADER_SIZE)
        fail("Reading BMP header");
    write(out_bmp, header, BMP_HEADER_SIZE);

    // Embed each bit of each character (including null)
    int char_idx = 0, bit_idx = 7;
    unsigned char bmp_byte;
    while (read(in_bmp, &bmp_byte, 1) == 1) {
        if (char_idx <= txt_size) {
            unsigned char current = text[char_idx];
            int bit = (current >> bit_idx) & 1;
            bmp_byte = insert_bit(bmp_byte, bit);
            bit_idx--;
            if (bit_idx < 0) {
                bit_idx = 7;
                char_idx++;
            }
        }
        write(out_bmp, &bmp_byte, 1);
    }

    close(in_bmp);
    close(out_bmp);
    free(text);
    return 0;
}

