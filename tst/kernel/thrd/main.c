/**
 * @file main.c
 * @version 6.0.0
 *
 * @section License
 * Copyright (C) 2014-2016, Erik Moqvist
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * This file is part of the Simba project.
 */

#include "simba.h"

static THRD_STACK(suspend_resume_stack, 256);

static void *suspend_resume_main(void *arg_p)
{
    thrd_set_name("resumer");
    thrd_resume(arg_p, 3);

    return (NULL);
}

static int test_suspend_resume(struct harness_t *harness_p)
{
    int err;
    struct thrd_t *thrd_p;

    thrd_p = thrd_spawn(suspend_resume_main,
                        thrd_self(),
                        10,
                        suspend_resume_stack,
                        sizeof(suspend_resume_stack));

    err = thrd_suspend(NULL);
    BTASSERT(err == 3, "err = %d", err);

    /* Wait for the spawned thread to terminate. */
    BTASSERT(thrd_join(thrd_p) == 0);

    return (0);
}

static int test_yield(struct harness_t *harness_p)
{
    BTASSERT(thrd_yield() == 0);

    return (0);
}

#if CONFIG_PREEMPTIVE_SCHEDULER == 1

static THRD_STACK(preemptive_stack, 256);

static void *preemptive_main(void *arg_p)
{
    thrd_set_name("preemptive");

    while (1);

    return (NULL);
}

static int test_preemptive(struct harness_t *harness_p)
{
    struct time_t timeout;
    struct time_t start, stop, duration;

    /* Spawn a low priority worker thread. */
    BTASSERT(thrd_spawn(preemptive_main,
                        NULL,
                        20,
                        preemptive_stack,
                        sizeof(preemptive_stack)) != NULL);

    BTASSERT(time_get(&start) == 0);

    /* Suspend this thread to make sure the worker thread is in its
       infinite loop. When the suspend timeout occurs, this thread
       will be scheduled since it has higher priority than the worker
       thread. */
    timeout.seconds = 0;
    timeout.nanoseconds = 10000000;
    BTASSERT(thrd_suspend(&timeout) == -ETIMEDOUT);

    BTASSERT(time_get(&stop) == 0);
    BTASSERT(time_diff(&duration, &stop, &start) == 0);

    BTASSERT(duration.seconds == 0);
    BTASSERT(duration.nanoseconds == 10000000);

    return (0);
}

#else

static int test_preemptive(struct harness_t *harness_p)
{
    return (1);
}

#endif

static int test_env(struct harness_t *harness_p)
{
    struct thrd_environment_variable_t variables[4];

    /* Set and get are not possible for a thread without an
       environment. */
    BTASSERT(thrd_init_env(NULL, 0) == 0);
    BTASSERT(thrd_set_env("CWD", "/") == -1);
    BTASSERT(thrd_get_env("CWD") == NULL);

    /* Initialize the environment for the current thread. */
    BTASSERT(thrd_init_env(variables, membersof(variables)) == 0);

    /* Set and get variables. */
    BTASSERT(thrd_set_env("N1", "V1") == 0);
    BTASSERT(thrd_get_env("N1") != NULL);
    BTASSERT(strcmp(thrd_get_env("N1"), "V1") == 0);

    BTASSERT(thrd_set_env("N2", "V2") == 0);
    BTASSERT(thrd_get_env("N2") != NULL);
    BTASSERT(strcmp(thrd_get_env("N2"), "V2") == 0);

    BTASSERT(thrd_set_env("N3", "V3") == 0);
    BTASSERT(thrd_get_env("N3") != NULL);
    BTASSERT(strcmp(thrd_get_env("N3"), "V3") == 0);

    BTASSERT(thrd_set_env("N4", "V4") == 0);
    BTASSERT(thrd_get_env("N4") != NULL);
    BTASSERT(strcmp(thrd_get_env("N4"), "V4") == 0);

    /* Overwrite a value. */
    BTASSERT(thrd_set_env("N4", "V44") == 0);
    BTASSERT(thrd_get_env("N4") != NULL);
    BTASSERT(strcmp(thrd_get_env("N4"), "V44") == 0);

    /* No free space. */
    BTASSERT(thrd_set_env("N5", "V5") == -1);

    /* Remove a variable. */
    BTASSERT(thrd_set_env("N2", NULL) == 0);

    /* Set and get another variable. */
    BTASSERT(thrd_set_env("N6", "V6") == 0);
    BTASSERT(thrd_get_env("N6") != NULL);
    BTASSERT(strcmp(thrd_get_env("N6"), "V6") == 0);

    /* Get a non-existing variable. */
    BTASSERT(thrd_get_env("N7") == NULL);

    BTASSERT(thrd_init_env(NULL, 0) == 0);

    return (0);
}

int main()
{
    struct harness_t harness;
    struct harness_testcase_t harness_testcases[] = {
        { test_suspend_resume, "test_suspend_resume" },
        { test_yield, "test_yield" },
        { test_preemptive, "test_preemptive" },
        { test_env, "test_env" },
        { NULL, NULL }
    };

    sys_start();

    harness_init(&harness);
    harness_run(&harness, harness_testcases);

    return (0);
}
