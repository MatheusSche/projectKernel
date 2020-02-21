# Damas C

Implementação de um programa de “Damas”, onde os dois usuários se
enfrentam utilizando dois programas distintos na mesma máquina. Para comunicação entre os programas
foi utilizado os conceitos de um dispositivo virtual do tipo caracter (/dev/so).
A comunicação entre os programas é feita através de um dispositivo virtual do tipo caracter.
Toda a comunicação é realizada através da chamada de sistema ioctl. </br>

<h3>Como executar...</h3>
<strong>Comandos para carregar o driver:</strong> </br>
1. sudo mknod /dev/so c 60 0 </br>
2. sudo chmod 666 /dev/so </br>
3. make </br>
4. sudo insmod driver.ko </br>
</br>
<strong>Comandos para carregar o jogo:</strong> </br>
1. gcc -o damas damas.c </br>
2. ./damas </br>

<strong>Para reiniciar o driver:</strong>
1. ./comand.sh


<h3>Terminal</h3>

<img src="assets/damasp3.png">



