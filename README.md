## Overview

This is a small application that searches for text on web pages.
The search starts from url provided by the user. All links found
on a page are used to further search via breadth-first search algorithm.
Downloading is multithreaded where the number of threads can be customized.

## Prerequisites:

* gcc: 6.5.0 20181026 (Ubuntu 6.5.0-2ubuntu1~14.04.1)
* Qt : QMake version 3.0 using Qt version 5.6.2

## Assumptions

* 5 seconds timeout for outgoing requests
* if redirected to another url -> request one
* received response payload is handled as UTF-8 text
* for url searching the following rule is used:
  substring that starts with HTTP:// and ends with invalid symbol,
  where valid symbols are A-Za-z0-9-._~:/?#[]@!$&()'*+,;=
