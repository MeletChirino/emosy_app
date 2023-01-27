Para hacer un servicio tienes que copiar y pegar lo sgte 

```
[Unit]
Description=Casa Andrea Electrical Monitor System
After=multi-user.target

[Service]
Type=simple
ExecStart=/usr/bin/python /path/to/my/script.py
if venv
/path/to/my/venv/bin/python /path/to/my/script
Restart=on-abort

[Install]
WantedBy=multi-user.target
```

Lo pegas en `/lib/systemd/system/my_service.service`
y finalmente lo activas con 

```
sudo systemctl enable my_service
sudo systemctl start my_service
```

## Arduino-cli

Para compilar el sw embebido pdebes ejecutar la sgte linea de codigo

```
arduino-cli compile --library "C:\Users\cruz_\Documents\emosy_app\Ryem" -v
arduino-cli compile --library "C:\Users\cruz_\Documents\emosy_app\Ryem" -v -u && stty -F /dev/ttyUSB0 raw 115200 && cat /dev/ttyUSB0
```

Debiste haber instalado el WifiManager
```
arduino-cli lib install WifiManager
```

Tambien puedes usar el comando `compile` que hace que todo sea mucho mas facil para ti. Puedes agregar `-v` o `-u` como argumentos para tener mas opciones de debug por si quieres montar el archivo enseguida.

Para instalar la tarjeta tienes que seguir los sgtes pasos:
- Ejectuar `arduino-cli config init` en la terminal. Esto te va a decir que se 
escribio en un archivo `arduino-cli.yaml`

- Vas a ese archivo y agregas esto:
```yaml
board_manager:
  additional_urls: [http://arduino.esp8266.com/stable/package_esp8266com_index.json]
```

- Ahora vuelves a la terminal para ejecutar lo sgte:
```
arduino-cli core update-index
arduino-cli core install esp8266:esp8266
``
Cuando haya terminado ya tienes la libreria instalada. Me gustaria mas tenerla yo mismo aqui en el repo, igual que el WifiManager pero eso lo hare despues.
