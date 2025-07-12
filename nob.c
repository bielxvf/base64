#define NOB_IMPLEMENTATION
#include "nob.h"

#define COMPILER "c++"

#define BUILD_DIR "build/"
#define CORE_DIR  "src/core/"
#define UTIL_DIR  "src/util/"

#define PROJECT_NAME "base64"

#define STD_VERSION "-std=c++23"
#define WARNINGS    "-Wall", "-Wextra"

#define LINKER_DEPS "build/BitStream.o", "build/base64.o"

#define LIBS         "-I./lib/cxxopts/"
#define INCLUDE_DIRS "include/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

    if (!strcmp(argv[1], "build")) {
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd,
            COMPILER, 
            STD_VERSION,
            WARNINGS, 
            "-I" INCLUDE_DIRS,
            "-o", BUILD_DIR "BitStream.o", 
            "-c",
            UTIL_DIR "BitStream.cpp");
        if (!nob_cmd_run_sync(cmd)) return 1;

        cmd.count = 0;
        nob_cmd_append(&cmd,
            COMPILER,
            STD_VERSION,
            WARNINGS,
            "-I" INCLUDE_DIRS,
            "-o", BUILD_DIR "base64.o",
            "-c",
            UTIL_DIR "base64.cpp");
        if (!nob_cmd_run_sync(cmd)) return 1;

        cmd.count = 0;
        nob_cmd_append(&cmd,
            COMPILER, 
            STD_VERSION,
            WARNINGS, 
            LIBS,
            "-I" INCLUDE_DIRS,
            "-o", BUILD_DIR PROJECT_NAME, 
            LINKER_DEPS,
            CORE_DIR "main.cpp");
        if (!nob_cmd_run_sync(cmd)) return 1;
    } else if (!strcmp(argv[1], "test")) {
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd,
                COMPILER,
                STD_VERSION,
                WARNINGS,
                "-I" INCLUDE_DIRS,
                LINKER_DEPS,
                "-o", BUILD_DIR "00_base64",
                "test/00_base64.cpp");
        if (!nob_cmd_run_sync(cmd)) return 1;

        nob_log(NOB_INFO, "Running test 00_base64");
        cmd.count = 0;
        nob_cmd_append(&cmd,
                "./" BUILD_DIR "00_base64");
        if (nob_cmd_run_sync(cmd)) {
            nob_log(NOB_INFO, "Test 00_base64 \x1b[32mPASSED\x1b[0m");
        } else {
            nob_log(NOB_INFO, "Test 00_base64 \x1b[31mFAILED\x1b[0m");
        }
    }

    return 0;
}
