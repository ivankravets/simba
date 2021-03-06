:mod:`log` --- Logging
======================

.. module:: log
   :synopsis: Logging.

The logging module consists of log objects and log handlers. A log
object filters log entries and a log handler writes log entries to an
output channel.

A log object called "log" and a log handler writing to standard output
are created during the log module initialization. The log handler can
be replaced by calling `log_set_default_handler_output_channel()`.

Normally one log object is created for each subsystem in an
application. This gives the user the power to control which parts of
the system to debug and/or monitor at runtime.

Sometimes it's useful to write log entries to multiple channels. This
is possible by creating and adding another log handler to the log
module.

Log levels
----------

There are five log levels defined; fatal, error, warning, info and
debug. The log levels are defined as ``LOG_<upper case level>`` in the
log module header file.

Log entry format
----------------

A log entry consists of a timestamp, log level, thread name, log
object name and the message. The timestamp is the log entry creation
time and the log level is one of fatal, error, warning, info and
debug. The thread name is the name of the thread that created the log
entry and the log object name is the name of the log object the entry
was printed on. The message is a user defined string.

.. code:: text

   <timestamp>:<log level>:<thread name>:<log object name>: <message>

Debug file system commands
--------------------------

Three debug file system commands are available, all located in the
directory ``debug/log/``.

+-----------------------------------+-----------------------------------------------------------------+
|  Command                          | Description                                                     |
+===================================+=================================================================+
|  ``list``                         | Print a list of all log objects.                                |
+-----------------------------------+-----------------------------------------------------------------+
|  ``print <string>``               | Print a log entry using the default log object and log |br|     |
|                                   | level ``LOG_INFO``. This command has no use except to test |br| |
|                                   | that the log module works.                                      |
+-----------------------------------+-----------------------------------------------------------------+
|  ``set_log_mask <obejct> <mask>`` | Set the log mask to ``<mask>`` for log object ``<object>``.     |
+-----------------------------------+-----------------------------------------------------------------+

Example output from the shell:

.. code-block:: text

    $ debug/log/list
         OBJECT NAME  MASK
             default  0x0f
    $ debug/log/print "Hello World!"
    $ debug/log/set_log_mask default 0x1f
    $ debug/log/list
         OBJECT NAME  MASK
             default  0x1f
    $ debug/log/print "Hello World!!!"
    56:info:main:default: Hello World!!!

Example
-------

Here are a few example outputs using three log objects; `foo`, `bar`
and the default log object `default`. All logs are from the main
thread as can be seen in the third field in the entries.

.. code:: text

   23:info:main:foo: A foo info message.
   24:info:main:bar: A bar info message.
   37:debug:main:bar: A bar debug message.
   56:error:main:default: A main error message.

----------------------------------------------

Source code: :github-blob:`src/debug/log.h`, :github-blob:`src/debug/log.c`

Test code: :github-blob:`tst/debug/log/main.c`

Test coverage: :codecov:`src/debug/log.c`

----------------------------------------------

.. doxygenfile:: debug/log.h
   :project: simba

.. |br| raw:: html

   <br />
