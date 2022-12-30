#ifndef HELPERS_H
#define HELPERS_H

char* allocateString(char* str);
int getArgCount(char** str);
int countCharOccurencesInStr(char c, char* str);
void waitForChild();
int safeDup(int fd);
void safeDup2(int oldfd, int newfd);

#endif