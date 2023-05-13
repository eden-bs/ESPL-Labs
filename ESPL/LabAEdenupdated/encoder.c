
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int positive_modulo(int a, int b) {
    int result = a % b;
    return result >= 0 ? result : result + b;
}


int main(int argc, char *argv[]) {
    FILE *in = stdin, *out = stdout;
    char *key = NULL;
    int idx = 0, curr = 0, len = 0, sign = 1, mode = 0;

    for (int i = 1; i < argc; i++) {
        if (strncmp(argv[i], "+D",2) == 0) mode = 1;
        else if (strncmp(argv[i], "-D",2) == 0) mode = 0;
        else if ((strncmp(argv[i], "+e", 2) == 0 || strncmp(argv[i], "-e", 2) == 0)) {
            if(argv[i][0] == '+')
                sign = 1;
            if(argv[i][0] == '-')
                sign = -1;
            key = argv[i] + 2;
            while(key[len]){
                len++;
            }
        }
        else if (strncmp(argv[i], "-i", 2) == 0) {
            in = fopen(argv[i] + 2, "r");
            if (!in) {
                fprintf(stderr, "Cant open input file");
                return 1;
            }
        }
        else if (strncmp(argv[i], "-o", 2) == 0) {
            out = fopen(argv[i] + 2, "w");
            if (!out) {
                fprintf(stderr, "Cant open input file");
                return 1;
            }
        }
        if(mode) fprintf(stderr, "%s\n", argv[i]);


    }

    while ((curr = fgetc(in)) != EOF) {
        if(key){
            if((curr >= '0' && curr <= '9') || (curr >= 'A' && curr <= 'Z') || (curr >= 'a' && curr <= 'z')){
                int shift = sign * (key[idx] - '0');
                if (curr >= '0' && curr <= '9') {
                    curr = positive_modulo(curr - '0' + shift, 10) + '0';
                }
                else if (curr >= 'A' && curr <= 'Z') {
                    curr = positive_modulo(curr - 'A' + shift, 26) + 'A';
                }
                else { 
                    curr = positive_modulo(curr - 'a' + shift, 26) + 'a';
                }


            }

            idx = (idx + 1) % len;
        }

    fputc(curr, out);
        }


    if(in != stdin)
        fclose(in);
    if(out != stdout)
     fclose(out);
    return 0;
}