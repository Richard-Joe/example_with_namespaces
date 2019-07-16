## Example with the Namespaces

### Introduction

目前，Linux实现了七种不同类型的命名空间。每个命名空间的目的是将一个特定的全局系统资源包装在一个抽象中，使得它在命名空间内的进程看来它们拥有自己独立的全局资源实例。 命名空间的总体目标之一是支持容器的实现，容器是轻量级虚拟化（以及其他用途）的工具，它为一组进程提供了一个错觉，即它们是系统上唯一的进程。

 - CLONE_NEWNET     –  isolate network devices
 - CLONE_NEWUTS     –  host and domain names (UNIX Timesharing System)
 - CLONE_NEWIPC     –  IPC objects
 - CLONE_NEWPID     –  PIDs
 - CLONE_NEWNS      –  mount points (file systems)
 - CLONE_NEWUSER    –  users and groups
 - CLONE_NEWCGROUP  -  Cgroup root directory (since Linux 4.6)


Namespaces API由三个系统调用 `clone()`、`unshare()` 和 `setns()` 和许多/proc文件组成。

创建Namespaces的一种方法是使用`clone()`
```c
int clone(int (*child_func)(void *), void *child_stack, int flags, void *arg);
```

### Requirements

* CMake

### Quickstart

```bash
mkdir build
cd build
cmake ..
cmake --build .
```
