# PIANO-F4HWN-APP
APP de piano para la version V6.0.0 del filmware F4HWN de Armel

## Requisitos
Este programa se ejecuta como una aplicación externa (Overlay App) y permite ocupar el teclado como un piano.
Requiere tener instalado el firmware [UV-K1-K5v3-firmware-custom de armel](https://github.com/armel/uv-k1-k5v3-firmware-custom) en su edición **Labs** para poder usarse.

**La app fue creada y testeada para la radio quansheng UV K5 (8) con el filmware F4HWN V6.0.0, no garantizo que funcione en otros modelos de quansheng**

## Como instalar?
Hay dos maneras:
1) Archivo precompilado:

1. Descarga el archivo ".zip" que contiene "Piano.app" y extráelo.
2. Ingresa a la herramienta web de instalación: [UVStudio Apps](https://armel.github.io/uvstudio/#apps).
3. Conecta tu radio al PC con el cable de programación. **La radio debe estar encendida en su modo normal.**
4. En la página, ve a **"App file (.app) / Choose file"** y busca y selecciona tu archivo "CWTX.app".
5. Elige el *slot* de memoria donde deseas instalar la app.
6. Presiona el botón **Install**.

2) Compilación desde el código fuente:

Si deseas modificar el código o compilar la aplicación por tu cuenta, necesitarás instalar las herramientas de compilación para ARM (`arm-none-eabi-gcc`). 

1. Preparar el entorno según tu Sistema Operativo:

**Linux (Debian / Ubuntu / Zorin OS / Mint):**
```Bash
sudo apt update
sudo apt install gcc-arm-none-eabi binutils-arm-none-eabi
```
**Linux (Arch / Manjaro):**

```Bash
sudo pacman -S arm-none-eabi-gcc arm-none-eabi-binutils
```

**Linux (Fedora):**

```Bash
sudo dnf install arm-none-eabi-gcc-cs arm-none-eabi-binutils
```
**macOS:**
Requiere Homebrew.

```Bash
brew install arm-none-eabi-gcc
```
**Windows:**

Dado que la compilación utiliza un script de bash (build.sh), la forma más sencilla en Windows es usar WSL (Windows Subsystem for Linux) instalando Ubuntu y luego ejecutando los comandos de Debian/Ubuntu mencionados arriba.

2. Compilar la aplicación
Una vez instaladas las dependencias [https://github.com/armel/uv-k1-k5v3-firmware-custom/archive/refs/tags/v6.0.0.zip](https://github.com/armel/uv-k1-k5v3-firmware-custom/archive/refs/tags/v6.0.0.zip), sigue estos pasos en tu terminal:

Descarga el código fuente y renombra la carpeta a "piano".

Mueve la carpeta a la ruta de las apps del firmware base:
.../uv-k1-k5v3-firmware-custom-6.0.0/App/apps/piano

Abre una terminal en esa carpeta y dale permisos de ejecución al script:

```Bash
chmod +x build.sh
```
Ejecuta el script de compilación:

```Bash
./build.sh
```
Si todo sale bien, se generará el archivo CWTX.app. Ahora puedes instalarlo siguiendo los pasos del Método 1.

## Manual y Características de PIANO-APP
Tiene dos modos, el "piano normal" y el "piano experto", para cambiar entre ellos se usa el boton "Menu".

Las (UP/DONW key) se usan para cambiar entre octavas (desde -2 a 2)

y tienen esta configuracion en modo normal:

1 = DO

2 = RE

3 = MI

4 = FA

5 = SOL

6 = LA

7 = SI

8 = DO

9 = RE

0 = MI

"*" = FA
  

y esta configuracion en modo experto:

1 = DO

2 = RE

3 = MI

4 = FA

5 = SOL

6 = LA

7 = SI


F + 1 = DO#

F + 2 = RE#

F + 4 = FA#

F + 5 = SOL#

F + 6 = LA#

## Usos (creo)
Se puede usar como base para sonidos en juegos hechos para la radio o para tener menus con sonido en las apps
