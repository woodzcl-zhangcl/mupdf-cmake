#include "File.h"

#include <iostream>

/*
 *
 * @param filePath
 * @param len
 * @return
 */
char *fileContent(const char *pathFile, int &len) {
#ifdef WIN32
    FILE *hFile = NULL;
    if (fopen_s(&hFile, pathFile, "rb") || NULL == hFile) {
        std::cout << "Cert Plain file is not exist!" << std::endl;
        return NULL;
    }
#else
    FILE *hFile = fopen(pathFile, "rb");
    if (!hFile) {
        std::cout << "Cert Plain file is not exist!" << std::endl;
        return NULL;
    }
#endif
    if (!hFile) {
        return NULL;
    }

    int ret = 0;
    ret = fseek(hFile, 0, SEEK_END);
    long lenFile = ftell(hFile);
    fseek(hFile, 0, SEEK_SET);

    char *fileContent = (char *) malloc(lenFile);
    long offset = 0;
    long readCount = 0;
    if (!fileContent) {
        fclose(hFile);
        std::cout << "memory was alloced error" << std::endl;
        return NULL;
    }
    else {
        readCount = fread(fileContent + offset, 1, lenFile, hFile);
        while (0 < readCount) {
            offset += readCount;
            readCount = fread(fileContent + offset, 1, lenFile - offset, hFile);
        }
        if (0 != lenFile - offset) {
            std::cout << "read file error" << std::endl;
            free(fileContent);
            fileContent = NULL;
        }
        else {
            len = offset;
        }
    }
    fclose(hFile);

    return fileContent;
}

void fileContentFree(char *&p) {
    if (p) {
        free(p);
        p = NULL;
    }
}

int fileContent(const char *pathFile, char *buf) {
#ifdef WIN32
    FILE *hFile = NULL;
    if (fopen_s(&hFile, pathFile, "rb") || NULL == hFile) {
        std::cout << "file is not exist!" << std::endl;
        return 0;
    }
#else
    FILE *hFile = fopen(pathFile, "rb");
    if (!hFile) {
        std::cout << "Cert Plain file is not exist!" << std::endl;
        return 0;
    }
#endif
    if (!hFile) {
        return 0;
    }

    int ret = 0;
    ret = fseek(hFile, 0, SEEK_END);
    long lenFile = ftell(hFile);
    fseek(hFile, 0, SEEK_SET);

    if (!buf) {
        return lenFile;
    }

    long offset = 0;
    long readCount = 0;

    readCount = fread(buf + offset, 1, lenFile, hFile);
    while (0 < readCount) {
        offset += readCount;
        readCount = fread(buf + offset, 1, lenFile - offset, hFile);
    }
    if (0 != lenFile - offset) {
        std::cout << "read file error" << std::endl;
        fclose(hFile);
        return 0;
    }

    fclose(hFile);

    return lenFile;
}

int fileContent(const char *buf, int len, const char *pathFile) {
#ifdef WIN32
    FILE *hFile = NULL;
    if (fopen_s(&hFile, pathFile, "wb") || NULL == hFile) {
        std::cout << "PrivateKey file is not available!" << std::endl;
        return 0;
    }
#else
    FILE *hFile = fopen(pathFile, "wb");
    if (!hFile) {
        std::cout << "PrivateKey file is not available!" << std::endl;
        return 0;
    }
#endif
    if (!hFile) {
        return 0;
    }

    long offset = 0;
    long writeCount = 0;
    while (offset < len) {
        writeCount = fwrite(buf + offset, 1, len - offset, hFile);
        offset += writeCount;
    }
    fclose(hFile);

    return 1;
}