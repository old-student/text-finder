Prerequisites:
    OS : Ubuntu 14.04.5 LTS
    gcc: 6.5.0 20181026 (Ubuntu 6.5.0-2ubuntu1~14.04.1)
    Qt : QMake version 3.0 using Qt version 5.6.2

The following assumptions are made:
    * 5 seconds timeout for outgoing requests
    * if redirected to another url, requesting one
    * received response payload is interpreted as UTF-8 text
    * for url searching the following rule is used:
      substring that starts with HTTP:// and ends with
      invalid symbol, where valid symbols are
      A-Za-z0-9-._~:/?#[]@!$&()'*+,;=
