---
title: Why there are only 33 possible IPv4 netmasks
---
For my 3rd year project, I needed to look into the Linux kernel networking API.
Whilst reading the architecture documents, I came across the following
innocuous looking statement: "...an array of 33 pointers, one for every
FIB[forwarding information base] zone". 
  
To me, it wasn't immediately obvious why there are just 33 possibilities for an
IPv4 netmask. After a little thinking and google-ing, I found the answer. Due
to the fact that we want to split the address into two halves: the subnet and
host part, and we use `ADDRESS && NETMASK` to obtain the subnet we want a mask
that has (in binary) zero or more 1's followed by 0 or more 0's (with a total
of 32 bits). It's as simple as that. Anding the mask with the address will
result in 'keeping' the subnet bits and 'ignoring' or 'setting to 0' the host
bits.
  
The complete listing of possible netmasks is below:
    
~~~ 
       {------------BINARY------------}    {---HEX---}    {--DECIMAL--}
01 - {00000000 00000000 00000000 00000000} {0x00000000} {000.000.000.000}
02 - {10000000 00000000 00000000 00000000} {0x80000000} {128.000.000.000}
03 - {11000000 00000000 00000000 00000000} {0xC0000000} {192.000.000.000}
04 - {11100000 00000000 00000000 00000000} {0xE0000000} {224.000.000.000}
05 - {11110000 00000000 00000000 00000000} {0xF0000000} {240.000.000.000}
06 - {11111000 00000000 00000000 00000000} {0xF8000000} {248.000.000.000}
07 - {11111100 00000000 00000000 00000000} {0xFC000000} {252.000.000.000}
08 - {11111110 00000000 00000000 00000000} {0xFE000000} {254.000.000.000}
09 - {11111111 00000000 00000000 00000000} {0xFF000000} {255.000.000.000}
10 - {11111111 10000000 00000000 00000000} {0xFF800000} {255.128.000.000}
11 - {11111111 11000000 00000000 00000000} {0xFFC00000} {255.192.000.000}
12 - {11111111 11100000 00000000 00000000} {0xFFE00000} {255.224.000.000}
13 - {11111111 11110000 00000000 00000000} {0xFFF00000} {255.240.000.000}
14 - {11111111 11111000 00000000 00000000} {0xFFF80000} {255.248.000.000}
15 - {11111111 11111100 00000000 00000000} {0xFFFC0000} {255.252.000.000}
16 - {11111111 11111110 00000000 00000000} {0xFFFE0000} {255.254.000.000}
17 - {11111111 11111111 00000000 00000000} {0xFFFF0000} {255.255.000.000}
18 - {11111111 11111111 10000000 00000000} {0xFFFF8000} {255.255.128.000}
19 - {11111111 11111111 11000000 00000000} {0xFFFFC000} {255.255.192.000}
20 - {11111111 11111111 11100000 00000000} {0xFFFFE000} {255.255.224.000}
21 - {11111111 11111111 11110000 00000000} {0xFFFFF000} {255.255.240.000}
22 - {11111111 11111111 11111000 00000000} {0xFFFFF800} {255.255.248.000}
23 - {11111111 11111111 11111100 00000000} {0xFFFFFC00} {255.255.252.000}
24 - {11111111 11111111 11111110 00000000} {0xFFFFFE00} {255.255.254.000}
25 - {11111111 11111111 11111111 00000000} {0xFFFFFF00} {255.255.255.000}
26 - {11111111 11111111 11111111 10000000} {0xFFFFFF80} {255.255.255.128}
27 - {11111111 11111111 11111111 11000000} {0xFFFFFFC0} {255.255.255.192}
28 - {11111111 11111111 11111111 11100000} {0xFFFFFFE0} {255.255.255.224}
29 - {11111111 11111111 11111111 11110000} {0xFFFFFFF0} {255.255.255.240}
30 - {11111111 11111111 11111111 11111000} {0xFFFFFFF8} {255.255.255.248}
31 - {11111111 11111111 11111111 11111100} {0xFFFFFFFC} {255.255.255.252}
32 - {11111111 11111111 11111111 11111110} {0xFFFFFFFE} {255.255.255.254}
33 - {11111111 11111111 11111111 11111111} {0xFFFFFFFF} {255.255.255.255}
~~~