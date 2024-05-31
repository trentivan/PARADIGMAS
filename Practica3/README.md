# Reporte de practica 3 Paradigmas de la programación

## Hernandez Ceseña Ivan Fernando

## SML
SML es un lenguaje de programacion funcional de proposito general.
Tiene tipos estáticos, lo que evita una amplia gama de errores de programación comunes, pero también cuenta con una potente inferencia de tipos, que requiere pocas declaraciones de tipos, si es que las hay. Esto facilita la creación de software que es fácil de entender, extensible y confiable.

Para esta practica se hace un TOUR por el lenguaje SML, se hace uso de las funciones basicas y se hace uso de las funciones de orden superior.

## Tipos de datos
SML tiene los siguientes tipos de datos:
- `int`: Enteros de precisión arbitraria.
- `real`: Números de punto flotante de precisión doble.
- `bool`: Valores booleanos.
- `string`: Cadenas de caracteres.
- `char`: Caracteres individuales.
- `list`: Listas homogéneas de longitud variable.
- `option`: Valores que pueden ser `SOME` de un tipo o `NONE`.
- `tuple`: Secuencias de valores de diferentes tipos.
- `record`: Colecciones de campos con nombres.
- etc.

Un ejemplo de estos son:
```sml
val u : unit = ()
val i : int = 42
val r : real = 3.14
val b : bool = true
val s : string = "hello"
val c : char = #"a"
val l : int list = [1, 2, 3]
val o : int option = SOME 42
val t : int * real = (42, 3.14)
```
etc.

Para esta practica se descargo un repositorio con varios archivos de ejemplo.
[archivos de ejemplo](imagen/archivos.png)


## Instalación
Para instalar SML en su sistema operativo, siga los siguientes pasos:
- Instalar Standard ML of New Jersey: https://www.smlnj.org/
  - SML/NJ contiene un administrador de compiladores interactivo/REPL que se utilizará para los ejemplos a lo largo de este recorrido.
  - SML/NJ también instalará una implementación de la biblioteca estándar, la biblioteca básica SML.
- Asegúrese de que SML/NJ se haya agregado a la ruta según corresponda para su arquitectura