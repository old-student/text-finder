Prerequisites:
    OS : Ubuntu 14.04.5 LTS
    gcc: 6.5.0 20181026 (Ubuntu 6.5.0-2ubuntu1~14.04.1)
    Qt : QMake version 3.0 using Qt version 5.6.2

The following assumptions are made:
    * 5 seconds timeout for outgoing requests
    * if redirected to another url, requesting one
    * received response payload is interpreted as UTF-8 text
    * for url searching the following rule is used:
      substring that starts with HTTP:// and ends with invalid symbol,
      where valid symbols are A-Za-z0-9-._~:/?#[]@!$&()'*+,;=

Suggestions how to test application:
    1) As the first step the individual workers should be tested
       independently as it was a single-threaded app (unit tests).
       They aim to fetch data from network using such class like
       QNetworkAccessManager. To remove this external dependency
       a fake object should be introduced. In this way the following
       cases of interest can be covered:
           * there is no response (check timout mechanism)
           * responses with text inside that should be found by app
           * responses of very large size
    2) After making sure that individual workers are fine, they should
       be run in a multi-threaded environment using different scenarios
       with small and large responses, when system idle as well as
       when busy (stress testing). Randomized tests are also thinkable.
    3) Since application is multi-threaded, it is always a great idea
       to run it under Valgrind that can detect many memory management
       and threading bugs.
