protocol
====
research and implementation of socket-based
custom protocol.

Format:
---

![](https://github.com/Lqinggang/protocol/blob/master/format.png)
(stale) <br>
description: <br>
header: 0xAA <br>
length: option (1 byte) + cmd (1 byte) + data (N byte) <br>
optoin: high 4-digit is version number, and low 4-digit is reseved <br>
cmd   : 0x01 <br>
data  : data[0] = 0x01 or 0x02 <br>
         0x01: heartbeat package <br>
		 0x02: report  <br> 
crc   : crc value of  length[2], option, cmd and data <br>
tail  : 0xFF<br>


Build:
---
run the following command in the root directory:
```c
make
```
clean up please use:
```c
make clean
```

Note:
---
you must be set the LD_LIBRARY_PATH use
export LD_LIBRARY_PATH=(path)/(to)/(lib)

