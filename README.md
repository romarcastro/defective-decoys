# Defective Decoys
An open-source turn-based game made with C.

 In a computer laboratory, two students must detect defective computers planted as decoys by one another. <br>
 Each must find all the defective computers on the other side of the laboratory to win the game.

**Game Mechanics:** <br>

***Phase 1: Setup Phase*** <br>
* In a 4x4 computer laboratory (grid), each player must plant their defective computers as decoys. Duplicate cell input is not allowed.

***Phase 2: Guessing Phase***<br>
* After setting up, the computer grid of the players is exchanged/transferred to their opponent. Be careful, each player has three lives only!
At every turn, the two players must avoid the defective computers on their opponent’s side by correctly opening a computer that is safe. If not, the computer malfunctions and explodes, losing a life.

* The game continues until one player has lost three lives.

## For users
**Quickstart**

`PC 1`
1. run server `./server` e.g. `./server`
3. run client `./client <server address>` e.g. `./client 192.168.33.1`

`PC 2`
1. run client `./client <server address>` e.g. `./client 192.168.33.1`

##  Development Team 👾
<ul>
  <li><a href="https://github.com/jkbicierro"> John Bicierro </a><br></li>
  <li><a href="https://github.com/erictanthegreat"> Eric Tan Jr. </a> <br></li>
  <li><a href="https://github.com/AnthonyDavis420"> John Ken Lanon </a> <br>
   <li><a href="https://github.com/romarcastro"> Romar Castro </a> <br></li>
</li>
</ul>

