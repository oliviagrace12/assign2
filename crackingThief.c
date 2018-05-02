/*-------------------------------------------------------------------------*
 *---									---*
 *---		crackingThief.c						---*
 *---									---*
 *---	    This file defines the crackingThief program.		---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1a		2018 April 20		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	"safeBustersHeaders.h"

#define		NAME		"Cracking Thief"
int		shouldStillRun	= 1;
int		numDigitsFound	= 0;
int		count0		= 0;
int		count1		= 0;
int		count2		= 0;

pid_t		safePid;

void		sigIntHandler	(int	sigNum
				)
{
  printf("%s \"Damn!  And I was close, too!\"\n",NAME);
  shouldStillRun	= 0;
}


void		tryNextCombination
				()
{
  switch  (numDigitsFound)
  {
  case 0 :
    count0++;
    break;
  case 1 :
    count1++;
    break;
  case 2 :
    count2++;
    break;
  }

  printf("%s \"Okay, let's try %d-%d-%d.\"\n",NAME,count0,count1,count2);
  //  YOUR CODE HERE to send SIG_TRY_NEXT_DIGIT to safePid
  kill(safePid, SIG_TRY_NEXT_DIGIT);
  //  done
}


void		sigWrongDigitHandler
				(int	sigNum
				)
{
  tryNextCombination();
}


void		sigRightDigitHandler
				(int	sigNum
				)
{
  numDigitsFound++;

  if  (numDigitsFound < 3)
  {
    printf("%s \"Only %d more to go\"\n",NAME,3-numDigitsFound);
    tryNextCombination();
  }
  else
  {
    shouldStillRun	= 0;
    printf("%s \"I got the goods!  Let's bug out!\"\n",NAME);
  }

}


int		main		(int		argc,
				 char*		argv[]
				)
{
  //  YOUR CODE HERE to check if have a command line argument
  if (argc <= 0) {
    exit(EXIT_FAILURE);
  }
  //  done

  //  YOUR CODE HERE to set safePid to the integer in the string in the command line argument
  safePid = strtol(*argv, NULL, 10);
  //  done  
  
  struct sigaction	act;

  //  YOUR CODE HERE to install sigIntHandler for SIG_QUIT
  memset(&act,'\0',sizeof(act));
  act.sa_handler = sigIntHandler;
  sigaction(SIG_QUIT,&act,NULL);
  //  done

  //  YOUR CODE HERE to install sigWrongDigitHandler for SIG_WRONG_DIGIT
  memset(&act,'\0',sizeof(act));
  act.sa_handler = sigWrongDigitHandler;
  sigaction(SIG_WRONG_DIGIT,&act,NULL);  
  //  done

  //  YOUR CODE HERE to install sigRightDigitHandler for SIG_RIGHT_DIGIT
  memset(&act,'\0',sizeof(act));
  act.sa_handler = sigRightDigitHandler;
  sigaction(SIG_RIGHT_DIGIT,&act,NULL);
  //  done

  tryNextCombination();

  while  (shouldStillRun)
  {
    sleep(5);
  }

  return(EXIT_SUCCESS);
}
