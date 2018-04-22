#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define CLOCK_SIZE 4

// Struct for results
struct result {
  int frame;
  int page;
  int use;
  int modify;
};

// Global pointer
int pointer;

// function declaration
void writeClockToFile(FILE* filePtr, int page, char* operation, struct result* results);

// Provided by the Prompt
int main()  {

      // 1.   Read a record.
      char inFileName[ ] = "testdata.txt";
      FILE* inFilePtr = fopen(inFileName, "r");
      if(inFilePtr == NULL) {
            printf("File %s could not be opened.\n", inFileName);
            exit(1);
      }

      char outFileName[ ] = "results.txt";
      FILE* outFilePtr = fopen(outFileName, "w");
      if(outFilePtr == NULL) {
            printf("File %s could not be opened.\n", outFileName);
            exit(1);
      }

      char* page;
      char* operation;
      struct result results[CLOCK_SIZE];
      pointer = 0;

      for (int i = 0; i < CLOCK_SIZE; i++) {
        results[i].frame = i;
        results[i].page = -1;
        results[i].use = 0;
        results[i].modify = 0;
      }

      fscanf(inFilePtr, "%d%c", &page, &operation);
      while(!feof(inFilePtr)) {
            fprintf(outFilePtr, "Page referenced: %d %c\n", page, operation);




            int page_int = page;
            writeClockToFile(outFilePtr, page_int, operation, results);
            fscanf(inFilePtr, "%d%c", &page, &operation);
      }//end while

      printf("\nfile testdata.txt has been successfully processed. \n");
      printf("Check for results.txt\n\n");

      fclose(inFilePtr);
      fclose(outFilePtr);
      return 0;
};

// Algorithm for page replacement
void writeClockToFile(FILE* filePtr, int page, char* operation, struct result* results) {
      int i = 0;

      // Format for fprintf
      fprintf(filePtr, " FRAME        PAGE       USE        MODIFY\n");

      int p = pointer;
      int running = 1;
      // 2.   Check to see if the page in the record is already in the clock and if it is update to appropriate flag(s).  
      // (This will not reposition the next frame pointer)
      while (running) {
        for (int i = 0; i < CLOCK_SIZE; i++) {

            if (results[i].page == page) {
              results[i].modify = 1;
              running = 0;
              break;
            }
        }
        if (running == 0) break;

        // 3. If step 2 fails, beginning at the current position of the next frame pointer scan the clock.   
        // During this scan make no changes to the use bit.  The first frame encountered with (u = 0; m = 0) is selected for replacement.
        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (results[index].use == 0 && results[index].modify == 0) {
              results[index].page = page;
              results[index].use = 1;
              if (operation == 'w') results[index].modify = 1;
              else results[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            }
        }
        if (running == 0) break;

        // 4. If step 3 fails, scan the clock again.  The first frame encountered with (u = 0; m = 1) is selected for replacement.  
        // During this scan, set the use bit to 0 for each frame that is bypassed.
        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (results[index].use == 0 && results[index].modify == 1) {
              results[index].page = page;
              results[index].use = 1;
              if (operation == 'w') results[index].modify = 1;
              else results[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            } else {
              results[index].use = 0;
            }
        }
        if (running == false) break;

        // 5. If step 4 fails, the frame pointer will have once again returned to its original position and all the frames will have a use bit of 0.  
        // Repeat step 3 and, if necessary, step 4.  By time a frame will be found for replacement.
        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (results[index].use == 0 && results[index].modify == 0) {
              results[index].page = page;
              results[index].use = 1;
              if (operation == 'w') results[index].modify = 1;
              else results[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            }
        }
        if (running == 0) break;

        for (int i = 0; i < CLOCK_SIZE; i++) {
            int index = (p + i) % CLOCK_SIZE;
            if (results[index].use == 0 && results[index].modify == 1) {
              results[index].page = page;
              results[index].use = 1;
              if (operation == 'w') results[index].modify = 1;
              else results[index].modify = 0;
              pointer = (index + 1) % CLOCK_SIZE;
              running = 0;
              break;
            } else {
              results[index].use = 0;
            }
        }
        if (running == false) break;

        break;
      }

      // 6. Write the updated clock to file results.txt
      for (  ; i < CLOCK_SIZE; i++) {
        if (pointer == i) {
            fprintf(filePtr, " %d            %d          %d           %d  <- next frame\n", results[i].frame, results[i].page, results[i].use, results[i].modify);
        } else {
            fprintf(filePtr, " %d            %d          %d           %d\n", results[i].frame, results[i].page, results[i].use, results[i].modify);
        }
      }
      fprintf(filePtr, " %s\n","");
};
