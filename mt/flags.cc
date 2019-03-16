#include <iostream>
#include <sched.h>
#include <bitset>
#include <string>


void printFlag(int flag, std::string flagName) {
    std::cout << std::bitset<32>(flag) << " : " << flagName <<std::endl; 
}

int main(int argc, char const *argv[]){


    printFlag (CLONE_VM, "CLONE_VM");
    printFlag (CLONE_FS, "CLONE_FS");
    printFlag (CLONE_FILES, "CLONE_FILES");
    printFlag (CLONE_SIGHAND, "CLONE_SIGHAND");
    printFlag (CLONE_THREAD, "CLONE_THREAD");
    printFlag (CLONE_SYSVSEM, "CLONE_SYSVSEM");
    printFlag (CLONE_SETTLS, "CLONE_SETTLS");
    printFlag (CLONE_PARENT_SETTID, "CLONE_PARENT_SETTID");
    printFlag (CLONE_CHILD_CLEARTID, "CLONE_CHILD_CLEARTID");


    int flags = 0;

    flags |= CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID;

    printFlag (flags, "FLAGS");
    // std::cout << "flags : " << flags << std::endl;

    std::cout << (int) flags << std::endl;

    std::cout << "------------- after adding CLONE_PTRACE -----------------" << std::endl;

    printFlag (CLONE_VM, "CLONE_VM");
    printFlag (CLONE_FS, "CLONE_FS");
    printFlag (CLONE_FILES, "CLONE_FILES");
    printFlag (CLONE_SIGHAND, "CLONE_SIGHAND");
    printFlag (CLONE_THREAD, "CLONE_THREAD");
    printFlag (CLONE_SYSVSEM, "CLONE_SYSVSEM");
    printFlag (CLONE_SETTLS, "CLONE_SETTLS");
    printFlag (CLONE_PARENT_SETTID, "CLONE_PARENT_SETTID");
    printFlag (CLONE_CHILD_CLEARTID, "CLONE_CHILD_CLEARTID");
    printFlag (CLONE_PTRACE, "CLONE_PTRACE");

    flags |= CLONE_PTRACE;
    std::cout << (int) flags << std::endl;


    return 0;
}
