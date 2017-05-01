# ngx-uname

> Nginx module for displaying `uname` as JSON (Linux only).

### Installation

Firstly clone this repo somewhere.

__Download Nginx source:__

```
$ wget https://nginx.org/download/nginx-VERSION.tar.gz
$ cat nginx-VERSION.tar.gz | gunzip | tar -x
$ cd nginx-VERSION
```

__Build as static module in Nginx:__

```
$ ./configure --add-module=/path/to/ngx-uname
$ make
$ sudo make install
```

### Usage

Specify a `location` in your Nginx configuration to display the data,
which will be output as JSON (`application/json`):

```nginx
location /uname {
  uname;
}
```

__Example output:__

```json
{
  "sysname": "Linux",
  "nodename": "my-awesome-server",
  "release": "4.4.0-72-generic",
  "version": "#93-Ubuntu SMP Fri Mar 31 14:07:41 UTC 2017",
  "machine": "x86_64"
}
```