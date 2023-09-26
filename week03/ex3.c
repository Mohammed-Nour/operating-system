#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#define MAX_PATH 2048
#define MAX_DATA 1024
#define MAX_LENGTH 63
#define MAX_SIZE 256

struct Directory;
struct File;

struct File {
  unsigned int id;
  char name[MAX_LENGTH];
  int size;
  char data[MAX_DATA];
  struct Directory * directory;
};

struct Directory {
  unsigned char nf;
  unsigned char nd;
  char name[MAX_LENGTH];
  char path[MAX_PATH];
  struct File * files[MAX_SIZE];
  struct Directory * directories[MAX_SIZE];
};

void add_file(struct File * file, struct Directory * dir) {
  if (file && dir) {
	  if(dir->nf<256) {
		file -> directory = dir;
		dir -> files[dir -> nf] = file;
		dir -> nf++;
	  }else {
		printf("You can't create file!");
	  }
  }

}
void overwrite_to_file(struct File * file,
  const char * str) {
  if (file != NULL && str != NULL) {
    if (strlen(str) < MAX_DATA) {
      strcpy(file -> data, str);
      file -> size = strlen(str);
    } else {
      printf("%s", "The size of data is too big!");
    }

  }

}
void append_to_file(struct File * file,
  const char * str) {
  if (file != NULL && str != NULL) {
    if (strlen(str) + file -> size < MAX_DATA) {
      file -> size += strlen(str);
      strcpy(file -> data, strcat(file -> data, str));
    } else {
      printf("%s", "The size of data is too big!");
    }
  }

}
void printp_file(struct File * file) {
  if (file != NULL) {
    printf("%s/%s\n", file -> directory -> path, file -> name);
  }

}
void add_dir(struct Directory * dir1, struct Directory * dir2) {
  if (dir1 && dir2) {
    dir2 -> directories[dir2 -> nd] = dir1;
    dir2 -> nd++;
    char temp_path[MAX_PATH];
    if (strcmp(dir2 -> path, "/")) {
      strcpy(temp_path, dir2 -> path);
      strcat(temp_path, "/");
      strcat(temp_path, dir1 -> name);
      strcpy(dir1 -> path, temp_path);
    } else {
      strcpy(temp_path, "/");
      strcat(temp_path, dir1 -> name);
      strcpy(dir1 -> path, temp_path);
    }
  }
}
void show_file(struct File * file) {
  if (file) {
    printf("%s ", file -> name);
  }
}
void show_dir(struct Directory * dir) {
  printf("\nDIRECTORY\n");
  printf(" path: %s\n", dir -> path);
  printf(" files:\n");
  printf(" [ ");
  for (int i = 0; i < dir -> nf; i++) {
    show_file(dir -> files[i]);
  }
  printf("]\n");
  printf(" directories:\n");
  printf(" { ");
  for (int i = 0; i < dir -> nd; i++) {
    show_dir(dir -> directories[i]);
  }
  printf("}\n");
}
int main() {
  struct Directory root;
  root.nd = 0;
  root.nf = 0;
  strcpy(root.name, "root");
  strcpy(root.path, "/");

  struct Directory home;
  home.nd = 0;
  home.nf = 0;
  strcpy(home.name, "home");
  strcpy(home.path, "/home");
  struct Directory bin;
  bin.nd = 0;
  bin.nf = 0;
  strcpy(bin.name, "bin");
  strcpy(bin.path, "/bin");

  struct File bash;
  bash.id = 0;
  bash.size = 0;
  strcpy(bash.name, "bash");

  struct File ex3_1;
  ex3_1.id = 1;
  ex3_1.size = 0;
  strcpy(ex3_1.name, "ex3_1.c");

  struct File ex3_2;
  ex3_2.id = 2;
  ex3_2.size = 0;
  strcpy(ex3_2.name, "ex3_2.c");

  add_dir( & home, & root);
  add_dir( & bin, & root);
  add_file( & bash, & bin);
  add_file( & ex3_1, & home);
  add_file( & ex3_2, & home);
  overwrite_to_file( & ex3_1, "int printf(const char * format, ...);\n");
  overwrite_to_file( & ex3_2, "//This is a comment in Clanguage\n");
  overwrite_to_file( & bash, "Bourne Again Shell!!\n");
  append_to_file( & ex3_1, "int main(){printf(\"Hello World!\")}\n");
for (unsigned char i = 0; i < bin.nf; i++) {
        printp_file(bin.files[i]);
    }

    for (unsigned char i = 0; i < home.nf; i++) {
        printp_file(home.files[i]);
    }
  show_dir( & root);
}
