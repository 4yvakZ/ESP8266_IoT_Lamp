
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

## 1. Пишем HTML страницу

Для начала напишем HTML страницу, которая будет отображаться при подключении к контроллеру.

Опробовать заранее можно в [онлайн редакторе](https://www.w3schools.com/howto/tryit.asp?filename=tryhow_css_rangeslider).

HTML код с необходимыми функиями на JavaScript:

```html
<!DOCTYPE html>
<html>
<head>
	<meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
	<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
	<style>
		html {
		 font-family: Arial;
		 display: inline-block;
		 margin: 0px auto;
		 text-align: center;
		}
		h2 { font-size: 3.0rem; }
		p { font-size: 2.0rem; }
		.value {
			width: 50px;
			display: inline-block;
		}
	</style>
</head>
<body>
	<h2>ESP8266 IoT Lamp</h2>
	<p>
		<i class="fas fa-temperature-high" style="color:#03d1ff;"></i>
		<span class="dht-labels">Температура</span>
		<span id="temperature">%TEMPERATURE%</span>
		<span>&deg;C</span>
	</p>

	<p>
		<i class="fas fa-sun" style="color:#ff9100;"></i>
		<span >Освещённость</span>
		<span id="brightness">%BRIGHTNESS%</span>
		<span>%</span>
	</p>

	<p>
		<span>R:</span>
		<input type="range" min="0" max="255" value="%R%" onchange="sendSlider(this)" id="RangeR">
		<span class="value" id="ValueR">
	</p>

	<p>
		<span>G:</span>
		<input type="range" min="0" max="255" value="%G%" onchange="sendSlider(this)" id="RangeG">
		<span class="value" id="ValueG">
	</p>

	<p>
		<span>B:</span>
		<input type="range" min="0" max="255" value="%B%" onchange="sendSlider(this)" id="RangeB">
		<span class="value" id="ValueB">
	</p>

	<p>
		<span>Радуга</span>
		<input type="checkbox" id="Rainbow" onchange="sendToggle(this)" %RAINBOW%>
	</p>

	<p>
		<span>Автояркость</span>
		<input type="checkbox" id="Autobrightness" onchange="sendToggle(this)" %AUTOBRIGHTNESS%>
	</p>
	<script>
		var sliders = new Array(document.getElementById("RangeR"),document.getElementById("RangeG"),
			document.getElementById("RangeB"));
		var outputs = new Array(document.getElementById("ValueR"),document.getElementById("ValueG"),
		document.getElementById("ValueB"));

		outputs[0].innerHTML = sliders[0].value;
		sliders[0].oninput = function() {
			outputs[0].innerHTML = this.value;
		}
		outputs[1].innerHTML = sliders[1].value;
		sliders[1].oninput = function() {
			outputs[1].innerHTML = this.value;
		}

		outputs[2].innerHTML = sliders[2].value;
		sliders[2].oninput = function() {
			outputs[2].innerHTML = this.value;
		}

		function sendSlider(element){
			var sliderValue = element.value;
			console.log(sliderValue);
			var xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?slider="+element.id+"&value="+sliderValue, true);
			xhr.send();
		}

		function sendToggle(element){
			var xhr = new XMLHttpRequest();
			if(element.checked){
				xhr.open("GET", "/update?toggle="+element.id+"&state=1", true);
			} else {
				xhr.open("GET", "/update?toggle="+element.id+"&state=0", true);
			}
			xhr.send();
		}

		setInterval(function ( ) {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function() {
					if (this.readyState == 4 && this.status == 200) {
						document.getElementById("temperature").innerHTML = this.responseText;
					}
				};
				xhttp.open("GET", "/temperature", true);
				xhttp.send();
			}, 1000 ) ;

		setInterval(function ( ) {
				var xhttp = new XMLHttpRequest();
				xhttp.onreadystatechange = function() {
					if (this.readyState == 4 && this.status == 200) {
						document.getElementById("brightness").innerHTML = this.responseText;
					}
				};
				xhttp.open("GET", "/brightness", true);
				xhttp.send();
			}, 1000 ) ;
	</script>
</body>
</html>
```

### Разбор HTML

Хеддер файла с необходимыми ссылками на картинки и основными свойствами отрисовки

```html
<head>
	<meta name="viewport" content="width=device-width, initial-scale=1" charset="UTF-8">
	<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css">
	<style>
		html {
		 font-family: Arial;
		 display: inline-block;
		 margin: 0px auto;
		 text-align: center;
		}
		h2 { font-size: 3.0rem; }
		p { font-size: 2.0rem; }
		.value {
			width: 50px;
			display: inline-block;
		}
	</style>
</head>
```

#### Первая часть тела страницы

Заголовок

```html
	<h2>ESP8266 IoT Lamp</h2>
```
Параграф с картинкой термометра, надписью "Температура" и соответствующим плейсхолдером
```html
	<p>
		<i class="fas fa-temperature-high" style="color:#03d1ff;"></i>
		<span class="dht-labels">Температура</span>
		<span id="temperature">%TEMPERATURE%</span>
		<span>&deg;C</span>
	</p>
```
Аналогично для освещённости
```html
	<p>
		<i class="fas fa-sun" style="color:#ff9100;"></i>
		<span >Освещённость</span>
		<span id="brightness">%BRIGHTNESS%</span>
		<span>%</span>
	</p>
```
Рисуем три слайдера для цветов адресной светодиодной ленты
```html
	<p>
		<span>R:</span>
		<input type="range" min="0" max="255" value="%R%" onchange="sendSlider(this)" id="RangeR">
		<span class="value" id="ValueR">
	</p>

	<p>
		<span>G:</span>
		<input type="range" min="0" max="255" value="%G%" onchange="sendSlider(this)" id="RangeG">
		<span class="value" id="ValueG">
	</p>

	<p>
		<span>B:</span>
		<input type="range" min="0" max="255" value="%B%" onchange="sendSlider(this)" id="RangeB">
		<span class="value" id="ValueB">
	</p>
```
Рисуем два чекбокса для двух режимов режимов
```html
	<p>
		<span>Радуга</span>
		<input type="checkbox" id="Rainbow" onchange="sendToggle(this)" %RAINBOW%>
	</p>

	<p>
		<span>Автояркость</span>
		<input type="checkbox" id="Autobrightness" onchange="sendToggle(this)" %AUTOBRIGHTNESS%>
	</p>
```
#### Вторая часть со скриптами

Обработка слайдеров

```html
<script>
		var sliders = new Array(document.getElementById("RangeR"),document.getElementById("RangeG"),
			document.getElementById("RangeB"));
		var outputs = new Array(document.getElementById("ValueR"),document.getElementById("ValueG"),
		document.getElementById("ValueB"));

		outputs[0].innerHTML = sliders[0].value;
		sliders[0].oninput = function() {
			outputs[0].innerHTML = this.value;
		}
		outputs[1].innerHTML = sliders[1].value;
		sliders[1].oninput = function() {
			outputs[1].innerHTML = this.value;
		}

		outputs[2].innerHTML = sliders[2].value;
		sliders[2].oninput = function() {
			outputs[2].innerHTML = this.value;
		}

		function sendSlider(element){
			var sliderValue = element.value;
			console.log(sliderValue);
			var xhr = new XMLHttpRequest();
			xhr.open("GET", "/update?slider="+element.id+"&value="+sliderValue, true);
			xhr.send();
		}
```
Обработка чекбоксов
```js
		function sendToggle(element){
			var xhr = new XMLHttpRequest();
			if(element.checked){
				xhr.open("GET", "/update?toggle="+element.id+"&state=1", true);
			} else {
				xhr.open("GET", "/update?toggle="+element.id+"&state=0", true);
			}
			xhr.send();
		}
```
Обновление значений температуры и освещённости
```js
		setInterval(function ( ) {
			var xhttp = new XMLHttpRequest();
			xhttp.onreadystatechange = function() {
					if (this.readyState == 4 && this.status == 200) {
						document.getElementById("temperature").innerHTML = this.responseText;
					}
				};
				xhttp.open("GET", "/temperature", true);
				xhttp.send();
			}, 1000 ) ;

		setInterval(function ( ) {
				var xhttp = new XMLHttpRequest();
				xhttp.onreadystatechange = function() {
					if (this.readyState == 4 && this.status == 200) {
						document.getElementById("brightness").innerHTML = this.responseText;
					}
				};
				xhttp.open("GET", "/brightness", true);
				xhttp.send();
			}, 1000 ) ;
```

Именно - эту страницу мы запишем в **page.h** в переменную

```c++
const char index_html[] PROGMEM = R"rawliteral(
  <!DOCTYPE html>
  <html>
  //Код страницы
  </html>
)rawliteral";
```




## 2. Пишем сервер на ESP8266

Для начала подключаем .zip библиотеки из репозитория **Скетч>Подключить библиотеку>Добавить .ZIP библиотеку**

Также через менеджер библиотек загружаем **Adaruit NeoPixel**

Подключаем их и созданный **page.h**

```c++
#include "ESP8266WiFi.h"
#include "ESPAsyncWebServer.h"
#include <Adafruit_NeoPixel.h>

#include "page.h"
```

Определяем параметры светодиодной ленты

```c++
#define LED_PIN 5
#define LED_COUNT 60
#define BRIGHTNESS 50

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
```

Задаём начальные параметры необходимых переменных

```c++
float temperature = 0;
int brightness = 0;
int R = 0;
int G = 0;
int B = 0;
bool autobrightness = LOW;
bool rainbow = LOW;
bool updateLED = LOW;
```

Настраеваем параметры WiFi сети к которой будем подключаться и создадим переменную сервера с портом 80

```c++
// Replace with your network credentials
const char* ssid = "ADL_Kinetic";
const char* password = "ADLaboratory";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
```

Напишем функцию заменяющую плейс холдеры

```c++
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(temperature);
  } else if(var == "BRIGHTNESS"){
    return String(brightness);
  } else if(var == "R"){
    return String(R);
  } else if(var == "G"){
    return String(G);
  } else if(var == "B"){
    return String(B);
  } else if(var == "AUTOBRIGHTNESS"){
    if (autobrightness){
      return "checked";
    } else {
      return String();
    }
  } else if(var == "RAINBOW"){
    if (rainbow){
      return "checked";
    } else {
      return String();
    }
  }
  return String();
}
```

### Функция setup

В первой части происходит инициализация адресной светодиодной ленты и установка её яркости. Далее инициализируется Serial порт. Затем происходит подключение к сети WiFi и после этого отправка на Serial порт IP-адреса контроллера в сети. 

```c++
void setup(){

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);

  // Serial port for debugging purposes
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP8266 Local IP Address
  Serial.println(WiFi.localIP());
```

Во второй части описаны функции вызываемые при получении HTTP GET-запроса от страницы, их можно было описать и отдельно вне setup. Также именно здесь используется **ESPAsyncWebServer.h** так как она сама парсит GET-запрос

```C++
// Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(temperature).c_str());
  });
  server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(brightness).c_str());
  });

  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String id;
    String value;
    if (request->hasParam("slider") & request->hasParam("value")) {
      id = request->getParam("slider")->value();
      value = request->getParam("value")->value();
      Serial.println(id+" "+value);
      if(id=="RangeR"){
        R = value.toInt();
      } else if (id=="RangeG"){
        G = value.toInt();
      } else if (id=="RangeB"){
        B = value.toInt();
      }
      Serial.printf("%d %d %d\n", R, G, B);
      updateLED = HIGH;
    } else if (request->hasParam("toggle") & request->hasParam("state")) {
      id = request->getParam("toggle")->value();
      value = request->getParam("state")->value();
      Serial.println(id+" "+value);
      if(id=="Rainbow"){
        rainbow = value.toInt();
      } else if (id=="Autobrightness"){
        autobrightness = value.toInt();
      }
      Serial.printf("%d %d\n", rainbow, autobrightness);
    }
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
}
```

### Функция loop

Здесь ничего сложного, просто при изменении булевых переменных в функциях в **setup** переклюаются режимы адресной светодиодной ленты и происходит эмуляция изменения значений температуры и освещённости


```
void loop() {
  if(rainbow){
    rainbowLed();
    updateLED = HIGH;
  } else if (updateLED) {
    colorWipe(strip.Color(R, G, B));
  }
  if(autobrightness){
    strip.setBrightness(brightness);
  } else {
    strip.setBrightness(BRIGHTNESS);
  }
  yield();
  temperature = millis()%100;
  brightness = millis()/10%100;
}
```

Далее преведены две функции заполнения леты одним цветом и включения режима радуги

```c++
void colorWipe(uint32_t color) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                        //  Pause for a moment
  }
}

void rainbowLed() {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show();
    yield();
  }
}
```

## Схема подключения адресной светодиодной ленты

Для работы светодиодной ленты необходимо подключить её к отдельному блоку питания, так как контроллер сгорит если попытаться запитать её от него. также вам понадобиться резистор 200-500 Ом (В лабе у меня на столе в коробке есть резисторы на 220 Ом).

![Схема подключения адресной светодиодной ленты к ESP8266](https://github.com/4yvakZ/ESP8266_IoT_Lamp/blob/development/%D0%A1%D1%85%D0%B5%D0%BC%D0%B0.png)
