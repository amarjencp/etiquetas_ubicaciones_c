
# Generador de Etiquetas de Ubicación en PDF

## Descripción

Este programa, `labels`, es una herramienta diseñada para generar etiquetas de ubicación en formato PDF para su uso en almacenes. Las etiquetas incluyen códigos de barras generados a partir de las coordenadas de ubicación dentro del almacén, como zona, bloque, fila y altura.

Cada etiqueta está diseñada para imprimir en formato A6 (148x105 mm) en orientación horizontal, y puede incluir uno o dos códigos de barras según la altura especificada.

### Dependencias

El programa depende de las siguientes bibliotecas para su funcionamiento:

- **libzint** versión 2.13.0: Utilizada para generar códigos de barras.
- **libhpdf** versión 2.4.4: Utilizada para crear y manipular archivos PDF.
- **libpng** versión 1.6.43: Utilizada por `libzint` para generar imágenes PNG.
- **zlib** versión 1.3.1: Biblioteca de compresión utilizada por `libhpdf` y `libpng`.

## Uso

### Compilación

Asegúrate de tener instaladas las bibliotecas mencionadas anteriormente. Luego, puedes compilar el programa utilizando el siguiente comando:

```bash
gcc -o labels labels.c -lzint -lhpdf -lpng -lz
```

### Ejecución

El programa requiere cuatro argumentos para ejecutarse:

1. `zona`: Número de zona en el almacén (un valor entero).
2. `bloques`: Número de bloques a generar (un valor entero).
3. `filas`: Número de filas por bloque (un valor entero).
4. `alturas`: Número de niveles de altura por fila (un valor entero).

Ejemplo de uso:

```bash
./labels 3 28 30 6
```

Este comando generará archivos PDF para la zona 3, con 28 bloques, 30 filas por bloque y 6 niveles de altura. Los archivos se guardarán en la carpeta `./output/` con un nombre de archivo que indica la zona y el bloque, por ejemplo, `labels_zona03_bloque01.pdf`.

### Notas

- **Carpeta de salida**: Asegúrate de que la carpeta `./output/` exista antes de ejecutar el programa, o créala manualmente.
- **Fuentes**: El programa utiliza la fuente `LinLibertine_RB.ttf` que debe estar disponible en la ruta `./libertine/LinLibertine_RB.ttf`. Si la fuente no se encuentra, el programa no podrá cargarla y fallará.

## Información Adicional

- **Rotación y Orientación**: Las etiquetas se generan en formato horizontal (paisaje) para maximizar el espacio de la página.
- **Códigos de Barras**: El código de barras generado es del tipo Code 128, un formato de alta densidad que es adecuado para representar números y texto.

### Limpieza de Archivos Temporales

El programa genera archivos PNG temporales para los códigos de barras. Estos archivos se eliminan automáticamente después de ser incrustados en el PDF.

## Autor

- **Tony M. Jenkins** - Desarrollador principal.

### Licencia

Este programa está bajo una licencia libre, permite la distribución y modificación bajo los términos de la misma.
