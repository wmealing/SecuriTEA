#include <stdio.h>
#include <unistd.h>


int main () {

  char *ar[] = {"whoami", NULL};
  int real = getuid();
  int euid = geteuid();
  
  printf("The REAL UID =: %d\n", real);
  printf("The EFFECTIVE UID =: %d\n", euid);

  printf("Press any key to continue..");
  getchar();
  
  execvp("whoami", ar);
  return (0);
}

