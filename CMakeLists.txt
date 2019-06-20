cmake_minimum_required(VERSION 3.13)
project(mupdf)

set(CMAKE_CXX_STANDARD 14)

# with -fPIC
IF(UNIX AND NOT WIN32)
    FIND_PROGRAM(CMAKE_UNAME uname /bin /usr/bin /usr/local/bin )
    IF(CMAKE_UNAME)
        EXEC_PROGRAM(uname ARGS -m OUTPUT_VARIABLE CMAKE_SYSTEM_PROCESSOR)
        SET(CMAKE_SYSTEM_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR} CACHE INTERNAL "processor type (i386 and x86_64)")
        IF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
            ADD_DEFINITIONS(-fPIC)
        ENDIF(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64")
    ENDIF(CMAKE_UNAME)
ENDIF(UNIX AND NOT WIN32)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/generated)

set(MuJS_SRC
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsarray.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsboolean.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsbuiltin.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jscompile.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsdate.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsdtoa.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsdump.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jserror.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsfunction.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsgc.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsintern.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jslex.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsmath.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsnumber.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsobject.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/json.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsparse.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsproperty.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsregexp.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsrun.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsstate.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsstring.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/jsvalue.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/regex.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/utf.c
        ${CMAKE_SOURCE_DIR}/thirdparty/mujs/utftype.c
        )
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/mujs/)
add_library(mujs ${MuJS_SRC})

set(FreeType_SRC
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftbase.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftbbox.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftbitmap.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftdebug.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftgasp.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftglyph.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftinit.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftstroke.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftsynth.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftsystem.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/fttype1.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/base/ftxf86.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/cff/cff.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/psaux/psaux.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/pshinter/pshinter.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/psnames/psnames.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/raster/raster.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/sfnt/sfnt.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/smooth/smooth.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/truetype/truetype.c
        ${CMAKE_SOURCE_DIR}/thirdparty/freetype/src/type1/type1.c
        )
include_directories(${CMAKE_SOURCE_DIR}/scripts/freetype ${CMAKE_SOURCE_DIR}/thirdparty/freetype/include)
add_definitions(
        -DFT2_BUILD_LIBRARY -DDARWIN_NO_CARBON
#        -DFT_CONFIG_MODULES_H="slimftmodules.h"
#        -DFT_CONFIG_OPTIONS_H="slimftoptions.h"
)
add_library(freetype ${FreeType_SRC})

set(JBig2Dec_SRC
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_arith.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_arith_iaid.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_arith_int.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_generic.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_halftone.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_huffman.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_image.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_metadata.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_mmr.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_page.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_refinement.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_segment.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_symbol_dict.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec/jbig2_text.c
        )
include_directories(${CMAKE_SOURCE_DIR}/thirdparty/jbig2dec)
add_definitions(-DHAVE_STDINT_H)
add_library(jbig2dec ${JBig2Dec_SRC})

set(Jpeg_SRC
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jaricom.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jcomapi.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdapimin.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdapistd.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdarith.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdatadst.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdatasrc.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdcoefct.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdcolor.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jddctmgr.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdhuff.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdinput.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdmainct.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdmarker.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdmaster.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdmerge.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdpostct.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdsample.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jdtrans.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jerror.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jfdctflt.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jfdctfst.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jfdctint.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jidctflt.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jidctfst.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jidctint.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jmemmgr.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jmemansi.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jquant1.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jquant2.c
        ${CMAKE_SOURCE_DIR}/thirdparty/jpeg/jutils.c
        )
include_directories(${CMAKE_SOURCE_DIR}/scripts/jpeg ${CMAKE_SOURCE_DIR}/thirdparty/jpeg)
add_definitions(-DSHARE_JPEG)
add_library(jpeg ${Jpeg_SRC})

set(OpenJpeg_SRC
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/bio.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/cidx_manager.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/cio.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/dwt.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/event.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/function_list.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/image.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/invert.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/j2k.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/jp2.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/mct.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/mqc.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/openjpeg.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/opj_clock.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/phix_manager.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/pi.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/ppix_manager.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/raw.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/t1.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/t2.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/tcd.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/tgt.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/thix_manager.c
        ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg/tpix_manager.c
        )
include_directories(${CMAKE_SOURCE_DIR}/scripts/openjpeg ${CMAKE_SOURCE_DIR}/thirdparty/openjpeg/libopenjpeg)
add_definitions(-DOPJ_STATIC -DOPJ_HAVE_STDINT_H)
add_library(openjpeg ${OpenJpeg_SRC})

set(ZLib_SRC
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/adler32.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/compress.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/crc32.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/deflate.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/inffast.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/inflate.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/inftrees.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/trees.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/uncompr.c
        ${CMAKE_SOURCE_DIR}/thirdparty/zlib/zutil.c
        )
add_definitions(-Dverbose=-1)
add_library(zlib ${ZLib_SRC})

file(GLOB MUPDF_CORE_SRC
        ${CMAKE_SOURCE_DIR}/source/cbz/*.c
        ${CMAKE_SOURCE_DIR}/source/fitz/*.c
        ${CMAKE_SOURCE_DIR}/source/img/*.c
        ${CMAKE_SOURCE_DIR}/source/pdf/*.c
        ${CMAKE_SOURCE_DIR}/source/tiff/*.c
        ${CMAKE_SOURCE_DIR}/source/xps/*.c
        )
add_library(mupdf_core ${MUPDF_CORE_SRC})
target_link_libraries(mupdf_core mujs freetype jbig2dec jpeg openjpeg zlib)

if (ANDROID)
    set(CMAKE_SHARED_LINKER_FLAGS -Wl,--hash-style=sysv)
endif ()

add_library(mupdf SHARED mupdf.c File.cpp util-memopen.c)
IF (ANDROID)
    target_link_libraries(mupdf mupdf_core log)
ELSEIF (IOS)
    target_link_libraries(mupdf mupdf_core)
ELSE ()
    IF (UNIX AND NOT WIN32)
        target_link_libraries(mupdf mupdf_core rt)
    ELSEIF (WIN32)
        target_link_libraries(mupdf mupdf_core)
    ENDIF ()
ENDIF ()

add_executable(pdf main.cpp)
target_link_libraries(pdf mupdf)