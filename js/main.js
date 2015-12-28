(function() {
  loadOptions();
  submitHandler();
})();

function submitHandler() {
  var $submitButton = $('#submitButton');

  $submitButton.on('click', function() {
    console.log('Submit');

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
  var $vibrateDisconnectCheckbox = $('#vibrateDisconnectCheckbox');
  var $visualDisconnectCheckbox = $('#visualDisconnectCheckbox');
  var $backgroundDisconnectColorPicker = $('#backgroundDisconnectColorPicker');
  var $fontDisconnectColorPicker = $('#fontDisconnectColorPicker');
  var $batteryVisibleCheckbox = $('#batteryVisibleCheckbox');
  //var $batteryPercentCheckbox = $('#batteryPercentCheckbox');
  var $customTextVisibleCheckbox = $('#customTextVisibleCheckbox');
  var $customTextValueField = $('#customTextValueField');
  
  if (localStorage.twentyFourHourFormat) {
    $timeFormatCheckbox[0].checked = localStorage.twentyFourHourFormat === 'true';
    $backgroundColorPicker[0].value = localStorage.backgroundColor;  
    $fontColorPicker[0].value = localStorage.fontColor;
    $backgroundDisconnectColorPicker[0].value = localStorage.backgroundDisconnectColor;
    $fontDisconnectColorPicker[0].value = localStorage.fontDisconnectColor;
    $weatherVisibleCheckbox[0].checked = localStorage.weatherVisible === 'true';
    $vibrateHourlyCheckbox[0].checked = localStorage.vibrateHourly === 'true';
    $vibrateDisconnectCheckbox[0].checked = localStorage.vibrateDisconnect === 'true';
    $visualDisconnectCheckbox[0].checked = localStorage.visualDisconnect === 'true';
    $batteryVisibleCheckbox[0].checked = localStorage.batteryVisible === 'true';
    //$batteryPercentCheckbox[0].checked = localStorage.batteryPercent === 'true';
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
  var $vibrateDisconnectCheckbox = $('#vibrateDisconnectCheckbox');
  var $visualDisconnectCheckbox = $('#visualDisconnectCheckbox');
  var $backgroundDisconnectColorPicker = $('#backgroundDisconnectColorPicker');
  var $fontDisconnectColorPicker = $('#fontDisconnectColorPicker');
  var $batteryVisibleCheckbox = $('#batteryVisibleCheckbox');
  //var $batteryPercentCheckbox = $('#batteryPercentCheckbox');
  var $customTextVisibleCheckbox = $('#customTextVisibleCheckbox');
  var $customTextValueField = $('#customTextValueField');

  var options = {
    twentyFourHourFormat: $timeFormatCheckbox[0].checked,
    backgroundColor: $backgroundColorPicker[0].value,
    fontColor: $fontColorPicker[0].value ,
    weatherVisible: $weatherVisibleCheckbox[0].checked,
    vibrateHourly: $vibrateHourlyCheckbox[0].checked,
    vibrateDisconnect: $vibrateDisconnectCheckbox[0].checked,
    visualDisconnect: $visualDisconnectCheckbox[0].checked,
    backgroundDisconnectColor: $backgroundDisconnectColorPicker[0].value,
    fontDisconnectColor: $fontDisconnectColorPicker[0].value,
    batteryVisible: $batteryVisibleCheckbox[0].checked,
    //batteryPercent: $batteryPercentCheckbox[0].checked,
    customTextVisible: $customTextVisibleCheckbox[0].checked,
    customTextValue: $customTextValueField[0].value
  };

    localStorage.twentyFourHourFormat = options.twentyFourHourFormat;
    localStorage.backgroundColor = options.backgroundColor;
    localStorage.fontColor = options.fontColor;
    localStorage.weatherVisible = options.weatherVisible;
    localStorage.vibrateHourly = options.vibrateHourly;
    localStorage.vibrateDisconnect = options.vibrateDisconnect;
    localStorage.visualDisconnect = options.visualDisconnect;
    localStorage.backgroundDisconnectColor = options.backgroundDisconnectColor;
    localStorage.fontDisconnectColor = options.fontDisconnectColor;
    localStorage.batteryVisible = options.batteryVisible;
    //localStorage.batteryPercent = options.batteryPercent;
    localStorage.customTextVisible = options.customTextVisible;
    localStorage.customTextValue = options.customTextValue;

  console.log('Got options: ' + JSON.stringify(options));
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
