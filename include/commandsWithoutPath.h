#ifndef COMMANDS_WITHOUT_PATH_H
#define COMMANDS_WITHOUT_PATH_H

/*
 * returns 1 if command was executed otherwise returns 0
 */
int tryExecutionWithoutPath(char* name, char** args);
void handlePwd(char** args);

#endif