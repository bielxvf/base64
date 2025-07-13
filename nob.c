#define NOB_IMPLEMENTATION
#include "nob.h"

#include <libgen.h>
#include <dirent.h>

#define COMPILER "c++"

#define BUILD_DIR "build/"
#define CORE_DIR  "src/core/"
#define UTIL_DIR  "src/util/"

#define PROJECT_NAME "base64"

#define STD_VERSION "-std=c++23"
#define WARNINGS    "-Wall", "-Wextra"

#define LINKER_DEPS "build/BitStream.o", "build/base64.o"

#define INCLUDE_DIRS "include/"

void strip_extension(const char *path, char *out, size_t out_size)
{
    strncpy(out, path, out_size - 1);
    out[out_size - 1] = '\0';

    char *dot = strrchr(out, '.');
    if (dot) {
        *dot = '\0';
    }
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_DIR)) return 1;

    if (!strcmp(argv[1], "build")) {
        if (!nob_mkdir_if_not_exists(BUILD_DIR "lib")) return 1;
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd,
                "curl",
                "-L",
                "-s",
                "-o", BUILD_DIR "lib/cxxopts.tar.gz",
                "https://github.com/jarro2783/cxxopts/archive/refs/tags/v3.3.1.tar.gz");
        if (!nob_cmd_run_sync(cmd)) return 1;

        cmd.count = 0;
        nob_cmd_append(&cmd,
                "tar",
                "-x",
                "-z",
                "-f", BUILD_DIR "lib/cxxopts.tar.gz",
                "-C", BUILD_DIR "lib/"); 
        if (!nob_cmd_run_sync(cmd)) return 1;


        cmd.count = 0;
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
            "-I" BUILD_DIR "lib/cxxopts-3.3.1/include",
            "-I" INCLUDE_DIRS,
            "-o", BUILD_DIR PROJECT_NAME, 
            LINKER_DEPS,
            CORE_DIR "main.cpp");
        if (!nob_cmd_run_sync(cmd)) return 1;
    } else if (!strcmp(argv[1], "test")) {
        if (!nob_mkdir_if_not_exists(BUILD_DIR "test/")) return 1;

        /* Compile everything under test/ */
        Nob_Cmd cmd = {0};
        DIR *dir;
        struct dirent *ent;
        if ((dir = opendir("./test")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
                    continue;
                }
                char src_path[2048];
                sprintf(src_path, "./test/%s", ent->d_name);
                char dst_path[2048];
                sprintf(dst_path, BUILD_DIR "test/%s", ent->d_name);
                strip_extension(dst_path, dst_path, sizeof(dst_path));
                cmd.count = 0;
                nob_cmd_append(&cmd,
                        COMPILER,
                        STD_VERSION,
                        WARNINGS,
                        "-I" INCLUDE_DIRS,
                        LINKER_DEPS,
                        "-o", dst_path,
                        src_path);
                if (!nob_cmd_run_sync(cmd)) return 1;
            }
            closedir(dir);
        }

        /* Run everything under build/test */
        if ((dir = opendir(BUILD_DIR "test/")) != NULL) {
            while ((ent = readdir(dir)) != NULL) {
                if (!strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..")) {
                    continue;
                }
                char path[2048];
                sprintf(path, "./"BUILD_DIR"test/%s", ent->d_name);
                nob_log(NOB_INFO, "Running test %s", ent->d_name);
                cmd.count = 0;
                nob_cmd_append(&cmd,
                        path);
                if (nob_cmd_run_sync(cmd)) {
                    nob_log(NOB_INFO, "Test %s \x1b[32mPASSED\x1b[0m", ent->d_name);
                } else {
                    nob_log(NOB_INFO, "Test %s \x1b[31mFAILED\x1b[0m", ent->d_name);
                }
            }
        }
    }

    return 0;
}
