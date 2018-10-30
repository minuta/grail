#ifndef __PTRACE_UTILS_H__
#define __PTRACE_UTILS_H__

#include <bits/types.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#include <functional>
#include <unordered_map>
#include <memory>
#include <tuple>
#include <utility>
#include <sstream>

enum SyscallHandlerStatusCode {
  SYSC_SUCCESS     = 0x1,  /* application will run again */
  SYSC_ERROR       = 0x2,  /* simulation  will stop */
  SYSC_SYSTEM_EXIT = 0x4,  /* application will stop */
  SYSC_DELAYED     = 0x8,  /* application will pause */ 
  SYSC_FAILURE     = 0x10, /* application will run again, failure logged */
  SYSC_MANUAL      = 0x20  /* same as DELAYED, but without logging       */
};

#define FAKE(ret_val) do {                                              \
    set_reg(pid, orig_rax, SYS_getpid);                                 \
    if (WaitForSyscall(pid) != 0) {                                     \
      return SYSC_ERROR;                                                \
    }                                                                   \
    set_reg(pid, rax, ret_val);                                         \
    NS_LOG_LOGIC(pid << ": [EE] FAKE syscall returned: " << ret_val);   \
  } while(0)

#define FAKE2(ret_val) do {                                             \
    set_reg(pid, orig_rax, SYS_getpid);                                 \
    if (WaitForSyscall(pid) != 0) {                                     \
      res = SYSC_ERROR;                                                 \
      break;                                                            \
    }                                                                   \
    set_reg(pid, rax, ret_val);                                         \
    NS_LOG_LOGIC(pid << ": [EE] FAKE syscall returned: " << ret_val);   \
  } while(0)

// Aligns the supplied size to the specified PowerOfTwo
#define ALIGN_SIZE( sizeToAlign, PowerOfTwo )       \
  (((sizeToAlign) + (PowerOfTwo) - 1) & ~((PowerOfTwo) - 1))

// Checks whether the supplied size is aligned to the specified PowerOfTwo
#define IS_SIZE_ALIGNED( sizeToTest, PowerOfTwo )  \
  (((sizeToTest) & ((PowerOfTwo) - 1)) == 0)

// More casts than needed to pass gcc's -Werror
#define WORDSIZE ((size_t)sizeof(char*))

#define ALIGN(sizeToAlign) (ALIGN_SIZE(sizeToAlign, WORDSIZE))

#define myoffsetof(a, b) __builtin_offsetof(a,b)
#define get_reg(child, name) __get_reg(child, myoffsetof(struct user, regs.name))
#define set_reg(child, name, val) __set_reg(child, myoffsetof(struct user, regs.name), val)
#define REG(name) myoffsetof(struct user, regs.name)

inline size_t __get_reg(pid_t child, int off) {
  errno = 0;
  size_t val = (size_t)ptrace(PTRACE_PEEKUSER, child, off);
  assert(errno == 0);
  return val;
}
inline size_t __set_reg(pid_t child, int off, uint64_t val) {
  errno = 0;
  size_t res = (size_t)ptrace(PTRACE_POKEUSER, child, off, val);
  assert(errno == 0);
  return res;
}

template<int Ctx, typename... Args> struct X;

template<typename T, typename... Args> struct X<1,T,Args...>
{
  static void ReadArgs(pid_t child, T& arg, Args&... args)
  {
    arg = (T)get_reg(child, rdi);
    X<2,Args...>::ReadArgs(child, args...);
  }
};
template<typename T, typename... Args> struct X<2,T,Args...>
{
  static void ReadArgs(pid_t child, T& arg, Args&... args)
  {
    arg = (T)get_reg(child, rsi);
    X<3,Args...>::ReadArgs(child, args...);
  }
};
template<typename T, typename... Args> struct X<3,T,Args...>
{
  static void ReadArgs(pid_t child, T& arg, Args&... args)
  {
    arg = (T)get_reg(child, rdx);
    X<4,Args...>::ReadArgs(child, args...);
  }
};
template<typename T, typename... Args> struct X<4,T,Args...>
{
  static void ReadArgs(pid_t child, T& arg, Args&... args)
  {
    arg = (T)get_reg(child, r10);
    X<5,Args...>::ReadArgs(child, args...);
  }
};
template<typename T, typename... Args> struct X<5,T,Args...>
{
  static void ReadArgs(pid_t child, T& arg, Args&... args)
  {
    arg = (T)get_reg(child, r8);
    X<6,Args...>::ReadArgs(child, args...);
  }
};
template<typename T, typename... Args> struct X<6,T,Args...>
{
  static void ReadArgs(pid_t child, T& arg, Args&... args)
  {
    arg = (T)get_reg(child, r9);
    X<7,Args...>::ReadArgs(child, args...);
  }
};
template<int Ctx> struct X<Ctx>
{
  static void ReadArgs(pid_t)
  {}
};
template<typename... Args> void read_args(pid_t child, Args&... args) 
{
  X<1,Args...>::ReadArgs(child, args...);
}
template<typename... Args> void read_args2(pid_t child, Args&... args) 
{
  X<2,Args...>::ReadArgs(child, args...);
}
template<typename... Args> void read_args3(pid_t child, Args&... args) 
{
  X<3,Args...>::ReadArgs(child, args...);
}
template<typename... Args> void read_args4(pid_t child, Args&... args) 
{
  X<4,Args...>::ReadArgs(child, args...);
}
template<typename... Args> void read_args5(pid_t child, Args&... args) 
{
  X<5,Args...>::ReadArgs(child, args...);
}
template<typename... Args> void read_args6(pid_t child, Args&... args) 
{
  X<6,Args...>::ReadArgs(child, args...);
}

inline size_t MemcpyFromTracee(int pid, void* to, void* from, size_t len) {
  // printf("<-- %p, len: %zu\n", from, len);
  size_t i;
  long tmp;
  for(i=0; i<len; i+=WORDSIZE) {
    tmp = ptrace(PTRACE_PEEKDATA, pid, (char*)from+i, 0);
    if(i+WORDSIZE <= len) {
      memcpy((char*)to+i, &tmp, WORDSIZE);
    }
    else {
      memcpy((char*)to+i, &tmp, len % WORDSIZE);
    }
  }
  return i;
}

inline size_t MemcpyToTracee(int pid, void* to, void* from, size_t len) {
  size_t i;
  for(i=0; i<len; i+=WORDSIZE) {
    // printf("--> %p, len: %zu\n", to, len);
    if(i+WORDSIZE <= len) {
      errno = 0;
      long res = ptrace(PTRACE_POKEDATA, pid, (char*)to+i, *(long*)((char*)from+i));
      if(res == -1) {
        printf("POKEDATA ERROR --> pid: %d, i: %zu, errno: %d/%s\n",pid,i,errno,strerror(errno));
        exit(1);
      }
    }
    else {
      errno = 0;
      long tmp = ptrace(PTRACE_PEEKDATA, pid, (char*)to+i, 0);
      if(errno != 0) {
        printf("PEEKDATA ERROR --> pid: %d, i: %zu, errno: %d/%s\n",pid,i,errno,strerror(errno));
        exit(1);
      }
      memcpy(&tmp, (char*)from+i, len % WORDSIZE);
      long res = ptrace(PTRACE_POKEDATA, pid, (char*)to+i, tmp);
      if(res == -1) {
        printf("PEEKDATA ERROR --> pid: %d, i: %zu, errno: %d/%s\n",pid,i,errno,strerror(errno));
        exit(1);
      }
    }
  }
  return i;
}

template<typename T>
void LoadFromTracee(int pid, T* to, T* from) {
  if(!from) return;
  MemcpyFromTracee(pid, to, from, sizeof(T));
}
template<typename T>
void StoreToTracee(int pid, T* from, T* to) {
  if(!to || !from) return;
  MemcpyToTracee(pid, to, from, sizeof(T));
}


inline int WaitForSyscall(int pid) {
  int status;
  while (1) {
    ptrace(PTRACE_SYSCALL, pid, 0, 0);
    waitpid(pid, &status, 0);
    if (WIFSTOPPED(status) && WSTOPSIG(status) == (SIGTRAP | 0x80))
      return 0;
    if (WIFEXITED(status))
      return 1;
  }
}




#endif // __PTRACE_UTILS_H__
