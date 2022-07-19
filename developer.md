# ¿Cómo se hizo?

Para hacer El AFD Se usó el metodo de las derivadas, se deja más claro dentro del código donde explico por qué y cómo funciona cada método pero a grandes rasgos el programa hace lo siguiente en ese orden:    


    - En el constructor de la clase Automata se separan los símbolos de la ER.
    - Una vez obtenidos los símbolos de la ER empieza a derivar la ER símbolo por símbolo y agrega a una lista los nuevos estados obtenidos, también al tiempo crea las transiciones de cada estado. Así hasta que no aparece ningún estado nuevo.
    - Se acortan los nombres de los ER obtenidos.


Con esto hecho, ya solo fue implementar un GUI Básico para que el usuario pueda interactuar con el autómata. 
Para esto se usó la librería de FLTK porque es una libreria cross-platform: Se intetó compilar para windows 
pero por falta de medios fue imposible hacerlo, así que solo existe versión para linux, 
si se desea compilar en Windows es posible, al menos en teoría.

Si se quiere ahondar más en cómo está hecho invito a ver el código.

## Importante

Faltó un método para hacer que el autómata finito sea minimo, pero hizo falta tiempo.

## Problemas al momento de desarrollar

Quise hacerlo en C++ porque es un lenguaje que me gusta y además quiero practicar más en este, 
pero no fue el lenguaje más indicado a causa de mis pocos conocimientos, fue muy duro hacer esta práctica, 
pero muy divertido, hace rato no me divertía programando.    

los principales problemas al usar este lenguaje fueron las listas, al ser listas ligadas, no puedo usar índices 
como haría en otros lenguajes, también al no tener la posibilidad de usar windows 
pasé mucho rato buscando que librería gráfica multiplataforma podía usar. 
Encontré muchas pero la más fácil de utilizar fue FLTK, con unos pocos ejemplo ya pude hacer una app básica.    

La función de derivar fue la más difícil de hacer de lejos, eran demasiados casos los cuales revisar. 
Por ejemplo, Hay que tener en cuenta si empieza con un '(', si es así entonces hay que verificar si 
termina con ')*' o ')+' o solo ')' o si justo después empieza otro paréntesis, 
es necesario saber si hay un '|' en el primer nivel (Por primer nivel me refiero a que no esté dentro de unos paréntesis). 
Después en base a lo anterior dividir la ER. 
También hay que tener en cuenta si después de dividir la ER la parte izquierda, 
que en el código he llamado 'u', contiene a la cadena vacía, para esto también hubo que crear otra función. 
Además de estas dos, tuve que crear una tercera por el problema más grande que tuve. 
Esta última función fue la de 'containER', Lo que hace es verificar si una ER contiene a otra,
Básicamente, es la que no permite que haya duplicados, y a la vez tuvo un efecto secundario que no esperaba: 
hace que los autómatas finitos determinísticos más cortos; aún si no fue mi intención cuando me dí cuenta 
intenté utilizarla para crear AFDs minímos, pero no fuí capaz, me faltó tiempo, 
creo que es posible hacerlo, pero me faltaría analizar el caso aún más detenidamente y fue la 
función de las tres que más rápido hice.    

Teniendo la función derivar el resto fue muy fácil la verdad.    
    

Gracias por leer. 

