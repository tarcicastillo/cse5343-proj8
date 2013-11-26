/*
 *Project 8
 *Tarci Castillo
 *
 *Loading  and Executing Files
 *Write routines to read ﬁles into memory and 
 *execute programs.Then write a basic shell program that 
 *will execute other programs and print out ASCII text ﬁles.
 *0x10 = video
 *0x13 = disk
 *0x16 = keyboard
 *0x21 = own
 *0xE = print character
 *0xD = enter
 *0x8 = backspace
 */
void printString(char *line);
void readString(char *line);
void readSector(char *buffer, int sector);
int mod(int a, int b);
void readFile(char *fileName, *buffer);
void executeProgram(char *name, int segment);
void terminate();
void handleInterrupt21(int ax, int bx, int cx, int dx);

int main() {
	/*
	 *all steps are called here
    */
    char line[80];
    char sectorBuf[13312];

	makeInterrupt21();

    /*STEP 1: Load File and Print It */
    /*printString("LOAD FILE AND PRINT FILE\n\r\0");
    interrupt(0x21, 3, "messag\0", buffer, 0); /*read file into buffer*/
    /*interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/

    /*STEP 2: Load Program & Execute it*/
   /* printString("\nLOAD PROGRAM THEN EXECUTE: \0");
    interrupt(0x21, 4, "tstprg\0", 0x2000, 0);

    /*STEP 3: Terminate a Program System Call */
   /* printString("\nTERMINATE PROGRAM SYSTEM CALL\n\r\0");
    interrupt(0X21, )

    /*Step 4: Creating my Own Interrupt */
    interrupt(0x21, 0x06, "shell\0", 0x2000; 0);

	while(1) {
		/*loops infinitly*/
	};
}
/*Step 1: Printing to the Screen - Interrupt 0x10 */
void printString(char *line) {
    int index = 0;
    char c = line[index];

    while(c != 0) {
        interrupt(0x10, 0xE * 256 + c, 0, 0, 0);
        c = line[++index];/*there is a difference between index++ & ++index*/
    }
}

/*Step 2: Reading from the Keyboard - Interrupt 0
 *0x10 = video
 *0x13 = disk
 *0x16 = keyboard
 *0x21 = own
 *0xE = print character
 *0xD = enter
 *0x8 = backspace*/
void readString(char *line) {
    int index = 0;
    char c = interrupt(0x16, 0, 0, 0, 0);

    while((c != 0xD) && (index < 80)) {
        interrupt(0x10, 0xE * 256 + c, 0, 0, 0);

        /*0x8 = backspace*/
        if(c != 0x8) {
            line[index] = c;
            index++;
        }else if(index > 0) {
            index--;
        }

        c = interrupt(0x16, 0, 0, 0, 0);
    }

    line[index] = 0;
}

/*Step 3: Reading a Sector from Disk - Interrupt 0x13 */
void readSector(char *buffer, int sector) {
    int relativeSec = mod(sector, 18) +1;
    int head = mod(sector/18, 2);
    int track = sector/36;

    interrupt(0x13, 2 *256 +1, buffer, track *256 + relativeSec, head *256);
}
/*Directly from the Proj 7 handout*/
int mod(int a, int b) {
    while(a >= b) {
        a = a - b;
    }
    return a;
}

/*STEP 1: Load File and Print It*/
void readFile(char *fileName, char *buffer) {
	
	char bufferSector[512];
	
	readSector(bufferSector, 0x02);

	int i;
	int j;
	for(i=0; i<512; i+=32) {
		for(j=0; j<6; j++) {
			if(bufferSector[i+j] != fileName[j])
				break;
		}
		if(j == 0x06)
			break;
	}
	if(j != 0x06)
		return;
	for(j = 0x06; j<32; j++) {
		if(bufferSector[i+j] == 0)
			break;
		readSector(buffer + (j-6) * 512, bufferSector[i+j]);
	}
}

/*STEP 2: Load Program & Execute it
 *parameters: name of program to run
 *			  the segment where I want to run
 *issues! - kinda??? fix?
 */
void executeProgram(char *name, int segment) {
	char buf[1000];
	int i;
	int w = mod(segment, 1000);
	/*interrupt(0x21, 0x3, name, buf, 0);*/

	if(w != 0 || segment <= 1000 || segment > 13312)
	 return;

    readFile(name, buffer);
	
	for(i=0; i<13312; i++){
		putInMemory(segment, i, buf[i]);
	}
	launchProgram(segment);
}

/*STEP 3: Terminate the Program System Call*/
/*issues??!! - kinda*/
void terminate() {
	interrupt(0x21, 4, "shell\0", 0x2000, 0);
}

/*Step 4: Create my own Interrupt */
/*get garbage values but confused as to why its that exactly (its contents on bx)*/
void handleInterrupt21(int ax, int bx, int cx, int dx) {
    if(ax == 0) {
        printString(bx);
        printString("\n\r\0");

    }else if(ax == 1) {
        readString(bx);
        printString("\n\r\0");

    }else if(ax == 2) {
        readSector(bx, cx);
        
    }else {
        printString("INVALID INTERRUPT 21 COMMAND...\n\r\0");
    }
}