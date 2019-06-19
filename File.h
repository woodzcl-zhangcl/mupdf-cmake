#ifndef LICENSETOOL_FILE_H
#define LICENSETOOL_FILE_H

char *fileContent(const char *pathFile, int &len);
void fileContentFree(char *&p);
int fileContent(const char *pathFile, char *buf);
int fileContent(const char *buf, int len, const char *pathFile);

#endif //LICENSETOOL_FILE_H
