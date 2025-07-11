#define NOB_IMPLEMENTATION

#include "nob.h"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"

#define PROJECT_NAME "base64"

#define STD_VERSION "-std=c++23"
#define WARNINGS    "-Wall", "-Wextra"

#define DEPS "BitStream.o"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd,
        "c++", 
        STD_VERSION,
        WARNINGS, 
        "-o", BUILD_FOLDER "BitStream.o", 
        "-c",
        SRC_FOLDER"BitStream.cpp");

    if (!nob_cmd_run_sync(cmd)) return 1;

    cmd.count = 0;
    nob_cmd_append(&cmd,
        "c++", 
        STD_VERSION,
        WARNINGS, 
        "-o", BUILD_FOLDER PROJECT_NAME, 
        BUILD_FOLDER DEPS,
        SRC_FOLDER"main.cpp");

    if (!nob_cmd_run_sync(cmd)) return 1;
}
