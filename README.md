# Лабораторные работы по параллельному программированию
Сборка:
```
make
```
Запуск (лабораторной работы №1):
```
./pp_1
```

Запуск лабораторных 9.1, 9.2, 9.3:
```
mpirun -n 4 ./pp_91 # -n - количество потоков
```

## Требования
Должны быть установлены ```g++```, ```make``` (входят в пакет ```build-essentials``` (ubuntu), а также 
```mpich # sudo apt-get install mpich```