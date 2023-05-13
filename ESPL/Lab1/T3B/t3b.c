#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char my_get(char c){
    return (char)fgetc(stdin);
}
/* Ignores c, reads and returns a character from stdin using fgetc. */

char cprt(char c){
    if(c < 0x20 || c > 0x7E){
        printf(".\n");
    }
    else
    {
        printf("%c\n",c);
    }
    return c;
    
}
/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns the value of c unchanged. */

char encrypt(char c){
    if(c < 0x20 || c > 0x7E){
        return c;
    }
    else
    {
        c++;
        return c;
    }
    
}
/* Gets a char c and returns its encrypted form by adding 1 to its value. If c is not between 0x20 and 0x7E it is returned unchanged */

char decrypt(char c){
    if(c < 0x20 || c > 0x7E){
        return c;
    }
    else
    {
        c--;
        return c;
    }

}
/* Gets a char c and returns its decrypted form by reducing 1 from its value. If c is not between 0x20 and 0x7E it is returned unchanged */

char xprt(char c){
    printf("The hexadecimal value of %c is %x\n", c, c);
    return c;
    
}
char* map(char *array, int array_length, char (*f) (char)){
  char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
  for(int i = 0; i<array_length;i++){
      mapped_array[i] = f(array[i]);
    
  }
    
  /* TODO: Complete during task 2.a */
  return mapped_array;
}

struct fun_desc {
    char *name;
    char (*fun)(char);
};

int main(int argc, char **argv){
    struct fun_desc menu[] = { { "my_get", my_get }, { "cprt", cprt },{"encrypt",encrypt},{"decrypt",decrypt},{"xprt",xprt}, { NULL, NULL } };
    char carray[5] = "";
    while(1){
        printf("Please choose a function (ctrl^D for exit):\n");

        for(int i = 0;i<5.;i++){
            printf("%d) %s \n",i,menu[i].name);
        }

            
        
        int number;
        scanf("%d", &number);
        if(!number && number != 0){
            break;
        }
        if(number > 4 || number < 0){
            printf("Out of Bounds\n");
            break;
        }
        printf("Within Bounds\n");

        char (*fun_pointer)(char) = menu[number].fun;

        char* carray_b = map(carray,5,fun_pointer);
        strcpy(carray, carray_b);


        }

    }

