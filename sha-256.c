#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>

//! hash values .. 
uint32_t h0 = 0x6a09e667;
uint32_t h1 = 0xbb67ae85;
uint32_t h2 = 0x3c6ef372;
uint32_t h3 = 0xa54ff53a;
uint32_t h4 = 0x510e527f;
uint32_t h5 = 0x9b05688c;
uint32_t h6 = 0x1f83d9ab;
uint32_t h7 = 0x5be0cd19;

//! constants - cube roots of 1st 64 primes 
uint32_t constants[] = {
0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
     0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
      0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
       0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };



// Helper macros for right rotation and right shift
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define SHR(x, n) (x >> n)

void print_binary_big_endian(uint32_t value){
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1); // Right shift and mask to get the i-th bit
    }
}


// SHA-256 σ0 and σ1 functions
uint32_t sigma0(uint32_t x) {
    return ROTR(x, 7) ^ ROTR(x, 18) ^ SHR(x, 3);
}
uint32_t sigma1(uint32_t x) {
    return ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10);
}

// Function to convert a 0/1 char array of 8 bits into a uint8_t number
uint32_t binary_string_to_uint32_t(const char* binary_string) {
    uint32_t _SHA_256_ = 0;
    for (int i = 0; i < 32; i++) {
        // Shift _SHA_256_ left by 1 to make room for the next bit
        _SHA_256_ <<= 1;
        
        // If the character is '1', set the least significant bit of _SHA_256_
        if (binary_string[i] == '1') {
            _SHA_256_ |= 1;
        }
    }
    return _SHA_256_;
}
char* uint32ToBinaryString(uint32_t number) {
    char* binary_string = calloc(33, sizeof(char));
    for (int i = 31; i >= 0; i--) {
        binary_string[i] = (number & 1) ? '1' : '0';
        number >>= 1;
    }
    binary_string[32] = '\0'; // Null-terminate the string
    return binary_string;
}

char* sha_256(char* msg, int size) {

    // convert text to binary ...
    int size_in_bits = size * 8;
    int file_size = size_in_bits;
    printf("\nFile size : %d\n", size);
    printf("\nSize in bits : %d\n", file_size);

    char* binary = (char*)calloc(size_in_bits + 2, sizeof(char));
    
    for (int i = 0; i < size; i++) {
        unsigned char ch = msg[i];
        for (int j = 7; j >= 0; j--) {
            strcat(binary, (ch & (1 << j)) ? "1" : "0");
        }
    }
    binary[size_in_bits] = '\0';
    printf("\n%s\n", binary);

    // append a 1 
    strcat(binary, "1");
    size_in_bits++;
    binary[size_in_bits] = '\0';  
    printf("\n%s\n", binary);

    // append zeros until data is a multiple of 512, less 64 bits 
    int bits_we_have = (size_in_bits) % 512;
    printf("\nBits we have (\%512) :%d\n", bits_we_have);
    int no_of_zero = (512 - bits_we_have);
    if (no_of_zero < 64) {
        no_of_zero += 512;
    }   
    printf("\nNo of zeros : %d\n", no_of_zero);
    printf("\nno of zeros + size in bits : %d\n", no_of_zero + size_in_bits);
    
    char* new_mem = calloc(size_in_bits + no_of_zero + 1, sizeof(char));
    memcpy(new_mem, binary, size_in_bits);
    free(binary);
    printf("\n%s\n", new_mem);

    for (int i = 0; i < no_of_zero; i++) {
        strcat(new_mem, "0");
    }
    size_in_bits += no_of_zero;
    new_mem[size_in_bits] = '\0';
    printf("\n%s\n", new_mem);

    // place the size at the end 
    char* c_file_size = calloc(64, sizeof(char));

    // Convert decimal to binary
    for (int i = 63; i >= 0; i--) {
        c_file_size[i] = (file_size & 1) ? '1' : '0'; // Extract the least significant bit
        file_size >>= 1; // Shift number right by 1 to process the next bit
    }
    c_file_size[64] = '\0';
    printf("\n%s\n", c_file_size);

    for (int i = 0; i < 64; i++) {
        new_mem[size_in_bits - i - 1] = c_file_size[63 - i];
    }
    printf("\n%s\n", new_mem);
    free(c_file_size);


    // make message schedule 
    
    uint32_t no_of_chunks = size_in_bits/512; //no of chunks required to store msg
    char*** _chunks_64x32_ = (char***)calloc(no_of_chunks, sizeof(char**)); // allocating chunks
    printf("\nno of 512 chunks : %d\n", no_of_chunks);
    int idx = 0;
    uint8_t words_from_data = 512/32;  // 16 words in 512 bits
    uint8_t word_size = 32;     
    uint8_t last_48 = 48;

    for (uint32_t i = 0; i < no_of_chunks; i++) {
        _chunks_64x32_[i] = (char**)calloc(64, sizeof(char*));   // each chunk will have 64 rows of words
        for (uint16_t j = 0; j < 64; j++) {
            _chunks_64x32_[i][j] = (char*)calloc(33, sizeof(char));  //each word will have 32 chars  
            
            if (j < 16) {           // copying the msg in first 16 rows/words
                for (uint8_t k = 0; k < 32; k++) {
                    if (idx == size_in_bits) break;
                    _chunks_64x32_[i][j][k] = new_mem[idx++];
                    printf("%c", _chunks_64x32_[i][j][k]);
                }
            }
        else {                  // appending zeros in the rest 48 words
                for (uint8_t k = 0; k < 32; k++) {
                    _chunks_64x32_[i][j][k] = '0';
                }
            }
            _chunks_64x32_[i][j][32] = '\0';
        }
    }
    free(new_mem);

/*
    !Modify the zero-ed indexes at the end of the array using the following algorithm:
    !For i from w[16…63]:
     ~-> s0 = (w[i-15] rightrotate 7) xor (w[i-15] rightrotate 18) xor (w[i-15] rightshift 3)
     ~-> s1 = (w[i- 2] rightrotate 17) xor (w[i- 2] rightrotate 19) xor (w[i- 2] rightshift 10)
     ~-> w[i] = w[i-16] + s0 + w[i-7] + s1
*/

    uint32_t** w = (uint32_t**)calloc(no_of_chunks, sizeof(uint32_t*));
    for (uint32_t i = 0; i < no_of_chunks; i++) {

        //converting the char array to uint32
        w[i] = (uint32_t*)calloc(64, sizeof(uint32_t));
        for (uint8_t j = 0; j < 16; j++) {
            w[i][j] = binary_string_to_uint32_t(_chunks_64x32_[i][j]);
        }
        for (uint8_t j = 16; j < 64; j++) w[i][j] = 0;

        // applying the sigma functions
        for (uint8_t j = 16; j < 64; j++) {
            uint32_t s0 = sigma0(w[i][j - 15]);  // Calculate σ0
            uint32_t s1 = sigma1(w[i][j - 2]);   // Calculate σ1
            w[i][j] = w[i][j - 16] + s0 + w[i][j - 7] + s1;  // Compute w[i]
        }
    }

    // printing chunks 
    for (uint32_t i = 0; i < no_of_chunks; i++) {
        printf("\nChunk %d\n", i);
        for (uint16_t j = 0; j < 64; j++) {
            printf("    w%d\t|>  ", j);
            for (uint8_t k = 0; k < 32; k++) {
                printf("%c", _chunks_64x32_[i][j][k]);
            }
            printf("\n");
        }
        printf("\n\n\n");
    }

    // free the memory of chunks in char
    for (uint32_t i = 0; i < no_of_chunks; i++) {
        for (uint8_t j = 0; j < 64; j++) {
            free(_chunks_64x32_[i][j]);
        }
        free(_chunks_64x32_[i]);
    }
    free(_chunks_64x32_);

    for (uint32_t i = 0; i < no_of_chunks; i++) {
        printf("\nChunk %d\n", i);
        for (uint8_t j = 0; j < 64; j++) {
            printf("    w%d\t|>  ", j);
            print_binary_big_endian(w[i][j]);
            printf("\n");
        }
        printf("\n\n\n");
    }

    // compression
    for (uint32_t i = 0; i < no_of_chunks; i++) {
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        uint32_t e = h4;
        uint32_t f = h5;
        uint32_t g = h6;
        uint32_t h = h7;

        for (uint32_t j = 0; j < 64; j++) {
            uint32_t S1 = ROTR(e, 6) ^ ROTR(e, 11) ^ ROTR(e, 25);
            uint32_t choice = (e & f) ^ ((~e) & g);
            uint32_t temp1 = h + S1 + choice + constants[j] + w[i][j];
            uint32_t S0 = ROTR(a, 2) ^ ROTR(a, 13) ^ ROTR(a, 22);
            uint32_t majority = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + majority;

            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }

        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
        h4 += e;
        h5 += f;
        h6 += g;
        h7 += h;
    }
    // free the memory of w
    for (uint32_t i = 0; i < no_of_chunks; i++) {
        free(w[i]);
    }
    free(w);

    printf("\nSHA-256 (binary) : ");
    print_binary_big_endian(h0);
    print_binary_big_endian(h1);
    print_binary_big_endian(h2);
    print_binary_big_endian(h3);
    print_binary_big_endian(h4);
    print_binary_big_endian(h5);
    print_binary_big_endian(h6);
    print_binary_big_endian(h7);

    char* _SHA_256_ = calloc(65, sizeof(char));
    sprintf(_SHA_256_, "%08x%08x%08x%08x%08x%08x%08x%08x", h0, h1, h2, h3, h4, h5, h6, h7);

    printf("\n\nSHA-256 (hex) : %s\n\n", _SHA_256_);
    return _SHA_256_;
}

char* read_file_to_string(const char* filename) {
    
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Could not open file %s\n", filename);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);  

    char* buffer = (char*)calloc((file_size + 1), sizeof(char));
    if (buffer == NULL) {
        printf("Error: Memory allocation failed\n");
        fclose(file);
        return NULL;
    }
    size_t read_size = fread(buffer, sizeof(char), file_size, file);
    buffer[read_size] = '\0';  
    fclose(file);

    return buffer;
}


int main(int argc, char* argv[]) {
    if (argc < 5) {
        fprintf(stderr, "Usage: %s --fpath <file_path> --sha256path <output_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *file_path = NULL;
    char *output_path = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--fpath") == 0 && i + 1 < argc) {
            file_path = argv[++i];
        } else if (strcmp(argv[i], "--sha256path") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        }
    }

    if (!file_path || !output_path) {
        fprintf(stderr, "Invalid arguments. Ensure --fpath and --sha256path are provided.\n");
        return EXIT_FAILURE;
    }

    struct stat st;
    if (stat(file_path, &st) != 0) {
        perror("Error accessing file");
        return EXIT_FAILURE;
    }

    struct sysinfo info;
    if (sysinfo(&info) == 0 && st.st_size > info.totalram) {
        fprintf(stderr, "File size exceeds available system memory. Terminating.\n");
        return EXIT_FAILURE;
    }

    char* content = read_file_to_string(file_path);
    if (!content) {
        fprintf(stderr, "Failed to read file content.\n");
        return EXIT_FAILURE;
    }

    char* hash_output = sha_256(content, strlen(content));
    free(content);

    if (!hash_output) {
        fprintf(stderr, "SHA-256 computation failed.\n");
        return EXIT_FAILURE;
    }

    FILE* out_file = fopen(output_path, "w");
    if (!out_file) {
        perror("Error opening output file");
        free(hash_output);
        return EXIT_FAILURE;
    }
    fprintf(out_file, "%s\n", hash_output);
    fclose(out_file);
    free(hash_output);

    printf("SHA-256 hash written to: %s\n", output_path);
    return EXIT_SUCCESS;
}
