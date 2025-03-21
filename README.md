# cifrados-clasicos
Programas que cifran y descifran cifrados clásicos. Incluyen algunas funcionalidades de criptoanálisis.

# Como usar
Normalmente los programas tienen esta estructura de argumentos:
programa [-d|-e] [msg] [key]

Parámetros que van entre `<>` son **opcionales** mientras que entre `[]` son **obligatorios**

Para ver la estructura de los argumentos, basta con llamar al programa con un número incorrecto de argumentos.
Por ejemplo: `./cesar`

En el primer argumento sólo se puede poner -d ó -e. -d significa descifrar y -e cifrar.
El mensaje es el mensaje en claro / texto cifrado. Hay que ponerlo entrecomillado si tiene espacios. `"ESTE ES UN MENSAJE"`
Key es la clave. También hay que ponerla entrecomillada si tiene espacios. `"CLAVE QUE ES MUY BUENA"`

Hay otros programas que no tienen las mismas opciones, como por ejemplo que tengan varias claves o que tengan otros argumentos:
- Cifrado **César**: permite, además de -d o -e, permite -b, en el que descrifra por fuerza bruta y **descarta la clave** que se ha puesto.
**La clave es un número (número de posiciones hacia la derecha de desplazamiento), no una letra**
**Es obligatorio introducir la clave incluso si se descarta.**
Por ejemplo: `./cesar -b "IPMB" 12`. Mostrará por pantalla todos los mensajes hasta dar con "HOLA", que es el correcto.

- Cifrado **Alberti**. Una clave tal y como `Mb,4,3i` se compone de:
	- Posición inicial de los alfabetos (Mb).
	- Periodo de cambio (4).
	- Cuánto avanzar (3).
	- En qué dirección (i).
	- (Opcional) Un alfabeto personalizado
El programa de alberti aceptaría esta clave así:
`./alberti -e "HOLA" "Mb" "4" "3" "i"`
- **ADFGX** tiene una opción que activa el modo ADFGVX (Nótese la V). Se activa con un `-a` detrás de la última clave

# Soporte de cifradores/descifradores (Nombre de ejecutable entre paréntesis)
- ADFGX & ADFGVX (adfgx)
- Alberti (alberti)
- César (cesar)
- CT-46 (ct46)
- Hill (hill)
- Homofónicos (homophonic)
- Playfair (playfair)
- Tabla de Polibius (polybius)
- Porta/Bellaso (porta)
- Barajado (shuffle)
- Trasposición Interrumpida (shuffle_interrupted)
- Straddling Checkerboard (straddlingcheckerboard)
- Trasposición (trasposition)
- Ubchi (ubchi)
- Vernam (vernam)
- Vigenere (vigenere)

# Soporte de técnicas de criptoanálisis
- Chi cuadrado (chisquared)
- Índice de Coincidencia (coincidence)
- Método Kasiski (kasiski)
- Método de Kullback (kullback)

# Otros
- Permutación numérica (numper)

# Compilar
Necesario `meson` y `g++`
`sudo apt install meson g++`

```
git clone https://github.com/DisableGraphics/SegTI-Utils.git
cd SegTI-Utils
meson setup builddir
meson compile -C builddir
```