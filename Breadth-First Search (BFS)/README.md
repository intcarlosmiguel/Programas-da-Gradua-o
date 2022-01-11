# Breadth-First Search
Quando se trata de uma rede regular é bastante simples encontrar quantos passos podem se chegar de um ponto A até B, contudo para redes mais complexas isso não pode ser tão simples. Para resolver esse problema existe o algoritmo de Breadth-First Search (BFS) na qual escolhemos um sítio *A* da nossa rede como raiz do algoritmo, cada vizinho de *A* tem distância 1, agora vamos para os vizinho dos vizinhos de *A* e cada um deles têm distância 2 da raiz e assim por diante. Se existir um sítio *B* que tenha distância 5 a partir de um caminho e 6 de outro (isso é possível se a rede for muito complexa) selecionamos o menor caminho até *B*.

Referências:
1. https://pt.wikipedia.org/wiki/Busca_em_largura
