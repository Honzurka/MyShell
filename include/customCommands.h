#ifndef CUSTOM_COMMANDS_H
#define CUSTOM_COMMANDS_H

/*
* returns `id` of the custom command or `-1`
*/
int getCustomCommandID(char* command);
void handleCustomCommand(int commandID, char** args);
void handleExit(char** args);
void handleCd(char** args);

#endif
