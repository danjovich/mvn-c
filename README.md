# MVN C

Essa é uma máquina virtual inspirada na utilizada na matéria PCS3616 - Sistemas de Programação da Engenharia de Computação Escola Politécnica da USP.

Ela implementa a seguinte especificação:

| OP|  Ações |
| --- | --- |
| 0 | IC := OI |
| 1 | Se AC= 0 então IC := OI senão IC := IC+1 |
| 2 | Se AC<0 então IC := OI senão IC := IC+1 |
| 3 | AC := OI; IC := IC+1 |
| 4 | AC := AC+MEM[OI]; IC := IC+1 |
| 5 | AC := AC-MEM[OI]; IC := IC+1 |
| 6 | AC := AC*MEM[OI]; IC := IC+1 |
| 7 | AC := int(AC/MEM[OI]); IC := IC+1 |
| 8 | AC := MEM[OI]; IC := IC+1 |
| 9 | MEM[OI] := AC; IC := IC+1 |
| A | MEM[OI] := IC+1; IC := OI+1 |
| B | IC := MEM[OI] |
| C | IC := OI |
| D | aguarda; AC := dado de entrada; IC := IC+1 |
| E | dado de saída := AC; aguarda; IC := IC+1 |
| F | Nada, por ora; IC := IC+1 |