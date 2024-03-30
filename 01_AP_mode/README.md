# AP mode

Con ap_mode.ino vamos a poder crear una red wifi con nuestro ESP32. En el ejemplo la red va tener una contraseña,
en caso de querer una red abierta le pasamos el parametro de contraseña vacio.

### Run

Para correr este script dentro del ESP32:

- Abrir el script con el ide de arduino
- Conectar el ESP32 y seleccionar el arduino en tools -> ports -> 001
- Realizar un upload del codigo al arduino (verificar que se esta subiendo en el puerto 1)
- Seleccionar la lupa de "Serial Monitor" y elegir la opcion de "115200 baud"