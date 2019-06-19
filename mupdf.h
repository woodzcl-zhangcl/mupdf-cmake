#ifndef TEST_THIRDPARTY_H
#define TEST_THIRDPARTY_H

typedef void *CTX;
typedef void *HSTREAM;
typedef void *HDOC;

typedef struct {
    CTX ctx;
    void *file;
    HSTREAM hstream;
    HDOC hdoc;
}PDFHandle, *PPDFHandle;

#ifdef __cplusplus
extern "C" {
#endif

PPDFHandle open_pdf_mem_r(unsigned char *data, int len);

PPDFHandle open_pdf_file(const char *pathFile);

void close_pdf(PPDFHandle handle);

int count_pages_pdf(PPDFHandle handle);

int get_page_content_png(PPDFHandle handle, int page_index, unsigned char *bufPNG);

#ifdef __cplusplus
}
#endif

#endif //TEST_THIRDPARTY_H
