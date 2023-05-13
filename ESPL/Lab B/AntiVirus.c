#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <endian.h>

typedef struct virus {
    unsigned short SigSize;
    char virusName[16];
    unsigned char* sig;
} virus;

typedef struct link link;

struct link {
    link *nextVirus;
    virus *vir;
};



virus* readVirus(FILE* file) {
    virus *v = (virus*)malloc(sizeof(virus));
    if (v == NULL) {
        perror("Error allocating memory");
        exit(1);
    }

    fread(&(v->SigSize), sizeof(unsigned short), 1, file);

    if(v->SigSize == 0) {
        free(v);
        return NULL;
    }

    fread(v->virusName, sizeof(char), 16, file);

    v->sig = (unsigned char*)malloc(v->SigSize * sizeof(unsigned char));

    if (v->sig == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
    
    fread(v->sig, sizeof(unsigned char), v->SigSize, file);

    
    return v;
}

void printVirus(virus* virus, FILE* output) {
    fprintf(output, "Virus Name: %s\n", virus->virusName);
    fprintf(output, "Signature Length: %hu\n", virus->SigSize);
    fprintf(output, "Signature: ");
    for (int i = 0; i < virus->SigSize; i++) {
        fprintf(output, "%02X ", virus->sig[i]);
    }
    fprintf(output, "\n");
}

void list_print(link* virus_list) {
    link* curr = virus_list;
    while (curr != NULL) {
        printVirus(curr->vir, stdout); // Pass stdout as the output stream
        curr = curr->nextVirus;
    }
}

link* list_append(link* virus_list, virus* data) {
    link* new_link = (link*)malloc(sizeof(link));
    if (new_link == NULL) {
        perror("Error allocating memory");
        exit(1);
    }
    new_link->nextVirus = NULL;
    new_link->vir = data;

    if (virus_list == NULL) 
        return virus_list = new_link;
    
    link* curr = virus_list;
    while (curr->nextVirus != NULL) 
        curr = curr->nextVirus;
    
    curr->nextVirus = new_link;
    
    return virus_list;
}

void list_free(link* virus_list) {
    link* curr = virus_list;
    while (curr != NULL) {
        link* next = curr->nextVirus;
        free(curr->vir->sig);
        free(curr->vir);
        free(curr);
        curr = next;
    }
}

void detect_virus(char *buffer, unsigned int size, link *virus_list) {
    for (unsigned int i = 0; i < size; i++) {
        link *curr = virus_list;
        while (curr != NULL) {
            virus *v = curr->vir;
            if (i + v->SigSize <= size && memcmp(buffer + i, v->sig, v->SigSize) == 0) {
                printf("Virus detected!\n");
                printf("Starting byte location: %u\n", i);
                printf("Virus name: %s\n", v->virusName);
                printf("Virus signature size: %hu\n", v->SigSize);
            }
            curr = curr->nextVirus;
        }
    }
}

void neutralize_virus(char *fileName, int signatureOffset) {
    FILE *file = fopen(fileName, "r+b");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    if (fseek(file, signatureOffset, SEEK_SET) != 0) {
        perror("Error seeking in file");
        fclose(file);
        return;
    }

    unsigned char ret_instruction = 0xC3; // RET instruction
    if (fwrite(&ret_instruction, sizeof(unsigned char), 1, file) != 1) {
        perror("Error writing to file");
    }

    fclose(file);
}

void fix_file(char *filename, link *virus_list) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fseek(file, 0, SEEK_END);
    unsigned int file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned int buffer_size = 10 * 1024; // 10K bytes
    unsigned int size_to_read = (file_size < buffer_size) ? file_size : buffer_size;

    char *buffer = (char *)malloc(buffer_size);
    if (buffer == NULL) {
        perror("Error allocating memory");
        exit(1);
    }

    if (fread(buffer, sizeof(char), size_to_read, file) != size_to_read) {
        perror("Error reading file");
        free(buffer);
        fclose(file);
        return;
    }
    fclose(file);

    for (unsigned int i = 0; i < size_to_read; i++) {
        link *curr = virus_list;
        while (curr != NULL) {
            virus *v = curr->vir;
            if (i + v->SigSize <= size_to_read && memcmp(buffer + i, v->sig, v->SigSize) == 0) {
                neutralize_virus(filename, i);
                printf("Virus neutralized at byte location: %u\n", i);
            }
            curr = curr->nextVirus;
        }
    }

    free(buffer);
}

int main() {
    link* virus_list = NULL;
    char choice[10];
    char filename[256];
    FILE* file = NULL;

    while (1) {
        printf("1) Load signatures\n");
        printf("2) Print signatures\n");
        printf("3) Detect viruses\n");
        printf("4) Fix file\n");
        printf("5) Quit\n");
        printf("Enter your choice: ");
        fgets(choice, sizeof(choice), stdin);
        sscanf(choice, "%s", choice);

    if (strcmp(choice, "1") == 0) {
        if (file != NULL) {
            fclose(file);
        }
        printf("Enter signature file name: ");
        fgets(filename, sizeof(filename), stdin);
        sscanf(filename, "%s", filename);
        file = fopen(filename, "rb");
        if (file == NULL) {
            perror("Error opening file");
            continue;
        }
        list_free(virus_list);

        char magic[4];
        if (fread(magic, sizeof(char), 4, file) != 4) {
            perror("Error reading magic number");
            exit(1);
        }
        while(!feof(file)) {
            virus* v = readVirus(file);
            if(v == NULL)
                break;
            virus_list = list_append(virus_list, v);
        }



        fclose(file);
        printf("Signatures loaded successfully.\n");

    }
    else if (strcmp(choice, "2") == 0) {
            if (virus_list == NULL) {
                printf("No signatures loaded.\n");
            } else {
                list_print(virus_list);
            }
        } else if (strcmp(choice, "3") == 0) {
            if (virus_list == NULL) {
            printf("No signatures loaded.\n");
        } else {
            printf("Enter file name to scan: ");
            fgets(filename, sizeof(filename), stdin);
            sscanf(filename, "%s", filename);
            file = fopen(filename, "rb");
            if (file == NULL) {
                perror("Error opening file");
                continue;
            }

            fseek(file, 0, SEEK_END);
            unsigned int file_size =ftell(file);
            fseek(file, 0, SEEK_SET);

            unsigned int buffer_size = 10 * 1024; // 10K bytes
            unsigned int size_to_read = (file_size < buffer_size) ? file_size : buffer_size;

            char *buffer = (char *)malloc(buffer_size);
            if (buffer == NULL) {
                perror("Error allocating memory");
                exit(1);
            }

            if (fread(buffer, sizeof(char), size_to_read, file) != size_to_read) {
                perror("Error reading file");
                free(buffer);
                fclose(file);
                continue;
            }
            fclose(file);

            detect_virus(buffer, size_to_read, virus_list);

            free(buffer);
        }
        } else if (strcmp(choice, "4") == 0) {
            if (virus_list == NULL) {
            printf("No signatures loaded.\n");
        } else {
            printf("Enter file name to fix: ");
            fgets(filename, sizeof(filename), stdin);
            sscanf(filename, "%s", filename);
            fix_file(filename, virus_list);
        }
        } else if (strcmp(choice, "5") == 0) {
            if (file != NULL) {
                fclose(file);
            }
            list_free(virus_list);
            printf("Quitting...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }
    

    return 0;
}
