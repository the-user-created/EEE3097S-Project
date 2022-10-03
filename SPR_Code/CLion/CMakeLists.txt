cmake_minimum_required(VERSION 3.19)
project(test C)

set(CMAKE_C_STANDARD 11)

add_executable(test
        main.c
        heatshrink/heatshrink_common.h
        heatshrink/heatshrink_config.h
        heatshrink/heatshrink_encoder.c
        heatshrink/heatshrink_encoder.h
        heatshrink/heatshrink_decoder.c
        heatshrink/heatshrink_decoder.h
        blowfish/constants.h
        blowfish/blowfish.c
        blowfish/blowfish.h
        crc32/crc32.c
        crc32/crc32.h)
