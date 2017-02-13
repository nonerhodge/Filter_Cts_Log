#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char * getFileName(char *buffer, char *path, char *name) {
  memcpy(name, path, strlen(path));
  char *p1=strstr(buffer, "(");
  char *p2=strstr(buffer, ")");
  char *p3=strstr(buffer, "started:");
  memcpy(name+strlen(path), p1+1, p2-p1-1);
  memcpy(name+strlen(path)+(p2-p1)-1, "#", 1);
  memcpy(name+strlen(path)+(p2-p1), p3+9, (p1-p3)-9);
  //printf("The output file is %s\n", name);
  return name;
}

int main(int argc, char **argv){
  if (argc != 3) {
    printf("#################################################\n");
    printf("#### Usage: ./cts_log_filter xxxx.log ./xxxx ####\n");
    printf("#################################################\n");
    exit(1);
  }
  char *inFile = argv[1];
  char *outPath = argv[2];
  printf("\n");
  printf(">>>>>>>>>>>>>>>>>>>>>>>> Filter the file %s to %s starting!!!\n", inFile, outPath);
  FILE *inFp = fopen(inFile,"r");
  if(!inFp) {
    printf("The cts log can't be opened!\n");
    exit(1);
  }
  char buffer[1024] = {0};
  char *t = NULL;
  bool hasGetCase = false;
  int failedcases = 0;

  int startPoint = 0;
  int stopPoint = 0;
  char outFile[256] = {0};
  FILE *outFp;
  while(!feof(inFp)) {
    fgets(buffer, sizeof(buffer), inFp);
    if (!strstr(buffer, "TestRunner: started:")
        && !hasGetCase) {
      continue;
    }
    if (!hasGetCase) {
      startPoint = ftell(inFp)-strlen(buffer);
      memset(outFile, 0, 256);
      getFileName(buffer, outPath, outFile);
      hasGetCase = true;
      continue;
    }
    if (strstr(buffer, "TestRunner: finished:")) {
      startPoint = 0;
      hasGetCase = false;
      continue;
    }
    if (strstr(buffer, "end exception")) {
      stopPoint = ftell(inFp);
      if (startPoint == 0) {
        printf("error state! can't find start point!\n");
      }
      printf("The failed case is %s\n", outFile);
      failedcases++;
      outFp = fopen(outFile, "w");
      fseek(inFp, startPoint, SEEK_SET);
      while(ftell(inFp)<stopPoint) {
        fgets(buffer, sizeof(buffer), inFp);
        if (strlen(buffer) > 0) {
          fwrite(buffer, strlen(buffer), 1, outFp);
        }
      }
      fflush(outFp);
      fclose(outFp);
    }
  }
  fclose(inFp);
  printf("<<<<<<<<<<<<<<<<<<<<<<<< Filter done, there are %d cases failed!!!\n", failedcases);
  printf("\n");
  exit(0);
}
