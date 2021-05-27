const char index_html[] PROGMEM = R"rawliteral(
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
  		<input type="range" min="0" max="255" value="%R%" class="red-thumb slider" onchange="sendSlider(this)" id="RangeR">
  		<span class="value" id="ValueR">
  	</p>

  	<p>
  		<span>G:</span>
  		<input type="range" min="0" max="255" value="%G%" class="green-thumb slider" onchange="sendSlider(this)" id="RangeG">
  		<span class="value" id="ValueG">
  	</p>

  	<p>
  		<span>B:</span>
  		<input type="range" min="0" max="255" value="%B%" class="blue-thumb slider" onchange="sendSlider(this)" id="RangeB">
  		<span class="value" id="ValueB">
  	</p>

  	<p>
  		<span>Радуга</span>
  		<label class="switch">
  			<input type="checkbox" id="Rainbow" onchange="sendToggle(this)" %RAINBOW%>
  			<span class="slider switch"></span>
  		</label>
  	</p>

  	<p>
  		<span>Автояркость</span>
  		<label class="switch">
  			<input type="checkbox" id="Autobrightness" onchange="sendToggle(this)" %AUTOBRIGHTNESS%>
  			<span class="slider switch"></span>
  		</label>
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

  		setInterval(function ( ) {
  				var xhttp = new XMLHttpRequest();
  				xhttp.onreadystatechange = function() {
  					if (this.readyState == 4 && this.status == 200) {
  						document.getElementById("RangeR").value = this.responseText;
  						document.getElementById("ValueR").innerHTML = this.responseText;
  					}
  				};
  				xhttp.open("GET", "/RangeR", true);
  				xhttp.send();
  			}, 1000 ) ;

  		setInterval(function ( ) {
  				var xhttp = new XMLHttpRequest();
  				xhttp.onreadystatechange = function() {
  					if (this.readyState == 4 && this.status == 200) {
  						document.getElementById("RangeG").value = this.responseText;
  						document.getElementById("ValueG").innerHTML = this.responseText;
  					}
  				};
  				xhttp.open("GET", "/RangeG", true);
  				xhttp.send();
  			}, 1000 ) ;

  		setInterval(function ( ) {
  				var xhttp = new XMLHttpRequest();
  				xhttp.onreadystatechange = function() {
  					if (this.readyState == 4 && this.status == 200) {
  						document.getElementById("RangeB").value = this.responseText;
  						document.getElementById("ValueB").innerHTML = this.responseText;
  					}
  				};
  				xhttp.open("GET", "/RangeB", true);
  				xhttp.send();
  			}, 1000 ) ;

  			setInterval(function ( ) {
  					var xhttp = new XMLHttpRequest();
  					xhttp.onreadystatechange = function() {
  						if (this.readyState == 4 && this.status == 200) {
  							if(this.responseText == "checked"){
  								document.getElementById("Rainbow").checked = true;
  							} else {
  								document.getElementById("Rainbow").checked = false;
  							}
  						}
  					};
  					xhttp.open("GET", "/Rainbow", true);
  					xhttp.send();
  				}, 1000 ) ;

  			setInterval(function ( ) {
  					var xhttp = new XMLHttpRequest();
  					xhttp.onreadystatechange = function() {
  						if (this.readyState == 4 && this.status == 200) {
  							if(this.responseText == "checked"){
  								document.getElementById("Autobrightness").checked = true;
  							} else {
  								document.getElementById("Autobrightness").checked = false;
  							}
  						}
  					};
  					xhttp.open("GET", "/Autobrightness", true);
  					xhttp.send();
  				}, 1000 ) ;
  	</script>
  </body>
  </html>
)rawliteral";
