/*
 *
 * Code to get weather information from current location
 *
 */

var myAPIKey = 'fee4e550fc62b56b45c4f478fdcf0ec3';
var xhrRequest = function (url, type, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open(type, url);
  xhr.send();
};
function locationSuccess(pos) {
  // Construct URL
  var url = "http://api.openweathermap.org/data/2.5/weather?lat=" +
      pos.coords.latitude + "&lon=" + pos.coords.longitude + '&appid=' + myAPIKey;
  console.log("lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude);
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].main;      
      console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}
function locationError(err) {
  console.log("Error requesting location!");
}
function getWeather() {
  navigator.geolocation.getCurrentPosition(
    locationSuccess,
    locationError,
    {timeout: 15000, maximumAge: 60000}
  );
}
// Listen for when the watchface is opened
Pebble.addEventListener('ready', function(e) {
    console.log("PebbleKit JS ready!");
    // Get the initial weather
    getWeather();
  });
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e) {
    console.log("AppMessage received!");
    getWeather();
  });                   


/*
 *
 * Code to get configuration settings
 *
 */

Pebble.addEventListener('showConfiguration', function() {
  // PUT HERE THE FINAL WEBSITE LINK
  var url = 'http://42557273.ngrok.io/index.html';
  // 
  console.log('Showing configuration page: ' + url);
  Pebble.openURL(url);
});
Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  console.log('Configuration page returned: ' + JSON.stringify(configData));
  if (configData.backgroundColor) {
    Pebble.sendAppMessage({
      "KEY_MILITARY_TIME": configData.twentyFourHourFormat,
      
      "KEY_BACKGROUND_COLOR" : parseInt(configData.backgroundColor, 16),
      "KEY_FONT_COLOR": parseInt(configData.fontColor, 16),
      "KEY_BACKGROUND_DISCONNECT_COLOR": parseInt(configData.backgroundDisconnectColor, 16),
      "KEY_FONT_DISCONNECT_COLOR": parseInt(configData.fontDisconnectColor, 16),
      
      "KEY_WEATHER_VISIBLE" : configData.weatherVisible,
 
      "KEY_BATTERY_VISIBLE": configData.batteryVisible,
      //"KEY_BATTERY_PERCENT": configData.batteryPercent, 
      
      "KEY_VIBRATE_HOURLY": configData.vibrateHourly,
      
      "KEY_CUSTOM_TEXT_VISIBLE": configData.customTextVisible,
      "KEY_CUSTOM_TEXT_VALUE": configData.customTextValue,


      //"KEY_VIBRATE_DISCONNECT": configData.vibrateDissconnect,
      //"KEY_VISUAL_DISCONNECT": configData.visualDisconnect,
      /*
      "KEY_VIBRATION_MIN_TIME": null,
      "KEY_VIBRATION_MAX_TIME": null,
      "KEY_VIBRATION_MIN_DAY": null,
      "KEY_VIBRATION_MAX_DAY": null,
      
      "KEY_VIBRATION_HOURLY_STYLE": null,
      "KEY_VIBRATION_DISCONNECT_STYLE": null,
      */
    }, function() {
      console.log('Send successful!');
    }, function() {
      console.log('Send failed!');
    });
  }
});