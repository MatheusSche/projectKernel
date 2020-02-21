# Damas multijogador via driver

Jogo de damas para 2 jogadores simultaneos, feito via kernel do linux. </br>

<h3>Como executar...</h3>
<strong>Comandos para carregar o driver:</strong> </br>
1. sudo mknod /dev/so c 60 0
2. sudo chmod 666 /dev/so
3. make
4. sudo insmod driver.ko
</br>
<strong>Comandos para carregar o jogo:</strong> </br>
1. gcc -o damas damas.c
2. ./damas
</br>
<strong>Para reiniciar o driver:</strong> </br>
1. ./comand.sh
</br>

<h3>Terminal</h3>

<img src="assets/damasp3.png">



