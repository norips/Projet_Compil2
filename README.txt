PROJET DE COMPILATION.
Raphaël DRUON, Timothée JOURDE, Amélie DOMENGER, Abdoul DIALLO.

Pour compiler l'ensemble des exécutables :
$ make

Cela va produire les exécutables suivants :

- exAnalyseSem : analyseur syntaxique et sémantique Pseudo Pascal (questions 1 et 2)
                 $ ./exAnalyseSem < test/quicksort.pp
                 
- interPP      : interpréteur Pseudo Pascal avec ramasse-miettes (question 3 et 7)
                 $ ./interPP < test/quicksort.pp
                 $ ./interPP < test/gc_test.pp

- compPP       : compilateur Pseudo Pascal vers C3A (question 4)
                 $ ./compPP < test/quicksort.pp

- interC3A     : interpréteur C3A (question 5)
                 $ ./compPP < test/quicksort.pp > quicksort.c3a
                 $ ./interC3A < quicksort.c3a

- compPPY86    : compilateur Pseudo Pascal vers Y86 (question 6)
                 $ ./compPPY86 < test/quicksort.pp
