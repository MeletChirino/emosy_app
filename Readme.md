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
