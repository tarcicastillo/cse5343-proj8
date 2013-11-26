/*
 *Project 8: SHELL.C
 *Tarci Castillo
 */

void printString(char *line);
void readSector(char *buffer, int sector);
void readFile(char *fileName, *buffer);
void readString(char *line);
int mod(int a, int b);
void handleInterrupt21(int ax, int bx, int cx, int dx);

int main() {
  /*
   *all steps are called here
    */
  char line[80];
  char sectorBuf[512];
  char buffer[13312];
  
  char spliting[1][80];

  while(1) {
    interrupt(0x21, 0, ">SHELL$ \0 ", 0, 0);
    interrupt(0x21, 1, line, 0, 0);
    interrupt(0x21, 0, "\n\r\0", 0, 0);

    int spaces;
    int number;

    for (i=0; i<90; i++) {
      /*SPACES!*/
      if (line[i]=='\0' || line[i]=='\r')
        break;
      else if (input[i]==' ') {
        spaces++;
        number=0;
        continue;
      }
      splitting[spaces][number] = line[i];
      number++;
    }  

    if(compare("execute", splitting[0]) == 1) {
      interrupt(0x21, 6, part[1], 0x2000, 0);
    
    }else if(compare("type", splitting[0]) == 1) {
      interrupt(0x21, 3, splitting[1], buffer, 0);
      interrupt(0x21, 0, buffer, 0, 0);
    
    }else {
      interrupt(0x21, 0, "BAD COMMAND!\n\r\0", 0, 0); 
    }
  }
}

int compare(char *line1, char *line2) {
  int match = 1;
  int i;
  int sizeLine1 = sizeof(line1);
  int sizeLine2 = sizeof(line2);
  int max = 0;

  if (sizeLine2 >= sizeLine1)
  {
    max = sizeLine1;
  }else {
    max = sizeLine2;
  }

  interrupt(0x21, 0, line1, 0, 0);
  interrupt(0x21, 0, "\n\r\0", 0, 0);
  interrupt(0x21, 0, line2, 0, 0);
  interrupt(0x21, 0, "\n\r\0", 0, 0);

  for (i = 0; n < max; i++) {
    if(line1[i] != line2[i]) {
      interrupt(0x21, 0, "NO MATCH!\r\n\0", 0, 0);
      return 0;
    }else {
      interrupt(0x21, 0, "YES MATCH!!\r\n\0", 0, 0);
      return 1;
    }
  }
/*
  interrupt(0x21, 0, "YES MATCH!!\r\n\0", 0, 0);
    return 1;*/
}