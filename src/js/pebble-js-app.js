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
  // console.log("lat=" + pos.coords.latitude + "&lon=" + pos.coords.longitude);
  // Send request to OpenWeatherMap
  xhrRequest(url, 'GET', 
    function(responseText) {
      // responseText contains a JSON object with weather info
      var json = JSON.parse(responseText);

      // Temperature in Kelvin requires adjustment
      var temperature = Math.round(json.main.temp - 273.15);
      // console.log("Temperature is " + temperature);

      // Conditions
      var conditions = json.weather[0].main;      
      // console.log("Conditions are " + conditions);
      
      // Assemble dictionary using our keys
      var dictionary = {
        "KEY_TEMPERATURE": temperature,
        "KEY_CONDITIONS": conditions
      };

      // Send to Pebble
      Pebble.sendAppMessage(dictionary,
        function(e) {
          // console.log("Weather info sent to Pebble successfully!");
        },
        function(e) {
          // console.log("Error sending weather info to Pebble!");
        }
      );
    }      
  );
}
function locationError(err) {
  // console.log("Error requesting location!");
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
    // console.log("PebbleKit JS ready!");
    // Get the initial weather
    getWeather();
  });
// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage', function(e) {
    // console.log("AppMessage received!");
    getWeather();
  });                   


/*
 *
 * Code to get configuration settings
 *
 */

Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://bogdanorzea.github.io/sfclock/index.html';
  // console.log('Showing configuration page: ' + url);
  Pebble.openURL(url);
});

Pebble.addEventListener('webviewclosed', function(e) {
  var configData = JSON.parse(decodeURIComponent(e.response));
  // console.log('Configuration page returned: ' + JSON.stringify(configData));
  if (configData.backgroundColor) {
    Pebble.sendAppMessage(
{      "KEY_MILITARY_TIME": configData.twentyFourHourFormat,
      "KEY_BACKGROUND_COLOR" : parseInt(configData.backgroundColor, 16),
      "KEY_FONT_COLOR": parseInt(configData.fontColor, 16),
      "KEY_WEATHER_VISIBLE" : configData.weatherVisible,
      "KEY_VIBRATE_HOURLY": configData.vibrateHourly,
      "KEY_VIBRATE_HOURLY_STYLE": configData.vibrateHourlyStyle,
      "KEY_VIBRATE_START_TIME": configData.vibrateStartTime,
      "KEY_VIBRATE_END_TIME": configData.vibrateEndTime,
      "KEY_VIBRATE_START_DAY": configData.vibrateStartDay,
      "KEY_VIBRATE_END_DAY": configData.vibrateEndDay,
      "KEY_VIBRATE_DISCONNECT": configData.vibrateDissconnect,
      "KEY_VIBRATE_DISCONNECT_STYLE": configData.vibrateDisconnectStyle,
      "KEY_VISUAL_DISCONNECT": configData.visualDisconnect,
      "KEY_BACKGROUND_DISCONNECT_COLOR": parseInt(configData.backgroundDisconnectColor, 16),
      "KEY_FONT_DISCONNECT_COLOR": parseInt(configData.fontDisconnectColor, 16),
      "KEY_BATTERY_VISIBLE": configData.batteryVisible,
      "KEY_BATTERY_TYPE": configData.batterType,
      "KEY_CUSTOM_TEXT_VISIBLE": configData.customTextVisible,
      "KEY_CUSTOM_TEXT_VALUE": configData.customTextValue
    }, function() {
      // console.log('Send successful!');
    }, function() {
      // console.log('Send failed!');
    });
  }
});