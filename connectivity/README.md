# Connectivity

Este componente contiene la logica necesaria para la conectividad entre distintos nodos de la red. 

### Configuracion

Es necasario las siguientes variables:
- DEVICE_IS_ROOT_NODE: Si es o no el nodo root de la red
- DEVICE_ORIENTATION: Orientacion del ESP. [0, 1, 2, 3] siendo [Norte, Sur, Este, Oeste]. Para las orientaciones Norte y Sur vamos a mirar solo canales impares mientras que para Este y Oeste solo canales pares para conectarnos. Dependiendo el canal que le provea wifi al nodo va ser los canales que ofrescan sus ESP.
- DEVICE_UUID: uuid del dispositivo, sirve para evitar que dos ESP del mismo nodo se conecten.
