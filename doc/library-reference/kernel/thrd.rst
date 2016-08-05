:mod:`thrd` --- Threads
=======================

.. module:: thrd
   :synopsis: Threads.

A thread is the basic execution entity in the OS. A pre-emptive or
cooperative scheduler controls the execution of threads.

Debug file system commands
--------------------------

Four debug file system commands are available, all located in the
directory ``kernel/thrd/``.

+----------------------------------------+----------------------------------------------------------------+
|  Command                               | Description                                                    |
+========================================+================================================================+
|  ``list``                              | Print a list of all threads.                                   |
+----------------------------------------+----------------------------------------------------------------+
|  ``set_log_mask <thread name> <mask>`` | Set the log mask of thread ``<thread name>`` to ``mask``.      |
+----------------------------------------+----------------------------------------------------------------+
|  ``monitor/set_period_ms <ms>``        | Set the monitor thread sampling period to ``<ms>`` |br|        |
|                                        | milliseconds.                                                  |
+----------------------------------------+----------------------------------------------------------------+
|  ``monitor/set_print <state>``         | Enable(``1``)/disable(``0``) monitor statistics to be |br|     |
|                                        | printed periodically.                                          |
+----------------------------------------+----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

   $ kenel/thrd/list
               NAME           PARENT        STATE  PRIO   CPU  LOGMASK
               main                       current     0    0%     0x0f
                                main        ready   127    0%     0x0f
                                main        ready   -80    0%     0x0f

----------------------------------------------

Source code: :github-blob:`src/kernel/thrd.h`, :github-blob:`src/kernel/thrd.c`

Test code: :github-blob:`tst/kernel/thrd/main.c`

Test coverage: :codecov:`src/kernel/thrd.c`

----------------------------------------------

.. doxygenfile:: kernel/thrd.h
   :project: simba

.. |br| raw:: html

   <br />