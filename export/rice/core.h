/*
 * file: Core.h
 * author: filebin
 * date: 2024-06-18 20:24:30
 */
#ifndef INCLUDE_CORE_H_
#define INCLUDE_CORE_H_

typedef struct _RicePackageManager RicePackageManager;

typedef struct RiceRuntimeCore {
    RicePackageManager *package_manager;
} RiceRuntimeCore;

RiceRuntimeCore *rice_core_new();
void rice_core_run(RiceRuntimeCore *core);

#endif