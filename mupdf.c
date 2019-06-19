#include "mupdf.h"

#include <string.h>
#include <stdio.h>

#include "mupdf/fitz.h"
#include "mupdf/pdf.h"

#include "util-memopen.h"

static int mark() {
    const char *input = (char *) "fractal-whitepaper.pdf";
    float zoom, rotate;
    int page_number, page_count;
    fz_context *ctx;
    fz_document *doc;
    fz_pixmap *pix;
    fz_page *page = NULL;
    fz_matrix ctm;//第一页为0
    page_number = 0;
    //100%缩放比
    zoom = 100;
    //旋转为0
    rotate = 0;

    //创建上下文
    ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!ctx) {
        //"cannot create mupdf context"
        return 1;
    }

    //注册文档控制
    fz_try(ctx)fz_register_document_handlers(ctx);
    fz_catch(ctx) {
        //"cannot register document handlers"
        fz_free_context(ctx);
        return 1;
    }

    //打开文档
    fz_try(ctx)doc = fz_open_document(ctx, input);
    fz_catch(ctx) {
        //"cannot open document";
        fz_free_context(ctx);
        return 1;
    }

    //取得总的页数
    fz_try(ctx)page_count = fz_count_pages(doc);
    fz_catch(ctx) {
        //"cannot count number of pages:";
        fz_close_document(doc);
        fz_free_context(ctx);
        return 1;
    }

    if (page_number < 0 || page_number >= page_count) {
        //"page number out of range;
        fz_close_document(doc);
        fz_free_context(ctx);
        return 1;
    }

    page = fz_load_page(doc, page_number);

    //计算缩放以及旋转
    fz_scale(&ctm, zoom / 100, zoom / 100);
    fz_pre_rotate(&ctm, rotate);

    // Take the page bounds and transform them by the same matrix that
    // we will use to render the page.
    fz_rect bounds;
    fz_bound_page(doc, page, &bounds);
    fz_transform_rect(&bounds, &ctm);

    // Create a blank pixmap to hold the result of rendering. The
    // pixmap bounds used here are the same as the transformed page
    // bounds, so it will contain the entire page. The page coordinate
    // space has the origin at the top left corner and the x axis
    // extends to the right and the y axis extends down.
    fz_irect bbox;
    fz_round_rect(&bbox, &bounds);

    pix = fz_new_pixmap_with_bbox(ctx, fz_device_rgb(ctx), &bbox);
    fz_clear_pixmap_with_value(ctx, pix, 0xff);

    // Create a draw device with the pixmap as its target.
    // Run the page with the transform.
    fz_device *dev = fz_new_draw_device(ctx, pix);
    fz_run_page(doc, page, dev, &ctm, NULL);
    fz_free_device(dev);

    //渲染成图片
    unsigned char *samples = pix->samples;
    int width = fz_pixmap_width(ctx, pix);
    int height = fz_pixmap_height(ctx, pix);

    fz_write_png(ctx, pix, "out.png", 0);

    fz_buffer *buffer = NULL;
    buffer = fz_new_png_from_pixmap(ctx, pix);

    FILE *pgm_fd = fopen("out_png.png","wb");
    fwrite(buffer->data, buffer->len, 1, pgm_fd);
    fclose(pgm_fd);

    fz_drop_buffer(ctx, buffer);

    fz_drop_pixmap(ctx, pix);
    fz_free_page(doc, page);
    fz_close_document(doc);
    fz_free_context(ctx);

    return 1;
}


#ifdef __cplusplus
extern "C" {
#endif

PPDFHandle open_pdf_mem_r(unsigned char *data, int len) {
    if (!data || 0 >= len) {
        return NULL;
    }

    size_t size = sizeof(PDFHandle);
    PPDFHandle handle = (PPDFHandle) malloc(size);
    memset(handle, 0, size);

    if (!handle) {
        return NULL;
    }

    handle->ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!handle->ctx) {
        free(handle);
        return NULL;
    }

    fz_context *fz_ctx = (fz_context *)handle->ctx;

    fz_try(fz_ctx)
        fz_register_document_handlers((fz_context *) handle->ctx);
    fz_catch(fz_ctx) {
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
        return NULL;
    }

#ifdef USE_FMEM_WRAPPER
    handle->file = SCFmemopen(data, len, "rb");
#else
    int fd = SCFmemopen(data, len, "rb");
#endif

    fz_try(fz_ctx)
#ifdef USE_FMEM_WRAPPER
        handle->hstream = fz_open_fd(fz_ctx, fileno((FILE *) handle->file));
#else
        handle->hstream = fz_open_fd(fz_ctx, fd);
#endif
    fz_catch(fz_ctx) {
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
        return NULL;
    }

    fz_try(fz_ctx)handle->hdoc = fz_open_document_with_stream(fz_ctx, "application/pdf", (fz_stream *) handle->hstream);
    fz_catch(fz_ctx) {
        fz_close((fz_stream *) handle->hstream);
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
        return NULL;
    }

    return handle;
}

PPDFHandle open_pdf_file(const char *pathFile) {
    if (!pathFile) {
        return NULL;
    }

    size_t size = sizeof(PDFHandle);
    PPDFHandle handle = (PPDFHandle) malloc(size);
    memset(handle, 0, size);

    if (!handle) {
        return NULL;
    }

    handle->ctx = fz_new_context(NULL, NULL, FZ_STORE_UNLIMITED);
    if (!handle->ctx) {
        free(handle);
        return NULL;
    }

    fz_context *fz_ctx = (fz_context *)handle->ctx;

    fz_try(fz_ctx)
        fz_register_document_handlers((fz_context *) handle->ctx);
    fz_catch(fz_ctx) {
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
        return NULL;
    }

    fz_try(fz_ctx)
        handle->hstream = fz_open_file(fz_ctx, pathFile);
    fz_catch(fz_ctx) {
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
        return NULL;
    }

    fz_try(fz_ctx)
        handle->hdoc = fz_open_document_with_stream(fz_ctx, "application/pdf", (fz_stream *) handle->hstream);
    fz_catch(fz_ctx) {
        fz_close((fz_stream *) handle->hstream);
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
        return NULL;
    }

    return handle;
}

void close_pdf(PPDFHandle handle) {

    if (handle) {
        fz_close_document((fz_document *) handle->hdoc);
        if (handle->file) {
            fclose((FILE *) handle->file);
        }
        fz_close((fz_stream *) handle->hstream);
        fz_free_context((fz_context *) handle->ctx);
        free(handle);
    }
}

int count_pages_pdf(PPDFHandle handle) {
    int cp = 0;

    if (handle) {
        fz_context *fz_ctx = (fz_context *) handle->ctx;

        fz_try(fz_ctx)
                cp = fz_count_pages((fz_document *) handle->hdoc);
        fz_catch(fz_ctx) {
            return cp;
        }
    }

    return cp;
}

int get_page_content_png(PPDFHandle handle, int page_index, unsigned char *bufPNG) {
    float zoom, rotate;
    fz_pixmap *pix;
    fz_page *page = NULL;
    fz_matrix ctm;//第一页为0
    //100%缩放比
    zoom = 100;
    //旋转为0
    rotate = 0;

    int lenPNG = 0;

    int page_count = 0;

    if (handle) {
        fz_context *fz_ctx = (fz_context *) handle->ctx;

        fz_try(fz_ctx)
                page_count = fz_count_pages((fz_document *) handle->hdoc);
        fz_catch(fz_ctx) {
            return 0;
        }

        if (page_index < 0 || page_index >= page_count) {
            printf("page number out of range\n");
            return 0;
        }

        page = fz_load_page((fz_document *) handle->hdoc, page_index);

        //计算缩放以及旋转
        fz_scale(&ctm, zoom / 100, zoom / 100);
        fz_pre_rotate(&ctm, rotate);

        // Take the page bounds and transform them by the same matrix that
        // we will use to render the page.
        fz_rect bounds;
        fz_bound_page((fz_document *) handle->hdoc, page, &bounds);
        fz_transform_rect(&bounds, &ctm);

        // Create a blank pixmap to hold the result of rendering. The
        // pixmap bounds used here are the same as the transformed page
        // bounds, so it will contain the entire page. The page coordinate
        // space has the origin at the top left corner and the x axis
        // extends to the right and the y axis extends down.
        fz_irect bbox;
        fz_round_rect(&bbox, &bounds);

        pix = fz_new_pixmap_with_bbox((fz_context *) handle->ctx, fz_device_rgb((fz_context *) handle->ctx), &bbox);
        fz_clear_pixmap_with_value((fz_context *) handle->ctx, pix, 0xff);

        // Create a draw device with the pixmap as its target.
        // Run the page with the transform.
        fz_device *dev = fz_new_draw_device((fz_context *) handle->ctx, pix);
        fz_run_page((fz_document *) handle->hdoc, page, dev, &ctm, NULL);
        fz_free_device(dev);

        fz_buffer *buffer = NULL;

        buffer = fz_new_png_from_pixmap((fz_context *) handle->ctx, pix);

        //fwrite(buffer->data, buffer->len, 1, pgm_fd);
        lenPNG = buffer->len;
        if (bufPNG) {
            memcpy(bufPNG, buffer->data, buffer->len);
        }

        fz_drop_buffer((fz_context *) handle->ctx, buffer);

        fz_drop_pixmap((fz_context *) handle->ctx, pix);
        fz_free_page((fz_document *) handle->hdoc, page);
    }

    return lenPNG;
}

#ifdef __cplusplus
}
#endif
