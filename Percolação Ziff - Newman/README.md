# Percolação de Ziff - Newman

## O que é percolação?

Fenômenos crítios são assuntos amplamente estudados por cientistas, um deles em especial é a percolação. Pense que temos uma floresta cheia de árvores, nessa floresta podem ter espaços vazios ou um espaço com uma árvore. Então de repente acontece um incêndio em uma dessas árvores, se esse incêncio conseguir chegar nas duas extremidades da floresta podemos dizer que o sistema "percolou". A percolação acontece quando alguma informação atinge toda a nossa rede, ele é uma fenômeno que tem como caracterísitca uma transição de segunda ordem, o nascimento de um enorme agregado percolante, dessa informação e ela separa a rede de dois estágios, um na qual esse grande agregado é muito menor que o tamanho da rede e um segundo estágio na qual o agregado tem tamanho da ordem da rede. No nosso exemplo, essa informação seria o incêndio, a rede seria a floresta como todo e o agregado percolante seria o conjunto de árvores que foram queimadas.

## Como funciona o algoritmo de Ziff-Newman?

Temos uma rede quadrada de tamanho *L* com *N = LxL* sítios, existe nessa rede uma probabilidade *p* de um sítio estar ocupado e uma probabildiade *1 - p* de não estar ocupado. A partir desses pontos isolados iremos ligar um a um e aumentando nosso parâmetro de controle *p* até o momento em que a rede percolará com uma probabilidade *p_critica*. A grande ideia por trás é a implementação da ideia de árvores e raízes, na qual quando vão se formando os agregados isolados escolhemos um dos sítios pertencentes a esse agregado e escolhemos ele como a raíz do agregado, que guarda o tamanho do agregado, e cada outro sítio será um galho que apontará para raiz. Isso é o que fará o algoritmo ser tão preciso e tão rápido.

## Referências
1. https://en.wikipedia.org/wiki/Percolation_theory
2. https://arxiv.org/abs/cond-mat/0101295
3. Scaling theory of percolation clusters - Stauffer D.