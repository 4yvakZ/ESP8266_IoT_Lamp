# ESP8266_IoT_Lamp

## Установка ESP8266 для Arduino IDE

[Полный гайд](https://randomnerdtutorials.com/how-to-install-esp8266-board-arduino-ide/)

В найстроках в дополнительных ссылках для менеджера плат добавляем две строки

```json
https://dl.espressif.com/dl/package_esp32_index.json,
http://arduino.esp8266.com/stable/package_esp8266com_index.json
```

Далее в менеджере плат ищем esp8266 и устанавливаем
После установки в списке плат должна появиться **ESP8266 Boards (3.0.0)>Generic ESP8266  Module**. После чего можно присупать к программированию самого контроллера

## 1. Пишем сервер на ESP8266

Для начала подключаем .zip библиотек из репозитория **Скетч>Подключить библиотеку>Добавить .ZIP библиотеку**

