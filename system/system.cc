/**************************************************************************//**
  \file     system.cc
  \brief    This source file implements the function of software system information display.
  \author   Lao·Zhu
  \version  V1.0.1
  \date     20. September 2022
 ******************************************************************************/

#include "system.h"
#include <iostream>
#include <sys/stat.h>
#include "gitver.h"
#include <sched.h>
#include <sys/resource.h>
#include <sys/file.h>
#include <unistd.h>

/*!
    \brief  Show software system compilation time and version
*/
void SystemInfo::DisplayCompileInfo(const std::string &version) {
    std::cout << "Compiler version: " << CXX_COMPILER_VERSION << "\n";
    std::cout << "GTK library version: " << GTK_LIBRARY_VERSION << "\n";
    std::cout << "GTKMM library version: " << GTKMM_LIBRARY_VERSION << "\n";
    std::cout << "Git version: " << GIT_VERSION_HASH << "\n";
    std::cout << "Compile time: " << __TIMESTAMP__ << "\n";
    std::cout << "Software version: " << version << "\n";
}

bool SystemInfo::FileExists(const std::string &filename) {
    struct stat buffer{};
    return (stat(filename.c_str(), &buffer) == 0);
}

bool SystemInfo::write_all_bytes_(const char *path, const void *data) noexcept {
    FILE *f = fopen(path, "wb+");
    if (nullptr == f)
        return false;
    fwrite((char *) data, 4, 1, f);
    fflush(f);
    fclose(f);
    return true;
}

void SystemInfo::set_priority_to_max() noexcept {
    char path_[260];
    snprintf(path_, sizeof(path_), "/proc/%d/oom_adj", getpid());

    char level_[] = "-17";
    write_all_bytes_(path_, level_);

    /* Processo pai deve ter prioridade maior que os filhos. */
    setpriority(PRIO_PROCESS, 0, -20);

    /* ps -eo state,uid,pid,ppid,rtprio,time,comm */
    struct sched_param param_{};
    param_.sched_priority = sched_get_priority_max(SCHED_FIFO); // SCHED_RR
    sched_setscheduler(getpid(), SCHED_RR, &param_);
}

static char read_buffer[1024 * 1024 * 128] = {0}, next_read_char = 0, last_line_flag = 0;
ssize_t current_read_number = 0, current_read_pos = 0, last_read_pos = 0;
char *my_fgetline(int fd) {
    if (last_line_flag == 1)
        return nullptr;
    process_buffer_again:
    last_read_pos = current_read_pos;
    read_buffer[current_read_pos] = next_read_char;
    while (read_buffer[current_read_pos++] != '\n' && current_read_pos != current_read_number);
    next_read_char = read_buffer[current_read_pos];
    read_buffer[current_read_pos] = read_buffer[current_read_pos - 1] = '\0';
    if (current_read_pos == current_read_number) {
        if (current_read_number < sizeof(read_buffer)) {
            last_line_flag = 1;
            return &read_buffer[last_read_pos];
        }
        lseek(fd, last_read_pos - current_read_pos, SEEK_CUR);
        current_read_number = read(fd, read_buffer, sizeof(read_buffer));
        current_read_pos = 0, last_read_pos = 0;
        next_read_char = read_buffer[0];
        goto process_buffer_again;
    }
    return &read_buffer[last_read_pos];
}

int my_fopen(const std::string &filename) {
    int fd = open(filename.c_str(), O_RDONLY);
    current_read_number = read(fd, read_buffer, sizeof(read_buffer));
    next_read_char = read_buffer[0];
    return fd;
}
