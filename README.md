# Trabajo Final ESP32

## Introduccion

En este proyecto desarrollaremos todo lo relacionado a la connexion y comunicacion entre varios ESP32. 
Se desarrollara desde el descubrimiento de los nodos vecinos hasta la comunicacion entre ellos, como el protocolo a utilizar y la forma de enviar y recibir mensajes.

## Instalacion & Ejecucion

### Paso 1:

Comenzar instalando los [pre-requisitos mencionados](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/get-started/linux-macos-setup.html#get-started-prerequisites). Para mac:

### Paso 2:

Descargar ESP32:

```
mkdir -p ~/esp
cd ~/esp
git clone -b v5.2.2 --recursive https://github.com/espressif/esp-idf.git
```

### Paso 3:

Instalar ESP-IDF:

```
cd ~/esp/esp-idf
./install.sh esp32
```

### Paso 4:

Exportar el comando, dentro de /Users/<user_name>/esp/esp-idf. Hay que hacer este export por consola o agregar el comando al PATH:

```
. ./export.sh
```

### Paso 5:

Buildear el proyecto, dentro de connectivity:

```
idf.py build
```
