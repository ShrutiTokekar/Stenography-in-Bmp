# Steganography in BMP Images

## Description
This project implements a basic steganography technique using C to hide and extract secret messages inside 24-bit BMP images. It works by modifying the least significant bits (LSBs) of the image's pixel data to embed the contents of a text file without visibly altering the image.

## How It Works
- **Hiding:** Each character of the message is split into bits and stored in the LSBs of consecutive image bytes.
- **Terminator:** A null byte (`'\0'`) is added to indicate the end of the hidden message.
- **Extraction:** The LSBs are read from the stego image and reassembled into characters until the null terminator is reached.

## Author
Shruti Tokekar  
Computer Science – East Stroudsburg University
