Every command used to compile:
gcc -c paged_mem_simulator.c
gcc -fpic -c paged_memory.c
gcc -shared -o libdynpagedmem.so paged_memory.o
echo $LD_LIBRARY_PATH
pwd -> /home/kcottre/cs3100/Week6/Assignment6
LD_LIBRARY_PATH=/home/kcottre/cs3100/Week6/Assignment6
echo $LD_LIBRARY_PATH -> /home/kcottre/cs3100/Week6/Assignment6
gcc paged_mem_simulator.o -L. -ldynpagedmem -lm -o PMS