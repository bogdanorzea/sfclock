(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');
  $submitButton.on('click', function() {
    // console.log('Submit');
    var return_to = getQueryParam('return_to', 'pebblejs://close#');
    document.location = return_to + encodeURIComponent(JSON.stringify(getAndStoreConfigData()));
  });
}

function loadOptions() {
  var $timeFormatCheckbox = $('#timeFormatCheckbox');
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $fontColorPicker = $('#fontColorPicker');
  var $weatherVisibleCheckbox = $('#weatherVisibleCheckbox');
  var $vibrateHourlyCheckbox = $('#vibrateHourlyCheckbox');
  var $vibrateHourlyStyleSelect = $('#vibrateHourlyStyleSelect');
  var $vibrateStartTimeSelect = $('#vibrateStartTimeSelect');
  var $vibrateEndTimeSelect = $('#vibrateEndTimeSelect');
  var $vibrateStartDaySelect = $('#vibrateStartDaySelect');
  var $vibrateEndDaySelect = $('#vibrateEndDaySelect');
  var $vibrateDisconnectCheckbox = $('#vibrateDisconnectCheckbox');
  var $vibrateDisconnectStyleSelect = $('#vibrateDisconnectStyleSelect');
  var $visualDisconnectCheckbox = $('#visualDisconnectCheckbox');
  var $backgroundDisconnectColorPicker = $('#backgroundDisconnectColorPicker');
  var $fontDisconnectColorPicker = $('#fontDisconnectColorPicker');
  var $batteryVisibleCheckbox = $('#batteryVisibleCheckbox');
  var $batteryTypeSelect = $('#batteryTypeSelect');
  var $customTextVisibleCheckbox = $('#customTextVisibleCheckbox');
  var $customTextValueField = $('#customTextValueField');
  
  // console.log('Existing options: ' + JSON.stringify(localStorage));

  if (localStorage.twentyFourHourFormat) {
    $timeFormatCheckbox[0].checked = localStorage.twentyFourHourFormat === 'true';
    $backgroundColorPicker[0].value = localStorage.backgroundColor;  
    $fontColorPicker[0].value = localStorage.fontColor;
    $backgroundDisconnectColorPicker[0].value = localStorage.backgroundDisconnectColor;
    $fontDisconnectColorPicker[0].value = localStorage.fontDisconnectColor;
    $weatherVisibleCheckbox[0].checked = localStorage.weatherVisible === 'true';
    $vibrateHourlyCheckbox[0].checked = localStorage.vibrateHourly === 'true';
    $vibrateHourlyStyleSelect[0].value = getVibrateString(localStorage.vibrateHourlyStyle);
    $vibrateStartTimeSelect[0].value = getHourString(localStorage.vibrateStartTime);
    $vibrateEndTimeSelect[0].value = getHourString(localStorage.vibrateEndTime);
    $vibrateStartDaySelect[0].value = getDayString(localStorage.vibrateStartDay);
    $vibrateEndDaySelect[0].value = getDayString(localStorage.vibrateEndDay);
    $vibrateDisconnectCheckbox[0].checked = localStorage.vibrateDisconnect === 'true';
    $vibrateDisconnectStyleSelect[0].value = getVibrateString(localStorage.vibrateDisconnectStyle);
    $visualDisconnectCheckbox[0].checked = localStorage.visualDisconnect === 'true';
    $batteryVisibleCheckbox[0].checked = localStorage.batteryVisible === 'true';
    $batteryTypeSelect[0].value = getBatteryString(localStorage.batteryType);
    $customTextVisibleCheckbox[0].checked = localStorage.customTextVisible === 'true';
    $customTextValueField[0].value = localStorage.customTextValue;
  }
}

function getAndStoreConfigData() {
  var $timeFormatCheckbox = $('#timeFormatCheckbox');
  var $backgroundColorPicker = $('#backgroundColorPicker');
  var $fontColorPicker = $('#fontColorPicker');
  var $weatherVisibleCheckbox = $('#weatherVisibleCheckbox');
  var $vibrateHourlyCheckbox = $('#vibrateHourlyCheckbox');
  var $vibrateHourlyStyleSelect = $('#vibrateHourlyStyleSelect');
  var $vibrateStartTimeSelect = $('#vibrateStartTimeSelect');
  var $vibrateEndTimeSelect = $('#vibrateEndTimeSelect');
  var $vibrateStartDaySelect = $('#vibrateStartDaySelect');
  var $vibrateEndDaySelect = $('#vibrateEndDaySelect');
  var $vibrateDisconnectCheckbox = $('#vibrateDisconnectCheckbox');
  var $vibrateDisconnectStyleSelect = $('#vibrateDisconnectStyleSelect');
  var $visualDisconnectCheckbox = $('#visualDisconnectCheckbox');
  var $backgroundDisconnectColorPicker = $('#backgroundDisconnectColorPicker');
  var $fontDisconnectColorPicker = $('#fontDisconnectColorPicker');
  var $batteryVisibleCheckbox = $('#batteryVisibleCheckbox');
  var $batteryTypeSelect = $('#batteryTypeSelect');
  var $customTextVisibleCheckbox = $('#customTextVisibleCheckbox');
  var $customTextValueField = $('#customTextValueField');

  var options = {
    twentyFourHourFormat: $timeFormatCheckbox[0].checked,
    backgroundColor: $backgroundColorPicker[0].value,
    fontColor: $fontColorPicker[0].value ,
    weatherVisible: $weatherVisibleCheckbox[0].checked,
    vibrateHourly: $vibrateHourlyCheckbox[0].checked,
    vibrateHourlyStyle: getVibrateInteger($vibrateHourlyStyleSelect[0].value),
    vibrateStartTime: parseInt($vibrateStartTimeSelect[0].value.substring(0,2)),
    vibrateEndTime: parseInt($vibrateEndTimeSelect[0].value.substring(0,2)),
    vibrateStartDay: getDayInteger($vibrateStartDaySelect[0].value),
    vibrateEndDay: getDayInteger($vibrateEndDaySelect[0].value),
    vibrateDisconnect: $vibrateDisconnectCheckbox[0].checked,
    vibrateDisconnectStyle: getVibrateInteger($vibrateDisconnectStyleSelect[0].value),
    visualDisconnect: $visualDisconnectCheckbox[0].checked,
    backgroundDisconnectColor: $backgroundDisconnectColorPicker[0].value,
    fontDisconnectColor: $fontDisconnectColorPicker[0].value,
    batteryVisible: $batteryVisibleCheckbox[0].checked,
    batteryType: getBatteryInteger($batteryTypeSelect[0].value),
    customTextVisible: $customTextVisibleCheckbox[0].checked,
    customTextValue: $customTextValueField[0].value
  };

    localStorage.twentyFourHourFormat = options.twentyFourHourFormat;
    localStorage.backgroundColor = options.backgroundColor;
    localStorage.fontColor = options.fontColor;
    localStorage.weatherVisible = options.weatherVisible;
    localStorage.vibrateHourly = options.vibrateHourly;
    localStorage.vibrateHourlyStyle = options.vibrateHourlyStyle;
    localStorage.vibrateStartTime = options.vibrateStartTime;
    localStorage.vibrateEndTime = options.vibrateEndTime;
    localStorage.vibrateStartDay = options.vibrateStartDay;
    localStorage.vibrateEndDay = options.vibrateEndDay;
    localStorage.vibrateDisconnect = options.vibrateDisconnect;
    localStorage.vibrateDisconnectStyle = options.vibrateDisconnectStyle;
    localStorage.visualDisconnect = options.visualDisconnect;
    localStorage.backgroundDisconnectColor = options.backgroundDisconnectColor;
    localStorage.fontDisconnectColor = options.fontDisconnectColor;
    localStorage.batteryVisible = options.batteryVisible;
    localStorage.batteryType = options.batteryType;
    localStorage.customTextVisible = options.customTextVisible;
    localStorage.customTextValue = options.customTextValue;

  // console.log('Got options: ' + JSON.stringify(options));
  return options;
}

function getQueryParam(variable, defaultValue) {
  var query = location.search.substring(1);
  var vars = query.split('&');
  for (var i = 0; i < vars.length; i++) {
    var pair = vars[i].split('=');
    if (pair[0] === variable) {
      return decodeURIComponent(pair[1]);
    }
  }
  return defaultValue || false;
}

// Function returns: 0 for "Sunday" and so on. 
function getDayInteger(day) {
  switch (day) {
      case "Sunday":
          day = 0;
          break;
      case "Monday":
          day = 1;
          break;
      case "Tuesday":
          day = 2;
          break;
      case "Wednesday":
          day = 3;
          break;
      case "Thursday":
          day = 4;
          break;
      case "Friday":
          day = 5;
          break;
      case "Saturday":
          day = 6;
          break;
  }
  return day;
} 
function getDayString(day) {
  day = parseInt(day);
  switch (day) {
      case 0:
          day = "Sunday";
          break;
      case 1:
          day = "Monday";
          break;
      case 2:
          day = "Tuesday";
          break;
      case 3:
          day = "Wednesday";
          break;
      case 4:
          day = "Thursday";
          break;
      case 5:
          day = "Friday";
          break;
      case 6:
          day = "Saturday";
          break;
  }
  return day;
} 

//Function returns: - 1 for "Percentage" 
//                  - 0 for "Progressbar"
function getBatteryInteger(battery){
  if (battery === "Percentage"){
    return 1
  }
  return 0
}
function getBatteryString(battery){
  battery = parseInt(battery);
  if (battery === 0){
    return "Progressbar"
  }
  return "Percentage"
}

// Function returns the hour as a string
function getHourString(hour) {
  hour = parseInt(hour);
  if (hour < 10){
    return "0" + hour + ":00"
  }
  return hour + ":00"
} 

//Function returns: - 0 for "Short pulse" 
//                  - 1 for "Long pulse"
//                  - 2 for "Double pulse"
function getVibrateInteger(vibrate) {
  switch (vibrate) {
      case "Short pulse":
          vibrate = 0;
          break;
      case "Long pulse":
          vibrate = 1;
          break;
      case "Double pulse":
          vibrate = 2;
          break;
  }
  return vibrate;
} 
function getVibrateString(vibrate) {
  vibrate = parseInt(vibrate);
  switch (vibrate) {
      case 0: 
          vibrate = "Short pulse";
          break;
      case 1:
          vibrate = "Long pulse";
          break;
      case 2:
          vibrate = "Double pulse";
          break;
  }
  return vibrate;
} 