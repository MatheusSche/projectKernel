# Damas multijogador via driver

Jogo de damas para 2 jogadores simultaneos, feito via kernel do linux. </br>

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
</br>

<h3>Terminal</h3>

<img src="assets/damasp3.png">



