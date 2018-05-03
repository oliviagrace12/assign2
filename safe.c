#include "safeBustersHeaders.h"

int shouldStillRun = 1;
int safeIsOpened = 0;
int comboOne;
int comboTwo;
int comboThree;

void simpleHandle(int signalNum) {
  shouldStillRun = 0;
}

void advancedHandle(int signalNum, siginfo_t* infoPtr, void* dataPtr) {
  int returnSig;
  if (comboOne > 0) {
    returnSig = computeResponse(&comboOne);
  } else if (comboTwo > 0) {
    returnSig = computeResponse(&comboTwo);
  } else if (comboThree > 0) {
    returnSig = computeResponse(&comboThree);
    if (returnSig == SIG_RIGHT_DIGIT) {
      safeIsOpened = 1;
    }
  }
  kill(infoPtr->si_pid, returnSig);
}

int computeResponse(int* countPtr) {
  int toReturn;

  sleep(1);
  (*countPtr)--;

  if (*countPtr == 0) {
    printf("Safe \"Click!\"\n");
    toReturn = SIG_RIGHT_DIGIT;
  } else {
    printf("Safe \"Nope.\"\n");
    toReturn = SIG_WRONG_DIGIT;
  }

  return(toReturn);
}

int main(int argc, char* argv[]) {
  srand(getpid());

  struct sigaction simple;
  memset(&simple, '\0', sizeof(simple));
  simple.sa_handler = simpleHandle;
  sigaction(SIG_QUIT, &simple, NULL);

  struct sigaction advanced;
  memset(&advanced, '\0', sizeof(advanced));
  advanced.sa_sigaction = advancedHandle;
  sigaction(SIG_TRY_NEXT_DIGIT, &advanced, NULL);
  
  comboOne = (rand() % 16) + 1;
  comboTwo = (rand() % 16) + 1;
  comboThree = (rand() % 16) + 1;

  printf("Safe \"Don't tell, but my combination is %d-%d-%d\"\n", comboOne, comboTwo, comboThree);
  
  while (shouldStillRun) {
    sleep(1);
  }

  if (safeIsOpened) {
    printf("Safe \"You got me!\"\n");
  } else {
    printf("Safe \"You couldn't crack me!\"\n");
  }

  return(EXIT_SUCCESS); 
}







