## Juan the Ripper
### Carlos Sebastian Madrigal Rodriguez

## Compilacion y Ejecucion:

### Make:
```bash
make
```

### g++
``` sh
g++ -Wall -Wextra -Werror -pedantic-errors-std=c++2a -pthread -g -o main hashVal.cppSHA512.cpp SHA512.h
```
### Ejemplos de ejecucion:
``` sh
./juan --wordlist=[ruta del diccionario] [hash]
```