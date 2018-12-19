protocol
====
research and implementation of socket-based
custom protocol.

Format:
---
![](https://github.com/Lqinggang/protocol/format.png)


description:
header: 0xAA
length: option (1 byte) + cmd (1 byte) + data (N byte)
optoin: high 4-digit is version number, and low 4-digit is reseved
cmd   : 0x04
data  : data[0] = 0x01 or 0x02
         0x01: heartbeat package
		 0x02: report 
crc   : crc value of  length[2], option, cmd and data
tail  : 0x55


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

