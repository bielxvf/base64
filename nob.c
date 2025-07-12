#define NOB_IMPLEMENTATION

#include "nob.h"

#define BUILD_DIR "build/"
#define CORE_DIR  "src/core/"
#define UTIL_DIR  "src/util/"

#define PROJECT_NAME "base64"

#define STD_VERSION "-std=c++23"
#define WARNINGS    "-Wall", "-Wextra"

#define LINKER_DEPS "BitStream.o"

#define INCLUDE_DIRS "./lib/cxxopts/"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd,
        "c++", 
        STD_VERSION,
        WARNINGS, 
        "-o", BUILD_DIR "BitStream.o", 
        "-c",
        UTIL_DIR "BitStream.cpp");

    if (!nob_cmd_run_sync(cmd)) return 1;

    cmd.count = 0;
    nob_cmd_append(&cmd,
        "c++", 
        STD_VERSION,
        WARNINGS, 
        "-I" INCLUDE_DIRS,
        "-I" UTIL_DIR,
        "-o", BUILD_DIR PROJECT_NAME, 
        BUILD_DIR LINKER_DEPS,
        CORE_DIR "main.cpp");

    if (!nob_cmd_run_sync(cmd)) return 1;
}
