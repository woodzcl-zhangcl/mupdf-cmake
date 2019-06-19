#include <stdio.h>
#include <iostream>
#include <string.h>

#include "File.h"
#include "mupdf.h"

int main() {
    printf("Hello MuPDF\n");

    int lenFile = fileContent("fractal-whitepaper.pdf", NULL);
    if (0 >= lenFile) {
        std::cout << "File is not exist!" << std::endl;
        return 1;
    }

    unsigned char *bufFile = (unsigned char *) malloc(lenFile);
    if (!bufFile) {
        std::cout << "Mem is not correct!" << std::endl;
        return 1;
    }
    memset(bufFile, 0, lenFile);

    lenFile = fileContent("fractal-whitepaper.pdf", (char *) bufFile);

    PPDFHandle handle = open_pdf_mem_r(bufFile, lenFile);

    int count_page = count_pages_pdf(handle);

    std::cout << "pages = " << count_page << std::endl;

    int lenPNG = get_page_content_png(handle, 0, NULL);
    if (0 < lenPNG) {
        unsigned char *bufPNG = (unsigned char *) malloc(lenPNG);

        if (bufPNG) {
            lenPNG = get_page_content_png(handle, 0, bufPNG);
            int ret = fileContent((const char *) bufPNG, lenPNG, "out_first_img.png");
            if (1 == ret) {
                std::cout << "first one image from pdf file is completed output, named out_first_img.png" << std::endl;
            }
            free(bufPNG);
        }
    }

    close_pdf(handle);

    free(bufFile);

    return 0;
}