.. s documentation master file, created by
   sphinx-quickstart on Sun Dec 20 08:56:50 2015.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

|gitter|_
|buildstatus|_
|codecov|_
|donate|_

Welcome to Simba's documentation!
=================================

`Simba` is an Embedded Programming Platform. It aims to make embedded
programming easy and portable.

Project homepage: https://github.com/eerimoq/simba

.. toctree::
   :maxdepth: 1
   :titlesonly:
   :hidden:

   installation
   user-guide
   developer-guide
   boards
   examples
   library-reference

Features
--------

* :doc:`Threads<library-reference/kernel/thrd>` scheduled by a
  priority based cooperative or preemptive scheduler.

* Channels for inter-thread communication
  (:doc:`Queue<library-reference/sync/queue>`, 
  :doc:`Event<library-reference/sync/event>`).

* :doc:`Timers<library-reference/kernel/timer>`.

* :doc:`Counting semaphores<library-reference/sync/sem>`.

* Device drivers (:doc:`SPI<library-reference/drivers/spi>`,
  :doc:`UART<library-reference/drivers/uart>`, ...)

* A simple :doc:`shell<library-reference/oam/shell>`.

* :doc:`Logging<library-reference/debug/log>`.

* Internet protocols (:doc:`TCP<library-reference/inet/socket>`, 
  :doc:`UDP<library-reference/inet/socket>`, 
  :doc:`HTTP<library-reference/inet/http_server>`, ...).

* :doc:`Debug file system<library-reference/filesystems/fs>`.

* File systems (:doc:`FAT16<library-reference/filesystems/fat16>`,
  :doc:`SPIFFS<library-reference/filesystems/spiffs>`).

See the :doc:`library-reference` for a full list of features.

Design goals
------------

* Rapid development.
* Clean interfaces.
* Small memory footprint.
* No dynamic memory allocation.
* Portability.

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

.. |buildstatus| image:: https://travis-ci.org/eerimoq/simba.svg
.. _buildstatus: https://travis-ci.org/eerimoq/simba

.. |gitter| image:: https://badges.gitter.im/eerimoq/simba.svg
.. _gitter: https://gitter.im/eerimoq/simba

.. |codecov| image:: https://codecov.io/gh/eerimoq/simba/branch/master/graph/badge.svg
.. _codecov: https://codecov.io/gh/eerimoq/simba

.. |donate| image:: https://img.shields.io/badge/paypal-donate-yellow.svg
.. _donate: https://www.paypal.com/cgi-bin/webscr?cmd=_donations&business=X2NWV6RHV58GC&lc=SE&item_name=Simba%20OS%20and%20Build%20Framework&currency_code=USD&bn=PP%2dDonationsBF%3abtn_donateCC_LG%2egif%3aNonHosted
